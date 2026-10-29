/*
 * Stub header for X11/Xlib.h - macOS compatibility layer
 * Provides stub function declarations for X11 Xlib
 */

#ifndef _XLIB_H_
#define _XLIB_H_

#include <X11/X.h>
#include <stdarg.h>
#include <stddef.h>

/* XEvent structure declarations */
typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;
    Display *display;
    int extension;
    int evtype;
} XAnyEvent;

typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;
    Display *display;
    Window window;
    int x, y;
    int x_root, y_root;
    unsigned int state;
    int keycode;
    Bool same_screen;
} XKeyEvent;

typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;
    Display *display;
    Window window;
    int x, y;
    int x_root, y_root;
    unsigned int state;
    int button;
    Bool same_screen;
} XButtonEvent;

typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;
    Display *display;
    Window window;
    int x, y;
    int x_root, y_root;
    unsigned int state;
    int detail;
    Bool same_screen;
} XMotionEvent;

typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;
    Display *display;
    Drawable drawable;
    int x, y;
    int width, height;
    int count;
    int major_code;
    int minor_code;
} XExposeEvent;

typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;
    Display *display;
    Drawable drawable;
    int x, y;
    int width, height;
    int major_code;
    int minor_code;
    int count;
    int drawable_major_code;
    int drawable_minor_code;
} XGraphicsExposeEvent;

typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;
    Display *display;
    Drawable drawable;
    int major_code;
    int minor_code;
} XNoExposeEvent;

typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;
    Display *display;
    Window window;
    int width, height;
    int border_width;
    int mode;
} XConfigureEvent;

typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;
    Display *display;
    Window window;
    Bool override_redirect;
} XUnmapEvent;

typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;
    Display *display;
    Window window;
    Atom atom;
    Time time;
    int state;
} XPropertyEvent;

typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;
    Display *display;
    Window window;
    Atom selection;
    Atom target;
    Atom property;
    Time time;
} XClientMessageEvent;

typedef struct {
    unsigned char type;
    unsigned char request;
    unsigned short sequence;
    unsigned long resourceid;
    unsigned long serial;
    unsigned char error_code;
    unsigned char major_opcode;
    unsigned char minor_opcode;
    unsigned int data[3];
} XErrorEvent;

typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;
    Display *display;
    int extension;
    int evtype;
} XGenericEvent;

typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;
    Display *display;
    int extension;
    int evtype;
    void *data;
} XGenericEventCookie;

typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;
    Display *display;
    Window window;
    long length;
    char *data;
} XClientMessageEvent;

typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;
    Display *display;
    Window window;
    Atom selection;
    Window target;
    Window requestor;
    Atom property;
    Time time;
} XSelectionRequestEvent;

typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;
    Display *display;
    Window requestor;
    Atom selection;
    Atom target;
    Atom property;
    Time time;
} XSelectionClearEvent;

typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;
    Display *display;
    Window requestor;
    Atom selection;
    Atom target;
    Atom property;
    Time time;
} XSelectionEvent;

typedef struct {
    int type;
    int detail;
    unsigned long serial;
    Bool send_event;
    Display *display;
    Window event;
    Window focus;
    int mode;
} XFocusChangeEvent;

typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;
    Display *display;
    Window window;
    int state;
} XVisibilityEvent;

typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;
    Display *display;
    Window window;
    int subclass_event;
    int state;
} XCreateWindowEvent;

typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;
    Display *display;
    Window event;
    Window child;
    int x, y;
    unsigned int width, height;
    int border_width;
    Bool override_redirect;
} XMapRequestEvent;

typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;
    Display *display;
    Window window;
    int state;
} XMapEvent;

typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;
    Display *display;
    Window window;
    Bool from_configure;
} XReparentEvent;

typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;
    Display *display;
    Window window;
    unsigned int state;
} XUnmapEvent;

typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;
    Display *display;
    Window window;
    Bool override_redirect;
    Visual *visual;
    unsigned int x, y;
    unsigned int width, height;
    unsigned int border_width;
    int depth;
} XWindowAttributes;

typedef struct {
    Pixmap background_pixmap;
    unsigned long background_pixel;
    unsigned long border_pixel;
    Pixmap border_pixmap;
    int border_width;
    int depth;
    int visual;
    Window bit_gravity;
    Window win_gravity;
    int backing_store;
    unsigned long backing_planes;
    unsigned long backing_pixel;
    Bool save_under;
    long event_mask;
    long do_not_propagate_mask;
    Bool override_redirect;
    Colormap colormap;
    Cursor cursor;
} XSetWindowAttributes;

typedef struct {
    unsigned char *value;
    int format;
    long nitems;
    Atom type;
    long bytes_after;
    unsigned long mask;
} XTextProperty;

typedef struct {
    unsigned long flags;
    Cursor cursor;
    Pixel fore, back;
    Pixel normal_fore, normal_back;
    Pixel light_fore, light_back;
    Pixel dark_fore, dark_back;
    char *string;
    char *auto_repeat_mode;
} XWMHints;

typedef struct {
    char *res_name;
    char *res_class;
} XClassHint;

typedef struct _XComposeStatus {
    char *input_buffer;
    int input_index;
    int input_chars;
    int input_length;
    char *buffer_start;
} XComposeStatus;

typedef struct {
    int x, y;
    unsigned int width, height;
    int border_width;
    int depth;
} XWindowChanges;

typedef struct _XIC *XIC;
typedef struct _XIM *IM;
typedef int Status;

typedef struct _XIMStyles *XIMStyles;
typedef struct _XV* XV;

/* XGCValues structure */
typedef struct {
    GCFunction function;
    unsigned long function;
    GCFillStyle fill_style;
    int fill_style;
    int fill_rule;
    int tile_stip_x_orig;
    int tile_stip_y_orig;
    int clip_x_orig;
    int clip_y_orig;
    Mask clip_mask;
    int dash_offset;
    unsigned char dashes;
    int arc_mode;
    int linestyle;
    int cap_style;
    int join_style;
    Pixmap tile;
    Pixmap stipple;
    int font;
    int subwindow_mode;
    Bool graphics_exposures;
    int clip_mask;
    int clip_x;
    int clip_y;
    int arc_mode;
    unsigned long background;
    int foreground;
    unsigned long alpha;
} XGCValues;

/* XPoint, XRectangle, XArc structures */
typedef struct {
    int x, y;
} XPoint;

typedef struct {
    int x, y;
    unsigned int width, height;
} XRectangle;

typedef struct {
    short x, y;
    unsigned short width, height;
    int angle1, angle2;
} XArc;

/* XSizeHints */
typedef struct {
    long x, y;
    unsigned int width, height;
    unsigned min_width, min_height;
    unsigned max_width, max_height;
    unsigned width_inc, height_inc;
    struct {
        int min, max;
    } min_aspect, max_aspect;
    int base_width, base_height;
    int win_gravity;
} XSizeHints;

/* XEvent union */
typedef union _XEvent {
    int type;
    XAnyEvent xany;
    XKeyEvent xkey;
    XButtonEvent xbutton;
    XMotionEvent xmotion;
    XExposeEvent xexpose;
    XGraphicsExposeEvent xgraphicsexpose;
    XNoExposeEvent xnoexpose;
    XConfigureEvent xconfigure;
    XUnmapEvent xunmap;
    XMapEvent xmap;
    XPropertyEvent xproperty;
    XClientMessageEvent xclient;
    XSelectionRequestEvent xselectionrequest;
    XSelectionClearEvent xselectionclear;
    XSelectionEvent xselection;
    XErrorEvent xerror;
    XMapRequestEvent xmaprequest;
    XReparentEvent xreparent;
    XFocusChangeEvent xfocus;
    XVisibilityEvent xvisibility;
    XCreateWindowEvent xcreatewindow;
    XGenericEvent xgeneric;
    XGenericEventCookie xcookie;
    long pad[24];
} XEvent;

/* XPointer for internal use */
typedef void *XPointer;

/* XIM/XIC types */
typedef char *XPointer;

/* Display macros */
#define DefaultScreen(dpy) 0
#define DefaultRootWindow(dpy) ((Window)1)
#define DefaultVisual(dpy, s) ((Visual*)1)
#define DefaultGC(dpy, s) ((GContext)1)
#define DisplayWidth(dpy, s) 800
#define DisplayHeight(dpy, s) 600
#define DisplayWidthMM(dpy, s) 200
#define DisplayHeightMM(dpy, s) 150
#define DefaultScreenOfDisplay(dpy) ((Screen*)1)
#define ScreenOfDisplay(dpy, s) ((Screen*)1)
#define DefaultColormap(dpy, s) ((Colormap)1)
#define BlackPixel(dpy, s) 0L
#define WhitePixel(dpy, s) 1L
#define AllPlanes (~0UL)
#define ConnectionNumber(dpy) 0

/* Function declarations */
extern Display *XOpenDisplay(const char *display_name);
extern int XCloseDisplay(Display *display);
extern int XFlush(Display *display);
extern int XSync(Display *display, Bool discard);
extern int XSetErrorHandler(int (*handler)(Display *, XErrorEvent *));
extern int XSetIOErrorHandler(int (*handler)(Display *));

extern int XNextEvent(Display *display, XEvent *event);
extern int XPeekEvent(Display *display, XEvent *event);
extern int XWindowEvent(Display *display, Window w, long emask, XEvent *event);
extern int XCheckWindowEvent(Display *display, Window w, long emask, XEvent *event);
extern int XMaskEvent(Display *display, long emask, XEvent *event);
extern int XCheckMaskEvent(Display *display, long emask, XEvent *event);
extern int XCheckTypedEvent(Display *display, int evtp, XEvent *event);
extern int XSendEvent(Display *display, Window w, Bool propagate, long event_mask, XEvent *event);
extern Bool XFilterEvent(XEvent *event, Window w);
extern int XPending(Display *display);
extern Bool XIfEvent(Display *display, XEvent *event, Bool (*predicate)(Display *, XEvent *, XPointer), XPointer arg);
extern Bool XCheckIfEvent(Display *display, XEvent *event, Bool (*predicate)(Display *, X Event *, XPointer), XPointer arg);
extern int XEventsQueued(Display *display, int mode);
extern void XGetEventData(Display *display, XGenericEventCookie *cookie);
extern void XFreeEventData(Display *display, XGenericEventCookie *cookie);
extern int XGetConnectionNumber(Display *dpy);

/* Window functions */
extern Window XCreateWindow(Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int vclass, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes);
extern Window XCreateSimpleWindow(Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, unsigned long border, unsigned long background);
extern int XDestroyWindow(Display *display, Window w);
extern int XUnmapWindow(Display *display, Window w);
extern int XMapWindow(Display *display, Window w);
extern int XMapRaised(Display *display, Window w);
extern int XStoreName(Display *display, Window w, const char *window_name);
extern void XSetWMName(Display *display, Window w, XTextProperty *text_prop);
extern int XSetWMProperties(Display *display, Window w, XTextProperty *window_name, XTextProperty *icon_name, char **argv, int argc, XSizeHints *hints, XWMHints *wm_hints, XClassHint *class_hints);
extern int XSelectInput(Display *display, Window w, long event_mask);
extern int XMoveWindow(Display *display, Window w, int x, int y);
extern int XResizeWindow(Display *display, Window w, unsigned int width, unsigned int height);
extern int XMoveResizeWindow(Display *display, Window w, int x, int y, unsigned int width, unsigned int height);
extern int XConfigureWindow(Display *display, Window w, unsigned int value_mask, XWindowChanges *values);
extern int XGetWindowProperty(Display *display, Window w, Atom property, long long_offset, long long_length, Bool del, Atom req_type, Atom *actual_type_return, int *actual_format_return, unsigned long *nitems_return, unsigned long *bytes_after_return, unsigned char **prop_return);
extern int XChangeProperty(Display *display, Window w, Atom property, Atom type, int format, int mode, const unsigned char *data, int nelements);
extern int XGetWindowAttributes(Display *display, Window w, XWindowAttributes *window_attributes_return);
extern int XChangeWindowAttributes(Display *display, Window w, unsigned long valuemask, XSetWindowAttributes *attributes);
extern int XSetWMHints(Display *display, Window w, XWMHints *wm_hints);
extern Bool XTranslateCoordinates(Display *display, Window src_w, Window dest_w, int src_x, int src_y, int *dest_x_return, int *dest_y_return, Window *child_return);
extern Bool XQueryExtension(Display *display, const char *name, int *major_opcode_return, int *first_event_return, int *first_error_return);
extern Atom XInternAtom(Display *display, const char *atom_name, Bool only_if_exists);
extern char *XFetchName(Display *display, Window w, char **name_return);
extern int XGetTransientForHint(Display *display, Window w, Window *prop_window_return);

/* Cursor functions */
extern int XDefineCursor(Display *display, Window w, Cursor cursor);
extern int XUndefineCursor(Display *display, Window w);
extern Cursor XcursorLibraryLoadCursor(Display *display, const char *name);
extern void XFreeCursor(Display *display, Cursor cursor);
extern Cursor XCreatePixmapCursor(Display *display, Pixmap source, Pixmap mask, XColor *foreground, XColor *background, int x, int y);

/* String/atom functions */
extern int Xutf8TextPerCharExtents(Display *display, const char *string, int nbytes, unsigned int *ascent_return, unsigned int *descent_return, unsigned int *max_width_return);
extern int Xutf8StringLengthForWidth(Display *display, const char *string, int nbytes, int width);
extern long XSelectEvent(Display *display, Window w, long event_mask);

/* Keysym functions */
extern KeySym XKeycodeToKeysym(Display *display, KeyCode key, int index);
extern KeySym XKeysymToKeycode(Display *display, KeySym keysym);
extern KeySym XkbKeycodeToKeysym(Display *display, KeyCode key, int index, int group);
extern KeySym *XGetKeyboardMapping(Display *display, KeyCode first_keycode, int keycode_count, int *keysyms_per_keycode_return);
extern int XLookupString(XKeyEvent *event, char *buffer, int nbytes, KeySym *keysym, XComposeStatus *status);
extern int XmbLookupString(XIC xic, XKeyEvent *event, char *buffer, int nbytes, KeySym *keysym, Status *status);
extern int XwcLookupString(XIC xic, XKeyEvent *event, wchar_t *buffer, int nbytes, KeySym *keysym, Status *status);
extern int Xutf8LookupString(XIC xic, XKeyEvent *event, char *buffer, int nbytes, KeySym *keysym, Status *status);

/* XIM/XIC functions */
extern XIC XCreateIC(XIM xim, ...);
extern void XDestroyIC(XIC xic);
extern int XSetICValues(XIC xic, ...);
extern int XGetICValues(XIC xic, ...);
extern Status XFilterKeyEvent(XKeyEvent *event);

/* Xlibint functions */
extern void XLockDisplay(Display *display);
extern void XUnlockDisplay(Display *display);
extern void *Xmalloc(size_t size);

/* XTextProperty functions */
extern int XStringListToTextProperty(char **list, int count, XTextProperty *text_prop);
extern int XmbTextPropertyToTextList(XTextProperty *text_prop, char ***list_return);
extern int Xutf8TextPropertyToTextList(XTextProperty *text_prop, char ***list_return);
extern int XmbTextListToTextProperty(char **list, int count, int order, XTextProperty *text_prop);
extern int Xutf8TextListToTextProperty(char **list, int count, int order, XTextProperty *text_prop);
extern void XFree(XPointer ptr);
extern void XFreeTextProperty(XTextProperty *text_prop);
extern void XmuFree(char *ptr);
extern int XmbLookupString(XIC xic, XKeyEvent *event, char *buffer, int nbytes, KeySym *keysym, Status *status);

/* XShm functions (forward-declare XShmSegmentInfo which is in XShm.h) */
typedef struct _XShmSegmentInfo XShmSegmentInfo;
extern int XShmPutImage(Display *display, Drawable d, GC gc, XImage *image, int src_x, int src_y, int dest_x, int dest_y, unsigned int width, unsigned int height, Bool always_consumed);

/* XRender forward declarations */
typedef struct _XRenderPictFormat XRenderPictFormat;
typedef struct _XRenderPicture Picture;

/* Xinerama forward declarations */
typedef struct _XineramaScreenInfo XineramaScreenInfo;
extern int XineramaQueryScreens(Display *display, int *number_of_screens);

/* Xrandr forward declarations */
typedef struct _XRRScreenResources XRRScreenResources;
typedef struct _XRRCrtcGamma XRRCrtcGamma;
typedef struct _XRROutputInfo XRROutputInfo;
typedef struct _XRRCrtcInfo XRRCrtcInfo;
extern XRRScreenResources *XRRGetScreenResources(Display *display, Window window);
extern XRRCrtcGamma *XRRGetCrtcGamma(Display *display, RRCrtc crtc);
extern int XRRSetCrtcGamma(Display *display, RRCrtc crtc, XRRCrtcGamma *gamma);
extern void XRRFreeCrtcGamma(XRRCrtcGamma *gamma);
extern XRRScreenResources *XRRGetScreenResourcesCurrent(Display *display, Window window);
extern void XRRFreeScreenResources(XRRScreenResources *resources);
extern XRROutputInfo *XRRGetOutputInfo(Display *display, XRRScreenResources *resources, RROutput output);
extern void XRRFreeOutputInfo(XRROutputInfo *info);
extern XRRCrtcInfo *XRRGetCrtcInfo(Display *display, XRRScreenResources *resources, RRCrtc crtc);
extern void XRRFreeCrtcInfo(XRRCrtcInfo *info);
extern Atom *XRRListOutputProperties(Display *display, RROutput output, int *nprops);
extern int XRRSetCrtcConfig(Display *display, XRRScreenResources *resources, RRCrtc crtc, Time timestamp, int x, int y, RRMode mode, Rotation rotation, RROutput *outputs, int noutputs);
extern int XRRSetScreenConfig(Display *display, XRRScreenConfiguration *config, Window draw, SizeID sizeindex, Rotation rotation, Time timestamp);
extern int XRRSetScreenSize(Display *display, Window window, int width, int height, int mwidth, int mheight);
extern XRRCrtcTransformAttributes *XRRGetCrtcTransform(Display *display, RRCrtc crtc, int *ninput_params, int **input_params);

/* XF86VidMode forward declarations */
extern int XF86VidModeGetModeLine(Display *display, int screen, XF86VidModeModeLine *modeline, XF86VidModeMonitor *monptr);
extern int XF86VidModeGetAllModeLines(Display *display, int screen, int *nmode_lines, XF86VidModeModeInfo ***modelines);

/* XFixes forward declarations */
typedef XID PointerBarrier;
extern PointerBarrier XFixesCreatePointerBarrier(Display *dpy, Window w, int x1, int y1, int x2, int y2, int directions, int num_devices, int *devices);
extern void XFixesDestroyPointerBarrier(Display *dpy, PointerBarrier b);

/* Xlib-xcb.h - XGetXCBConnection macro */
#include <xcb/xcb.h>
static inline xcb_connection_t *XGetXCBConnection(Display *dpy) {
    return (xcb_connection_t *)dpy;
}

/* XKB forward declarations */
extern Bool XkbUseExtension(Display *display, unsigned int *major_version_return, unsigned int *minor_version_return);
extern int XkbQueryExtension(Display *display, unsigned int *codes_major_op_return, unsigned int *codes_minor_op_return, int *extension_event_return, int *extension_error_return, int flags);
extern Bool XkbSelectEventDetails(Display *dpy, unsigned int which, unsigned int detail, unsigned long mask, unsigned long details);

/* Visual type */
typedef struct {
    int depth;
} Visual;

/* XColor */
typedef struct {
    unsigned long pixel;
    unsigned short red, green, blue;
    unsigned char flags;
    unsigned char pad;
} XColor;

/* XImage */
typedef struct _XImage {
    int width, height;
    int xoffset;
    int format;
    char *data;
    int byte_order;
    int bitmap_unit;
    int bitmap_bit_order;
    int bitmap_pad;
    int depth;
    int bytes_per_line;
    int bits_per_pixel;
    unsigned long red_mask, green_mask, blue_mask;
    void *obdata;
    struct _XFuncs {
        int (*get_image)();
        int (*put_image)();
        int (*destroy)();
        int (*set_res)();
    } *functions;
    int (*update)();
} XImage;

#endif /* _XLIB_H_ */
