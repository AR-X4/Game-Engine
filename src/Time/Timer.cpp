//--------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "Timer.h"

namespace Azul
{
	//---------------------------------------------------------------------------
	// CONSTRUCTORS / DESTRUCTORS / ASSIGNMENT:
	//---------------------------------------------------------------------------
	Timer::Timer() :
		privTicMark(Time::Duration::MAX)
	{ }


	Timer::~Timer()
	{ }


	//---------------------------------------------------------------------------
	// TIMING METHODS:
	//---------------------------------------------------------------------------
	void Timer::Tic()
	{
		this->privTicMark = Timer::privGetSystemTime();
	}


	const Time Timer::Toc() const
	{
		Time elapsedTime;

		// If tick has been called...
		if (Time(Time::Duration::MAX) != this->privTicMark)
		{
			elapsedTime = Timer::privGetSystemTime() - this->privTicMark;
		}

		return(elapsedTime);
	}
}

// ---  End of File ---
