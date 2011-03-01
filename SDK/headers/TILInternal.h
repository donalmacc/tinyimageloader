#ifndef _TILINTERNAL_H_
#define _TILINTERNAL_H_

#include "TILSettings.h"

namespace til
{
	// =========================================
	// Internal functions
	// =========================================

	/*!
		@name Internal
		These functions are internal and shouldn't be called by developers.
	*/
	//@{

	//! Adds an error to the logging stack
	/*!
		\param a_Message The message to post
		\param a_File The file it originated from
		\param a_Line The line it originated from

		\note Internal method. 

		An implementation of printf for errors. 
		The parameters in a_Message are parsed and sent to the attached logging function.
		This can be the internal logger or one of your own.
	*/
	extern void AddError(char* a_Message, char* a_File, int a_Line, ...);
	//! Adds a debug message to the logging stack
	/*!
		\param a_Message The message to post
		\param a_File The file it originated from
		\param a_Line The line it originated from

		\note Internal method. 

		An implementation of printf for debug messages. 
		The parameters in a_Message are parsed and sent to the attached logging function.
		This can be the internal logger or one of your own.
	*/
	extern void AddDebug(char* a_Message, char* a_File, int a_Line, ...);

	extern void TIL_AddWorkingDirectory(char* a_Dst, size_t a_MaxLength, const char* a_Path);

	class FileStream;

	//! Default FileStream function.
	/*!
		\param a_Path File path
		\param a_Options Options to load with

		\return FileStream handle

		\note Internal method.

		Opens a file and returns a FileStream handle. If unsuccessful, it returns NULL.
	*/
	extern FileStream* OpenStreamDefault(const char* a_Path, uint32 a_Options);
	//! Opens a FileStream.
	/*!
		\note Internal method.	

		This function pointer is called by implementations to get a handle to a file.
	*/
	static FileStreamFunc g_FileFunc = OpenStreamDefault;

	//@}
}

#endif