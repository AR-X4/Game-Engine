//-----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include <math.h>    // <--- ONLY location for <math.h> allowed
#include "Trig.h"

namespace Azul
{

	// Do your magic here
	float Trig::cos(const float& f)  { return cosf(f); }
	float Trig::sin(const float& f)  { return sinf(f); }
	float Trig::tan(const float& f)  { return tanf(f); }
	float Trig::atan(const float& f) { return atanf(f); }
	float Trig::atan2(const float& f0, const float& f1) { return atan2f(f0, f1); }
	float Trig::acos(const float& f) { return acosf(f); }
	float Trig::asin(const float& f) { return asinf(f); }
	float Trig::sqrt(const float& f) { return sqrtf(f); }
	float Trig::rsqrt(const float& f) { return 1.0f/sqrtf(f); }

	void Trig::cossin(float& y, float& z, const float& x) {
		y = cosf(x);
		z = sinf(x);
	}

}

// ---  End of File ---------------
