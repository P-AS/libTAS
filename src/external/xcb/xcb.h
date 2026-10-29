/*
 * Stub header for xcb/xcb.h - macOS compatibility layer
 * Provides stub types and macros for XCB
 */

#ifndef __xcb_xcb_h_
#define __xcb_xcb_h_

#include <stdint.h>
#include <stddef.h>

/* Connection */
typedef struct xcb_connection_t xcb_connection_t;

/* Cookie types for asynchronous requests */
typedef struct {
    unsigned sequence;
} xcb_void_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_query_extension_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_list_fonts_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_get_font_metrics_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_list_fonts_with_info_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_create_window_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_configure_window_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_change_window_attributes_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_get_window_attributes_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_query_tree_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_list_children_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_map_window_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_unmap_window_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_get_geometry_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_query_tree_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_get_atom_name_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_change_property_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_delete_property_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_get_property_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_list_properties_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_set_selection_owner_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_get_selection_owner_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_grab_keyboard_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_ungrab_keyboard_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_grab_button_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_ungrab_button_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_query_pointer_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_warp_pointer_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_get_keyboard_mapping_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_set_keyboard_mapping_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_change_keyboard_mapping_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_get_motion_events_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_translate_coordinates_cookie_t;

typedef struct {
    unsigned sequence;
} xcb_query_keymap_cookie_t;

/* Reply types */
typedef struct {
    uint8_t response_type;
    uint8_t keys[16];
    uint8_t pad0[58];
} xcb_query_keymap_reply_t;

typedef struct {
    uint8_t response_type;
    uint8_t motion_history[4];
    uint8_t pad0[58];
} xcb_query_pointer_reply_t;

/* XCB window type */
typedef uint32_t xcb_window_t;
typedef uint32_t xcb_atom_t;
typedef uint32_t xcb_visualid_t;
typedef uint32_t xcb_gcontext_t;
typedef uint32_t xcb_cursor_t;
typedef uint32_t xcb_colormap_t;
typedef uint32_t xcb_pixmap_t;
typedef uint32_t xcb_drawable_t;
typedef uint32_t xcb_font_t;
typedef uint32_t xcb_keycode_t;
typedef uint32_t xcb_timestamp_t;

/* XCB constants */
#define XCB_NONE 0
#define XCB_COPY_FROM_PARENT 0
#define XCB_CURRENT_TIME 0
#define XCB_NO_SYMBOL 0

/* XCB GrabStatus */
#define XCB_GRAB_STATUS_SUCCESS 0
#define XCB_GRAB_STATUS_ALREADY_GRABBED 1
#define XCB_GRAB_STATUS_INVALID_TIME 2
#define XCB_GRAB_STATUS_NOT_VIEWABLE 3
#define XCB_GRAB_STATUS_FROZEN 4

/* XCB Event types */
#define XCB_KEY_PRESS 2
#define XCB_KEY_RELEASE 3
#define XCB_BUTTON_PRESS 4
#define XCB_BUTTON_RELEASE 5
#define XCB_MOTION_NOTIFY 6
#define XCB_ENTER_NOTIFY 7
#define XCB_LEAVE_NOTIFY 8
#define XCB_FOCUS_IN 9
#define XCB_FOCUS_OUT 10
#define XCB_KEYMAP_NOTIFY 11
#define XCB_EXPOSE 12
#define XCB_GRAPHICS_EXPOSE 13
#define XCB_NO_EXPOSE 14
#define XCB_VISIBILITY_NOTIFY 15
#define XCB_CREATE_NOTIFY 16
#define XCB_DESTROY_NOTIFY 17
#define XCB_UNMAP_NOTIFY 18
#define XCB_MAP_NOTIFY 19
#define XCB_MAP_REQUEST 20
#define XCB_REPARENT_NOTIFY 21
#define XCB_CONFIGURE_NOTIFY 22
#define XCB_CONFIGURE_REQUEST 23
#define XCB_GRAVITY_NOTIFY 24
#define XCB_RESIZE_REQUEST 25
#define XCB_CIRCULATE_NOTIFY 26
#define XCB_CIRCULATE_REQUEST 27
#define XCB_PROPERTY_NOTIFY 28
#define XCB_SELECTION_CLEAR 29
#define XCB_SELECTION_REQUEST 30
#define XCB_SELECTION_NOTIFY 31
#define XCB_COLORMAP_NOTIFY 32
#define XCB_CLIENT_MESSAGE 33
#define XCB_MAPPING_NOTIFY 34

/* XCB KeyPress/Release event */
typedef struct {
    uint8_t response_type;
    uint8_t detail;
    uint16_t sequence;
    uint32_t time;
    xcb_window_t root;
    xcb_window_t event;
    xcb_window_t child;
    int16_t event_x;
    int16_t event_y;
    int16_t root_x;
    int16_t root_y;
    uint16_t state;
    uint8_t mode;
    uint8_t pad0[2];
} xcb_key_press_event_t;

typedef struct {
    uint8_t response_type;
    uint8_t detail;
    uint16_t sequence;
    uint32_t time;
    xcb_window_t root;
    xcb_window_t event;
    xcb_window_t child;
    int16_t event_x;
    int16_t event_y;
    int16_t root_x;
    int16_t root_y;
    uint16_t state;
    uint8_t mode;
    uint8_t pad0[2];
} xcb_key_release_event_t;

/* XCB Button event */
typedef struct {
    uint8_t response_type;
    uint8_t detail;
    uint16_t sequence;
    uint32_t time;
    xcb_window_t root;
    xcb_window_t event;
    xcb_window_t child;
    int16_t event_x;
    int16_t event_y;
    int16_t root_x;
    int16_t root_y;
    uint16_t state;
    uint8_t mode;
    uint8_t pad0[2];
} xcb_button_press_event_t;

typedef struct {
    uint8_t response_type;
    uint8_t detail;
    uint16_t sequence;
    uint32_t time;
    xcb_window_t root;
    xcb_window_t event;
    xcb_window_t child;
    int16_t event_x;
    int16_t event_y;
    int16_t root_x;
    int16_t root_y;
    uint16_t state;
    uint8_t mode;
    uint8_t pad0[2];
} xcb_button_release_event_t;

/* XCB Motion event */
typedef struct {
    uint8_t response_type;
    uint8_t detail;
    uint16_t sequence;
    uint32_t time;
    xcb_window_t root;
    xcb_window_t event;
    xcb_window_t child;
    int16_t event_x;
    int16_t event_y;
    int16_t root_x;
    int16_t root_y;
    uint16_t state;
    uint8_t mode;
    uint8_t same_screen;
    uint8_t pad0;
} xcb_motion_notify_event_t;

/* XCB EnterNotify event */
typedef struct {
    uint8_t response_type;
    uint8_t detail;
    uint16_t sequence;
    uint32_t time;
    xcb_window_t root;
    xcb_window_t event;
    xcb_window_t child;
    int16_t event_x;
    int16_t event_y;
    int16_t root_x;
    int16_t root_y;
    uint16_t state;
    uint8_t mode;
    uint8_t focus;
    uint8_t pad0;
} xcb_enter_notify_event_t;

/* XCB Generic event */
typedef struct {
    uint8_t response_type;
    uint8_t extension;
    uint16_t sequence;
    uint32_t length;
} xcb_generic_event_t;

/* XCB Error types */
typedef struct {
    uint8_t response_type;
    uint8_t error_code;
    uint16_t sequence;
    uint32_t resource_id;
    uint32_t sequence_number;
    uint8_t major_opcode;
    uint8_t minor_opcode;
    uint8_t value_id;
    uint8_t pad0[16];
} xcb_generic_error_t;

/* XCB request opcodes */
#define XCB_QUERY_KEYMAP 7
#define XCB_GRAB_KEYBOARD 19
#define XCB_UNGRAB_KEYBOARD 20
#define XCB_GRAB_KEY 21
#define XCB_UNGRAB_KEY 22
#define XCB_QUERY_POINTER 31
#define XCB_WARP_POINTER 37
#define XCB_GRAB_POINTER 39
#define XCB_UNGRAB_POINTER 40

/* Helper macros */
#define xcb_generate_id(c) ((c)->generate_id((c)))

#endif /* __xcb_xcb_h_ */
