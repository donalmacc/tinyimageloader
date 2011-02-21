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

static GLuint* g_Texture;
static unsigned int g_TextureTotal;
static unsigned int g_TextureCurrent = 2;
float* g_PosX;
float* g_PosY;
float* g_ScaleX;
float* g_ScaleY;

FILE* g_Log;

void LoggingFunc(til::MessageData* a_Data)
{
	fopen_s(&g_Log, "til.log", "a");
	char msg[1024];
	//sprintf(msg, "%s (at line %i in file %s)", a_Data->message, a_Data->source_line, a_Data->source_file);
	sprintf_s(msg, 1024, "%s\n", a_Data->message);
	fputs(msg, g_Log);
	fclose(g_Log);
}

void TILFW::Setup()
{
	s_WindowWidth = 640;
	s_WindowHeight = 480;
}

void TILFW::Init()
{
	// Open log for writing

	fopen_s(&g_Log, "til.log", "w+");
	fclose(g_Log);

	// Initialize TinyImageLoader

	til::TIL_Init();

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

	//til::Image* load = til::TIL_Load("media\\PNG\\avatar.png", TIL_FILE_ADDWORKINGDIR | TIL_DEPTH_A8B8G8R8);
	til::Image* load = til::TIL_Load("media\\ICO\\d8eba2bcc1af567ce8f596f3005980dadd13f704.ico", TIL_FILE_ADDWORKINGDIR | TIL_DEPTH_A8B8G8R8);
	//til::Image* load = til::TIL_Load("media\\GIF\\rolypolypandap1.gif", TIL_FILE_ADDWORKINGDIR | TIL_DEPTH_A8B8G8R8);

	g_TextureTotal = load->GetFrameCount();
	g_Texture = new GLuint[g_TextureTotal];

	g_ScaleX = new float[g_TextureTotal];
	g_ScaleY = new float[g_TextureTotal];
	g_PosX = new float[g_TextureTotal];
	g_PosY = new float[g_TextureTotal];

	// Load the pixel data into the texture

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
			GL_TEXTURE_2D, 0, GL_RGBA, 
			load->GetWidth(i), load->GetHeight(i),
			0, 
			GL_RGBA, GL_UNSIGNED_BYTE, load->GetPixels(i)
		);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Center the image on the screen

		g_ScaleX[i] = (float)load->GetWidth(i) * 10.f;
		g_ScaleY[i] = (float)load->GetHeight(i) * 10.f;
		g_PosX[i] = ((float)s_WindowWidth / 2.f) - (g_ScaleX[i] / 2.f);
		g_PosY[i] = ((float)s_WindowHeight / 2.f) - (g_ScaleY[i] / 2.f);
	}
}

void TILFW::Tick(float a_DT)
{
	if (s_KeysReleased['q'] || s_KeysReleased['Q']) { g_TextureCurrent--; }
	if (s_KeysReleased['w'] || s_KeysReleased['W']) { g_TextureCurrent++; }
	g_TextureCurrent = g_TextureCurrent % g_TextureTotal;
}

void TILFW::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, g_Texture[g_TextureCurrent]);

	// Make sure we don't screw up our meticulously designed matrices

	glPushMatrix();
		
		// Push it to the correct position
		glTranslatef(g_PosX[g_TextureCurrent], g_PosY[g_TextureCurrent], 0.0f);
		// Set the image size
		glScalef(g_ScaleX[g_TextureCurrent], g_ScaleY[g_TextureCurrent], 1.0f);

		// Draw a quad
		glBegin(GL_QUADS);
			glTexCoord2f(0.f, 1.f); glVertex2f(0.0f, 1.0f);
			glTexCoord2f(1.f, 1.f); glVertex2f(1.0f, 1.0f);
			glTexCoord2f(1.f, 0.f); glVertex2f(1.0f, 0.0f);
			glTexCoord2f(0.f, 0.f); glVertex2f(0.0f, 0.0f);
		glEnd();

	glPopMatrix();
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	TILFW* app = new TILFW;
	return app->Exec(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}