/////////////////////////////////////////////////////////
//                                                     //
// TinyImageLoader                                     //
//                                                     //
// Example: OpenGL Texture                             //
//                                                     //
// Shows how to use TinyImageLoader to load image data //
// into an OpenGL texture.                             //
//                                                     //
/////////////////////////////////////////////////////////

#include "Framework.h"

#include "TinyImageLoader.h"

namespace TILFW
{

#ifndef DOXYGEN_SHOULD_SKIP_THIS

	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>

	// We overload the new operator so that we get 
	// information of the leak's whereabouts (line and file)

	#ifdef _DEBUG
		#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
		#define new DEBUG_NEW
	#endif

	#include <windows.h>
	#include <direct.h>
	#include <sys\stat.h>

	_CrtMemState g_MemStart, g_MemEnd, g_MemDiff;

	void MyError(til::MessageData* a_Data)
	{
		MessageBoxA(NULL, a_Data->message, "TinyImageLoader - Tests", MB_OK | MB_ICONERROR);
	}

#endif

	void Framework::Setup()
	{
		s_WindowWidth = 640;
		s_WindowHeight = 480;
	}

	void Framework::Init(const char** a_CommandLine, int a_Commands)
	{
		// Initialize TinyImageLoader

		til::TIL_Init();

		til::TIL_SetErrorFunc(MyError);

		til::Image* load_bmp = til::TIL_Load("media\\BMP\\concrete.bmp", TIL_FILE_ADDWORKINGDIR | TIL_DEPTH_A8B8G8R8);
		delete load_bmp;

		til::Image* load_png = til::TIL_Load("media\\PNG\\avatar.png", TIL_FILE_ADDWORKINGDIR | TIL_DEPTH_A8B8G8R8);
		delete load_png;

		til::Image* load_gif = til::TIL_Load("media\\GIF\\rolypolypandap1.gif", TIL_FILE_ADDWORKINGDIR | TIL_DEPTH_A8B8G8R8);
		delete load_gif;

		til::Image* load_ico = til::TIL_Load("media\\ICO\\d8eba2bcc1af567ce8f596f3005980dadd13f704.ico", TIL_FILE_ADDWORKINGDIR | TIL_DEPTH_A8B8G8R8);
		delete load_ico;

		til::Image* load_tga = til::TIL_Load("media\\TGA\\earth.tga", TIL_FILE_ADDWORKINGDIR | TIL_DEPTH_A8B8G8R8);
		delete load_tga;

		til::TIL_ShutDown();

		_CrtMemCheckpoint(&g_MemEnd);

		if (_CrtMemDifference(&g_MemDiff, &g_MemStart, &g_MemEnd))
		{
			_CrtDumpMemoryLeaks();

			MessageBoxA(NULL, "Memory leaks detected!", "TinyImageLoader - Tests", MB_OK | MB_ICONERROR);

			return;
		}

		MessageBoxA(NULL, "Rejoice! No memory leaks detected.", "TinyImageLoader - Tests", MB_OK | MB_ICONINFORMATION);
	}

	void Framework::Tick(float a_DT)
	{

	}

	void Framework::Render()
	{
		
	}

};

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	TILFW::Framework* app = new TILFW::Framework;

	_CrtMemCheckpoint(&TILFW::g_MemStart);

	app->Exec(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	delete app;

	return 0;
}