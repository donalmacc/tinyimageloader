#include "Framework.h"

#include "TinyImageLoader.h"

namespace TILFW
{

	static GLuint* g_Texture;
	static unsigned int g_TextureTotal;
	static unsigned int g_TextureCurrent = 0;
	static til::Image* g_Load;
	static float g_PosX, g_PosY;
	static float g_ScaleX, g_ScaleY;
	static float g_Scale = 1.f;
	bool g_Change = true;

	static FILE* g_Log;
	static char* g_LogPath;

	void CustomLoggingFunc(til::MessageData* a_Data)
	{
		fopen_s(&g_Log, g_LogPath, "a");
		char msg[1024];
		sprintf_s(msg, 1024, "%s\n", a_Data->message);
		fputs(msg, g_Log);
		fclose(g_Log);

		printf(msg);
	}

	void CustomPitchFunc(til::uint32 a_Width, til::uint32 a_Height, til::uint8 a_BPP, til::uint32& a_PitchX, til::uint32& a_PitchY)
	{
		til::uint32 high = (a_Width > a_Height) ? a_Width : a_Height;

		til::uint32 closest = 0;
		til::uint32 move = high;
		while (move >>= 1) { closest++; }
		if (high - (1 << closest) > 0) { closest++; }

		a_PitchX = 1 << closest;
		a_PitchY = a_PitchX;
	}

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

		til::TIL_SetDebugFunc(CustomLoggingFunc);
		til::TIL_SetErrorFunc(CustomLoggingFunc);

		// We can set a custom pitch function

		//til::TIL_SetPitchFunc(CustomPitchFunc);

		// Set up projection
		// Now we can draw quads using screen coordinates

		glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, s_WindowWidth, s_WindowHeight, 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		glClearColor(0.f, 0.f, 0.f, 1.f);

		// Load an image
		// Note: OpenGL reverses the pixel components
		// TIL_DEPTH_A8B8G8R8 = GL_RGBA

		if (a_Commands == 1)
		{
			g_Load = til::TIL_Load("media\\ICO\\d8eba2bcc1af567ce8f596f3005980dadd13f704.ico", TIL_FILE_ADDWORKINGDIR | TIL_DEPTH_A8B8G8R8);
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
		glGenTextures(g_TextureTotal, g_Texture);

		glEnable(GL_TEXTURE_2D);

		for (unsigned int i = 0; i < g_TextureTotal; i++)
		{
			glBindTexture(GL_TEXTURE_2D, g_Texture[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RGBA,
				g_Load->GetPitchX(i), g_Load->GetPitchY(i),
				0,
				GL_RGBA, GL_UNSIGNED_BYTE, g_Load->GetPixels(i)
			);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Framework::Tick(float a_DT)
	{
		if (s_KeysReleased['Q'] || s_KeysReleased[VK_LEFT]) 
		{ 
			if (g_TextureCurrent == 0)
			{ 
				g_TextureCurrent = g_TextureTotal - 1; 
			}
			else
			{
				g_TextureCurrent--;
			}
			g_Change = true;
		}
		if (s_KeysReleased['W'] || s_KeysReleased[VK_RIGHT]) 
		{ 
			g_TextureCurrent++;
			if (g_TextureCurrent == g_TextureTotal) { g_TextureCurrent = 0; }
			g_Change = true;
		}

		if (s_KeysReleased['O'] || s_KeysReleased[VK_UP])
		{
			g_Scale += 0.05f;
			g_Change = true;
		}
		if (s_KeysReleased['P'] || s_KeysReleased[VK_DOWN])
		{
			if (g_Scale > 0.05f)
			{
				g_Scale -= 0.05f;
				g_Change = true;
			}
		}

		if (s_KeysReleased['1'])
		{
			g_Scale = 1.f;
			g_Change = true;
		}
		else if (s_KeysReleased['2'])
		{
			g_Scale = 2.f;
			g_Change = true;
		}
		else if (s_KeysReleased['3'])
		{
			g_Scale = 4.f;
			g_Change = true;
		}
		else if (s_KeysReleased['4'])
		{
			g_Scale = 8.f;
			g_Change = true;
		}

		if (g_Change)
		{
			g_ScaleX = (float)g_Load->GetPitchX(g_TextureCurrent) * g_Scale;
			g_ScaleY = (float)g_Load->GetPitchY(g_TextureCurrent) * g_Scale;
			g_PosX = ((float)s_WindowWidth / 2.f) - (g_ScaleX / 2.f);
			g_PosY = ((float)s_WindowHeight / 2.f) - (g_ScaleY / 2.f);
		}

		g_Change = false;
	}

	void Framework::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, g_Texture[g_TextureCurrent]);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Make sure we don't screw up our meticulously designed matrices

		glPushMatrix();
			
			// Push it to the correct position
			glTranslatef(g_PosX, g_PosY, 0.0f);
			// Set the image size
			glScalef(g_ScaleX, g_ScaleY, 1.0f);

			// Draw a quad
			glBegin(GL_QUADS);
				glTexCoord2f(1.f, 0.f); glVertex2f(1.0f, 0.0f);
				glTexCoord2f(0.f, 0.f); glVertex2f(0.0f, 0.0f);
				glTexCoord2f(0.f, 1.f); glVertex2f(0.0f, 1.0f);
				glTexCoord2f(1.f, 1.f); glVertex2f(1.0f, 1.0f);
			glEnd();

		glPopMatrix();

		glDisable(GL_BLEND);

		DisplayText(10, 20, "Dimensions: (%i x %i)", g_Load->GetWidth(g_TextureCurrent), g_Load->GetHeight(g_TextureCurrent));
		DisplayText(10, 40, "Pitch: (%i x %i)", g_Load->GetPitchX(g_TextureCurrent), g_Load->GetPitchY(g_TextureCurrent));
		DisplayText(10, 60, "Frames: %i", g_Load->GetFrameCount());
		DisplayText(10, 80, "Current frame: %i", g_TextureCurrent + 1);
		DisplayText(10, 100, "Size: %.2f", g_Scale);

		DisplayText(10, s_WindowHeight - 60, "Set size: (1, 2, 3, 4)", 0);
		DisplayText(10, s_WindowHeight - 40, "Change frame: (<-, ->)", 0);
		DisplayText(10, s_WindowHeight - 20, "Change size: (^, v)", 0);
	}

	void Framework::CloseDown()
	{
		til::TIL_Release(g_Load);

		til::TIL_ShutDown();
	}

}; // namespace TILFW

int main(int argc, char** argv)
{
	TILFW::Framework* app = new TILFW::Framework;

	app->Exec(argc, argv);
	delete app;

	return 0;
}
