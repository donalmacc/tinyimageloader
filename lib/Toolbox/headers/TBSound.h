#ifndef _TBSOUND_H_
#define _TBSOUND_H_

#include "TBSource.h"
#include "TBAudioData.h"

#ifndef _WINDOWS_

	#define WIN32_LEAN_AND_MEAN
	#include "windows.h"

#endif

namespace tb
{

	struct AudioManager;

	struct SoundData
	{
		char m_FileName[256];
		char m_Name[256];

		float* m_Points;
		UINT m_TotalPoints;
		UINT m_TotalBlocks;

		int m_Channels;
	};

	struct ChannelData;

	class Sound : public Source
	{

		friend struct AudioManager;

	public:

		void Set3D();

		void Play();
		void Pause();
		void Stop();

		bool IsPlaying();
		bool IsPaused();
		bool IsLooping();

		void SetVolume(float a_Volume);
		void ChangeVolume(float a_VolumeChange);
		void SetLooping(bool a_State);

		void SetPosition(Vec3& a_Position);
		void SetDistance(float a_Near, float a_Far);

	protected:

		Sound(SoundData* a_Data);
		~Sound();

		virtual bool AddBlock(float* a_Target);

		SoundData* m_Data;
		ChannelData* m_Channel;
		WorldData* m_World;

	}; // class Sound

}; // namespace tb

#endif