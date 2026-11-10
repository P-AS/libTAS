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

#ifndef LIBTAS_HAS_WAYLAND_POINTER

std::unique_ptr<GlobalPointer> createImageCapturePointer()
{
    return nullptr;
}

#else

#include <cstring>
#include <vector>
#include <poll.h>
#include <wayland-client.h>
#include "ext-image-capture-source-v1-client-protocol.h"
#include "ext-image-copy-capture-v1-client-protocol.h"
#include "xdg-output-unstable-v1-client-protocol.h"

/* The ext-image-copy-capture protocol has "cursor sessions" that report where the
 * pointer is on what is captured, without capturing any image. A capture source
 * of a window only reports positions inside that window, so we use the outputs. */
class ImageCapturePointer : public GlobalPointer {
public:
    ~ImageCapturePointer() override
    {
        if (display)
            wl_display_disconnect(display);
    }

    const char* name() const override { return "ext-image-copy-capture"; }

    bool connect()
    {
        display = wl_display_connect(nullptr);
        if (!display)
            return false;

        wl_registry* registry = wl_display_get_registry(display);
        wl_registry_add_listener(registry, &registry_listener, this);
        wl_display_roundtrip(display);

        if (!copy_manager || !source_manager || !seat)
            return false;

        pointer_device = wl_seat_get_pointer(seat);
        for (Output* output : outputs)
            startSession(output);
        connected = true;

        wl_display_roundtrip(display);
        return wl_display_get_error(display) == 0;
    }

    bool query(int& x, int& y) override
    {
        /* Read what the compositor sent, without waiting for anything */
        while (wl_display_prepare_read(display) != 0)
            wl_display_dispatch_pending(display);
        wl_display_flush(display);

        pollfd pfd = {wl_display_get_fd(display), POLLIN, 0};
        if (poll(&pfd, 1, 0) > 0)
            wl_display_read_events(display);
        else
            wl_display_cancel_read(display);
        wl_display_dispatch_pending(display);

        if (wl_display_get_error(display) != 0)
            return false;

        if (!known)
            return false;
        x = last_x;
        y = last_y;
        return true;
    }

private:
    struct Output {
        ImageCapturePointer* parent;
        wl_output* output;
        int transform = 0;
        int mode_width = 0, mode_height = 0;
        int logical_x = 0, logical_y = 0;
        int logical_width = 0, logical_height = 0;
    };

    void startSession(Output* output)
    {
        if (xdg_output_manager) {
            zxdg_output_v1* xdg_output = zxdg_output_manager_v1_get_xdg_output(xdg_output_manager, output->output);
            zxdg_output_v1_add_listener(xdg_output, &xdg_output_listener, output);
        }
        ext_image_capture_source_v1* source = ext_output_image_capture_source_manager_v1_create_source(source_manager, output->output);
        ext_image_copy_capture_cursor_session_v1* session = ext_image_copy_capture_manager_v1_create_pointer_cursor_session(copy_manager, source, pointer_device);
        ext_image_copy_capture_cursor_session_v1_add_listener(session, &session_listener, output);
    }

    /* Positions are in pixels of the output, relative to it */
    void updatePosition(const Output* output, int x, int y)
    {
        /* The size of the mode is before the transform of the output */
        bool swapped = output->transform & 1;
        int width = swapped ? output->mode_height : output->mode_width;
        int height = swapped ? output->mode_width : output->mode_height;

        last_x = output->logical_x + ((width > 0 && output->logical_width > 0) ? (x * output->logical_width / width) : x);
        last_y = output->logical_y + ((height > 0 && output->logical_height > 0) ? (y * output->logical_height / height) : y);
        known = true;
    }

    static void handleGlobal(void* data, wl_registry* registry, uint32_t id, const char* interface, uint32_t version)
    {
        auto* self = static_cast<ImageCapturePointer*>(data);
        if (!std::strcmp(interface, ext_image_copy_capture_manager_v1_interface.name)) {
            self->copy_manager = static_cast<ext_image_copy_capture_manager_v1*>(wl_registry_bind(registry, id, &ext_image_copy_capture_manager_v1_interface, 1));
        }
        else if (!std::strcmp(interface, ext_output_image_capture_source_manager_v1_interface.name)) {
            self->source_manager = static_cast<ext_output_image_capture_source_manager_v1*>(wl_registry_bind(registry, id, &ext_output_image_capture_source_manager_v1_interface, 1));
        }
        else if (!std::strcmp(interface, zxdg_output_manager_v1_interface.name)) {
            self->xdg_output_manager = static_cast<zxdg_output_manager_v1*>(wl_registry_bind(registry, id, &zxdg_output_manager_v1_interface, version < 2 ? version : 2));
        }
        else if (!std::strcmp(interface, wl_seat_interface.name) && !self->seat) {
            self->seat = static_cast<wl_seat*>(wl_registry_bind(registry, id, &wl_seat_interface, 1));
        }
        else if (!std::strcmp(interface, wl_output_interface.name)) {
            Output* output = new Output{self, static_cast<wl_output*>(wl_registry_bind(registry, id, &wl_output_interface, 1))};
            wl_output_add_listener(output->output, &output_listener, output);
            self->outputs.push_back(output);
            if (self->connected)
                self->startSession(output);
        }
    }
    static void handleGlobalRemove(void*, wl_registry*, uint32_t) {}

    static void handleEnter(void*, ext_image_copy_capture_cursor_session_v1*) {}
    static void handleLeave(void*, ext_image_copy_capture_cursor_session_v1*) {}
    static void handlePosition(void* data, ext_image_copy_capture_cursor_session_v1*, int32_t x, int32_t y)
    {
        auto* output = static_cast<Output*>(data);
        output->parent->updatePosition(output, x, y);
    }
    static void handleHotspot(void*, ext_image_copy_capture_cursor_session_v1*, int32_t, int32_t) {}

    static void handleGeometry(void* data, wl_output*, int32_t, int32_t, int32_t, int32_t, int32_t, const char*, const char*, int32_t transform)
    {
        static_cast<Output*>(data)->transform = transform;
    }
    static void handleMode(void* data, wl_output*, uint32_t flags, int32_t width, int32_t height, int32_t)
    {
        if (flags & WL_OUTPUT_MODE_CURRENT) {
            static_cast<Output*>(data)->mode_width = width;
            static_cast<Output*>(data)->mode_height = height;
        }
    }
    static void handleOutputDone(void*, wl_output*) {}
    static void handleOutputScale(void*, wl_output*, int32_t) {}

    static void handleLogicalPosition(void* data, zxdg_output_v1*, int32_t x, int32_t y)
    {
        static_cast<Output*>(data)->logical_x = x;
        static_cast<Output*>(data)->logical_y = y;
    }
    static void handleLogicalSize(void* data, zxdg_output_v1*, int32_t width, int32_t height)
    {
        static_cast<Output*>(data)->logical_width = width;
        static_cast<Output*>(data)->logical_height = height;
    }
    static void handleXdgDone(void*, zxdg_output_v1*) {}
    static void handleXdgName(void*, zxdg_output_v1*, const char*) {}
    static void handleXdgDescription(void*, zxdg_output_v1*, const char*) {}

    static constexpr wl_registry_listener registry_listener = {handleGlobal, handleGlobalRemove};
    static constexpr ext_image_copy_capture_cursor_session_v1_listener session_listener = {handleEnter, handleLeave, handlePosition, handleHotspot};
    static constexpr wl_output_listener output_listener = {handleGeometry, handleMode, handleOutputDone, handleOutputScale};
    static constexpr zxdg_output_v1_listener xdg_output_listener = {handleLogicalPosition, handleLogicalSize, handleXdgDone, handleXdgName, handleXdgDescription};

    wl_display* display = nullptr;
    wl_seat* seat = nullptr;
    wl_pointer* pointer_device = nullptr;
    ext_image_copy_capture_manager_v1* copy_manager = nullptr;
    ext_output_image_capture_source_manager_v1* source_manager = nullptr;
    zxdg_output_manager_v1* xdg_output_manager = nullptr;
    std::vector<Output*> outputs;
    bool connected = false;
    bool known = false;
    int last_x = 0, last_y = 0;
};

std::unique_ptr<GlobalPointer> createImageCapturePointer()
{
    auto pointer = std::make_unique<ImageCapturePointer>();
    if (!pointer->connect())
        return nullptr;
    return pointer;
}

#endif
