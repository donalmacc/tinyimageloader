#ifndef _TILINTERNAL_H_
#define _TILINTERNAL_H_

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

	//! Adds working directory to a path
	/*!
		\param a_Dst Where to put it
		\param a_MaxLength The length of the destination buffer
		\param a_Path The path to add

		\note Internal method. 
		
		Adds the working directory to a path and puts it in the destination buffer.
		If the destination buffer is too small, the path is truncated to fit.
	*/
	extern void AddWorkingDirectory(char* a_Dst, size_t a_MaxLength, const char* a_Path);

	//@}
}

#endif