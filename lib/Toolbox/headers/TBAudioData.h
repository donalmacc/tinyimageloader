#ifndef _TBAUDIODATA_H_
#define _TBAUDIODATA_H_

#include "TBVec3.h"

namespace tb
{
	
	struct WorldData
	{
		Vec3 m_Position;
		float m_Near, m_Far;
	};

	struct Volume3D
	{
		union
		{
			struct { float volumeRonL, volumeRonR, volumeLonL, volumeLonR; };
			float volume[4];
		};
	};

	struct Complex
	{
		union
		{
			struct { float sine; float cosine; };
			struct { float real; float imaginary; };
			float val[2];
		};
	};

}; // namespace tb

#endif