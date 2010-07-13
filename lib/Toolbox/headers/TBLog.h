#ifndef _TBLOG_H_
#define _TBLOG_H_

#define LOG_INFO(message, ...)	Log::Write(__FILE__, __LINE__, "INFO", message, __VA_ARGS__)
#define LOG_TRACE(message, ...)	Log::Write(__FILE__, __LINE__, "TRACE", message, __VA_ARGS__)
#define LOG_ERROR(message, ...)	Log::Write(__FILE__, __LINE__, "ERROR", message, __VA_ARGS__)
#define LOG_FATAL(message, ...)	Log::Write(__FILE__, __LINE__, "FATAL", message, __VA_ARGS__)

#include "TBSettings.h"

#include <string>

namespace tb
{

	struct Log
	{
		
		static void Init(char* a_File);
		static void Fatal(char* a_File, int a_Line, char* a_Severity, char* a_Message, ...);
		static void Write(char* a_File, int a_Line, char* a_Severity, char* a_Message, ...);

		static std::string s_LogFileName;
		static bool s_LogFile;
		static char s_MessageBuffer[1024];
		static char s_OutputBuffer[1024];

	}; // struct Log

}; // namespace tb

#endif