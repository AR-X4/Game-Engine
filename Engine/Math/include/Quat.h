

#ifndef ENGINE_MATH_QUAT_H
#define ENGINE_MATH_QUAT_H

#include "Matrix.h"

//------------------------------------------------------------------------
// 
//  This class contains functions and data structure for a 4-dimensional
//  Quaternions.  The quaternions are internally stored interms of the
//  imaginary components (x,y,z) and the real components (real).
// 
//  In the Math library, quaternions can be thought of as "rotational"
//  matrices, everything that you can represent with rotational matrices
//  you can use with quaternions.  Such as quaternion concatenation,
//  inverses, transpose, normalize, multiplying with matrices, and with
//  vectors.
// 
//------------------------------------------------------------------------
namespace Azul
{
	class Quat final : public Align16
	{
	public:
		enum class Rot1
		{
			X,
			Y,
			Z
		};

		enum class Special
		{
			Zero,
			Identity
		};

		enum class Rot
		{
			AxisAngle,
		};

		enum class Orient
		{
			LocalToWorld,
			WorldToLocal
		};

		enum class Rot3
		{
			XYZ
			// future combos... here
		};

	public:

		//---Big Four---
		Quat();
		~Quat();
		Quat(const Quat& other);
		Quat& operator=(const Quat& other);

		//---Specialized Constructors---
		Quat(const float& qx, const float& qy, const float& qz, const float& qw);
		Quat(const Vect& qv, const float& qs);
		Quat(const Matrix& RotMatrix);
		Quat(const Special type);
		Quat(const Rot1 axis, const float& rotAngle);
		Quat(const Rot3, const float& xRot, const float& yRot, const float& zRot);
		Quat(const Rot, const Vect& rotAxis, const float& rotAngle);
		Quat(const Orient orient, const Vect& forward, const Vect& up);
		

		//---Accessors---
		//Setters
		Quat& set(const Matrix& RotMatrix);
		Quat& set(const float& qx, const float& qy, const float& qz, const float& qw);
		Quat& set(const Rot1 axis, const float& rotAngle);
		Quat& set(const Rot3, const float& xRot, const float& yRot, const float& zRot);
		Quat& set(const Vect& qv, const float& qs);
		Quat& set(const Rot, const Vect& rotAxis, const float& rotAngle);
		Quat& set(const Orient orient, const Vect& forward, const Vect& up);
		Quat& set(const Special type);

		Quat& setVect(const Vect& v);


		float& qx();
		float& qy();
		float& qz();
		float& real();

		float& operator[](const x_enum);
		float& operator[](const y_enum);
		float& operator[](const z_enum);
		float& operator[](const w_enum);


		//Getters
		void getVect(Vect& v) const;
		void getAxis(Vect& v) const;
		const float getAngle() const;

		const float operator[](const x_enum) const;
		const float operator[](const y_enum) const;
		const float operator[](const z_enum) const;
		const float operator[](const w_enum) const;

		const float qx() const;
		const float qy() const;
		const float qz() const;
		const float real() const;

		//---Operators---
		const Quat operator+() const;
		const Quat operator+(const Quat& other) const;
		const Quat operator+(const float scalar) const;
		Quat& operator+=(const Quat& other);
		Quat& operator+=(const float scalar);

		const Quat operator-();
		const Quat operator-(const Quat& other) const;
		const Quat operator-(const float scalar) const;
		Quat& operator-=(const Quat& other);
		Quat& operator-=(const float scalar);

		const Quat operator*(const Quat& other) const;
		const Quat operator*(const float scalar) const;
		const Matrix operator*(const Matrix& other) const;
		Quat& operator*=(const Quat& other);
		Quat& operator*=(const float scalar);

		const Quat operator/(const Quat& other) const;
		const Quat operator/(const float scalar) const;
		Quat& operator/=(const Quat& other);
		Quat& operator/=(const float scalar);

		const Quat multByElement(const Quat& other);

		//---Comparison Functions---
		const bool isEqual(const Quat& q, const float& tolerance = MATH_TOLERANCE) const;
		const bool isNegEqual(const Quat& q, const float& tolerance = MATH_TOLERANCE) const;
		const bool isEquivalent(const Quat& q, const float& tolerance = MATH_TOLERANCE) const;
		const bool isConjugateEqual(const Quat& q, const float& tolerance = MATH_TOLERANCE) const;
		const bool isIdentity(const float& tolerance = MATH_TOLERANCE) const;
		const bool isZero(const float& tolerance = MATH_TOLERANCE) const;
		const bool isNormalized(const float& tolerance = MATH_TOLERANCE) const;

		//---Math Functions---
		const Quat getConj() const;
		Quat& conj();

		const Quat getT() const;
		Quat& T();

		const float mag() const;
		const float magSquared() const;
		const float invMag() const;

		Quat& norm();
		const Quat getNorm() const;

		Quat& inv();
		const Quat getInv() const;

		void Lqcvq(const Vect& vIn, Vect& vOut) const;
		void Lqvqc(const Vect& vIn, Vect& vOut) const;

		const float dot(const Quat& other) const;

	private:

		// anonymous union
		union
		{
			__m128	_mq;

			// anonymous struct
			struct
			{
				float _qx;
				float _qy;
				float _qz;
				float _qw;
			};
		};

	};

	//non-member functions
	Quat operator + (const float& scalar, const Quat& other);
	Quat operator - (const float& scalar, const Quat& other);
	Quat operator / (const float& scalar, const Quat& other);
	Quat operator * (const float& scalar, const Quat& other);

}

#endif 

// ---  End of File ---------------
