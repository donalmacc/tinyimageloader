/*!
	\file example-opengl-texture.cpp
*/

#include "Framework.h"

#include "TinyImageLoader.h"

namespace TILFW
{

#ifndef DOXYGEN_SHOULD_SKIP_THIS

	static GLuint* g_Texture;
	static unsigned int g_TextureTotal;
	static unsigned int g_TextureCurrent = 0;
	til::Image* g_Load;
	float g_PosX, g_PosY;
	float g_ScaleX, g_ScaleY;
	float g_Scale = 1.f;
	bool g_Change = true;

	FILE* g_Log;
	char* g_LogPath;

	void LoggingFunc(til::MessageData* a_Data)
	{
		fopen_s(&g_Log, g_LogPath, "a");
		char msg[1024];
		//sprintf_s(msg, "%s (at line %i in file %s)", a_Data->message, a_Data->source_line, a_Data->source_file);
		sprintf_s(msg, 1024, "%s\n", a_Data->message);
		fputs(msg, g_Log);
		fclose(g_Log);
	}

#endif

	//! Setting up some stuff
	void Framework::Setup()
	{
		s_WindowWidth = 640;
		s_WindowHeight = 480;
	}

	void Framework::Init(const char** a_CommandLine, int a_Commands)
	{
		// Initialize TinyImageLoader

		til::TIL_Init();

		// Open log for writing

		g_LogPath = new char[TIL_MAX_PATH];
		til::TIL_AddWorkingDirectory(g_LogPath, TIL_MAX_PATH, "til.log");
		fopen_s(&g_Log, g_LogPath, "w+");
		fclose(g_Log);

		// We want to handle incoming debug messages

		til::TIL_SetDebugFunc(LoggingFunc);
		til::TIL_SetErrorFunc(LoggingFunc);

		// Set up projection
		// Now we can draw quads using screen coordinates

		glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, s_WindowWidth, s_WindowHeight, 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		glClearColor(0.f, 0.f, 0.f, 0.f);
		// Make sure the quads show up
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		// Load an image
		// Note: OpenGL reverses the pixel components
		// TIL_DEPTH_A8B8G8R8 = GL_RGBA

		if (a_Commands == 1)
		{
			g_Load = til::TIL_Load("media\\PNG\\avatar.png", TIL_FILE_ADDWORKINGDIR | TIL_DEPTH_A8B8G8R8);
		}
		else
		{
			g_Load = til::TIL_Load(a_CommandLine[1], TIL_FILE_ABSOLUTEPATH | TIL_DEPTH_A8B8G8R8);
		}

		if (!g_Load)
		{
			char msg[256];
			sprintf(msg, "Could not load image. Check the log for details.", a_CommandLine[1]);
			MessageBoxA(NULL, msg, "TinyImageLoader", MB_OK);

			exit(0);
		}

		// Load the pixel data into the texture

		g_TextureTotal = g_Load->GetFrameCount();
		g_Texture = new GLuint[g_TextureTotal];

		for (unsigned int i = 0; i < g_TextureTotal; i++)
		{
			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, &g_Texture[i]);

			glBindTexture(GL_TEXTURE_2D, g_Texture[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RGB, 
				g_Load->GetWidth(i), g_Load->GetHeight(i),
				0, 
				GL_RGBA, GL_UNSIGNED_BYTE, g_Load->GetPixels(i)
			);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void Framework::Tick(float a_DT)
	{
		if (s_KeysReleased['q'] || s_KeysReleased['Q']) 
		{ 
			g_TextureCurrent--;
			g_Change = true;
		}
		if (s_KeysReleased['w'] || s_KeysReleased['W']) 
		{ 
			g_TextureCurrent++;
			g_Change = true;
		}
		
		if (s_KeysReleased['O'])
		{
			g_Scale += 0.05f;
			g_Change = true;
		}
		if (s_KeysReleased['P'])
		{
			if (g_Scale > 0.05f)
			{
				g_Scale -= 0.05f;
				g_Change = true;
			}
		}

		if (g_Change)
		{
			g_TextureCurrent = g_TextureCurrent % g_TextureTotal;

			g_ScaleX = (float)g_Load->GetWidth(g_TextureCurrent) * g_Scale;
			g_ScaleY = (float)g_Load->GetHeight(g_TextureCurrent) * g_Scale;
			g_PosX = ((float)s_WindowWidth / 2.f) - (g_ScaleX / 2.f);
			g_PosY = ((float)s_WindowHeight / 2.f) - (g_ScaleY / 2.f);
		}

		g_Change = false;
	}

	void Framework::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_BLEND); 
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindTexture(GL_TEXTURE_2D, g_Texture[g_TextureCurrent]);

		// Make sure we don't screw up our meticulously designed matrices

		glPushMatrix();
			
			// Push it to the correct position
			glTranslatef(g_PosX, g_PosY, 0.0f);
			// Set the image size
			glScalef(g_ScaleX, g_ScaleY, 1.0f);

			// Draw a quad
			glBegin(GL_QUADS);
				glTexCoord2f(0.f, 1.f); glVertex2f(0.0f, 1.0f);
				glTexCoord2f(1.f, 1.f); glVertex2f(1.0f, 1.0f);
				glTexCoord2f(1.f, 0.f); glVertex2f(1.0f, 0.0f);
				glTexCoord2f(0.f, 0.f); glVertex2f(0.0f, 0.0f);
			glEnd();

		glPopMatrix();

		glDisable(GL_BLEND);
	}

	void Framework::CloseDown()
	{
		til::TIL_Release(g_Load);

		til::TIL_ShutDown();
	}

}; // namespace TILFW

#ifndef DOXYGEN_SHOULD_SKIP_THIS

	int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	{
		TILFW::Framework* app = new TILFW::Framework;
		return app->Exec(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	}

#endif
