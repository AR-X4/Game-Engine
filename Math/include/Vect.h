

#ifndef ENGINE_MATH_VECT_H
#define ENGINE_MATH_VECT_H

// Includes to handle SIMD register types
#include <xmmintrin.h>
#include <smmintrin.h> 

#include "Constants.h"

// This file assumes Framework.h is included in executable for Align16

namespace Azul
{
	// forward declare
	class Matrix;
	class Quat;

	class Vect final : public Align16
	{
	public:

		//---Big Four---
		Vect();
		Vect(const Vect& t);
		Vect& operator = (const Vect& t);
		~Vect() = default;

		//---Specialized Constructors---
		Vect(const float& tx, const float& ty, const float& tz, const float& tw = 1.0f);
		Vect(const Quat& quatIn);

		//---Overloaded Operators---
		const Vect operator + (const Vect& t) const;
		const Vect operator - (const Vect& t) const;
		const Vect operator * (const float& s) const;
		const Vect operator * (const Vect& t) const;
		const Vect operator * (const Matrix& m) const;
		const Vect operator * (const Quat& q) const;
		Vect& operator *= (const Matrix& m);
		Vect& operator *= (const float& s);
		Vect& operator *= (const Quat& q);
		Vect& operator += (const Vect& v);
		Vect& operator -= (const Vect& v);

		Vect& operator /= (const float& s);

		const Vect operator + () const;
		const Vect operator - () const;

		float& operator [] (const x_enum);
		float& operator [] (const y_enum);
		float& operator [] (const z_enum);
		float& operator [] (const w_enum);

		const float operator [] (const x_enum) const;
		const float operator [] (const y_enum) const;
		const float operator [] (const z_enum) const;
		const float operator [] (const w_enum) const;

		//---Math Functions---
		const Vect cross(const Vect& t) const;
		const float dot(const Vect& t) const;
		Vect& norm();
		const Vect getNorm() const;
		const float mag() const;
		const float magSqr() const;
		const float getAngle(const Vect& v) const;

		//---Checks---
		const bool isEqual(const Vect& v, const float& ep = MATH_TOLERANCE) const;
		const bool isZero(const float& ep = MATH_TOLERANCE) const;

		//---Setters---
		Vect& set(const float& x, const float& y, const float& z);
		Vect& set(const float& x, const float& y, const float& z, const float& w);
		Vect& set(const Vect& v);

		//---Accessors---
		const float x() const;
		const float y() const;
		const float z() const;
		const float w() const;

		float& x();
		float& y();
		float& z();
		float& w();

	private:

		union
		{
			__m128	_mv;

			// anonymous struct
			struct
			{
				float _vx;
				float _vy;
				float _vz;
				float _vw;
			};
		};
	};

	//non-member functions
	const Vect operator * (const float& s, const Vect& t);
}

#endif

// ---  End of File ---------------
