#ifndef _TILSETTINGS_H_
#define _TILSETTINGS_H_

// =========================================
// Defines
// =========================================

#define TIL_VERSION_MAJOR                 1
#define TIL_VERSION_MINOR                 0
#define TIL_VERSION_BUGFIX                0

#define TIL_FILE_ABSOLUTEPATH             0x000000001
#define TIL_FILE_ADDWORKDINGDIR           0x000000002

#define TIL_DEPTH_A8R8G8B8                0x000010000
#define TIL_DEPTH_R8G8B8A8                0x000020000
#define TIL_DEPTH_R8G8B8                  0x000030000
#define TIL_DEPTH_R5G6B5                  0x000040000

#define TIL_FORMAT_PNG                    0x000000001
#define TIL_FORMAT_GIF                    0x000000002
#define TIL_FORMAT_GIF_ANIMATED           0x000000004
#define TIL_FORMAT_BMP                    0x000000008
#define TIL_FORMAT_TGA                    0x000000010

#ifndef _CRT_SECURE_NO_WARNINGS
	#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef TIL_FORMAT
	#define TIL_FORMAT                    (TIL_FORMAT_PNG | TIL_FORMAT_GIF | TIL_FORMAT_GIF_ANIMATED | TIL_FORMAT_BMP | TIL_FORMAT_TGA)
#endif

#ifndef TIL_ERROR_EXPLAIN
	#define	TIL_ERROR_EXPLAIN(msg, ...)    printf("TinyImageLoader Error: "msg" (line %i)\n", __VA_ARGS__, __LINE__)
#endif

namespace til
{

	// =========================================
	// Unsigned
	// =========================================

	typedef long long                         uint64;
	typedef unsigned long                     uint32;
	typedef unsigned short                    uint16;
	typedef unsigned char                     uint8;

	// =========================================
	// Signed
	// =========================================

	typedef unsigned long long                int64;
	typedef long                              int32;
	typedef short                             int16;
	typedef char                              int8;

	typedef unsigned long                     COLORDATA;

	// =========================================
	// Datatypes
	// =========================================

	typedef unsigned char                     byte;
	typedef unsigned short                    word;
	typedef unsigned long                     dword;

	// =========================================
	// Colors
	// =========================================

	typedef uint8                             color_8b;
	typedef uint16                            color_16b;
	typedef uint32                            color_32b;

	// just checking the extension for now

	#define TIL_EXTENSION_MAKE(a, b, c, d)    (((d) << 24) + ((c) << 16) + ((b) << 8) + (a))

	#if (TIL_FORMAT & TIL_FORMAT_PNG)
		const uint32 TIL_FORMAT_PNG_EXT =      TIL_EXTENSION_MAKE('.', 'p', 'n', 'g');
	#endif

	#if (TIL_FORMAT & TIL_FORMAT_GIF)
		const uint32 TIL_FORMAT_GIF_EXT =      TIL_EXTENSION_MAKE('.', 'g', 'i', 'f');
	#endif

	#if (TIL_FORMAT & TIL_FORMAT_BMP)
		const uint32 TIL_FORMAT_BMP_EXT =      TIL_EXTENSION_MAKE('.', 'b', 'm', 'p');
	#endif

	#if (TIL_FORMAT & TIL_FORMAT_TGA)
		const uint32 TIL_FORMAT_TGA_EXT =      TIL_EXTENSION_MAKE('.', 't', 'g', 'a');
	#endif

}; // namespace til

#endif