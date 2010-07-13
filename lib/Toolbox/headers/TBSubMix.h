#ifndef _TBSUBMIX_H_
#define _TBSUBMIX_H_

#include "TBSource.h"

#include <vector>

namespace tb
{

	struct AudioManager;

	class SubMix : public Source
	{

		friend struct AudioManager;

	public:

		SubMix();
		~SubMix();

		void AddChild(Source* a_Source);

	protected:

		void Tick(float* a_Target);
		virtual bool AddBlock(float* a_Target) { return false; }

		std::vector<Source*> m_Children;

	}; // class SubMix

}; // namespace tb

#endif