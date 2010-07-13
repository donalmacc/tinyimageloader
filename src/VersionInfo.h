#ifndef _VERSIONINFO_H_
#define _VERSIONINFO_H_

#define PLATFORM_WINDOWS		0
#define PLATFORM_LINUX			1
#define PLATFORM_MACINTOSH		2
#define PLATFORM_SOMETHINGELSE	1000

#define COMPILER_VS6			1200
#define COMPILER_VSDOTNET		1300
#define COMPILER_VS2005			1400
#define COMPILER_VS2008			1500
#define COMPILER_GPP			100

#ifndef PLATFORM

	/*
		Detect which platform this is being run on.
		Thanks go to Pixel Toaster for most of the flags.
	*/

	#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
		#define PLATFORM						PLATFORM_WINDOWS
		#define PLATFORM_NAME					"Windows"
	#elif defined(__APPLE__) || defined(__MACH__)
		#define PLATFORM						PLATFORM_MACINTOSH
		#define PLATFORM_NAME					"Macintosh"
	#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__CYGWIN__)
		#define PLATFORM						PLATFORM_LINUX
		#define PLATFORM_NAME					"Linux"
	#else
		#define PLATFORM						PLATFORM_SOMETHINGELSE
		#define PLATFORM_NAME					"Something Else"
	#endif

	/*
		How many bits is this system?
	*/

	// Windows
	#if (defined(_WIN64) || defined(WIN64))
		#define PLATFORM_BITS					64
	// Macintosh
	#elif (defined(__LP64__) || defined(_LP64) || defined(__ppc64__))
		#define PLATFORM_BITS					64
	// Linux
	#elif (defined(__x86_64__) || defined(__64BIT__) || (__WORDSIZE == 64))
		#define PLATFORM_BITS					64
	#else
		#define PLATFORM_BITS					32
	#endif

	/*
		Determine which compiler was used to compile this program.
	*/

	#ifdef _MSC_VER
		#define COMPILER_VERSION				_MSC_VER

		#if (COMPILER_VERSION >= 1500)
			#define COMPILER					COMPILER_VS2008
			#define COMPILER_NAME				"Visual Studio 2008"
		#elif (COMPILER_VERSION >= 1400)
			#define COMPILER					COMPILER_VS2005
			#define COMPILER_NAME				"Visual Studio 2005"
		#elif (COMPILER_VERSION >= 1300)
			#define COMPILER					COMPILER_VSDOTNET
			#define COMPILER_NAME				"Visual Studio .NET"
		#elif (COMPILER_VERSION >= 1200)
			#define COMPILER					COMPILER_VS6
			#define COMPILER_NAME				"Visual Studio 6"
		#else
			#error This version of Visual Studio is not supported.
		#endif
	#elif defined(__GNUC__)
		#define COMPILER_VERSION				(__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
		
		// __VERSION__
		#define COMPILER_NAME					"G++"

		#define PLATFORM						PLATFORM_LINUX
	#endif	

	/*
		Compiler specific options
	*/

	#if PLATFORM == PLATFORM_WINDOWS
		// Support for Windows 98
		#if COMPILER_VERSION >= COMPILER_VS6 && COMPILER_VERSION < COMPILER_VSDOTNET
			#pragma comment(linker, "/OPT:NOWIN98")
		#endif

		#if COMPILER_VERSION >= COMPILER_VSDOTNET
			#define FAST_CALL __declspec(noinline) __fastcall
		#else
			#define FAST_CALL __fastcall
		#endif
	#endif

#endif

#endif