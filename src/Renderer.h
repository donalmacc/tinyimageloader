// Template, major revision 3, beta
// IGAD/NHTV - Jacco Bikker - 2006-2009
//
// Note:
// This version of the template attempts to setup a rendering surface in system RAM
// and copies it to VRAM using DMA. On recent systems, this yields extreme performance,
// and flips are almost instant. For older systems, there is a fall-back path that
// uses a more conventional approach offered by SDL. If your system uses this, the
// window caption will indicate this. In this case, you may want to tweak the video
// mode setup code for optimal performance.
//
// Abstracted away by Quinten Lansu (knight666)

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "Includes.h"
#include "Settings.h"

#include "Input.h"

#include "surface.h"
#include "game.h"

using namespace std;
using namespace tb;

namespace Tmpl8
{

	typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);

	struct Renderer
	{
		inline static char* GetWorkingDirectory() { return workingdir; }

		static bool Init(int argc, char** argv);
		static void Run();
		static void Quit();

		static void Swap();

		static bool								running;
		static Game*							game;
		static Surface*							frontbuffer;
		static Surface*							backbuffer;
		static Pixel*							pixels;

	private:

		static PFNGLGENBUFFERSPROC				glGenBuffers;
		static PFNGLBINDBUFFERPROC				glBindBuffer;
		static PFNGLBUFFERDATAPROC				glBufferData;
		static PFNGLMAPBUFFERPROC				glMapBuffer;
		static PFNGLUNMAPBUFFERPROC				glUnmapBuffer;
		static PFNWGLSWAPINTERVALFARPROC		wglSwapIntervalEXT;
		static GLuint							fbPBO[2];
		static unsigned int						framebufferTexID[2];
		static unsigned char*					framedata;
		static LARGE_INTEGER					lasttime;
		static float							cpu_frequency;
		static bool								vbo;
		static double							lastftime;
		static LARGE_INTEGER					start;				
		static LARGE_INTEGER					end;
		static char								workingdir[_MAX_PATH];

#ifdef LIMIT_FPS

		static unsigned long					m_PhysicsCurrent, m_PhysicsStart;
		static float							m_PhysicsTime;

		static char*                            s_Arguments;

#endif

	}; // struct Renderer

}; // namespace Tmpl8

#endif