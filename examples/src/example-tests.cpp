#include "Framework.h"

#include "TinyImageLoader.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

// We overload the new operator so that we get 
// information of the leak's whereabouts (line and file)

#ifdef _DEBUG
	#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
	#define new DEBUG_NEW
#endif

namespace TILFW
{

#ifndef DOXYGEN_SHOULD_SKIP_THIS

	#include <windows.h>
	#include <direct.h>
	#include <sys\stat.h>

	_CrtMemState g_MemStart, g_MemEnd, g_MemDiff;

	void MyError(til::MessageData* a_Data)
	{
		MessageBoxA(NULL, a_Data->message, "TinyImageLoader - Tests", MB_OK | MB_ICONERROR);
	}

	void MyDebug(til::MessageData* a_Data)
	{

	}

#endif

	void Framework::Setup()
	{
		s_WindowWidth = 640;
		s_WindowHeight = 480;
	}

	void Framework::Init(const char** a_CommandLine, int a_Commands)
	{
		_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

		_CrtMemCheckpoint(&TILFW::g_MemStart);

		// Initialize TinyImageLoader

		til::TIL_Init();

		til::Image* load_bmp = til::TIL_Load("media\\BMP\\concrete.bmp", TIL_FILE_ADDWORKINGDIR | TIL_DEPTH_A8B8G8R8);
		til::TIL_Release(load_bmp);

		til::Image* load_gif = til::TIL_Load("media\\GIF\\rolypolypandap1.gif", TIL_FILE_ADDWORKINGDIR | TIL_DEPTH_A8B8G8R8);
		til::TIL_Release(load_gif);

		til::Image* load_png = til::TIL_Load("media\\PNG\\avatar.png", TIL_FILE_ADDWORKINGDIR | TIL_DEPTH_A8B8G8R8);
		til::TIL_Release(load_png);

		til::Image* load_ico = til::TIL_Load("media\\ICO\\d8eba2bcc1af567ce8f596f3005980dadd13f704.ico", TIL_FILE_ADDWORKINGDIR | TIL_DEPTH_A8B8G8R8);
		til::TIL_Release(load_ico);

		til::Image* load_tga = til::TIL_Load("media\\TGA\\earth.tga", TIL_FILE_ADDWORKINGDIR | TIL_DEPTH_A8B8G8R8);
		til::TIL_Release(load_tga);

		til::Image* load_dds1 = til::TIL_Load("media\\DDS\\assaultrifle01.dds", TIL_FILE_ADDWORKINGDIR | TIL_DEPTH_A8B8G8R8);
		til::TIL_Release(load_dds1);

		til::Image* load_dds2 = til::TIL_Load("media\\DDS\\grace_cube.dds", TIL_FILE_ADDWORKINGDIR | TIL_DEPTH_A8B8G8R8);
		til::TIL_Release(load_dds2);

		til::TIL_ShutDown();

		_CrtMemCheckpoint(&g_MemEnd);

		if (_CrtMemDifference(&g_MemDiff, &g_MemStart, &g_MemEnd))
		{
			_CrtDumpMemoryLeaks();

			MessageBoxA(NULL, "Memory leaks detected!", "TinyImageLoader - Tests", MB_OK | MB_ICONERROR);

			return;
		}

		MessageBoxA(NULL, "Rejoice! No memory leaks detected.", "TinyImageLoader - Tests", MB_OK | MB_ICONINFORMATION);

		exit(0);
	}

	void Framework::Tick(float a_DT)
	{

	}

	void Framework::Render()
	{
		
	}

	void Framework::CloseDown()
	{

	}

};

int main(int argc, char** argv)
{
	TILFW::Framework* app = new TILFW::Framework;

	app->Exec(argc, argv);
	delete app;

	return 0;
}