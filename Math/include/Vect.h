

#ifndef ENGINE_MATH_VECT_H
#define ENGINE_MATH_VECT_H

// Includes to handle SIMD register types
#include <xmmintrin.h>
#include <smmintrin.h> 
#include "Align16Math.h"

namespace Azul
{
	class Matrix;

	class Vect final : public Align16Math
	{
	public:

		//---Big Four---
		Vect();
		Vect(const Vect& t);
		Vect& operator = (const Vect& t);
		~Vect() = default;

		//---Specialized Constructors---
		Vect(const float& tx, const float& ty, const float& tz, const float& tw = 1.0f);

		//---Overloaded Operators---
		Vect operator + (const Vect & t) const;
		Vect operator - (const Vect & t) const;
		Vect operator * (const float& s) const;
		Vect operator * (const Vect& t) const;
		Vect operator * (const Matrix& m) const;
		Vect& operator *= (const Matrix& m);
		Vect& operator *= (const float& s);
		Vect& operator += (const Vect& v);
		Vect& operator -= (const Vect& v);

		Vect operator + () const;
		Vect operator - () const;

		float& operator [] (const x_enum);
		float& operator [] (const y_enum);
		float& operator [] (const z_enum);
		float& operator [] (const w_enum);

		const float operator [] (const x_enum) const;
		const float operator [] (const y_enum) const;
		const float operator [] (const z_enum) const;
		const float operator [] (const w_enum) const;

		//---Math Functions---
		Vect cross(const Vect& t) const;
		const float dot(const Vect& t) const;
		Vect& norm();
		Vect getNorm() const;
		const float mag() const;
		const float magSqr() const;
		const float getAngle(const Vect& v) const;

		//---Checks---
		const bool isEqual(const Vect& v, const float& ep) const;
		const bool isEqual(const Vect& v) const;
		const bool isZero(const float& ep) const;
		const bool isZero() const;

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
	Vect operator * (const float& s, const Vect& t);
	
}

#endif

// ---  End of File ---------------
