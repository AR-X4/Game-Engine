

#ifndef ENGINE_MATH_MATRIX_H
#define ENGINE_MATH_MATRIX_H

#include "Vect.h"

namespace Azul
{
	//Align instances to 16 bytes for anonymous union data struct
	class Matrix final : public Align16Math
	{
	public:

		//---Enums---
		enum class Special
		{
			Zero,
			Identity
		};

		enum class Rot1
		{
			X,
			Y,
			Z
		};

		enum class Trans
		{
			XYZ
		};

		enum class Scale
		{
			XYZ
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

		enum class Row
		{
			i0,
			i1,
			i2,
			i3
		};

		enum class Rot3
		{
			XYZ
			// future combos... here
		};

	//public:

		//---Big Four---
		Matrix();
		Matrix(const Matrix & tmp); // copy constructor
		Matrix& operator=(const Matrix & tmp);// move assignment operator
		~Matrix() = default;

		//---Specialized Constructors---
		Matrix(const Vect & tv0, const Vect & tv1, const Vect & tv2, const Vect & tv3);
		Matrix(const Orient InEnum, const Vect& zAxisIn, const Vect& yAxisIn);//constructor with world orientation
		Matrix(const Rot, const Vect& RotAxis, const float& RotAngle);
		Matrix(const Rot1 InEnum, const float& RotAngle);
		Matrix(const Rot3, const float& RotX, const float& RotY, const float& RotZ);
		Matrix(const Trans, const float& TransX, const float& TransY, const float& TransZ);
		Matrix(const Trans, const Vect& TransV);
		Matrix(const Scale, const float& ScaleX, const float& ScaleY, const float& ScaleZ);
		Matrix(const Scale, const Vect& ScaleV);
		Matrix(const Special InEnum);

		//---Math Functions---
		float det() const;		   //Determinent
		Matrix& T();			   //Transpose
		Matrix getT() const;	   //Get Transpose
		Matrix& inv();			   //Inverse
		Matrix getInv() const;     //Get inverse
		Matrix getAdjugate() const;//Get Adjugate

		//---Checks---
		const bool isIdentity(const float& ep) const;
		const bool isIdentity() const;
		const bool isEqual(const Matrix& t) const;

		//---Setters---
		Matrix& set(const Special InEnum);
		Matrix& set(const Trans, const float& TransX, const float& TransY, const float& TransZ);
		Matrix& set(const Trans, const Vect& TransV);
		Matrix& set(const Scale, const float& ScaleX, const float& ScaleY, const float& ScaleZ);
		Matrix& set(const Scale, const Vect& ScaleV);
		Matrix& set(const Rot1 InEnum, const float& RotAngle);
		Matrix& set(const Row InEnum, const Vect& RowV);
		Matrix& set(const Rot3, const float& RotX, const float& RotY, const float& RotZ);
		Matrix& set(const Rot, Vect& RotAxis, const float& RotAngle);
		Matrix& set(const Orient InEnum, const Vect& DOF, const Vect& yAxis);
		Matrix& set(const Vect& Row0, const Vect& Row1, const Vect& Row2, const Vect& Row3);

		//---Getters---
		Vect get(const Row InEnum) const;

		//---Overloaded Operators---
		Matrix operator * (const Matrix & t) const;
		Matrix operator * (const float& s)   const;
		Matrix operator + (const Matrix& t)  const;
		Matrix operator - (const Matrix& t)  const;
		Matrix& operator += (const Matrix& t);
		Matrix& operator -= (const Matrix& t);
		Matrix& operator *= (const Matrix& t);
		Matrix& operator *= (const float& s);
		//unary
		Matrix operator + () const;
		Matrix operator - () const;

		float& operator [] (const m0_enum );
		float& operator [] (const m1_enum );
		float& operator [] (const m2_enum );
		float& operator [] (const m3_enum );
		float& operator [] (const m4_enum );
		float& operator [] (const m5_enum );
		float& operator [] (const m6_enum );
		float& operator [] (const m7_enum );
		float& operator [] (const m8_enum );
		float& operator [] (const m9_enum );
		float& operator [] (const m10_enum);
		float& operator [] (const m11_enum);
		float& operator [] (const m12_enum);
		float& operator [] (const m13_enum);
		float& operator [] (const m14_enum);
		float& operator [] (const m15_enum);
		
		const float operator [] (const m0_enum ) const;
		const float operator [] (const m1_enum ) const;
		const float operator [] (const m2_enum ) const;
		const float operator [] (const m3_enum ) const;
		const float operator [] (const m4_enum ) const;
		const float operator [] (const m5_enum ) const;
		const float operator [] (const m6_enum ) const;
		const float operator [] (const m7_enum ) const;
		const float operator [] (const m8_enum ) const;
		const float operator [] (const m9_enum ) const;
		const float operator [] (const m10_enum) const;
		const float operator [] (const m11_enum) const;
		const float operator [] (const m12_enum) const;
		const float operator [] (const m13_enum) const;
		const float operator [] (const m14_enum) const;
		const float operator [] (const m15_enum) const;


		//---Data Accessors---
		float& m0();
		float& m1();
		float& m2();
		float& m3();
		float& m4();
		float& m5();
		float& m6();
		float& m7();
		float& m8();
		float& m9();
		float& m10();
		float& m11();
		float& m12();
		float& m13();
		float& m14();
		float& m15();

		float m0() const;
		float m1() const;
		float m2() const;
		float m3() const;
		float m4() const;
		float m5() const;
		float m6() const;
		float m7() const;
		float m8() const;
		float m9() const;
		float m10() const;
		float m11() const;
		float m12() const;
		float m13() const;
		float m14() const;
		float m15() const;
		

	private:

		//---Priv Helpers... for duplicate code in specialized constructors and setters---
		void SetRotAxisAngle(const Vect& RotAxis, const float& RotAngle);
		void SetOrientation(const Orient InEnum, const Vect& zAxisIn, const Vect& yAxisIn);
		void SetSpecial(const Special InEnum);
		void SetTrans(const Vect& TransV);
		void SetScale(const Vect& ScaleV);
		void SetRotation(const float& RotX, const float& RotY, const float& RotZ);
		void SetAxisRotation(const Rot1 InEnum, const float& RotAngle);

		//---Data---
		union
		{
			struct
			{
				Vect _v0;
				Vect _v1;
				Vect _v2;
				Vect _v3;
			};

			struct
			{
				float _m0;
				float _m1;
				float _m2;
				float _m3;
				float _m4;
				float _m5;
				float _m6;
				float _m7;
				float _m8;
				float _m9;
				float _m10;
				float _m11;
				float _m12;
				float _m13;
				float _m14;
				float _m15;
			};
		};
	};

	//non-member functions
	Matrix operator * (const float& s, const Matrix& t);
}

#endif

// ---  End of File ---------------
