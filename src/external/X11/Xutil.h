/*
 * Stub header for X11/Xutil.h - macOS compatibility layer
 * Provides stub declarations for X11 utility functions
 */

#ifndef _XUTIL_H_
#define _XUTIL_H_

#include <X11/Xlib.h>

/* XTextProperty */
typedef struct {
    unsigned char *value;
    int format;
    long nitems;
    Atom type;
    long bytes_after;
} XTextProperty;

/* XSizeHints */
typedef struct {
    long x, y;
    unsigned int width, height;
    int min_width, min_height;
    int max_width, max_height;
    int width_inc, height_inc;
    struct {
        int min, max;
    } min_aspect, max_aspect;
    int base_width, base_height;
    int win_gravity;
} XSizeHints;

/* XWMHints */
#define InputHint (1L << 0)
#define StateHint (1L << 1)
#define IconPixmapHint (1L << 2)
#define IconWindowHint (1L << 3)
#define IconPositionHint (1L << 4)
#define IconMaskHint (1L << 5)
#define WindowGroupHint (1L << 6)
#define AllHints (InputHint|StateHint|IconPixmapHint|IconWindowHint|IconPositionHint|IconMaskHint|WindowGroupHint)
#define XNumberHints (1L << 7)
#define PSize (1L << 0)
#define PPosition (1L << 1)
#define PMinSize (1L << 2)
#define PMaxSize (1L << 3)
#define PMask (PSize|PPosition|PMinSize|PMaxSize)

typedef struct {
    unsigned long flags;
    int input;
    int initial_state;
    Pixmap icon_pixmap;
    Window icon_window;
    int icon_x, icon_y;
    Pixmap icon_mask;
    Window window_group;
} XWMHints;

/* XClassHint */
typedef struct {
    char *res_name;
    char *res_class;
} XClassHint;

/* XFontStruct */
typedef struct _XFontStruct XFontStruct;

/* XGCValues (forward declaration) */
typedef struct {
    int function;
    int fill_style;
    int fill_rule;
    int tile_stip_x_orig;
    int tile_stip_y_orig;
    int clip_x_orig;
    int clip_y_orig;
    GC clip_mask;
    int dash_offset;
    unsigned char dashes;
    int arc_mode;
    int line_width;
    int line_style;
    int cap_style;
    int join_style;
    Pixmap tile;
    Pixmap stipple;
    int mask;
    int subwindow_mode;
    Bool graphics_exposures;
    int clip_x;
    int clip_y;
    int foreground;
    int background;
    int tile_pixmap;
} XGCValues;

/* Functions */
extern void XSetWMName(Display *display, Window w, XTextProperty *text_prop);
extern void XSetWMIconName(Display *display, Window w, XTextProperty *text_prop);
extern int XSetWMProperties(Display *display, Window w, XTextProperty *window_name, XTextProperty *icon_name, char **argv, int argc, XSizeHints *hints, XWMHints *wm_hints, XClassHint *class_hints);
extern void XGetWMName(Display *display, Window w, XTextProperty *text_prop);
extern void XGetWMIconName(Display *display, Window w, XTextProperty *text_prop);
extern XWMHints *XGetWMHints(Display *display, Window w);
extern int XSetWMHints(Display *display, Window w, XWMHints *wm_hints);
extern int XSetClassHint(Display *display, Window w, XClassHint *class_hints);
extern XClassHint *XGetClassHint(Display *display, Window w, XClassHint *class_hints);
extern int XStringListToTextProperty(char **list, int count, XTextProperty *text_prop);
extern int XmbTextPropertyToTextList(XTextProperty *text_prop, char ***list_return);
extern int Xutf8TextPropertyToTextList(XTextProperty *text_prop, char ***list_return);
extern int XmbTextListToTextProperty(char **list, int count, int order, XTextProperty *text_prop);
extern int Xutf8TextListToTextProperty(char **list, int count, int order, XTextProperty *text_prop);
extern XFontStruct *XLoadQueryFont(Display *display, const char *fontname);
extern void XFreeFont(Display *display, XFontStruct *fontstruct);
extern int XFreeFontNames(char **fontname_list);
extern char **XListFonts(Display *display, const char *pattern, int nmax, int *actual_count_return);
extern char **XListFontsWithInfo(Display *display, const char *pattern, int nmax, int *actual_count_return);
extern void XFreeStringList(char **stringlist);
extern Status Xutf8TextExtents(XFontStruct *fontstruct, const char *string, int nbytes, int *direction, int *chars_fetched, int *bytes_fetched, XRectangle *overall_ink, XRectangle *overall_logical);
extern void XLookupString(XKeyEvent *event, char *buffer, int nbytes, KeySym *keysym, XComposeStatus *status);
extern void XSetStandardProperties(Display *display, Window w, const char *title, const char *icon_name, Pixmap pixmap, char **argv, int argc, int window_hints);
extern Status XStringToTextProperty(const char *string, XTextProperty *text_prop);
extern void XFree(XPointer ptr);

#endif /* _XUTIL_H_ */
