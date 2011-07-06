/*! 
	\file framework.h
*/

#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

// Standard

#include <stdio.h>
#include <windows.h>

// STL

#include <vector>

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
		int Exec(int argc, char** argv);
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

		void DisplayText(unsigned int a_X, unsigned int a_Y, const char* a_Text, ...);

		unsigned int s_WindowWidth; //!< Width of the window
		unsigned int s_WindowHeight; //!< Height of the window
		bool s_Exit; //!< Exits the program if true

		static bool s_KeysPressed[256]; //!< List of keys held down
		static bool s_KeysReleased[256]; //!< List of keys released in the last frame

	protected:

		struct TextData
		{
			unsigned int x, y;
			char* msg;
		};

		std::vector<TextData*> m_TextList;

	}; // struct Framework

}; // namespace TILFW

#endif