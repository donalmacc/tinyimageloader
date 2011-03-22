/*! 
	\file framework.h
*/

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

/*! 
	\namespace TILFW
	\brief TinyImageLoader Framework namespace.

	The framework used by the examples.
*/

namespace TILFW
{

	class DoxygenSaysWhat;

	class Framework
	{

	public:

		//! Main entry point for the examples
		int Exec(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
		//! Setting up parameters
		void Setup();
		//! Initializing
		void Init(const char** a_CommandLine, int a_Commands);
		//! Main loop
		void Tick(float a_DT);
		//! Rendering
		void Render();
		//! Closing down
		void CloseDown();

		unsigned int s_WindowWidth; //!< Width of the window
		unsigned int s_WindowHeight; //!< Height of the window
		bool s_Exit; //!< Exits the program if true

		static bool s_KeysPressed[256]; //!< List of keys held down
		static bool s_KeysReleased[256]; //!< List of keys released in the last frame

	}; // struct Framework

}; // namespace TILFW

#endif