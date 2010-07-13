#ifndef _TBSOUND3D_H_
#define _TBSOUND3D_H_

#include "TBSound.h"

namespace tb
{

	class Sound3D : public Sound
	{

		friend struct AudioManager;

	protected:

		Sound3D(SoundData* a_Data);
		~Sound3D();

		bool AddBlock(float* a_Target);
		
	}; // class Sound3D

}; // namespace tb

#endif