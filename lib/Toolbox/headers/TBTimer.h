	// Code by:
// Quinten Lansu (knight666)
// Copyright 2008-2010

#ifndef _TBTIMER_H_
#define _TBTIMER_H_

#include "TBSettings.h"

#include "windows.h"

#ifndef TB_TIMER_PRECISION
	
	#define TB_TIMER_PRECISION     TB_TIMER_PRECISION_NORMAL

#endif

#if (TB_TIMER_PRECISION == TB_TIMER_PRECISION_HIGH)

	#include <mmsystem.h>
	typedef __int64 TimeStack;

#else

	#include <winbase.h>
	typedef unsigned long TimeStack;

#endif

#if (TB_TIMER_PRECISION == TB_TIMER_PRECISION_NORMAL)

	#define TB_GETTIME() timeGetTime()

#elif (TB_TIMER_PRECISION == TB_TIMER_PRECISION_LOW)

	#define TB_GETTIME() GetTickCount()

#endif

namespace tb
{

	class Timer
	{

	public:

		Timer::Timer(int a_Samples = 1)
		{
			m_Samples = a_Samples;
			m_Result = 0;

#if (TB_TIMER_PRECISION == TB_TIMER_PRECISION_HIGH)

			QueryPerformanceFrequency(&m_LargeEnd);
			m_Frequency = 1000.f / (float)m_LargeEnd.QuadPart;

#endif

			Reset();
		}

		Timer::~Timer()
		{

		}

		void Timer::Start()
		{

#if (TB_TIMER_PRECISION == TB_TIMER_PRECISION_HIGH)

			QueryPerformanceCounter(&m_LargeStart);

#else

			m_Start = TB_GETTIME();

#endif

		}

		void Timer::End()
		{

#if (TB_TIMER_PRECISION == TB_TIMER_PRECISION_HIGH)

			QueryPerformanceCounter(&m_LargeEnd); 
			m_Total += m_LargeEnd.QuadPart - m_LargeStart.QuadPart; 

#else

			m_Total += TB_GETTIME() - m_Start;

#endif

		}

		void Timer::Reset()
		{
			m_Passes = 0;
			m_Average = 0;
			m_Total = 0;
		}

		inline float GetAverage()
		{
			if (++m_Passes == m_Samples)
			{
				m_Average = (float)(m_Total) / (float)(m_Samples);

#if (TB_TIMER_PRECISION == TB_TIMER_PRECISION_HIGH)

				m_Average *= m_Frequency;

#endif

				m_Total = 0;
				m_Passes = 0;
			}

			return m_Average;
		}

		inline float GetFPS()
		{
			float ms = GetAverage();
			float fps = 0;

			if (ms > 0) { fps = 1000.f / ms; }

			return fps;
		}

		inline int GetDifference(bool a_Reset = false)
		{
			if (++m_Passes == m_Samples)
			{
				m_Result = m_Total;
				if (a_Reset) { Reset(); }
			}

			return (int)m_Result;
		}

	private:

#if (TB_TIMER_PRECISION == TB_TIMER_PRECISION_HIGH)

		LARGE_INTEGER m_LargeStart;
		LARGE_INTEGER m_LargeEnd;
		float m_Frequency;

#endif

		TimeStack m_Total;
		TimeStack m_Start;
		TimeStack m_Result;

		int m_Passes;
		int m_Samples;
		float m_Average;

	}; // class Timer

}; // namespace tb

#endif