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

static GLuint g_Texture;

float g_PosX, g_PosY;
float g_ScaleX, g_ScaleY;

void TILFW::Setup()
{
	s_WindowWidth = 640;
	s_WindowHeight = 480;
}

void TILFW::Init()
{
	// Initialize TinyImageLoader

	til::TIL_Init();

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

	til::Image* load = til::TIL_Load("media\\PNG\\avatar.png", TIL_FILE_ADDWORKINGDIR | TIL_DEPTH_A8B8G8R8);

	// Load the pixel data into the texture

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &g_Texture);
	glBindTexture(GL_TEXTURE_2D, g_Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA, 
		load->GetWidth(), load->GetHeight(),
		0, 
		GL_RGBA, GL_UNSIGNED_BYTE, load->GetPixels()
	);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Center the image on the screen

	g_ScaleX = (float)load->GetWidth();
	g_ScaleY = (float)load->GetHeight();
	g_PosX = ((float)s_WindowWidth / 2.f) - (g_ScaleX / 2.f);
	g_PosY = ((float)s_WindowHeight / 2.f) - (g_ScaleY / 2.f);
}

void TILFW::Tick(float a_DT)
{

}

void TILFW::Render()
{
	glBindTexture(GL_TEXTURE_2D, g_Texture);

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
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	TILFW* app = new TILFW;
	return app->Exec(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}