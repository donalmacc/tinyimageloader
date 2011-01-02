#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

// Standard

#include <stdio.h>
#include <windows.h>

// OpenGL

extern "C" 
{
	#include "GL/glew.h" 
}
#include "GL/gl.h"
#include "GL/wglew.h"
#include "GL/glext.h"
#include "GL/wglext.h"

// Warnings

#ifndef _CRT_SECURE_NO_WARNINGS
	#define _CRT_SECURE_NO_WARNINGS
#endif

struct TILFW
{

	int Exec(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	void Setup();
	void Init();
	void Tick(float a_DT);
	void Render();

	unsigned int s_WindowWidth, s_WindowHeight;
	bool s_Exit;

};

#endif