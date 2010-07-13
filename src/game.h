// Template, major revision 3, beta
// IGAD/NHTV - Jacco Bikker - 2006-2009

#ifndef _GAME_H_
#define _GAME_H_

#include "Settings.h"
#include "Includes.h"

namespace Tmpl8 
{

	typedef unsigned long Pixel;

	class Surface;

	class Game
	{

	public:

		void SetTarget(Surface* a_Surface) { m_Screen = a_Surface; }
		void Init();
		void Tick(float a_DT);
		void Quit();

		void PreRender() { }
		void Render();
		void PostRender() { }

	private:

		Surface* m_Screen;

	}; // class Game

}; // namespace Tmpl8

#endif