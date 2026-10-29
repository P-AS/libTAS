/*
    Stub EGL header for macOS (Apple removed EGL support).
    Provides minimal types and declarations needed for compilation.
*/

#ifndef LIBTAS_STUB_EGL_H_INCLUDED
#define LIBTAS_STUB_EGL_H_INCLUDED

/* API entry attribute (not used on macOS) */
#define EGLAPIENTRY

#ifdef __cplusplus
extern "C" {
#endif

/* Basic EGL types */
typedef void* EGLNativeDisplayType;
typedef void* EGLNativePixmapType;
typedef void* EGLNativeWindowType;
typedef int EGLBoolean;
typedef int EGLint;

/* Handle types */
typedef struct _EGLDisplay* EGLDisplay;
typedef struct _EGLConfig* EGLConfig;
typedef struct _EGLSurface* EGLSurface;
typedef struct _EGLContext* EGLContext;

/* Enumerants - match macOS OpenGL types */
typedef unsigned int GLenum;
typedef unsigned int EGLenum;

/* Constants */
#define EGL_SUCCESS 0x3000
#define EGL_NO_CONTEXT ((EGLContext)0)
#define EGL_NO_DISPLAY ((EGLDisplay)0)
#define EGL_NO_SURFACE ((EGLSurface)0)
#define EGL_OPENGL_API 0x30A0
#define EGL_CONTEXT_MAJOR_VERSION 0x3098
#define EGL_CONTEXT_MINOR_VERSION 0x30FB
#define EGL_CONTEXT_OPENGL_PROFILE_MASK 0x30FD
#define EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT 0x00000001
#define EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT 0x00000002
#define EGL_NONE 0x3038
#define EGL_TRUE 1
#define EGL_FALSE 0

/* OpenGL ES APIs */
#define EGL_OPENGL_ES_API 0x30A0
#define EGL_OPENGL_ES2_BIT 0x0004

/* Function pointer types */
typedef void (*__eglMustCastToProperFunctionPointerType)(void);
typedef __eglMustCastToProperFunctionPointerType (EGLAPIENTRY* PFNEGLGETPROCADDRESSPROC)(const char *);

/* Stub function declarations */
static inline void* eglGetProcAddress(const char *procName) { return 0; }
static inline EGLBoolean eglMakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx) { return EGL_SUCCESS; }
static inline EGLBoolean eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) { return EGL_SUCCESS; }
static inline EGLBoolean eglSwapInterval(EGLDisplay dpy, EGLint interval) { return EGL_SUCCESS; }
static inline EGLBoolean eglBindAPI(EGLenum api) { return EGL_SUCCESS; }
static inline EGLContext eglCreateContext(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list) { return EGL_NO_CONTEXT; }

#ifdef __cplusplus
}
#endif

#endif /* LIBTAS_STUB_EGL_H_INCLUDED */
