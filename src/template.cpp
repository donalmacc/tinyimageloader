// Template, major revision 3, beta
// IGAD/NHTV - Jacco Bikker - 2006-2009

#include "template.h"

#include "Settings.h"
#include "Includes.h"
#include "Renderer.h"

namespace Tmpl8 
{ 
	void NotifyUser(char* a_String)
	{
		HWND app = FindWindow(NULL, APPNAME);
		MessageBox(app, a_String, "ERROR", MB_OK);
		exit(0);
	}
}

using namespace Tmpl8;
using namespace std;

int main(int argc, char** argv) 
{  
	Renderer::Init(argc, argv);
	Renderer::Run();

	return 1;
}