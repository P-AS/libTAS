/*
 * Stub header for xcb/xcbext.h - macOS compatibility layer
 * Provides stub XCB extension and generic event extras
 */

#ifndef __xcbext_h_
#define __xcbext_h_

#include <stdint.h>

/* XCB extension descriptor */
typedef struct {
    const char *name;
    int major_opcode;
    int first_event;
    int first_error;
} xcb_extension_t;

/* XCB generic event extras */
typedef struct {
    uint8_t type;
    uint8_t extension;
    uint16_t sequence;
    uint32_t length;
} xcb_generic_event_t;

/* XCB extension variable (external declaration) */
extern xcb_extension_t xcb_generic_id;

#endif /* __xcbext_h_ */
