#ifndef _TBLISTENER_H_
#define _TBLISTENER_H_

#include "TBVec3.h"
#include "TBAudioData.h"

namespace tb
{

	struct AudioManager;
	struct WorldData;

	class Listener
	{

		friend struct AudioManager;

	public:

		Listener(Vec3& a_Position, Vec3& a_Direction);
		~Listener();

		void SetPosition(Vec3& a_Position);
		void SetDirection(Vec3& a_Direction);

		void Translate(Vec3& a_Speed);

	private:

		Volume3D GetVolume3D(WorldData* a_Data);

		Vec3 m_Position;
		Vec3 m_Forward, m_Up;
		Vec3 m_EarRight, m_EarLeft;
		
	}; // class Listener

}; // namespace tb

#endif