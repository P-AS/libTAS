/*
 * Stub header for X11/X.h - macOS compatibility layer
 * Provides stub types and constants for X11 X.h
 */

#ifndef _X_H_
#define _X_H_

#include <X11/Xlib.h>

/* Basic types */
#ifndef _XCONCAT
#define _XCONCAT(a, b) a##b
#endif

/* Boolean */
typedef int Bool;
#define False 0
#define True 1

/* Status */
typedef int Status;
#define Success 0
#define BadRequest 1
#define BadImplementation 2

/* XID base types */
typedef unsigned long XID;
typedef unsigned long Mask;
typedef XID Window;
typedef XID Drawable;
typedef XID Pixmap;
typedef XID Cursor;
typedef XID Colormap;
typedef XID GContext;
typedef XID KeySym;
typedef unsigned char KeyCode;
typedef XID Atom;
typedef XID Font;

/* Visual and Depth */
typedef struct {
    int depth;
    int rgb_bits;
} VisualDepth;

/* Screen structure */
typedef struct _Screen {
    int root;
    int width, height;
    int widthMM, heightMM;
    int rootdepth;
} Screen;

/* Display structure */
typedef struct _XDisplay Display;

/* XValue: min, max, precision */
#define USBase 0
#define XSBase 1
#define YSBase 2
#define USPosition 3
#define XSPosition 4
#define YSPosition 5

/* Window class constants */
#define InputOutput 1
#define InputOnly 2
#define CopyFromParent 0
#define AnyPropertyType 0L
#define AnyKey 0
#define AnyButton 0
#define None 0L
#define ParentRelative 1L
#define ReplaceProperty 0
#define SetModeReplace 0
#define SetModeInsert 1
#define SetModePrepend 2
#define SetModeAppend 3

/* Error codes */
#define BadLength 9
#define BadMatch 8
#define BadValue 2
#define BadDrawable 10
#define BadWindow 11
#define BadAtom 14
#define BadAccess 9
#define BadAlloc 15

/* Request codes */
#define XC_NO_OPERATION 0

/* Graphics mask bits */
#define GCFunction 0x00000000L
#define GCFillStyle 0x00000002L
#define GCFillRule 0x00000004L
#define GCTile 0x00000008L
#define GCStipple 0x00000010L
#define GCTileStipXOrigin 0x00000020L
#define GCTileStipYOrigin 0x00000040L
#define GCFont 0x00000080L
#define GCSubwindowMode 0x00000100L
#define GCClipXOrigin 0x00000200L
#define GCClipYOrigin 0x00000400L
#define GCClipMask 0x00000800L
#define GCDashList 0x00001000L
#define GCDashOffset 0x00002000L
#define GCArcMode 0x00004000L
#define GCLinestyle 0x00008000L
#define GCCapStyle 0x00010000L
#define GCJoinStyle 0x00020000L
#define GCSaveUnder 0x00080000L
#define GCBackground 0x00001000L
#define GCCursor 0x00040000L
#define GCChangeGC 1
#define GCCopy 2

/* Window configuration window event mask bits */
#define CWBackPixel 0x00000000L
#define CWBorderPixel 0x00000002L
#define CWBitGravity 0x00000004L
#define CWWinGravity 0x00000008L
#define CWBackingStore 0x00000010L
#define CWBackingPlanes 0x00000020L
#define CWBackingPixel 0x00000040L
#define CWOverrideRedirect 0x00000080L
#define CWSaveUnder 0x00000100L
#define CWEventMask 0x00000200L
#define CWDontPropagate 0x00000400L
#define CWColormap 0x00000800L
#define CWCursor 0x00001000L

/* Event mask bits */
#define NoEventMask 0L
#define KeyPressMask 0x00000001L
#define KeyReleaseMask 0x00000002L
#define ButtonPressMask 0x00000004L
#define ButtonReleaseMask 0x00000008L
#define EnterWindowMask 0x00000010L
#define LeaveWindowMask 0x00000020L
#define PointerMotionMask 0x00000040L
#define PointerMotionHintMask 0x00000080L
#define Button1MotionMask 0x00000100L
#define Button2MotionMask 0x00000200L
#define Button3MotionMask 0x00000400L
#define Button4MotionMask 0x00000800L
#define Button5MotionMask 0x00001000L
#define ButtonMotionMask 0x00002000L
#define KeymapStateMask 0x00004000L
#define ExposureMask 0x00008000L
#define VisibilityChangeMask 0x00010000L
#define StructureNotifyMask 0x00020000L
#define ResizeRedirectMask 0x00040000L
#define SubstructureNotifyMask 0x00080000L
#define SubstructureRedirectMask 0x00100000L
#define FocusChangeMask 0x00200000L
#define PropertyChangeMask 0x00400000L
#define ColormapChangeMask 0x00800000L
#define OwnerGrabButtonMask 0x01000000L

/* Property mode */
#define PropModeReplace 0
#define PropModePrepend 1
#define PropModeAppend 2

/* Visual class */
#define StaticColor 0
#define PseudoColor 1
#define TrueColor 2
#define DirectColor 3
#define StaticGray 4
#define GrayScale 5

/* Fill style */
#define FillSolid 0
#define FillTiled 1
#define FillStippled 2
#define FillOpaqueStippled 3

/* Fill rule */
#define EvenOddRule 0
#define WindingRule 1

/* Line style */
#define LineSolid 0
#define LineOnOffDash 1
#define LineDoubleDash 2

/* Cap style */
#define CapNotLast 0
#define CapButt 1
#define CapRound 2
#define CapProjecting 3

/* Join style */
#define JoinMiter 0
#define JoinRound 1
#define JoinBevel 2

/* Arc mode */
#define ArcChord 0
#define ArcPieSlice 1

/* Gravity constants */
#define ForgetGravity 0
#define WestGravity 1
#define EastGravity 2
#define NorthGravity 3
#define SouthGravity 4
#define NorthWestGravity 5
#define NorthEastGravity 6
#define SouthWestGravity 7
#define SouthEastGravity 8
#define CenterGravity 9
#define StaticGravity 10

/* Visibility constants */
#define VisibilityUnobscured 0
#define VisibilityPartiallyObscured 1
#define VisibilityFullyObscured 2

/* Place constant */
#define PlaceOnTop 0
#define PlaceOnBottom 1

/* Color set state */
#define ColormapCreated 0
#define ColormapUnCreated 1

/* Attribute change values */
#define ChangeGC 0
#define CopyGC 1
#define ChangeGCMask 0x00000000L

/* Input focus model */
#define PointerRoot 0
#define RootWindow 1

/* Focus model */
#define FocusInModelPointer 0
#define FocusInModelNonlinear 1
#define FocusInModelNonlinearVirtual 2
#define FocusOutModelPointer 0
#define FocusOutModelNonlinear 1
#define FocusOutModelNonlinearVirtual 2

/* Focus return mode */
#define ReturnModePointer 0
#define ReturnModeRoot 1
#define ReturnModeNone 2

/* Window stacking mode */
#define Above 0
#define Below 1
#define TopIf 2
#define BottomIf 3
#define Opposite 4

/* Window stacking context */
#define RevertToNone 0
#define RevertToParent 1
#define RevertToPointerRoot 2

/* Cursor definitions */
#define CursorCursor 0
#define CursorInput 1

/* Window background pixel */
#define UseCursor 0
#define UseSeed 1

/* Keysym shifts */
#define ShiftMapMask 0
#define LockMapMask 1
#define ControlMapMask 2
#define Mod1MapMask 3
#define Mod2MapMask 4
#define Mod3MapMask 5
#define Mod4MapMask 6
#define Mod5MapMask 7

/* XLookupStatus return values */
#define XLookupPosition 0
#define XLookupString 1
#define XLookupKeySym 2
#define XLookupBoth 3
#define XLookupChars 4

/* Keysym behavior */
#define XkbIM_NoFlags 0
#define XkbIM_AutoRepeat 1

/* Lock behavior */
#define XkbLC_NoFlags 0
#define XkbLC_AutoReload 1

/* XkbOptions */
#define XkbBS_NoFlags 0
#define XkbBS_Warn 1

/* Lock action */
#define XkbLA_NoFlags 0
#define XkbLA_AutoReload 1
#define XkbLA_AutoMask 2

/* Grab mode */
#define GrabModeAsync 0
#define GrabModeSync 1

/* Grab status */
#define GrabSuccess 0
#define GrabAlreadyGrabbed 1
#define GrabInvalidTime 2
#define GrabNotViewable 3
#define GrabFrozen 4

#define AllowBoth 0
#define AsyncPointer 1
#define SyncPointer 2
#define ReplayPointer 3
#define AsyncKeyboard 4
#define SyncKeyboard 5
#define ReplayKeyboard 6
#define AsyncBoth 7
#define SyncBoth 8

/* Button state */
#define Button1Mask 0x00000100L
#define Button2Mask 0x00000200L
#define Button3Mask 0x00000400L
#define Button4Mask 0x00000800L
#define Button5Mask 0x00001000L

/* Time */
typedef unsigned long Time;

/* Timestamp */
typedef unsigned long Timestamp;

/* Selection request types */
#define InheritSelection 0

/* ICCCM WM_STATE constants */
#define ICCCMWithdrawnState 0
#define ICCCMIconicState 1
#define ICCCMNormalState 2

#endif /* _X_H_ */
