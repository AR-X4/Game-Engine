

#ifndef ENGINE_MATH_TRIG_H
#define ENGINE_MATH_TRIG_H

namespace Azul
{
	class Trig
	{
	public:

		// Do your magic here

		static float cos(const float& f);
		static float sin(const float& f);
		static float tan(const float& f);
		static float atan(const float& f);
		static float atan2(const float& f0, const float& f1);
		static float acos(const float& f);
		static float asin(const float& f);
		static void cossin(float& y, float& z, const float& x);
		static float sqrt(const float& f);
		static float rsqrt(const float& f);


	};
}

#endif

// ---  End of File ---------------
