/*
 * Stub header for X11/XKBlib.h - macOS compatibility layer
 * Provides stub XKB types and function declarations
 */

#ifndef _XKBLIB_H_
#define _XKBLIB_H_

#include <X11/Xlib.h>
#include <X11/X.h>

/* XKB key types */
#define XkbUseCoreKbd 0

/* XKB types */
typedef unsigned int XkbDescPtr;
typedef unsigned int XkbClientMapPtr;
typedef unsigned int XkbServerMapPtr;
typedef unsigned int XkbKeyTypePtr;

/* XKB definitions */
#define XKB_KEY_Alphabetic 0x00010001
#define XkbNKeyTypes 8

/* XKB key type names */
#define XkbSIMPLE_TYPE_NAME "SINGLE"
#define XkbLVL1_TYPE_NAME "ONE_LEVEL"
#define XkbALPHABETIC_TYPE_NAME "ALPHABETIC"
#define XkbVENDOR_TYPE_BASE "VENDOR"

/* XKB functions */
extern Bool XkbUseExtension(Display *display, unsigned int *major_version_return, unsigned int *minor_version_return);
extern int XkbQueryExtension(Display *display, unsigned int *codes_major_op_return, unsigned int *codes_minor_op_return, int *extension_event_return, int *extension_error_return, int flags);
extern Bool XkbSelectEventDetails(Display *dpy, unsigned int which, unsigned int detail, unsigned long mask, unsigned long details);

/* XKB keysym functions */
extern KeySym XkbKeycodeToKeysym(Display *display, KeyCode key, int index, int group);

/* XKB lookup functions */
extern int XkbLibraryKeyCodeForKeySym(Display *dpy, KeySym keysym);

#endif /* _XKBLIB_H_ */
