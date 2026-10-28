/* Beginning of the private Xlib data for its XCB connection
 * (struct _X11XCBPrivate), from libX11's src/Xxcbint.h, which is not
 * installed. Only the fields up to next_xid are defined, so that libTAS can
 * reach the resource id that Xlib keeps for its next allocation. The layout
 * changed between libX11 versions, so both are given and the caller must
 * check which one matches.
 *
 * Copyright (C) 2003-2006 Jamey Sharp, Josh Triplett
 * This file is licensed under the MIT license. See the file COPYING. */

#ifndef LIBTAS_XXCBINT_H
#define LIBTAS_XXCBINT_H

#include <X11/Xlib.h>
#include <xcb/xcb.h>
#include <stdint.h>

/* Layout of recent libX11 versions, with the next_response field */
typedef struct _X11XCBPrivateStart {
    xcb_connection_t *connection;
    void *pending_requests;
    void *pending_requests_tail;
    xcb_generic_event_t *next_event;
    void *next_response;
    char *real_bufmax;
    char *reply_data;
    int reply_length;
    int reply_consumed;
    uint64_t last_flushed;
    int event_owner;
    XID next_xid;
} _X11XCBPrivateStart;

/* Layout of older libX11 versions (such as 1.6), without next_response */
typedef struct _X11XCBPrivateStartOld {
    xcb_connection_t *connection;
    void *pending_requests;
    void *pending_requests_tail;
    xcb_generic_event_t *next_event;
    char *real_bufmax;
    char *reply_data;
    int reply_length;
    int reply_consumed;
    uint64_t last_flushed;
    int event_owner;
    XID next_xid;
} _X11XCBPrivateStartOld;

#endif
