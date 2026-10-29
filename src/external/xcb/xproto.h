/*
 * Stub header for xcb/xproto.h - macOS compatibility layer
 * Provides stub XCB protocol types and constants
 */

#ifndef __xproto_h_
#define __xproto_h_

#include <stdint.h>

/* XCB protocol constant */
#define XCB_REQUEST_HEADER_SIZE 32
#define XCB_REPLY_HEADER_SIZE 32
#define XCB_ERROR_HEADER_SIZE 32

/* XCB protocol request opcodes (core X11) */
#define XCB_CREATE_WINDOW 1
#define XCB_CHANGE_WINDOW_ATTRIBUTES 2
#define XCB_GET_WINDOW_ATTRIBUTES 3
#define XCB_DESTROY_WINDOW 4
#define XCB_DESTROY_WINDOWS 5
#define XCB_CHANGE_SAVE_SET 6
#define XCB_REPARENT_WINDOW 7
#define XCB_MAP_WINDOW 8
#define XCB_MAP_SUBWINDOWS 9
#define XCB_UNMAP_WINDOW 10
#define XCB_UNMAP_SUBWINDOWS 11
#define XCB_CONFIGURE_WINDOW 13
#define XCB_CIRCULATE_WINDOW 15
#define XCB_GET_GEOMETRY 16
#define XCB_QUERY_TREE 17
#define XCB_INTERNAL_ERROR 18
#define XCB_QUERY_POINTER 19
#define XCB_GET_SELECTION_OWNER 23
#define XCB_SET_SELECTION_OWNER 24
#define XCB_CHANGE_PROPERTY 18
#define XCB_DELETE_PROPERTY 19
#define XCB_GET_PROPERTY 20
#define XCB_LIST_PROPERTIES 21
#define XCB_SET_MAPPED_WINDOW 22

/* XCB keyboard/mouse opcodes */
#define XCB_GET_KEYBOARD_MAPPING 25
#define XCB_SET_KEYBOARD_MAPPING 26
#define XCB_CHANGE_KEYBOARD_MAPPING 26
#define XCB_GET_MOTION_EVENTS 27
#define XCB_TRANSLATE_COORDINATES 28
#define XCB_LIST_INPUT_DEVICES 29
#define XCB_WARP_POINTER 30
#define XCB_SET_INPUT_FOCUS 31
#define XCB_GET_INPUT_FOCUS 32
#define XCB_QUERY_KEYMAP 33
#define XCB_OPEN_FONT 34
#define XCB_CLOSE_FONT 35
#define XCB_QUERY FONT 36
#define XCB_LIST_FONTS 37
#define XCB_LIST_FONT_WITH_INFO 38
#define XCB_POLY_TEXT_8BIT 39
#define XCB_POLY_TEXT_32 40
#define XCB_POLY_SEGMENT 41
#define XCB_POLY_LINE 42
#define XCB_POLY_ARCHIVE 43
#define XCB_POLY_POINT 44
#define XCB_POLY_ARROW 45
#define XCB_POLY_FILL 46
#define XCB_POLYGON 47
#define XCB_POLY_LINE_STROKE 48
#define XCB_POLYGON_STROKE 49
#define XCB_POLY_CIRCLE 50
#define XCB_POLY_ARC 51
#define XCB_POLY_ARCHIVE 52
#define XCB_POLY_FILL_ARCHIVE 53
#define XCB_POLY_FILL_STROKE 54
#define XCB_POLY_FILL_ARCHIVE 55
#define XCB_POLY_FILL_STROKE_ARCHIVE 56
#define XCB_POLY_LINE_ARCHIVE 57
#define XCB_POLY_FILL_LINE_ARCHIVE 58
#define XCB_POLY_FILL_STROKE_LINE_ARCHIVE 59
#define XCB_POLY_FILL_STROKE_ARCHIVE 60
#define XCB_POLY_FILL_ARCHIVE 61
#define XCB_POLY_FILL_STROKE_ARCHIVE 62
#define XCB_POLY_FILL_ARCHIVE 63
#define XCB_POLY_FILL_STROKE_ARCHIVE 64
#define XCB_POLY_FILL_ARCHIVE 65
#define XCB_POLY_FILL_STROKE_ARCHIVE 66
#define XCB_POLY_FILL_ARCHIVE 67
#define XCB_POLY_FILL_STROKE_ARCHIVE 68
#define XCB_POLY_FILL_ARCHIVE 69

#define XCB_GRAB_KEYBOARD 34
#define XCB_UNGRAB_KEYBOARD 35
#define XCB_GRAB_BUTTON 36
#define XCB_UNGRAB_BUTTON 37
#define XCB_CHANGE_GRAB_BUTTON 38
#define XCB_GRAB_POINTER 39
#define XCB_UNGRAB_POINTER 40
#define XCB_CHANGE_ACTIVE_GRAB 41
#define XCB_GRAB_KEY 42
#define XCB_UNGRAB_KEY 43
#define XCB_ALLOW_KEY_EVENTS 44
#define XCB_GRAB_POINTER 45
#define XCB_UNGRAB_POINTER 46
#define XCB_ALLOW_EVENTS 47
#define XCB_GRAB_KEYBOARD 48
#define XCB_UNGRAB_KEYBOARD 49
#define XCB_GET_KEYBOARD_STATE 50
#define XCB_POLY_POINT 51
#define XCB_POLY_LINE 52
#define XCB_POLY_ARC 53
#define XCB_POLY_FILL_ARCHIVE 54
#define XCB_POLY_FILL_STROKE_ARCHIVE 55
#define XCB_POLY_FILL_ARCHIVE 56
#define XCB_POLY_FILL_STROKE_ARCHIVE 57
#define XCB_POLY_FILL_ARCHIVE 58
#define XCB_POLY_FILL_STROKE_ARCHIVE 59
#define XCB_POLY_FILL_ARCHIVE 60
#define XCB_POLY_FILL_STROKE_ARCHIVE 61
#define XCB_POLY_FILL_ARCHIVE 62
#define XCB_POLY_FILL_STROKE_ARCHIVE 63
#define XCB_POLY_FILL_ARCHIVE 64
#define XCB_POLY_FILL_STROKE_ARCHIVE 65
#define XCB_POLY_FILL_ARCHIVE 66
#define XCB_POLY_FILL_STROKE_ARCHIVE 67
#define XCB_POLY_FILL_ARCHIVE 68
#define XCB_POLY_FILL_STROKE_ARCHIVE 69

/* XCB protocol types */
typedef uint8_t xcb_void_t;
typedef uint8_t xcb_generic_request_t;
typedef uint8_t xcb_generic_reply_t;

/* XCB point */
typedef struct {
    int16_t x;
    int16_t y;
} xcb_point_t;

/* XCB rectangle */
typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
} xcb_rectangle_t;

/* XCB arc */
typedef struct {
    int16_t x;
    int16_t y;
    uint16_t width;
    uint16_t height;
    int16_t angle1;
    int16_t angle2;
} xcb_arc_t;

/* XCB atom */
typedef uint32_t xcb_atom_t;

/* XCB window */
typedef uint32_t xcb_window_t;

/* XCB pixmap */
typedef uint32_t xcb_pixmap_t;

/* XCB cursor */
typedef uint32_t xcb_cursor_t;

/* XCB font */
typedef uint32_t xcb_font_t;

/* XGC type */
typedef uint32_t xcb_gcontext_t;

/* XCB visual */
typedef uint32_t xcb_visualid_t;

/* XCB color */
typedef uint32_t xcb_colormap_t;

/* XCB client message format */
typedef struct {
    uint8_t format;
    uint8_t data[20];
} xcb_client_message_event_t;

/* XCB configure window */
typedef struct {
    uint32_t border_width;
    uint32_t sibling;
    uint32_t stack_mode;
} xcb_config_window_t;

#endif /* __xproto_h_ */
