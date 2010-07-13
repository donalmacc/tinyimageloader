#ifndef _INCLUDES_H_
#define _INCLUDES_H_

#pragma warning (disable : 4530) // complaint about exception handler
#pragma warning (disable : 4273)
#pragma warning (disable : 4311) // pointer truncation from HANDLE to long

#include <windows.h>

#undef	WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN

// File I/O
#include "io.h"
#include <ios>
#include <iostream>
#include <fstream>

// OpenGL
extern "C" 
{ 
	#include "glew.h" 
}
#include "gl.h"
#include "wglext.h"

// SDL
#include "SDL.h"
#include "SDL_syswm.h"

// FreeImage
#include "freeimage.h"


#include "math.h"
#include <stdlib.h>
#include "string.h"

#include "fcntl.h"		// _O_TEXT

#include <ctime>

#include <iosfwd>
#include <xiosbase>

// Toolbox
#include "TBSettings.h"

//#define TB_COLOR_DEPTH    TB_COLOR_DEPTH_R5G6B5

#include "TBFileManager.h"
#include "TBINI.h"
#include "TBColor.h"
#include "TBTimer.h"
#include "TBThread.h"
#include "TBLog.h"
#include "TBVec2.h"
#include "TBVec3.h"
#include "TBMat3x3.h"
#include "TBMat4x4.h"
#include "TBQuaternion.h"
#include "TBMath.h"

#ifdef USE_CEGUI

	// CEGUI
	#pragma warning (disable : 4250)    // warning C4250: 'class' : inherits 'class::function' via dominance
	#undef max
	#undef min
	#include "CEGUI.h"
	#include "RendererModules/OpenGL/CEGUIOpenGLRenderer.h"
	#include "RendererModules/OpenGL/CEGUIOpenGLTexture.h"
	#include "RendererModules/OpenGL/CEGUIOpenGLTextureTarget.h"

#endif

#include "Input.h"

#endif