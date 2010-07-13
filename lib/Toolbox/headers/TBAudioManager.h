#ifndef _TBAUDIOMANAGER_H_
#define _TBAUDIOMANAGER_H_

//#include "TBSettings.h"
#include "TBMath.h"
#include "TBVec3.h"
#include "TBSound.h"
#include "TBListener.h"

// forward declarations

struct HWAVEOUT__; 
typedef struct HWAVEOUT__* HWAVEOUT;

namespace tb
{

	class Group;
	class Sound;
	struct WorldData;

	struct AudioManager
	{

		enum SampleRate
		{
			SR_HIGH    = 44100,
			SR_MEDIUM  = 22050,
			SR_LOW     = 11025
		};

		enum BitRate
		{
			BR_HIGH    = 32,
			BR_MEDIUM  = 16,
			BR_LOW     = 8
		};

		enum Channels
		{
			CH_STEREO  = 2,
			CH_MONO    = 1
		};

		enum Master
		{
			MASTER_LEFT,
			MASTER_RIGHT
		};

		static void Init (
			SampleRate a_SampleRate = SR_HIGH, 
			BitRate a_BitRate = BR_MEDIUM, 
			Channels a_Channels = CH_STEREO
		);
		static bool Load(const char* a_FileName, const char* a_Name);
		static void Close();

		static Group* CreateGroup(const char* a_Name);
		static void Register(Group* a_Group);
		static Group* GetGroup(const char* a_Name);

		static Sound* GetInstance(const char* a_Name, const char* a_Group = "Default");
		static Sound* Play(const char* a_Name, const char* a_Group = "Default");
		static void Play3D(const char* a_Name);

		static void SetMasterVolume(float a_Volume);

		// 3D Sound properties

		static void SetListener(Listener* a_Listener);
		static Listener* GetListener();
		static Volume3D GetVolume3D(WorldData* a_Data);

		// Normal sound functions

		static bool Play(Sound* a_Sound);
		static bool Pause(Sound* a_Sound);
		static bool Stop(Sound* a_Sound);

		static bool IsPlaying(Sound* a_Sound);
		static bool IsPaused(Sound* a_Sound);
		static bool IsLooping(Sound* a_Sound);

		static bool SetVolume(Sound* a_Sound, float a_Volume);

		static float g_MasterVolume[2];
		static int g_BlockAmount;
		static UINT g_BlockSize;

	private:

		static BOOL CALLBACK WaveOutProc(HWAVEOUT a_WaveOut, UINT a_Msg, DWORD a_Instance, DWORD a_Parameter1, DWORD a_Parameter2);
		static bool BuildBlock(int a_Block);
		static void BuildNext();
		static void PlayBlock();

		static void DeleteInstance(Sound* a_Instance);
	
	}; // struct AudioManager

}; // namespace tb

#endif