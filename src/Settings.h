// Code by:
// Quinten Lansu (knight666)
// Copyright 2008-2009

#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#pragma warning (disable : 4530) // complaint about exception handler
#pragma warning (disable : 4273)
#pragma warning (disable : 4311) // pointer truncation from HANDLE to long

#define APPNAME		"Template"

#define SCRWIDTH	800
#define SCRHEIGHT	600
#define SCRRATIO	(float)(SCRWIDTH) / (float)(SCRHEIGHT)
#define HALFWIDTH	SCRWIDTH / 2
#define HALFHEIGHT	SCRHEIGHT / 2

//#define USE_CEGUI
#ifdef USE_CEGUI
	#define PATH_CEGUI_FONTS      "data\\GUI"
	#define PATH_CEGUI_SCHEMES    "data\\GUI"
	#define PATH_CEGUI_IMAGESETS  "data\\GUI"
	#define PATH_CEGUI_LOOKNFEEL  "data\\GUI"
#endif

#include "VersionInfo.h"

//#define BACKBUFFER

#define COLOR_24BIT		0
#define COLOR_16BIT		1

#define COLOR_DEPTH		COLOR_24BIT

#if COLOR_DEPTH == COLOR_24BIT

	#define MASK_RED	0xFF0000
	#define MASK_GREEN	0x00FF00
	#define MASK_BLUE	0x0000FF

	#define SHIFT_RED	16
	#define SHIFT_GREEN	8
	#define SHIFT_BLUE	0

#elif COLOR_DEPTH == COLOR_16BIT

	#define MASK_RED	0xFF00
	#define MASK_GREEN	0x00FF
	#define MASK_BLUE	0x0000

#endif

#define INPUT_SDL
//#define INPUT_WIN32

//#define TIMER_PRECISION_HIGH
#define TIMER_PRECISION_LOW

#define LOG_SIZE	1024

#define LIMIT_FPS		60
#ifdef LIMIT_FPS

const float ONEFRAME =	(1000.f / (float)LIMIT_FPS);

#endif

struct Settings
{
	static bool _simd;
	static bool _timers;
};

#endif