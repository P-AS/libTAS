/*
 * Stub header for X11/Xlibint.h - macOS compatibility layer
 * Provides stub declarations for X11 internal Xlib functions
 */

#ifndef _XLIBINT_H_
#define _XLIBINT_H_

#include <X11/Xlib.h>
#include <stddef.h>

/* Display locking */
extern void XLockDisplay(Display *display);
extern void XUnlockDisplay(Display *display);

/* Memory allocation */
extern void *Xmalloc(size_t size);
extern void *Xcalloc(size_t nmemb, size_t size);
extern void *Xrealloc(void *ptr, size_t size);

#endif /* _XLIBINT_H_ */
