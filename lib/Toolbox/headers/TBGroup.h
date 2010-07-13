#ifndef _TBGROUP_H_
#define _TBGROUP_H_

#include "TBSubMix.h"

namespace tb
{

	class Group : public SubMix
	{

	public:
		
		Group(const char* a_Name);
		~Group();

		bool AddBlock(float* a_Target);

		const char* GetName() { return m_Name; }

	private:

		char m_Name[256];

	}; // class Group

}; // namespace tb

#endif