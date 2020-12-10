

#include "TimerController.h"

namespace Azul
{

	TimerController::TimerController(Time Delta, Time Min, Time Max)
		: tCurr(Time::Duration::ZERO),
		tDelta(Delta),
		tMax(Max),
		tMin(Min)
	{
	}

	void TimerController::Update()
	{
		tCurr += tDelta;

		// protection for time values for looping
		if (tCurr < tMin)
		{
			tCurr = tMax;
		}
		else if (tCurr > tMax)
		{
			tCurr = tMin;
		}
		else
		{
			// do nothing
		}
	}

	void TimerController::SetCurrTime(const Time time)
	{
		this->tCurr = time;
	}

	Time TimerController::GetCurrTime() const
	{
		return this->tCurr;
	}

	void TimerController::SetDeltaTime(const Time time)
	{
		this->tDelta = time;
	}

	Time TimerController::GetDeltaTime() const
	{
		return this->tDelta;
	}

	void TimerController::SetMaxTime(const Time time)
	{
		this->tMax = time;
	}

	Time TimerController::GetMaxTime() const
	{
		return this->tMax;
	}

	void TimerController::SetMinTime(const Time time)
	{
		this->tMin = time;
	}

	Time TimerController::GetMinTime() const
	{
		return this->tMin;
	}

}

// --- End of File ---
