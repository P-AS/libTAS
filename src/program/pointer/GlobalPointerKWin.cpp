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

#ifndef LIBTAS_HAS_GIO

std::unique_ptr<GlobalPointer> createKWinPointer()
{
    return nullptr;
}

#else

#include <cstdio>
#include <string>
#include <gio/gio.h>
#include <unistd.h>

/* KWin has no query for the position, but scripts can read it, can be loaded over
 * D-Bus, and can call us over D-Bus each time it changes. */
class KWinPointer : public GlobalPointer {
public:
    ~KWinPointer() override
    {
        if (script_loaded)
            callScripting("unloadScript", g_variant_new("(s)", plugin_name.c_str()));
        if (!script_path.empty())
            unlink(script_path.c_str());
        if (connection) {
            if (registration)
                g_dbus_connection_unregister_object(connection, registration);
            g_dbus_connection_close_sync(connection, nullptr, nullptr);
            g_object_unref(connection);
        }
        if (context)
            g_main_context_unref(context);
    }

    const char* name() const override { return "KWin script"; }

    bool connect()
    {
        /* Use our own connection, whose calls are handled by query() */
        gchar* address = g_dbus_address_get_for_bus_sync(G_BUS_TYPE_SESSION, nullptr, nullptr);
        if (!address)
            return false;
        connection = g_dbus_connection_new_for_address_sync(address,
            static_cast<GDBusConnectionFlags>(G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_CLIENT | G_DBUS_CONNECTION_FLAGS_MESSAGE_BUS_CONNECTION),
            nullptr, nullptr, nullptr);
        g_free(address);
        if (!connection)
            return false;

        if (!kwinIsRunning())
            return false;

        context = g_main_context_new();
        g_main_context_push_thread_default(context);
        GDBusNodeInfo* node = g_dbus_node_info_new_for_xml(
            "<node><interface name='org.libtas.Cursor'><method name='pos'>"
            "<arg type='i' direction='in'/><arg type='i' direction='in'/>"
            "</method></interface></node>", nullptr);
        static const GDBusInterfaceVTable vtable = {handleCall, nullptr, nullptr};
        registration = g_dbus_connection_register_object(connection, "/org/libtas/Cursor",
            node->interfaces[0], &vtable, this, nullptr, nullptr);
        g_dbus_node_info_unref(node);
        g_main_context_pop_thread_default(context);
        if (!registration)
            return false;

        return loadScript();
    }

    bool query(int& x, int& y) override
    {
        while (g_main_context_iteration(context, FALSE));

        if (!known)
            return false;

        /* KWin has read the script, don't leave it behind if we crash */
        if (!script_path.empty()) {
            unlink(script_path.c_str());
            script_path.clear();
        }

        x = last_x;
        y = last_y;
        return true;
    }

private:
    bool kwinIsRunning()
    {
        GVariant* reply = g_dbus_connection_call_sync(connection, "org.freedesktop.DBus",
            "/org/freedesktop/DBus", "org.freedesktop.DBus", "NameHasOwner",
            g_variant_new("(s)", "org.kde.KWin"), G_VARIANT_TYPE("(b)"),
            G_DBUS_CALL_FLAGS_NONE, 2000, nullptr, nullptr);
        if (!reply)
            return false;
        gboolean running = FALSE;
        g_variant_get(reply, "(b)", &running);
        g_variant_unref(reply);
        return running;
    }

    GVariant* callScripting(const char* method, GVariant* parameters)
    {
        return g_dbus_connection_call_sync(connection, "org.kde.KWin", "/Scripting",
            "org.kde.kwin.Scripting", method, parameters, nullptr,
            G_DBUS_CALL_FLAGS_NONE, 2000, nullptr, nullptr);
    }

    bool loadScript()
    {
        std::string script =
            "function report() {\n"
            "    var pos = workspace.cursorPos;\n"
            "    callDBus(\"" + std::string(g_dbus_connection_get_unique_name(connection)) + "\", "
            "\"/org/libtas/Cursor\", \"org.libtas.Cursor\", \"pos\", pos.x, pos.y);\n"
            "}\n"
            "workspace.cursorPosChanged.connect(report);\n"
            "report();\n";

        gchar* path = nullptr;
        int fd = g_file_open_tmp("libtas-kwin-XXXXXX.js", &path, nullptr);
        if (fd < 0)
            return false;
        script_path = path;
        g_free(path);
        bool written = write(fd, script.data(), script.size()) == static_cast<ssize_t>(script.size());
        close(fd);
        if (!written)
            return false;

        plugin_name = "libtas-cursor-" + std::to_string(getpid());
        GVariant* reply = callScripting("loadScript", g_variant_new("(ss)", script_path.c_str(), plugin_name.c_str()));
        if (!reply)
            return false;
        gint32 id = -1;
        g_variant_get_child(reply, 0, "i", &id);
        g_variant_unref(reply);
        if (id < 0)
            return false;
        script_loaded = true;

        reply = callScripting("start", nullptr);
        if (!reply)
            return false;
        g_variant_unref(reply);
        return true;
    }

    static void handleCall(GDBusConnection*, const gchar*, const gchar*, const gchar*, const gchar*,
        GVariant* parameters, GDBusMethodInvocation* invocation, gpointer data)
    {
        auto* self = static_cast<KWinPointer*>(data);
        g_variant_get(parameters, "(ii)", &self->last_x, &self->last_y);
        self->known = true;
        g_dbus_method_invocation_return_value(invocation, nullptr);
    }

    GDBusConnection* connection = nullptr;
    GMainContext* context = nullptr;
    guint registration = 0;
    std::string script_path;
    std::string plugin_name;
    bool script_loaded = false;
    bool known = false;
    int last_x = 0, last_y = 0;
};

std::unique_ptr<GlobalPointer> createKWinPointer()
{
    auto pointer = std::make_unique<KWinPointer>();
    if (!pointer->connect())
        return nullptr;
    return pointer;
}

#endif
