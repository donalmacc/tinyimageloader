#include "Renderer.h"

namespace Tmpl8
{

	Surface*					Renderer::frontbuffer = NULL;
	Surface*					Renderer::backbuffer = NULL;
	Pixel*						Renderer::pixels = NULL;
	Game*						Renderer::game;
	bool						Renderer::running = true;

	PFNGLGENBUFFERSPROC			Renderer::glGenBuffers;
	PFNGLBINDBUFFERPROC			Renderer::glBindBuffer;
	PFNGLBUFFERDATAPROC			Renderer::glBufferData;
	PFNGLMAPBUFFERPROC			Renderer::glMapBuffer;
	PFNGLUNMAPBUFFERPROC		Renderer::glUnmapBuffer;
	PFNWGLSWAPINTERVALFARPROC	Renderer::wglSwapIntervalEXT;
	GLuint						Renderer::fbPBO[2];
	unsigned int				Renderer::framebufferTexID[2];
	unsigned char*				Renderer::framedata;
	LARGE_INTEGER				Renderer::lasttime;
	float						Renderer::cpu_frequency;
	bool						Renderer::vbo = true;
	double						Renderer::lastftime = 0;
	LARGE_INTEGER				Renderer::start;
	LARGE_INTEGER				Renderer::end;
	char						Renderer::workingdir[_MAX_PATH];

#ifdef LIMIT_FPS

	unsigned long				Renderer::m_PhysicsCurrent;
	unsigned long				Renderer::m_PhysicsStart = GetTickCount();
	float						Renderer::m_PhysicsTime;

#endif


	char*                       Renderer::s_Arguments = NULL;

#ifdef USE_CEGUI

	CEGUI::OpenGLRenderer*      g_GUIRenderer;
	CEGUI::System*              g_GUISystem;
	CEGUI::Window*              Renderer::s_CEGUIContext;

#endif

	bool Renderer::Init(int argc, char** argv)
	{
		// get the working directory
		TCHAR dir[_MAX_PATH];
		GetModuleFileName(NULL, dir, _MAX_PATH);
		int lastslash;
		for (int i = 0; dir[i]; ++i) { if (dir[i] == '\\') { lastslash = i; } }
		for (int i = 0; i < lastslash + 1; ++i) { workingdir[i] = (char)dir[i]; }

		// redirect printf to console window
		static const WORD MAX_CONSOLE_LINES = 500;
		int hConHandle;
		long lStdHandle;
		CONSOLE_SCREEN_BUFFER_INFO coninfo;
		FILE *fp;
		AllocConsole();
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
		coninfo.dwSize.Y = MAX_CONSOLE_LINES;
		SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);
		lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
		hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
		fp = _fdopen( hConHandle, "w" );
		*stdout = *fp;
		setvbuf(stdout, NULL, _IONBF, 0);
		lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
		hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
		fp = _fdopen(hConHandle, "r");
		*stdin = *fp;
		setvbuf(stdin, NULL, _IONBF, 0);
		lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
		hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
		fp = _fdopen(hConHandle, "w");
		*stderr = *fp;
		setvbuf(stderr, NULL, _IONBF, 0);
		ios::sync_with_stdio();

		// Init SDL

		SDL_Init(SDL_INIT_VIDEO);
		SDL_SetVideoMode(SCRWIDTH, SCRHEIGHT, 0, SDL_OPENGL);
		SDL_EnableUNICODE(1);
		SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
		SDL_WM_SetCaption(APPNAME, NULL);

		while (1)
		{
			fbPBO[0] = fbPBO[1] = -1;
			glGenBuffers		=  (PFNGLGENBUFFERSPROC)		wglGetProcAddress("glGenBuffersARB");
			glBindBuffer		=  (PFNGLBINDBUFFERPROC)		wglGetProcAddress("glBindBufferARB");
			glBufferData		=  (PFNGLBUFFERDATAPROC)		wglGetProcAddress("glBufferDataARB");
			glMapBuffer			=  (PFNGLMAPBUFFERPROC)			wglGetProcAddress("glMapBufferARB");
			glUnmapBuffer		= (PFNGLUNMAPBUFFERPROC)		wglGetProcAddress("glUnmapBufferARB");
			wglSwapIntervalEXT	= (PFNWGLSWAPINTERVALFARPROC)	wglGetProcAddress("wglSwapIntervalEXT");
			if ((!glGenBuffers) || (!glBindBuffer) || (!glBufferData) || (!glMapBuffer) || (!glUnmapBuffer))
			{
				vbo = false;
				break;
			}
			if (glGetError())
			{
				vbo = false;
				break;
			}

			// set up the screen projection
			glViewport(0, 0, SCRWIDTH, SCRHEIGHT);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, 1, 0, 1, -1, 1);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glShadeModel(GL_SMOOTH);

			// create the texture to swap
			glGenTextures(2, framebufferTexID);
			if (glGetError()) 
			{
				vbo = false;
				break;
			}

			for (int i = 0; i < 2; i++) 
			{
				glBindTexture(GL_TEXTURE_2D, framebufferTexID[i]);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCRWIDTH, SCRHEIGHT, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
				glBindTexture(GL_TEXTURE_2D, 0);
				if (glGetError()) 
				{
					vbo = false;
					break;
				}
			}
			const int sizeMemory = 4 * SCRWIDTH * SCRHEIGHT;
			glGenBuffers(2, fbPBO);
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, fbPBO[0]);	
			glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, sizeMemory, NULL, GL_STREAM_DRAW_ARB);
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, fbPBO[1]);	
			glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, sizeMemory, NULL, GL_STREAM_DRAW_ARB);
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
			glBindTexture(GL_TEXTURE_2D, framebufferTexID[0]);
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, fbPBO[0]);
			framedata = (unsigned char*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY_ARB);
			if (!framedata)
			{
				vbo = false;
				break;
			}
			memset(framedata, 0, SCRWIDTH * SCRHEIGHT * 4);
			if (glGetError())
			{
				vbo = false;
				break;
			}

			glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
			glClear(GL_COLOR_BUFFER_BIT);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			if (wglSwapIntervalEXT) wglSwapIntervalEXT(0);
			LARGE_INTEGER ticks;
			QueryPerformanceFrequency(&ticks);
			cpu_frequency = (float)ticks.QuadPart / 1000.f;

			// set up the main frontbuffer
			frontbuffer = new Surface(SCRWIDTH, SCRHEIGHT, 0, SCRWIDTH);

			break;
		}

		if (!vbo)
		{
			SDL_SetVideoMode(SCRWIDTH, SCRHEIGHT, 0, SDL_SWSURFACE + SDL_ASYNCBLIT);
			SDL_Surface* s = SDL_GetVideoSurface();

			// set up the main frontbuffer
			frontbuffer = new Surface(SCRWIDTH, SCRHEIGHT, (Pixel*)s->pixels, s->pitch);
		}

		frontbuffer->InitCharset();
		pixels = frontbuffer->GetBuffer();

#ifdef USE_CEGUI

		// GUI

		CEGUI::Size screen_size(SCRWIDTH, SCRHEIGHT);
		g_GUIRenderer = &CEGUI::OpenGLRenderer::create(screen_size);
		g_GUISystem = &CEGUI::System::create(*g_GUIRenderer);

		CEGUI::DefaultResourceProvider* provider = (CEGUI::DefaultResourceProvider*)CEGUI::System::getSingleton().getResourceProvider();
		provider->setResourceGroupDirectory("fonts", PATH_CEGUI_FONTS);
		provider->setResourceGroupDirectory("schemes", PATH_CEGUI_SCHEMES);
		provider->setResourceGroupDirectory("imagesets", PATH_CEGUI_IMAGESETS);
		provider->setResourceGroupDirectory("looknfeel", PATH_CEGUI_LOOKNFEEL);

		CEGUI::Imageset::setDefaultResourceGroup("imagesets");
		CEGUI::Font::setDefaultResourceGroup("fonts");
		CEGUI::Scheme::setDefaultResourceGroup("schemes");
		CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeel");
		CEGUI::WindowManager::setDefaultResourceGroup("layouts");

		CEGUI::SchemeManager::getSingleton().create("WindowsLook.scheme");
		CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
		CEGUI::SchemeManager::getSingleton().create("VanillaSkin.scheme");

		CEGUI::Font* default_font = &CEGUI::FontManager::getSingleton().createFreeTypeFont("Consolas", 12.f, true, "consola.ttf", "fonts");
		CEGUI::System::getSingleton().setDefaultFont("Consolas");

		s_CEGUIContext = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "root"); 
		CEGUI::System::getSingleton().setGUISheet(s_CEGUIContext);

#endif

		// render one frame
		Swap();

		game = new Game();
		game->SetTarget(frontbuffer);
		game->Init();

		return true;
	}

	void Renderer::Run()
	{
		while (running)
		{

#ifdef LIMIT_FPS

			m_PhysicsCurrent = GetTickCount();
			float delta = (float)(m_PhysicsCurrent - m_PhysicsStart);
			m_PhysicsStart = m_PhysicsCurrent;

			m_PhysicsTime += delta;

			while (m_PhysicsTime > ONEFRAME)
			{
				// event loop

				SDL_Event event;
				while (SDL_PollEvent(&event)) 
				{
					switch (event.type)
					{

					case SDL_QUIT:
						{
							game->Quit();

							break;
						}

					case SDL_KEYDOWN:
						{
							if (event.key.keysym.sym == SDLK_ESCAPE) 
							{ 
								game->Quit();
							}

							tb::Keyboard::Press(event.key.keysym.scancode);

#ifdef USE_CEGUI

							CEGUI::System::getSingleton().injectKeyDown(event.key.keysym.scancode);

							// if not unicode
							if ((event.key.keysym.unicode & 0xFF80) == 0) 
							{
								CEGUI::System::getSingleton().injectChar(event.key.keysym.unicode & 0x7F);
							}

#endif

							break;
						}

					case SDL_KEYUP:
						{
							tb::Keyboard::Release(event.key.keysym.scancode);

#ifdef USE_CEGUI

							CEGUI::System::getSingleton().injectKeyUp(event.key.keysym.scancode);

#endif

							break;
						}

					case SDL_MOUSEMOTION:
						{
							tb::Mouse::Update(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);

#ifdef USE_CEGUI

							CEGUI::System::getSingleton().injectMousePosition(
								static_cast<float>(event.motion.x),
								static_cast<float>(event.motion.y)
							);

#endif

							break;
						}

					case SDL_MOUSEBUTTONDOWN:
						{
							tb::Mouse::Press(event.button.button);
							tb::Mouse::Scroll(event.button.button);

#ifdef USE_CEGUI

							switch (event.button.button)
							{
							case SDL_BUTTON_LEFT:
								CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
								break;
							case SDL_BUTTON_MIDDLE:
								CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MiddleButton);
								break;
							case SDL_BUTTON_RIGHT:
								CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::RightButton);
								break;
							}

#endif

							break;
						}

					case SDL_MOUSEBUTTONUP:
						{
							tb::Mouse::Release(event.button.button);

#ifdef USE_CEGUI

							switch (event.button.button)
							{
							case SDL_BUTTON_LEFT:
								CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
								break;
							case SDL_BUTTON_MIDDLE:
								CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::MiddleButton);
								break;
							case SDL_BUTTON_RIGHT:
								CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::RightButton);
								break;
							}

#endif

							break;
						}

					default:
						// more info on events in SDL: http://sdl.beuc.net/sdl.wiki/SDL_Event
						break;

					}
				}

				game->Tick(ONEFRAME);

				tb::Keyboard::Clear();
				tb::Mouse::Clear();

				m_PhysicsTime -= ONEFRAME;
			}

#else

			// calculate frame time and pass it to game->Tick
			QueryPerformanceCounter(&start);
			game->Tick((float)lastftime);
			QueryPerformanceCounter(&end);
			lastftime = float(end.QuadPart - start.QuadPart) / cpu_frequency;

#endif

			game->PreRender();
			game->Render();
			game->PostRender();

			Swap();
		}

		SDL_Quit();
	}

	void Renderer::Swap()
	{
		if (vbo) 
		{		
			// frame buffer swapping for VBO mode

			int index = 0;
			int nextindex;

			glEnable(GL_TEXTURE_2D);
			glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB);
			glBindTexture(GL_TEXTURE_2D, framebufferTexID[index]);
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, fbPBO[index]);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SCRWIDTH, SCRHEIGHT, GL_BGRA, GL_UNSIGNED_BYTE, 0); 

			// draw game quad

			glColor3f(1.0f, 1.0f, 1.0f);
			glBegin(GL_QUADS);
			glNormal3f(0, 0, 1);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 0.0f);
			glEnd();

			index = (index + 1) % 2;
			nextindex = (index + 1) % 2;
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, fbPBO[nextindex]);	
			framedata = (unsigned char*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY_ARB);

#ifdef USE_CEGUI

			// interface drawing

			// turn off VBO to make sure CEGUI works
			glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB);
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);

			CEGUI::System::getSingleton().renderGUI();

#endif

		}
		else	 
		{
			// frame buffer swapping for fall-back path

			SDL_Surface* s = SDL_GetVideoSurface();
			SDL_UpdateRect(s, 0, 0, 0, 0);
			framedata = (unsigned char*)s->pixels;
			frontbuffer->SetPitch(s->pitch / 4);
		}

		frontbuffer->SetBuffer((Pixel*)framedata);

		SDL_GL_SwapBuffers(); 
	}

	void Renderer::Quit()
	{
		running = false;
	}

};