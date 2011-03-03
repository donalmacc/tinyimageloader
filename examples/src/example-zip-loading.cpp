/*!
	\file example-zip-loading.cpp
*/

#include "Framework.h"

#include "TinyImageLoader.h"

extern "C" {
	#include "unzip.h"
};

#ifdef WIN32
	#define USEWIN32IOAPI
	#include "iowin32.h"
#endif

static GLuint* g_Texture;
static unsigned int g_TextureTotal;
static unsigned int g_TextureCurrent = 0;
til::Image* g_Load;
float g_PosX, g_PosY;
float g_ScaleX, g_ScaleY;
float g_Scale = 1.f;
bool g_Change = true;

FILE* g_Unzip;
char* g_UnzipPath;
FILE* g_Log;
char* g_LogPath;

#define EXA_FILE_ZIP   0x0000FF00

void LoggingFunc(til::MessageData* a_Data)
{
	fopen_s(&g_Log, g_LogPath, "a");
	char msg[1024];
	//sprintf_s(msg, "%s (at line %i in file %s)", a_Data->message, a_Data->source_line, a_Data->source_file);
	sprintf_s(msg, 1024, "%s\n", a_Data->message);
	fputs(msg, g_Log);
	fclose(g_Log);
}

class FileStreamZip : public til::FileStream
{

public:

	FileStreamZip()
	{
		m_FilePath = new char[TIL_MAX_PATH];

		m_ByteMax = 8192;
		m_ByteCurrent = 0;
		m_ByteTotal = 0;
		m_Read = new til::byte[m_ByteMax];
	}
	~FileStreamZip()
	{
		delete m_FilePath;
		delete m_Read;
	}

	bool OpenZip(const char* a_Path, til::uint32 a_Options)
	{
		m_ZipPath = new char[TIL_MAX_PATH];

		if (a_Options & TIL_FILE_ADDWORKINGDIR)
		{
			til::TIL_AddWorkingDirectory(m_ZipPath, TIL_MAX_PATH, a_Path);
		}
		else
		{
			strcpy(m_ZipPath, a_Path);
		}

		zlib_filefunc_def ffunc;
		fill_win32_filefunc(&ffunc);
		m_ZipFile = unzOpen2(m_ZipPath, &ffunc);

		m_Error = unzGetGlobalInfo(m_ZipFile, &m_ZipGlobalInfo);
		if (CheckError()) { return false; }

		char filename[256];
		for (int i = 0; i < m_ZipGlobalInfo.number_entry; i++)
		{
			m_Error = unzGetCurrentFileInfo(
				m_ZipFile, &m_ZipInfo, 
				filename, 256,
				NULL, 0,
				NULL, 0
			);
			if (CheckError()) { break; }

			//TIL_PRINT_DEBUG("Filename: %s", filename);

			m_Error = unzGoToNextFile(m_ZipFile);
			if (CheckError()) { break; }
		}

		m_Error = unzGoToFirstFile(m_ZipFile);
		if (CheckError()) { return false; }

		return true;
	}

	bool Open(const char* a_File, til::uint32 a_Options)
	{
		m_Error = unzLocateFile(m_ZipFile, a_File, 2);
		if (m_Error == UNZ_END_OF_LIST_OF_FILE) { return false; }

		m_Error = unzGetCurrentFileInfo(
			m_ZipFile, &m_ZipInfo, 
			m_FilePath, 256,
			NULL, 0, NULL, 0
		);
		if (CheckError()) { return false; }

		m_Error = unzOpenCurrentFile(m_ZipFile);
		if (CheckError()) { return false; }

		// read at least one block first
		ReadBlock();

		return true;
	}

	bool Read(void* a_Dst, til::uint32 a_ElementSize, til::uint32 a_Count = 1)
	{
		return ReadByte((byte*)a_Dst, a_ElementSize * a_Count);
	}

	bool ReadByte(til::byte* a_Dst, til::uint32 a_Count = 1)
	{
		// the main function for reading data
		// we read with m_ByteMax bytes at a time from the zip file
		// if TinyImageLoader requests a byte not yet loaded, 
		// we load a new block of bytes

		for (til::uint32 i = 0; i < a_Count; i++)
		{
			*a_Dst++ = *m_Buffer++;

			if (++m_ByteCurrent == m_ByteTotal) 
			{ 
				if (!ReadBlock())
				{
					return false; 
				}
			}
		}

		return true;
	}

	bool ReadWord(til::word* a_Dst, til::uint32 a_Count = 1)
	{
		return ReadByte((byte*)a_Dst, 2 * a_Count);
	}

	bool ReadDWord(til::dword* a_Dst, til::uint32 a_Count = 1)
	{
		return ReadByte((byte*)a_Dst, 4 * a_Count);
	}

	bool Seek(til::uint32 a_Offset, til::uint32 a_Options)
	{
		if (a_Options & TIL_FILE_SEEK_CURR)
		{
			for (til::uint32 i = 0; i < a_Offset; i++)
			{
				if (++m_ByteCurrent == m_ByteTotal) 
				{ 
					if (!ReadBlock())
					{
						return false; 
					}
				}
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	bool EndOfFile()
	{
		return m_EOF;
	}

	bool Close()
	{
		m_Error = unzCloseCurrentFile(m_ZipFile);
		return !CheckError();
	}

private:

	bool CheckError()
	{
		if (m_Error != UNZ_OK)
		{
			//TIL_ERROR_EXPLAIN("Error with zip decoding (code: %i)", m_Error);

			return true;
		}

		return false;
	}

	bool ReadBlock()
	{	
		int found = unzReadCurrentFile(m_ZipFile, m_Read, m_ByteMax);
		if (found < 0)
		{
			CheckError();
			return false;
		}
		else if (found > 0)
		{
			m_Buffer = m_Read;
			m_ByteCurrent = 0;
			m_ByteTotal = found;
		}

		return true;
	}

	int m_ByteCurrent, m_ByteTotal;
	unsigned int m_ByteMax;
	til::byte* m_Read;
	til::byte* m_Buffer;

	bool m_EOF;

	char* m_ZipPath;
	char m_FileName[256];
	unzFile m_ZipFile;
	unz_file_info m_ZipInfo;
	unz_global_info m_ZipGlobalInfo;

	int m_Error;

};

til::FileStream* OpenStreamZip(const char* a_Path, til::uint32 a_Options)
{
	FileStreamZip* result = new FileStreamZip();
	if (a_Options & EXA_FILE_ZIP)
	{
		if (result->OpenZip(a_Path, a_Options)) { return result; }
	}
	else
	{
		if (result->Open(a_Path, a_Options)) { return result; }
	}

	return NULL;
}

namespace TILFW
{

	//! Setting up some stuff
	void Framework::Setup()
	{
		s_WindowWidth = 640;
		s_WindowHeight = 480;
	}

	void Framework::Init(const char** a_CommandLine, int a_Commands)
	{
		til::TIL_Init();

		g_LogPath = new char[TIL_MAX_PATH];
		til::TIL_AddWorkingDirectory(g_LogPath, TIL_MAX_PATH, "til.log");
		fopen_s(&g_Log, g_LogPath, "w+");
		fclose(g_Log);

		til::TIL_SetDebugFunc(LoggingFunc);
		til::TIL_SetErrorFunc(LoggingFunc);

		FileStreamZip* stream = new FileStreamZip();
		stream->OpenZip("media\\ZIP\\zipped.zip", TIL_FILE_ADDWORKINGDIR);
		if (!stream->Open("zipped/rolypolypandap1.gif", TIL_FILE_ADDWORKINGDIR))
		{
			char msg[256];
			sprintf(msg, "Could not load image. Check the log for details.");
			MessageBoxA(NULL, msg, "TinyImageLoader", MB_OK);

			exit(0);
		}

		g_Load = til::TIL_Load(stream, TIL_DEPTH_A8B8G8R8);

		glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, s_WindowWidth, s_WindowHeight, 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		glClearColor(0.f, 0.f, 0.f, 0.f);
		// Make sure the quads show up
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		// Load the pixel data into the texture

		g_TextureTotal = g_Load->GetFrameCount();
		g_Texture = new GLuint[g_TextureTotal];

		for (unsigned int i = 0; i < g_TextureTotal; i++)
		{
			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, &g_Texture[i]);

			glBindTexture(GL_TEXTURE_2D, g_Texture[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RGBA, 
				g_Load->GetWidth(i), g_Load->GetHeight(i),
				0, 
				GL_RGBA, GL_UNSIGNED_BYTE, g_Load->GetPixels(i)
			);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		g_ScaleX = (float)g_Load->GetWidth(g_TextureCurrent);
		g_ScaleY = (float)g_Load->GetHeight(g_TextureCurrent);
		g_PosX = ((float)s_WindowWidth / 2.f) - (g_ScaleX / 2.f);
		g_PosY = ((float)s_WindowHeight / 2.f) - (g_ScaleY / 2.f);

		/*zlib_filefunc_def ffunc;
		fill_win32_filefunc(&ffunc);
		unzFile uf = unzOpen2("D:\\Projects\\TinyImageLoader2\\examples\\bin\\media\\ZIP\\avatar.zip", &ffunc);

		unz_global_info gi;
		int err;
		
		err = unzGetGlobalInfo(uf,&gi);

		unz_file_info file_info;
		char filename_inzip[256];
		err = unzGetCurrentFileInfo(
			uf, &file_info, 
			filename_inzip, sizeof(filename_inzip), 
			NULL, 0, NULL, 0
		);

		err = unzOpenCurrentFile(uf);

		g_UnzipPath = new char[TIL_MAX_PATH];
		til::TIL_AddWorkingDirectory(g_UnzipPath, TIL_MAX_PATH, "avatar.png");
		fopen_s(&g_Unzip, g_UnzipPath, "w+");
		fclose(g_Unzip);

		fopen_s(&g_Unzip, g_UnzipPath, "wb");

		til::byte* buf = new til::byte[8192];
		//err = unzReadCurrentFile(uf, buf, 8192);
		unsigned int buf_size = 8192;

		do
		{
			err = unzReadCurrentFile(uf, (void*)buf, buf_size);
			if (err < 0)
			{
				//printf("error %d with zipfile in unzReadCurrentFile\n",err);
				break;
			}
			else if (err > 0)
			{
				if (fwrite(buf, err, 1, g_Unzip) != 1)
				{
					//printf("error in writing extracted file\n");
					err = UNZ_ERRNO;
					break;
				}
			}
		}
		while (err > 0);

		fclose(g_Unzip);

		int i = 0;*/
	}

	void Framework::Tick(float a_DT)
	{

	}

	void Framework::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_BLEND); 
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindTexture(GL_TEXTURE_2D, g_Texture[g_TextureCurrent]);

		// Make sure we don't screw up our meticulously designed matrices

		glPushMatrix();
			
			// Push it to the correct position
			glTranslatef(g_PosX, g_PosY, 0.0f);
			// Set the image size
			glScalef(g_ScaleX, g_ScaleY, 1.0f);

			// Draw a quad
			glBegin(GL_QUADS);
				glTexCoord2f(0.f, 1.f); glVertex2f(0.0f, 1.0f);
				glTexCoord2f(1.f, 1.f); glVertex2f(1.0f, 1.0f);
				glTexCoord2f(1.f, 0.f); glVertex2f(1.0f, 0.0f);
				glTexCoord2f(0.f, 0.f); glVertex2f(0.0f, 0.0f);
			glEnd();

		glPopMatrix();

		glDisable(GL_BLEND);
	}

}; // namespace TILFW

#ifndef DOXYGEN_SHOULD_SKIP_THIS

	int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	{
		TILFW::Framework* app = new TILFW::Framework;
		return app->Exec(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	}

#endif
