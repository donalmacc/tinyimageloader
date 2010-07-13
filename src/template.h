// Template, major revision 3, beta
// IGAD/NHTV - Jacco Bikker - 2006-2009

#pragma once

#include "Settings.h"
// #define POSTPROCSHADERS

#include "math.h"
#include "stdlib.h"
#include "emmintrin.h"
#include "stdio.h"
#include "windows.h"

inline float Rand( float a_Range ) { return ((float)rand() / RAND_MAX) * a_Range; }
int filesize( FILE* f );
#define MALLOC64(x) _aligned_malloc(x,64)
#define FREE64(x) _aligned_free(x)

namespace Tmpl8 
{

#ifndef __INTEL_COMPILER
#define restrict
#endif

#define _fabs	fabsf
#define _cos	cosf
#define _sin	sinf
#define _acos	acosf
#define _floor	floorf
#define _ceil	ceilf
#define _sqrt	sqrtf
#define _pow	powf
#define _exp	expf

#define CROSS(A,B)		vector3(A.y*B.z-A.z*B.y,A.z*B.x-A.x*B.z,A.x*B.y-A.y*B.x)
#define DOT(A,B)		(A.x*B.x+A.y*B.y+A.z*B.z)
#define NORMALIZE(A)	{float l=1/_sqrt(A.x*A.x+A.y*A.y+A.z*A.z);A.x*=l;A.y*=l;A.z*=l;}
#define CNORMALIZE(A)	{float l=1/_sqrt(A.r*A.r+A.g*A.g+A.b*A.b);A.r*=l;A.g*=l;A.b*=l;}
#define LENGTH(A)		(_sqrt(A.x*A.x+A.y*A.y+A.z*A.z))
#define SQRLENGTH(A)	(A.x*A.x+A.y*A.y+A.z*A.z)
#define SQRDISTANCE(A,B) ((A.x-B.x)*(A.x-B.x)+(A.y-B.y)*(A.y-B.y)+(A.z-B.z)*(A.z-B.z))



#define PREFETCH(x) _mm_prefetch((const char*)(x),_MM_HINT_T0)
#define PREFETCH_ONCE(x) _mm_prefetch((const char*)(x),_MM_HINT_NTA)
#define PREFETCH_WRITE(x) _m_prefetchw((const char*)(x))

#define loadss(mem)			_mm_load_ss((const float*const)(mem))
#define broadcastps(ps)		_mm_shuffle_ps((ps),(ps), 0)
#define broadcastss(ss)		broadcastps(loadss((ss)))

	class Thread 
	{
	private:
		unsigned long* m_hThread;
	public:
		Thread() { m_hThread = NULL; }
		~Thread() { if (m_hThread != NULL) stop(); }
		unsigned long* handle() { return m_hThread; }
		void start();
		virtual void run() {};
		void sleep(long ms);
		void suspend();
		void resume();
		void kill();
		void stop();
		void setPriority(int p);
		static const int P_ABOVE_NORMAL;
		static const int P_BELOW_NORMAL;
		static const int P_HIGHEST;
		static const int P_IDLE;
		static const int P_LOWEST;
		static const int P_NORMAL;
		static const int P_CRITICAL;
	};

	extern "C" { unsigned int sthread_proc(void* param); }

}; // namespace Tmpl8
