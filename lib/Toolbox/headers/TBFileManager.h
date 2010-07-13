#ifndef _FILEMANAGER_H_
#define _FILEMANAGER_H_

#include "TBSettings.h"

namespace tb
{

	struct FileManager
	{

		static unsigned long GetMemoryUsage();

		static char* GetWorkingDirectory();
		static char* AddWorkingDirectory(char* a_FilePath);
		static char* GetDirectory(char* a_FilePath);
		static bool GetFileExists(char* a_FilePath);

		static char s_Output[256];

	private:

		static char s_WorkingDirectory[256];
		static bool s_Init;

	}; // struct FileManager

}; // namespace tb

#endif