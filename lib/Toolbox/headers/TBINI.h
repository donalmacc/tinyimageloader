#ifndef _INILOADER_H_
#define _INILOADER_H_

#include "TBSettings.h"

#include <vector>

namespace tb
{

	class INIParameter;
	class INISection;

	typedef std::vector<INIParameter>::iterator		INIParamIter;
	typedef std::vector<INISection>::iterator		INISectIter;
	typedef std::vector<INISection>					INISectList;

	class INIParameter
	{

	public:

		INIParameter();
		INIParameter(char* a_Name, char* a_Value);
		~INIParameter();

		bool IsName(char* a_String) { return (!strcmp(m_Name, a_String)); }

		char* GetName() { return m_Name; }
		char* GetValue() { return m_Value; }

		// NOTE: Works like sscanf.
		void Parse(char* a_Pattern, ...);

		void SetName(char* a_Name) { strcpy(m_Name, a_Name); }
		void SetValue(char* a_Value) { strcpy(m_Value, a_Value); }

	private:

		char m_Name[256];
		char m_Value[256];

	}; // class INIElement

	class INISection
	{

	public:

		INISection();
		~INISection();

		bool Compare(char* a_String) { return (!strcmp(m_Name, a_String)); }

		char* GetName() { return m_Name; }
		void SetName(char* a_Name) { strcpy(m_Name, a_Name); }

		void AddParameter(INIParameter& a_Parameter);

		INIParameter* GetParameterByName(char* a_Name);

		INIParamIter LoopBegin();
		INIParamIter LoopEnd();

	private:

		char m_Name[256];
		std::vector<INIParameter> m_Elements;

	}; // class INISection

	class INIRoot
	{

	public:

		INIRoot();
		INIRoot(char* a_FileName);
		~INIRoot();

		void Parse(char* a_FileName);
		void Clean();

		INISection* GetSectionByName(char* a_Name);
		INISectList GetSectionList(char* a_Name);

		INISectIter LoopBegin();
		INISectIter LoopEnd();

	private:

		std::vector<INISection> m_Sections;

	}; // class INIRoot

}; // namespace tb

#endif