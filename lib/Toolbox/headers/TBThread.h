#ifndef _TBTHREAD_H_
#define _TBTHREAD_H_

#include "TBSettings.h"

#include <wtypes.h>

namespace tb
{

	class Thread 
	{

	public:

		enum
		{
			P_IDLE			= THREAD_PRIORITY_IDLE,
			P_LOWEST		= THREAD_PRIORITY_LOWEST,
			P_BELOW_NORMAL	= THREAD_PRIORITY_BELOW_NORMAL,
			P_NORMAL		= THREAD_PRIORITY_NORMAL,
			P_ABOVE_NORMAL	= THREAD_PRIORITY_ABOVE_NORMAL,
			P_HIGHEST		= THREAD_PRIORITY_HIGHEST,
			P_CRITICAL		= THREAD_PRIORITY_TIME_CRITICAL
		};

		Thread() { m_Thread = NULL; }
		virtual ~Thread() { if (m_Thread) { Kill(); } }

		HANDLE GetHandle() { return m_Thread; }
		void Start();
		void Stop();
		virtual void Run() {};
		void Wait(long a_MilliSeconds);
		void Suspend();
		void Resume();
		void Kill();

		void SetPriority(int a_Priority);

	private:

		HANDLE m_Thread;

	}; // class Thread

}; // namespace tb

#endif