/*
	TinyImageLoader - load images, just like that

	Copyright (C) 2010 - 2011 by Quinten Lansu
	
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:
	
	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
*/

/*!
	\file ..\SDK\headers\TILImage.h
	\class til::Image "../SDK/headers/TILImage.h"
*/

#include "..\SDK\headers\TILImage.h"

#include <stdlib.h>

namespace til
{

	Image::Image()
	{
		m_Handle = NULL;
		m_FileName = NULL;
	}

	Image::~Image()
	{ 
		Close();
		if (m_FileName) { delete m_FileName; }
	}

	bool Image::Load(const char* a_FileName, uint32 a_Options)
	{
		size_t length = strlen(a_FileName) + 1;
		m_FileName = new char[length];
		strcpy_s(m_FileName, length, a_FileName);

		char path[TIL_MAX_PATH] = { 0 };

		if (a_Options == TIL_FILE_ABSOLUTEPATH)
		{
			strcpy_s(path, length, a_FileName);
		}
		else if (a_Options == TIL_FILE_ADDWORKINGDIR)
		{
			AddWorkingDirectory(path, TIL_MAX_PATH, a_FileName);

			TIL_PRINT_DEBUG("Final path: %s", path);
		}
		
		fopen_s(&m_Handle, path, "rb");
		if (!m_Handle)
		{
			TIL_ERROR_EXPLAIN("Could not open '%s'.", path);
			return false;
		}

		return true;
	}

	bool Image::Close()
	{
		if (m_Handle)
		{
			fclose(m_Handle);
			return true;
		}

		return false;
	}

	void Image::SetBPP(uint32 a_Options)
	{
		switch (a_Options)
		{

		case TIL_DEPTH_A8R8G8B8:
		case TIL_DEPTH_A8B8G8R8:
		case TIL_DEPTH_R8G8B8A8:
		case TIL_DEPTH_B8G8R8A8:
			{
				m_BPP = 4;
				break;
			}
		case TIL_DEPTH_R8G8B8:
			{
				m_BPP = 3;
				break;
			}
			
		case TIL_DEPTH_R5G6B5:
			{
				m_BPP = 2;
				break;
			}

		}	

		m_BPPIdent = (BitDepth)(a_Options >> 16);
	}

}; // namespace til