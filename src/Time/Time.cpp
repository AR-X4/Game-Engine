//--------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "Time.h"

namespace Azul
{
	//---------------------------------------------------------------------------
	// FRIENDS:
	//---------------------------------------------------------------------------
	const Time operator*(const float ratio, const Time &rhs)
	{
		return(Time(static_cast<Time::Representation>((double)ratio * (double)rhs.privRawTime)));
	}

	const Time operator*(const int ratio, const Time &rhs)
	{
		return(Time(ratio * rhs.privRawTime));
	}

	//---------------------------------------------------------------------------
	// CONSTRUCTORS:
	//---------------------------------------------------------------------------

	Time::Time(const Duration duration) : privRawTime(0)
	{
		// IMPORTANT: This is private information that is SUBJECT TO CHANGE!
		//
		// Currently: 1 second = 30000000
		//            1 us     = 30
		//            1 ms     = 30000
		//            1 NTSC   = 500000
		//            1 PAL    = 600000
		//
		// At 32 bits, this gives us a range of roughly -20 to 20 hours.
		const Time::Representation ONE_RAW_SECOND = 30000000;

		switch (duration)
		{
		case Duration::ZERO:
			this->privRawTime = 0;
			break;

		case Duration::NTSC_FRAME:
			this->privRawTime = ONE_RAW_SECOND / 60;
			break;

		case Duration::NTSC_30_FRAME:
			this->privRawTime = 2 * ONE_RAW_SECOND / 60;
			break;

		case Duration::PAL_FRAME:
			this->privRawTime = ONE_RAW_SECOND / 50;
			break;

		case Duration::ONE_SECOND:
			this->privRawTime = ONE_RAW_SECOND;
			break;

		case Duration::ONE_MILLISECOND:
			this->privRawTime = ONE_RAW_SECOND / 1000;
			break;

		case Duration::ONE_MICROSECOND:
			this->privRawTime = ONE_RAW_SECOND / 1000000;
			break;

		case Duration::ONE_MINUTE:
			this->privRawTime = 60 * ONE_RAW_SECOND;
			break;

		case Duration::ONE_HOUR:
			this->privRawTime = 60 * 60 * ONE_RAW_SECOND;
			break;

		case Duration::MIN:
			this->privRawTime = std::numeric_limits< Time::Representation >::min();
			break;

		case Duration::MAX:
			this->privRawTime = std::numeric_limits< Time::Representation >::max();
			break;

		case Duration::DWORD:
			assert(false);
			this->privRawTime = 0;
			break;

		default:
			assert(false);
			this->privRawTime = 0;
			break;
		}
	}

	//---------------------------------------------------------------------------
	// COMPARISONS:
	//---------------------------------------------------------------------------
	bool Time::operator==(const Time &rhs) const
	{
		return(this->privRawTime == rhs.privRawTime);
	}

	bool Time::operator!=(const Time &rhs) const
	{
		return(this->privRawTime != rhs.privRawTime);
	}

	bool Time::operator<(const Time &rhs) const
	{
		return(this->privRawTime < rhs.privRawTime);
	}

	bool Time::operator<=(const Time &rhs) const
	{
		return(this->privRawTime <= rhs.privRawTime);
	}

	bool Time::operator>(const Time &rhs) const
	{
		return(this->privRawTime > rhs.privRawTime);
	}

	bool Time::operator>=(const Time &rhs) const
	{
		return(this->privRawTime >= rhs.privRawTime);
	}

	//---------------------------------------------------------------------------
	// NEGATION / ADDITION / SUBTRACTION:
	//---------------------------------------------------------------------------
	const Time Time::operator-() const
	{
		return(Time(-this->privRawTime));
	}

	const Time Time::operator+(const Time &rhs) const
	{
		// !!! FIXME: Overflow checks
		return(Time(this->privRawTime + rhs.privRawTime));
	}

	const Time Time::operator-(const Time &rhs) const
	{
		// !!! FIXME: Overflow checks
		return(Time(this->privRawTime - rhs.privRawTime));
	}

	Time &Time::operator+=(const Time &rhs)
	{
		// !!! FIXME: Overflow checks
		this->privRawTime += rhs.privRawTime;
		return(*this);
	}

	Time &Time::operator-=(const Time &rhs)
	{
		// !!! FIXME: Overflow checks
		this->privRawTime -= rhs.privRawTime;
		return(*this);
	}

	//---------------------------------------------------------------------------
	// MULTIPLICATION:
	//---------------------------------------------------------------------------
	const Time Time::operator*(const float ratio) const
	{
		// !!! FIXME: Overflow checks
		return(Time(static_cast<Representation>((double)ratio * (double)this->privRawTime)));
	}

	const Time Time::operator*(const int ratio) const
	{
		// !!! FIXME: Overflow checks
		return(Time(this->privRawTime * ratio));
	}

	Time &Time::operator*=(const float ratio)
	{
		// !!! FIXME: Overflow checks
		this->privRawTime = static_cast<Representation>((double)ratio * (double)this->privRawTime);
		return(*this);
	}

	Time &Time::operator*=(const int ratio)
	{
		// !!! FIXME: Overflow checks
		this->privRawTime *= ratio;
		return(*this);
	}


	//---------------------------------------------------------------------------
	// DIVISION:
	//---------------------------------------------------------------------------
	float Time::operator/(const Time &denominator) const
	{
		// !!! FIXME: Divide by zero.
		return(static_cast<float>(this->privRawTime) / (float)denominator.privRawTime);
	}


	const Time Time::operator/(const float denominator) const
	{
		// !!! FIXME: Divide by zero.
		return(Time(static_cast<Representation>((float)this->privRawTime / denominator)));
	}

	const Time Time::operator/(const int denominator) const
	{
		assert((float)denominator != 0.0f);
		return(Time(this->privRawTime / denominator));
	}

	Time &Time::operator/=(const float denominator)
	{
		assert(denominator != 0.0f);
		this->privRawTime = (Representation)((double)this->privRawTime / (double)denominator);
		return(*this);
	}

	Time &Time::operator/=(const int denominator)
	{
		assert((float)denominator != 0.0f);
		this->privRawTime /= denominator;
		return(*this);
	}

	//---------------------------------------------------------------------------
	// DIVISION:
	//---------------------------------------------------------------------------
	int 	Time::Quotient(const Time &numerator, const Time &denominator)
	{
		// !!! FIXME: Divide by zero & check range
		assert((float)denominator.privRawTime != 0.0f);
		return(static_cast<int> (numerator.privRawTime / denominator.privRawTime));
	}

	const Time 	Time::Remainder(const Time &numerator, const Time &denominator)
	{
		return(Time(numerator.privRawTime % denominator.privRawTime));
	}

	//---------------------------------------------------------------------------
	// PRIVATE IMPLEMENTATION:
	//---------------------------------------------------------------------------
	Time::Time(const Time::Representation rawTime) : privRawTime(rawTime)
	{
	}

}

// ---  End of File ---
