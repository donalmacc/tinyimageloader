#ifndef _TBSETTINGS_H_
#define _TBSETTINGS_H_

// =========================================
// Version
// =========================================

#define TB_VERSION_MAJOR			2
#define TB_VERSION_MINOR			0
#define TB_VERSION_BUGS				0

// =========================================
// Timer
// =========================================

#define TB_TIMER_PRECISION_HIGH	    0
#define TB_TIMER_PRECISION_NORMAL   1
#define TB_TIMER_PRECISION_LOW      2

// #define TB_TIMER_PRECISION       TB_TIMER_PRECISION_NORMAL

// =========================================
// Input
// =========================================

#define TB_INPUT_SDL	0
#define TB_INPUT_WIN32	1
#define TB_INPUT_GLUT	2
 
// #define TB_INPUT		TB_INPUT_SDL

// =========================================
// Color
// =========================================

// types

#define TB_COLOR_DEPTH_A8R8G8B8           0
#define TB_COLOR_DEPTH_R8G8B8             1
#define TB_COLOR_DEPTH_R5G6B5             2

// 16-bit colors

#define TB_COLOR_TYPE_R5G6B5              unsigned short

#define TB_COLOR_TYPE_R5G6B5_SHIFT        4
#define TB_COLOR_TYPE_R5G6B5_SHIFT_A      0
#define TB_COLOR_TYPE_R5G6B5_SHIFT_R      8
#define TB_COLOR_TYPE_R5G6B5_SHIFT_G      4
#define TB_COLOR_TYPE_R5G6B5_SHIFT_B      0

#define TB_COLOR_TYPE_R5G6B5_BITMASK_A    0x00
#define TB_COLOR_TYPE_R5G6B5_BITMASK_R    0xF8
#define TB_COLOR_TYPE_R5G6B5_BITMASK_G    0x7E
#define TB_COLOR_TYPE_R5G6B5_BITMASK_B    0x1F

#define TB_COLOR_TYPE_R5G6B5_MASK_A       0x0000
#define TB_COLOR_TYPE_R5G6B5_MASK_C       0xFFFF
#define TB_COLOR_TYPE_R5G6B5_MASK_R       0xF800
#define TB_COLOR_TYPE_R5G6B5_MASK_G       0x07E0
#define TB_COLOR_TYPE_R5G6B5_MASK_B       0x001F
#define TB_COLOR_TYPE_R5G6B5_MASK_RB      0xF81F
#define TB_COLOR_TYPE_R5G6B5_MASK_AG      0x07E0

// 32-bit colors

#define TB_COLOR_TYPE_A8R8G8B8            unsigned long

#define TB_COLOR_TYPE_A8R8G8B8_SHIFT      8
#define TB_COLOR_TYPE_A8R8G8B8_SHIFT_A    24
#define TB_COLOR_TYPE_A8R8G8B8_SHIFT_R    16
#define TB_COLOR_TYPE_A8R8G8B8_SHIFT_G    8
#define TB_COLOR_TYPE_A8R8G8B8_SHIFT_B    0

#define TB_COLOR_TYPE_A8R8G8B8_BITMASK_A  0xFF
#define TB_COLOR_TYPE_A8R8G8B8_BITMASK_R  0xFF
#define TB_COLOR_TYPE_A8R8G8B8_BITMASK_G  0xFF
#define TB_COLOR_TYPE_A8R8G8B8_BITMASK_B  0xFF

#define TB_COLOR_TYPE_A8R8G8B8_MASK_A     0xFF000000
#define TB_COLOR_TYPE_A8R8G8B8_MASK_C     0x00FFFFFF
#define TB_COLOR_TYPE_A8R8G8B8_MASK_R     0x00FF0000
#define TB_COLOR_TYPE_A8R8G8B8_MASK_G     0x0000FF00
#define TB_COLOR_TYPE_A8R8G8B8_MASK_B     0x000000FF
#define TB_COLOR_TYPE_A8R8G8B8_MASK_RB    0x00FF00FF
#define TB_COLOR_TYPE_A8R8G8B8_MASK_AG    0xFF00FF00

#define TB_COLOR_TYPE_R8G8B8              unsigned long

#define TB_COLOR_TYPE_R8G8B8_SHIFT        8
#define TB_COLOR_TYPE_R8G8B8_SHIFT_A      24
#define TB_COLOR_TYPE_R8G8B8_SHIFT_R      16
#define TB_COLOR_TYPE_R8G8B8_SHIFT_G      8
#define TB_COLOR_TYPE_R8G8B8_SHIFT_B      0

#define TB_COLOR_TYPE_R8G8B8_BITMASK_A    0x00
#define TB_COLOR_TYPE_R8G8B8_BITMASK_R    0xFF
#define TB_COLOR_TYPE_R8G8B8_BITMASK_G    0xFF
#define TB_COLOR_TYPE_R8G8B8_BITMASK_B    0xFF

#define TB_COLOR_TYPE_R8G8B8_MASK_A       0x00000000
#define TB_COLOR_TYPE_R8G8B8_MASK_C       0x00FFFFFF
#define TB_COLOR_TYPE_R8G8B8_MASK_R       0x00FF0000
#define TB_COLOR_TYPE_R8G8B8_MASK_G       0x0000FF00
#define TB_COLOR_TYPE_R8G8B8_MASK_B       0x000000FF
#define TB_COLOR_TYPE_R8G8B8_MASK_RB      0x00FF00FF
#define TB_COLOR_TYPE_R8G8B8_MASK_AG      0x0000FF00

// #define TB_COLOR_DEPTH                     TB_COLOR_DEPTH_R8G8B8

#endif