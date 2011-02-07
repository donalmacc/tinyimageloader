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

#include "windows.h"
#include <psapi.h>
#include <direct.h>
#include <sys\stat.h>

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

void TILFW::Setup()
{
	s_WindowWidth = 640;
	s_WindowHeight = 480;
}

unsigned long GetMemoryUsage()
{
	HANDLE process;
	PROCESS_MEMORY_COUNTERS pmc;

	process = OpenProcess ( 
		PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
		FALSE, 
		GetCurrentProcessId()
	);

	if (!process) { return 0; }

	SIZE_T size = 0;
	if (GetProcessMemoryInfo(process, &pmc, sizeof(pmc)))
	{
		size = pmc.WorkingSetSize;
	}

	CloseHandle(process);

	return (unsigned long)size;
}

void TILFW::Init()
{
	_CrtMemState mem_start, mem_end, mem_diff;

	// Initialize TinyImageLoader

	til::TIL_Init();

	_CrtMemCheckpoint(&mem_start);

	til::Image* load = til::TIL_Load("media\\BMP\\concrete.bmp", TIL_FILE_ADDWORKINGDIR | TIL_DEPTH_A8B8G8R8);
	delete load;

	_CrtMemCheckpoint(&mem_end);

	til::TIL_ShutDown();

	//til::Image* load = til::TIL_Load("media\\PNG\\avatar.png", TIL_FILE_ADDWORKINGDIR | TIL_DEPTH_A8B8G8R8);
	//delete load;

	if (_CrtMemDifference(&mem_diff, &mem_start, &mem_end))
	{
		//_CrtMemDumpStatistics(&mem_diff);
		_CrtDumpMemoryLeaks();

		int j = 0;
	}

	int i = 0;
}

void TILFW::Tick(float a_DT)
{

}

void TILFW::Render()
{
	
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	TILFW* app = new TILFW;
	return app->Exec(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}