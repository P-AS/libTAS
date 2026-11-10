/*
    Copyright 2015-2026 Clément Gallet <clement.gallet@ens-lyon.org>

    This file is part of libTAS.

    libTAS is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libTAS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libTAS.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "GlobalPointer.h"

#include "config.h"

#ifndef LIBTAS_HAS_SCREENCAST_POINTER

std::unique_ptr<GlobalPointer> createScreenCastPointer()
{
    return nullptr;
}

#else

#include <atomic>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <gio/gio.h>
#include <pipewire/pipewire.h>
#include <spa/param/video/format-utils.h>
#include <spa/param/buffers.h>
#include <spa/buffer/meta.h>

/* Mutter, and niri which implements the same interface for its portal, give the
 * position as metadata of the buffers of a screencast, for each monitor. We never
 * look at the images. It really is a screencast, so it has a cost and the user may
 * be notified. */

#define SCREENCAST_SERVICE "org.gnome.Mutter.ScreenCast"
#define DISPLAYCONFIG_SERVICE "org.gnome.Mutter.DisplayConfig"
#define CURSOR_META_SIZE(width, height) (sizeof(spa_meta_cursor) + sizeof(spa_meta_bitmap) + (width) * (height) * 4)

class ScreenCastPointer : public GlobalPointer {
public:
    ~ScreenCastPointer() override
    {
        if (loop) {
            pw_thread_loop_stop(loop);
            for (auto& monitor : monitors) {
                if (monitor->stream)
                    pw_stream_destroy(monitor->stream);
            }
            if (core)
                pw_core_disconnect(core);
            if (pw_context_)
                pw_context_destroy(pw_context_);
            pw_thread_loop_destroy(loop);
        }
        if (connection) {
            /* Mutter ends the session when we disconnect, but niri keeps it, so
             * stop it ourselves. */
            if (!session_path.empty()) {
                GVariant* reply = g_dbus_connection_call_sync(connection, SCREENCAST_SERVICE,
                    session_path.c_str(), SCREENCAST_SERVICE ".Session", "Stop", nullptr, nullptr,
                    G_DBUS_CALL_FLAGS_NONE, 1000, nullptr, nullptr);
                if (reply)
                    g_variant_unref(reply);
            }
            g_dbus_connection_close_sync(connection, nullptr, nullptr);
            g_object_unref(connection);
        }
        if (context)
            g_main_context_unref(context);
    }

    const char* name() const override { return "Mutter screencast"; }

    bool connect()
    {
        gchar* address = g_dbus_address_get_for_bus_sync(G_BUS_TYPE_SESSION, nullptr, nullptr);
        if (!address)
            return false;
        connection = g_dbus_connection_new_for_address_sync(address,
            static_cast<GDBusConnectionFlags>(G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_CLIENT | G_DBUS_CONNECTION_FLAGS_MESSAGE_BUS_CONNECTION),
            nullptr, nullptr, nullptr);
        g_free(address);
        if (!connection)
            return false;

        if (!readMonitors() || monitors.empty())
            return false;

        context = g_main_context_new();
        g_main_context_push_thread_default(context);
        bool success = startSession();
        g_main_context_pop_thread_default(context);
        return success && startStreams();
    }

    bool query(int& x, int& y) override
    {
        const Monitor* latest = nullptr;
        uint64_t latest_serial = 0;
        for (const auto& monitor : monitors) {
            uint64_t serial = monitor->serial.load(std::memory_order_acquire);
            if (monitor->visible.load(std::memory_order_relaxed) && (serial > latest_serial)) {
                latest = monitor.get();
                latest_serial = serial;
            }
        }
        if (!latest)
            return false;

        x = latest->x + static_cast<int>(latest->position_x.load(std::memory_order_relaxed) / latest->scale);
        y = latest->y + static_cast<int>(latest->position_y.load(std::memory_order_relaxed) / latest->scale);
        return true;
    }

private:
    struct Monitor {
        ScreenCastPointer* parent = nullptr;
        std::string connector;
        std::string stream_path;
        int x = 0, y = 0;
        double scale = 1.0;
        guint32 node = 0;
        guint signal = 0;
        bool node_known = false;
        pw_stream* stream = nullptr;
        spa_hook listener = {};
        std::atomic<int> position_x{0}, position_y{0};
        std::atomic<bool> visible{false};
        std::atomic<uint64_t> serial{0};
    };

    GVariant* call(const char* service, const char* path, const char* interface, const char* method, GVariant* parameters)
    {
        GError* error = nullptr;
        GVariant* reply = g_dbus_connection_call_sync(connection, service, path, interface, method,
            parameters, nullptr, G_DBUS_CALL_FLAGS_NONE, 3000, nullptr, &error);
        if (error) {
            std::cerr << "Could not call " << interface << "." << method << ": " << error->message << std::endl;
            g_error_free(error);
        }
        return reply;
    }

    bool readMonitors()
    {
        GVariant* state = call(DISPLAYCONFIG_SERVICE, "/org/gnome/Mutter/DisplayConfig",
            DISPLAYCONFIG_SERVICE, "GetCurrentState", nullptr);
        if (!state)
            return false;

        /* (serial, monitors, logical monitors, properties) */
        GVariant* logical_monitors = g_variant_get_child_value(state, 2);
        GVariant* properties = g_variant_get_child_value(state, 3);

        /* The layout is either in logical pixels (1), or physical pixels (2) */
        guint32 layout_mode = 1;
        g_variant_lookup(properties, "layout-mode", "u", &layout_mode);

        GVariantIter iter;
        g_variant_iter_init(&iter, logical_monitors);
        GVariant* logical;
        while ((logical = g_variant_iter_next_value(&iter))) {
            /* (x, y, scale, transform, primary, [(connector, ...)], properties) */
            gint32 x, y;
            gdouble scale;
            g_variant_get_child(logical, 0, "i", &x);
            g_variant_get_child(logical, 1, "i", &y);
            g_variant_get_child(logical, 2, "d", &scale);

            GVariant* physical_monitors = g_variant_get_child_value(logical, 5);
            if (g_variant_n_children(physical_monitors) > 0) {
                GVariant* physical = g_variant_get_child_value(physical_monitors, 0);
                const gchar* connector = nullptr;
                g_variant_get_child(physical, 0, "&s", &connector);

                auto monitor = std::make_unique<Monitor>();
                monitor->parent = this;
                monitor->connector = connector;
                monitor->x = x;
                monitor->y = y;
                monitor->scale = (layout_mode == 1 && scale > 0.0) ? scale : 1.0;
                monitors.push_back(std::move(monitor));
                g_variant_unref(physical);
            }
            g_variant_unref(physical_monitors);
            g_variant_unref(logical);
        }
        g_variant_unref(properties);
        g_variant_unref(logical_monitors);
        g_variant_unref(state);
        return true;
    }

    static void handleStreamAdded(GDBusConnection*, const gchar*, const gchar*, const gchar*, const gchar*,
        GVariant* parameters, gpointer data)
    {
        auto* monitor = static_cast<Monitor*>(data);
        g_variant_get(parameters, "(u)", &monitor->node);
        monitor->node_known = true;
    }

    bool startSession()
    {
        GVariantBuilder options;
        g_variant_builder_init(&options, G_VARIANT_TYPE_VARDICT);
        GVariant* reply = call(SCREENCAST_SERVICE, "/org/gnome/Mutter/ScreenCast", SCREENCAST_SERVICE,
            "CreateSession", g_variant_new("(a{sv})", &options));
        if (!reply)
            return false;
        const gchar* session = nullptr;
        g_variant_get(reply, "(&o)", &session);
        session_path = session;
        g_variant_unref(reply);

        static const guint32 CURSOR_MODE_METADATA = 2;
        for (auto& monitor : monitors) {
            GVariantBuilder properties;
            g_variant_builder_init(&properties, G_VARIANT_TYPE_VARDICT);
            g_variant_builder_add(&properties, "{sv}", "cursor-mode", g_variant_new_uint32(CURSOR_MODE_METADATA));
            reply = call(SCREENCAST_SERVICE, session_path.c_str(), SCREENCAST_SERVICE ".Session",
                "RecordMonitor", g_variant_new("(sa{sv})", monitor->connector.c_str(), &properties));
            if (!reply)
                return false;
            const gchar* stream = nullptr;
            g_variant_get(reply, "(&o)", &stream);
            monitor->stream_path = stream;
            g_variant_unref(reply);

            monitor->signal = g_dbus_connection_signal_subscribe(connection, nullptr, SCREENCAST_SERVICE ".Stream",
                "PipeWireStreamAdded", monitor->stream_path.c_str(), nullptr, G_DBUS_SIGNAL_FLAGS_NONE,
                handleStreamAdded, monitor.get(), nullptr);
        }

        reply = call(SCREENCAST_SERVICE, session_path.c_str(), SCREENCAST_SERVICE ".Session", "Start", nullptr);
        if (!reply)
            return false;
        g_variant_unref(reply);

        /* The ids of the nodes come as signals, shortly */
        for (int i = 0; i < 300; i++) {
            while (g_main_context_iteration(context, FALSE));
            bool all_known = true;
            for (const auto& monitor : monitors)
                all_known = all_known && monitor->node_known;
            if (all_known)
                return true;
            g_usleep(10000);
        }
        std::cerr << "The screencast did not start" << std::endl;
        return false;
    }

    static void handleParamChanged(void* data, uint32_t id, const spa_pod* param)
    {
        auto* monitor = static_cast<Monitor*>(data);
        if (!param || (id != SPA_PARAM_Format))
            return;

        uint8_t buffer[1024];
        spa_pod_builder builder = SPA_POD_BUILDER_INIT(buffer, sizeof(buffer));
        const spa_pod* params[2];
        params[0] = static_cast<const spa_pod*>(spa_pod_builder_add_object(&builder,
            SPA_TYPE_OBJECT_ParamBuffers, SPA_PARAM_Buffers,
            SPA_PARAM_BUFFERS_buffers, SPA_POD_CHOICE_RANGE_Int(4, 2, 8),
            SPA_PARAM_BUFFERS_dataType, SPA_POD_CHOICE_FLAGS_Int((1 << SPA_DATA_MemFd) | (1 << SPA_DATA_MemPtr))));
        params[1] = static_cast<const spa_pod*>(spa_pod_builder_add_object(&builder,
            SPA_TYPE_OBJECT_ParamMeta, SPA_PARAM_Meta,
            SPA_PARAM_META_type, SPA_POD_Id(SPA_META_Cursor),
            SPA_PARAM_META_size, SPA_POD_CHOICE_RANGE_Int(static_cast<int>(CURSOR_META_SIZE(64, 64)),
                static_cast<int>(CURSOR_META_SIZE(1, 1)), static_cast<int>(CURSOR_META_SIZE(384, 384)))));
        pw_stream_update_params(monitor->stream, params, 2);
    }

    static void handleProcess(void* data)
    {
        auto* monitor = static_cast<Monitor*>(data);
        pw_buffer* buffer = pw_stream_dequeue_buffer(monitor->stream);
        if (!buffer)
            return;

        auto* cursor = static_cast<spa_meta_cursor*>(spa_buffer_find_meta_data(buffer->buffer, SPA_META_Cursor, sizeof(spa_meta_cursor)));
        if (cursor) {
            if (spa_meta_cursor_is_valid(cursor)) {
                bool moved = !monitor->visible.load(std::memory_order_relaxed) ||
                    (monitor->position_x.load(std::memory_order_relaxed) != cursor->position.x) ||
                    (monitor->position_y.load(std::memory_order_relaxed) != cursor->position.y);
                monitor->position_x.store(cursor->position.x, std::memory_order_relaxed);
                monitor->position_y.store(cursor->position.y, std::memory_order_relaxed);
                monitor->visible.store(true, std::memory_order_relaxed);
                if (moved)
                    monitor->serial.store(monitor->parent->next_serial.fetch_add(1) + 1, std::memory_order_release);
            }
            else {
                monitor->visible.store(false, std::memory_order_relaxed);
            }
        }
        pw_stream_queue_buffer(monitor->stream, buffer);
    }

    bool startStreams()
    {
        pw_init(nullptr, nullptr);
        loop = pw_thread_loop_new("libtas-pointer", nullptr);
        if (!loop)
            return false;
        pw_thread_loop_lock(loop);
        bool success = (pw_thread_loop_start(loop) == 0);
        if (success) {
            pw_context_ = pw_context_new(pw_thread_loop_get_loop(loop), nullptr, 0);
            core = pw_context_ ? pw_context_connect(pw_context_, nullptr, 0) : nullptr;
            success = (core != nullptr);
        }
        for (auto& monitor : monitors) {
            if (!success)
                break;
            success = connectStream(monitor.get());
        }
        pw_thread_loop_unlock(loop);
        return success;
    }

    bool connectStream(Monitor* monitor)
    {
        static const pw_stream_events events = []() {
            pw_stream_events e = {};
            e.version = PW_VERSION_STREAM_EVENTS;
            e.param_changed = handleParamChanged;
            e.process = handleProcess;
            return e;
        }();

        monitor->stream = pw_stream_new(core, "libtas-pointer", pw_properties_new(
            PW_KEY_MEDIA_TYPE, "Video", PW_KEY_MEDIA_CATEGORY, "Capture", PW_KEY_MEDIA_ROLE, "Screen", nullptr));
        if (!monitor->stream)
            return false;
        pw_stream_add_listener(monitor->stream, &monitor->listener, &events, monitor);

        /* Don't offer any modifier, to get shared memory buffers, as we don't look
         * at the images. With modifiers, niri stops the cast if only one of them is
         * common with us, as it then doesn't get the list that it expects. */
        uint8_t buffer[2048];
        spa_pod_builder builder = SPA_POD_BUILDER_INIT(buffer, sizeof(buffer));
        const spa_pod* params[1];
        spa_rectangle default_size = SPA_RECTANGLE(1280, 800), min_size = SPA_RECTANGLE(1, 1), max_size = SPA_RECTANGLE(16384, 16384);
        spa_fraction no_rate = SPA_FRACTION(0, 1), default_rate = SPA_FRACTION(60, 1), max_rate = SPA_FRACTION(1000, 1);
        params[0] = static_cast<const spa_pod*>(spa_pod_builder_add_object(&builder,
            SPA_TYPE_OBJECT_Format, SPA_PARAM_EnumFormat,
            SPA_FORMAT_mediaType, SPA_POD_Id(SPA_MEDIA_TYPE_video),
            SPA_FORMAT_mediaSubtype, SPA_POD_Id(SPA_MEDIA_SUBTYPE_raw),
            SPA_FORMAT_VIDEO_format, SPA_POD_CHOICE_ENUM_Id(5, SPA_VIDEO_FORMAT_BGRx, SPA_VIDEO_FORMAT_BGRx,
                SPA_VIDEO_FORMAT_BGRA, SPA_VIDEO_FORMAT_RGBx, SPA_VIDEO_FORMAT_RGBA),
            SPA_FORMAT_VIDEO_size, SPA_POD_CHOICE_RANGE_Rectangle(&default_size, &min_size, &max_size),
            SPA_FORMAT_VIDEO_framerate, SPA_POD_Fraction(&no_rate),
            SPA_FORMAT_VIDEO_maxFramerate, SPA_POD_CHOICE_RANGE_Fraction(&default_rate, &no_rate, &max_rate)));
        return pw_stream_connect(monitor->stream, PW_DIRECTION_INPUT, monitor->node,
            PW_STREAM_FLAG_AUTOCONNECT, params, 1) == 0;
    }

    GDBusConnection* connection = nullptr;
    std::string session_path;
    GMainContext* context = nullptr;
    pw_thread_loop* loop = nullptr;
    pw_context* pw_context_ = nullptr;
    pw_core* core = nullptr;
    std::vector<std::unique_ptr<Monitor>> monitors;
    std::atomic<uint64_t> next_serial{0};
};

std::unique_ptr<GlobalPointer> createScreenCastPointer()
{
    auto pointer = std::make_unique<ScreenCastPointer>();
    if (!pointer->connect())
        return nullptr;
    return pointer;
}

#endif
