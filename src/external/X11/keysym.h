/*
 * Stub header for X11/keysym.h - macOS compatibility layer
 * Provides stub XK_* keysym constants
 */

#ifndef _KEYSYMS_H_
#define _KEYSYMS_H_

#include <X11/X.h>

/* XK_ keysym constants (common ones used by libTAS) */
#define XK_VoidSymbol 0xffffff00UL

/* ASCII */
#define XK_space 0x0020
#define XK_exclam 0x0021
#define XK_quotedbl 0x0022
#define XK_numbersign 0x0023
#define XK_dollar 0x0024
#define XK_percent 0x0025
#define XK_ampersand 0x0026
#define XK_apostrophe 0x0027
#define XK_quoteright 0x0027
#define XK_parenleft 0x0028
#define XK_parenright 0x0029
#define XK_asterisk 0x002a
#define XK_plus 0x002b
#define XK_comma 0x002c
#define XK_minus 0x002d
#define XK_period 0x002e
#define XK_slash 0x002f
#define XK_0 0x0030
#define XK_1 0x0031
#define XK_2 0x0032
#define XK_3 0x0033
#define XK_4 0x0034
#define XK_5 0x0035
#define XK_6 0x0036
#define XK_7 0x0037
#define XK_8 0x0038
#define XK_9 0x0039
#define XK_colon 0x003a
#define XK_semicolon 0x003b
#define XK_less 0x003c
#define XK_equal 0x003d
#define XK_greater 0x003e
#define XK_question 0x003f
#define XK_at 0x0040
#define XK_A 0x0041
#define XK_B 0x0042
#define XK_C 0x0043
#define XK_D 0x0044
#define XK_E 0x0045
#define XK_F 0x0046
#define XK_G 0x0047
#define XK_H 0x0048
#define XK_I 0x0049
#define XK_J 0x004a
#define XK_K 0x004b
#define XK_L 0x004c
#define XK_M 0x004d
#define XK_N 0x004e
#define XK_O 0x004f
#define XK_P 0x0050
#define XK_Q 0x0051
#define XK_R 0x0052
#define XK_S 0x0053
#define XK_T 0x0054
#define XK_U 0x0055
#define XK_V 0x0056
#define XK_W 0x0057
#define XK_X 0x0058
#define XK_Y 0x0059
#define XK_Z 0x005a
#define XK_bracketleft 0x005b
#define XK_backslash 0x005c
#define XK_bracketright 0x005d
#define XK_underscore 0x005f
#define XK_grave 0x0060
#define XK_a 0x0061
#define XK_b 0x0062
#define XK_c 0x0063
#define XK_d 0x0064
#define XK_e 0x0065
#define XK_f 0x0066
#define XK_g 0x0067
#define XK_h 0x0068
#define XK_i 0x0069
#define XK_j 0x006a
#define XK_k 0x006b
#define XK_l 0x006c
#define XK_m 0x006d
#define XK_n 0x006e
#define XK_o 0x006f
#define XK_p 0x0070
#define XK_q 0x0071
#define XK_r 0x0072
#define XK_s 0x0073
#define XK_t 0x0074
#define XK_u 0x0075
#define XK_v 0x0076
#define XK_w 0x0077
#define XK_x 0x0078
#define XK_y 0x0079
#define XK_z 0x007a
#define XK_braceleft 0x007b
#define XK_bar 0x007c
#define XK_braceright 0x007d
#define XK_asciitilde 0x007e

/* Function keys */
#define XK_Escape 0xff1b
#define XK_F1 0xffbe
#define XK_F2 0xffbf
#define XK_F3 0xffc0
#define XK_F4 0xffc1
#define XK_F5 0xffc2
#define XK_F6 0xffc3
#define XK_F7 0xffc4
#define XK_F8 0xffc5
#define XK_F9 0xffc6
#define XK_F10 0xffc7
#define XK_F11 0xffc8
#define XK_F12 0xffc9
#define XK_F13 0xffca
#define XK_F14 0xffcb
#define XK_F15 0xffcc
#define XK_F16 0xffcd
#define XK_F17 0xffce
#define XK_F18 0xffcf
#define XK_F19 0xffd0
#define XK_F20 0xffd1

/* Control keys */
#define XK_Shift_L 0xffe1
#define XK_Shift_R 0xffe2
#define XK_Control_L 0xffe3
#define XK_Control_R 0xffe4
#define XK_Caps_Lock 0xffe5
#define XK_Shift_Lock 0xffe6
#define XK_Meta_L 0xffe7
#define XK_Meta_R 0xffe8
#define XK_Alt_L 0xffe9
#define XK_Alt_R 0xffea
#define XK_Super_L 0xffeb
#define XK_Super_R 0xffec
#define XK_Hyper_L 0xffed
#define XK_Hyper_R 0xffee

/* Navigation keys */
#define XK_Insert 0xff63
#define XK_Delete 0xffff
#define XK_Home 0xff50
#define XK_End 0xff57
#define XK_Page_Up 0xff55
#define XK_Page_Down 0xff56
#define XK_Up 0xff52
#define XK_Down 0xff54
#define XK_Left 0xff51
#define XK_Right 0xff53

/* Keypad */
#define XK_KP_Space 0x20
#define XK_KP_Tab 0x09
#define XK_KP_Enter 0xff0d
#define XK_KP_F1 0xffbe
#define XK_KP_F2 0xffbf
#define XK_KP_F3 0xffc0
#define XK_KP_F4 0xffc1
#define XK_KP_Home 0xff95
#define XK_KP_Left 0xff96
#define XK_KP_Up 0xff97
#define XK_KP_Right 0xff98
#define XK_KP_Down 0xff99
#define XK_KP_Page_Up 0xff9a
#define XK_KP_Page_Down 0xff9b
#define XK_KP_End 0xff9c
#define XK_KP_Begin 0xff9d
#define XK_KP_Start 0xff9d
#define XK_KP_Insert 0xff9e
#define XK_KP_Delete 0xff9f
#define XK_KP_Equal 0x003d
#define XK_KP_Multiply 0x002a
#define XK_KP_Add 0x002b
#define XK_KP_Separator 0x002c
#define XK_KP_Subtract 0x002d
#define XK_KP_Decimal 0x002e
#define XK_KP_Divide 0x002f
#define XK_KP_0 0x0030
#define XK_KP_1 0x0031
#define XK_KP_2 0x0032
#define XK_KP_3 0x0033
#define XK_KP_4 0x0034
#define XK_KP_5 0x0035
#define XK_KP_6 0x0036
#define XK_KP_7 0x0037
#define XK_KP_8 0x0038
#define XK_KP_9 0x0039

/* Special keys */
#define XK_BackSpace 0xff08
#define XK_Tab 0xff09
#define XK_Linefeed 0xff0a
#define XK_Clear 0xff0b
#define XK_Return 0xff0d
#define XK_Pause 0xff13
#define XK_Scroll_Lock 0xff14
#define XK_Sys_Req 0xff15
#define XK_Cancel 0xff17
#define XK_Help 0xff15
#define XK_BackSpace 0xff08
#define XK_Backspace 0xff08
#define XK_Menu 0xff67
#define XK_Print 0xff61
#define XK_Execute 0xff62
#define XK_Select 0xff60
#define XK_LastKey 0xfffd

/* Mode keys */
#define XK_Mode_switch 0xff7e
#define XK_KeySymbol 0xff7f

/* Dead keys */
#define XK_dead_grave 0x0100
#define XK_dead_acute 0x0101
#define XK_dead_circumflex 0x0102
#define XK_dead_tilde 0x0103
#define XK_dead_macron 0x0104
#define XK_dead_breve 0x0105
#define XK_dead_abovedot 0x0106
#define XK_dead_diaeresis 0x0107
#define XK_dead_abovering 0x0108
#define XK_dead_doubleacute 0x0109
#define XK_dead_caron 0x010a
#define XK_dead_cedilla 0x010b
#define XK_dead_ogonek 0x010c
#define XK_dead_iota 0x010d
#define XK_dead_voiced_sound 0x010e
#define XK_dead_semivoiced_sound 0x010f
#define XK_dead_belowdot 0x0110
#define XK_dead_hook 0x0111
#define XK_dead_horn 0x0112

#endif /* _KEYSYMS_H_ */
