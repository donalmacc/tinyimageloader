#include "Framework.h"

namespace TILFW
{

	bool Framework::s_KeysPressed[256];
	bool Framework::s_KeysReleased[256];

#ifndef DOXYGEN_SHOULD_SKIP_THIS

	#define CHECKOGLERROR() CheckOGLerror(__LINE__)

	void CheckOGLerror(int a_Line)
	{
		char errorString[0xffff];
		errorString[0] = 0;

		GLenum curError;
		do 
		{
			curError = glGetError();
			if( curError != GL_NO_ERROR )
			{
				char tmps[256];
				sprintf_s( tmps, 256, "'%s' ", gluErrorString(curError) );
				strcat_s( errorString, 0xffff, tmps );
			}
		} while ( curError != GL_NO_ERROR );

		if( strlen( errorString) != 0 )
		{
			//DebugBreak();
			char line[32];
			sprintf(line, " (line %i)", a_Line);
			strcat(errorString, line);

			MessageBoxA( NULL, errorString, "OpenGL errors", MB_ICONSTOP );
			exit(1);
		}
	}

	static unsigned long g_TimeCurrent, g_TimeStart;
	static float         g_TimeDelta;
	static float         g_PhysicsTime = 0;
	static float         g_RenderTime = 0;

	bool         g_Exit = false;
	bool         g_Active = true;
	HINSTANCE    g_Instance;
	HWND         g_Window;
	MSG          g_MSG;
	HDC          g_WindowContext;

	const float frame_physics = (1000.f / 60.f);

	LRESULT CALLBACK Win32Messages(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{

		case WM_ACTIVATE:
			{
				g_Active = ((LOWORD(wParam) != WA_INACTIVE) && !((BOOL)HIWORD(wParam)));
				break;
			}

		case WM_KEYDOWN:
			{
				Framework::s_KeysPressed[wParam] = true;

				break;
			}

		case WM_KEYUP:
			{
				Framework::s_KeysPressed[wParam] = false;
				Framework::s_KeysReleased[wParam] = true;

				break;
			}

		case WM_MOUSEMOVE:
			{

				break;
			}

		case WM_CLOSE:
		case WM_QUIT:
		case WM_DESTROY:
			g_Exit = true;
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);

		}

		return 0;
	}

#endif

	int Framework::Exec(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	{
		Framework::Setup();

		WNDCLASS temp = { 
			CS_HREDRAW | CS_VREDRAW | CS_OWNDC, // defines the class style 
			Win32Messages,	                    // pointer to the message handler function
			0,                                  // extra bytes following the window class structure
			0,                                  // extra bytes following the window instance
			g_Instance,                         // instance handle				
			LoadIcon(NULL, IDI_APPLICATION),    // icon handle
			LoadCursor(NULL, IDC_ARROW),        // cursor
			NULL,                               // background brush
			NULL,                               // menu name
			"Framework"                         // window title
		};
		if (!RegisterClass(&temp)) { return 0; }

		// center windows on screen

		const int x = GetSystemMetrics(SM_CXSCREEN) / 2 - s_WindowWidth / 2;
		const int y = GetSystemMetrics(SM_CYSCREEN) / 2 - s_WindowHeight / 2;

		const DWORD style = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
		RECT dim = { 
			0, 0, 
			s_WindowWidth, s_WindowHeight 
		};
		AdjustWindowRect(&dim, style, FALSE);
		const int w = dim.right - dim.left;
		const int h = dim.bottom - dim.top;

		g_Window = CreateWindowEx (
			0,
			"Framework",       // window class
			"Framework",       // window title
			style,             // visibility settings
			x, y,
			w, h,
			NULL, 
			NULL, 
			temp.hInstance, 
			NULL
		);
		if (!g_Window)
		{ 
			return -1; 
		}

		if (!(g_WindowContext = GetDC(g_Window))) 
		{ 
			return -1; 
		}

		PIXELFORMATDESCRIPTOR pfd = { 
			sizeof(PIXELFORMATDESCRIPTOR), 
			1, 
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA, 
			32, 
			0, 0, 0, 0, 0, 0, 
			8, 
			0, 
			0, 0, 0, 0, 
			0, 
			24, 
			0, 
			0,
			PFD_MAIN_PLANE,	
			0, 
			0, 0, 0
		};

		unsigned int format;
		if (!(format = ChoosePixelFormat(g_WindowContext, &pfd))) 
		{ 
			return -1; 
		}

		if (!SetPixelFormat(g_WindowContext, format, &pfd)) 
		{ 
			return -1; 
		}

		HGLRC hrc;
		if (!(hrc = wglCreateContext(g_WindowContext)))
		{ 
			return -1; 
		}

		if (!wglMakeCurrent(g_WindowContext, hrc))
		{ 
			return -1; 
		}

		// initialize opengl

		GLenum status = glewInit();
		if (status != GLEW_OK)
		{
			char error[256];
			sprintf(error, "%s", glewGetErrorString(status));
			MessageBoxA(NULL, error, "GLEW error", MB_OK | MB_ICONEXCLAMATION);
			return -1;
		}

		for (int i = 0; i < 256; i++) 
		{ 
			Framework::s_KeysPressed[i] = false; 
			Framework::s_KeysReleased[i] = false; 
		}

		// convert windows command line to c-style

		int argv;
		wchar_t** argc = CommandLineToArgvW(GetCommandLineW(), &argv);
		char** cmdline = new char*[argv];
		for (int i = 0; i < argv; i++)
		{
			cmdline[i] = new char[wcslen(argc[i]) + 1];
			wcstombs(cmdline[i], argc[i], wcslen(argc[i]) + 1);
		}

		Framework::Init((const char**)cmdline, argv);

		ShowWindow(g_Window, 1);
		UpdateWindow(g_Window);

		s_Exit = false;

		g_TimeStart = GetTickCount();

		while (1)
		{
			if (PeekMessage(&g_MSG, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&g_MSG);
				DispatchMessage(&g_MSG);
			}
			else
			{
				g_TimeCurrent = GetTickCount();
				g_TimeDelta = (float)(g_TimeCurrent - g_TimeStart);
				g_TimeStart = g_TimeCurrent;

				g_PhysicsTime += g_TimeDelta;

				while (g_PhysicsTime > frame_physics)
				{
					Framework::Tick(frame_physics);

					for (int i = 0; i < 256; i++) { Framework::s_KeysReleased[i] = false; }

					g_PhysicsTime -= frame_physics;
				}

				if (g_Active)
				{
					Framework::Render();
						
					SwapBuffers(g_WindowContext);
				}
			}

			if (s_Exit || g_Exit) { break; }
		}

		Framework::CloseDown();

		DestroyWindow(g_Window);

		return 0;
	}

}; // namespace TILFW