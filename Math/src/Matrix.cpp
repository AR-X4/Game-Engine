//-----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"

namespace Azul
{
	// Do your magic here
	//---Big Four---
	Matrix::Matrix() 
		: _m0(0.0f), _m1(0.0f), _m2(0.0f), _m3(0.0f), _m4(0.0f), 
		  _m5(0.0f), _m6(0.0f), _m7(0.0f), _m8(0.0f), _m9(0.0f), 
		  _m10(0.0f), _m11(0.0f), _m12(0.0f), _m13(0.0f), _m14(0.0f), _m15(0.0f)
	{}

	Matrix::Matrix(const Matrix& tmp) {

		this->_v0 = tmp._v0;
		this->_v1 = tmp._v1;
		this->_v2 = tmp._v2;
		this->_v3 = tmp._v3;
	}

	Matrix& Matrix::operator = (const Matrix& tmp) {
		
		this->_v0 = tmp._v0;
		this->_v1 = tmp._v1;
		this->_v2 = tmp._v2;
		this->_v3 = tmp._v3;

		return *this;
	}

	//---Specialized Constructors---
	Matrix::Matrix(const Vect& tv0, const Vect& tv1, const Vect& tv2, const Vect& tv3)
		:_v0(tv0), _v1(tv1), _v2(tv2), _v3(tv3)
	{};

	Matrix::Matrix(const Orient InEnum, const Vect& zAxisIn, const Vect& yAxisIn) {
		
		this->SetOrientation(InEnum, zAxisIn, yAxisIn);
	}

	Matrix::Matrix(const Rot, const Vect& RotAxis, const float& RotAngle) {

		this->SetRotAxisAngle(RotAxis, RotAngle);
	}

	Matrix::Matrix(const Rot1 InEnum, const float& RotAngle) {

		this->SetAxisRotation(InEnum, RotAngle);
	}

	Matrix::Matrix(const Rot3, const float& RotX, const float& RotY, const float& RotZ) {
	
		this->SetRotation(RotX, RotY, RotZ);
	}

	Matrix::Matrix(const Trans, const float& TransX, const float& TransY, const float& TransZ) {


		//set xAxis
		this->_m0 = 1.0f;
		this->_m1 = 0.0f;
		this->_m2 = 0.0f;
		this->_m3 = 0.0f;

		//set yAxis
		this->_m4 = 0.0f;
		this->_m5 = 1.0f;
		this->_m6 = 0.0f;
		this->_m7 = 0.0f;

		//set zAxis
		this->_m8 = 0.0f;
		this->_m9 = 0.0f;
		this->_m10 = 1.0f;
		this->_m11 = 0.0f;

		//set translation direction row
		this->_m12 = TransX;
		this->_m13 = TransY;
		this->_m14 = TransZ;
		this->_m15 = 1.0f;

		//Alternative way

		/*Matrix m1(Rot1::X, RotX);
		Matrix m2(Rot1::Y, RotY);
		Matrix m3(Rot1::Z, RotZ);

		*this = m1 * m2 * m3;*/
	}

	Matrix::Matrix(const Trans, const Vect& TransV) {
	
		this->SetTrans(TransV);
	}

	Matrix::Matrix(const Scale, const float& ScaleX, const float& ScaleY, const float& ScaleZ) {
	

		//set xAxis
		this->_m0 = ScaleX;
		this->_m1 = 0.0f;
		this->_m2 = 0.0f;
		this->_m3 = 0.0f;

		//set yAxis
		this->_m4 = 0.0f;
		this->_m5 = ScaleY;
		this->_m6 = 0.0f;
		this->_m7 = 0.0f;

		//set zAxis
		this->_m8 = 0.0f;
		this->_m9 = 0.0f;
		this->_m10 = ScaleZ;
		this->_m11 = 0.0f;

		//set translation direction row
		this->_m12 = 0.0f;
		this->_m13 = 0.0f;
		this->_m14 = 0.0f;
		this->_m15 = 1.0f;
	
	}

	Matrix::Matrix(const Scale, const Vect& ScaleV) {
		this->SetScale(ScaleV);
	}

	Matrix::Matrix(const Special InEnum) {
		
		this->SetSpecial(InEnum);
	}

	//---Math Functions---
	float Matrix::det() const {

		//TODO try to rearrange to reduce temporaries

		//float tmp0 = (this->_m10 * this->_m15) - (this->_m14 * this->_m11);
		float tmp0 = this->_m10;
		tmp0 *= this->_m15;
		float tmp6 = this->_m14;
		tmp6 *= this->_m11;
		tmp0 -= tmp6;

		//float tmp1 = (this->_m9 * this->_m15) - (this->_m13 * this->_m11);
		float tmp1 = this->_m9;
		tmp1 *= this->_m15;
		tmp6 = this->_m13;
		tmp6 *= this->_m11;
		tmp1 -= tmp6;

		//float tmp2 = (this->_m9 * this->_m14) - (this->_m13 * this->_m10);
		float tmp2 = this->_m9;
		tmp2 *= this->_m14;
		tmp6 = this->_m13;
		tmp6 *= this->_m10;
		tmp2 -= tmp6;

		//float tmp3 = (this->_m8 * this->_m15) - (this->_m12 * this->_m11);
		float tmp3 = this->_m8;
		tmp3 *= this->_m15;
		tmp6 = this->_m12;
		tmp6 *= this->_m11;
		tmp3 -= tmp6;

		//float tmp4 = (this->_m8 * this->_m14) - (this->_m12 * this->_m10);
		float tmp4 = this->_m8;
		tmp4 *= this->_m14;
		tmp6 = this->_m12;
		tmp6 *= this->_m10;
		tmp4 -= tmp6;

		//float tmp5 = (this->_m8 * this->_m13) - (this->_m12 * this->_m9);
		float tmp5 = this->_m8;
		tmp5 *= this->_m13;
		tmp6 = this->_m12;
		tmp6 *= this->_m9;
		tmp5 -= tmp6;

		//this->_m0* ((this->_m5 * tmp0) - (this->_m6 * tmp1) + (this->_m7 * tmp2))
		tmp6 = tmp0;
		tmp6 *= this->_m5;

		float tmp7 = tmp1;
		tmp7 *= this->_m6;
		tmp6 -= tmp7;

		tmp7 = tmp2;
		tmp7 *= this->_m7;
		tmp6 += tmp7;
		tmp6 *= this->_m0;

		//this->_m1* ((this->_m4 * tmp0) - (this->_m6 * tmp3) + (this->_m7 * tmp4))
		float tmp8 = tmp0;
		tmp8 *= this->_m4;
		
		tmp7 = tmp3;
		tmp7 *= this->_m6;
		tmp8 -= tmp7;

		tmp7 = tmp4;
		tmp7 *= this->_m7;
		tmp8 += tmp7;
		tmp8 *= this->_m1;

		//this->_m2* ((this->_m4 * tmp1) - (this->_m5 * tmp3) + (this->_m7 * tmp5))
		float tmp9 = tmp1;
		tmp9 *= this->_m4;
		
		tmp7 = tmp3;
		tmp7 *= this->_m5;
		tmp9 -= tmp7;

		tmp7 = tmp5;
		tmp7 *= this->_m7;
		tmp9 += tmp7;
		tmp9 *= this->_m2;

		//this->_m3* ((this->_m4 * tmp2) - (this->_m5 * tmp4) + (this->_m6 * tmp5))
		float temp5 = tmp2;
		temp5 *= this->_m4;
		
		tmp7 = tmp4;
		tmp7 *= this->_m5;
		temp5 -= tmp7;

		tmp7 = tmp5;
		tmp7 *= this->_m6;
		temp5 += tmp7;
		temp5 *= this->_m3;

		tmp6 -= tmp8;
		tmp6 += tmp9;
		tmp6 -= temp5;

		return tmp6;


		//-----Alternate less-optimized way-----
		//MANY copy constructors are implicitly called...

		/*float tmp0 = (this->_m10 * this->_m15) - (this->_m14 * this->_m11);
		float tmp1 = (this->_m9  * this->_m15) - (this->_m13 * this->_m11);
		float tmp2 = (this->_m9  * this->_m14) - (this->_m13 * this->_m10);
		float tmp3 = (this->_m8  * this->_m15) - (this->_m12 * this->_m11);
		float tmp4 = (this->_m8  * this->_m14) - (this->_m12 * this->_m10);
		float tmp5 = (this->_m8  * this->_m13) - (this->_m12 * this->_m9);
		
		return	  this->_m0 * ((this->_m5 * tmp0) - (this->_m6 * tmp1) + (this->_m7 * tmp2))
				- this->_m1 * ((this->_m4 * tmp0) - (this->_m6 * tmp3) + (this->_m7 * tmp4))
				+ this->_m2 * ((this->_m4 * tmp1) - (this->_m5 * tmp3) + (this->_m7 * tmp5))
				- this->_m3 * ((this->_m4 * tmp2) - (this->_m5 * tmp4) + (this->_m6 * tmp5));*/
	}

	Matrix& Matrix::T() {
		//			---Transpose---
		//x-Axis becomes x values of other rows
		//y-Axis becomes y values of other rows
		//z-Axis becomes z values of other rows
		//translation row becomes w values of other rows

		//swap y value of x-axis and x value of y-axis
		float tmp = this->_m1;
		this->_m1 = this->_m4;;
		this->_m4 = tmp;
		
		//swap z value of x-axis and x value of z-axis
		tmp = this->_m2;
		this->_m2 = this->_m8;
		this->_m8 = tmp;

		//swap w value of x-axis and x value of translation row
		tmp = this->_m3;
		this->_m3 = this->_m12;
		this->_m12 = tmp;

		//swap z value of y-axis and y value of z-axis
		tmp = this->_m6;
		this->_m6 = this->_m9;
		this->_m9 = tmp;

		//swap w value of y-axis and y value of translation row
		tmp = this->_m7;
		this->_m7 = this->_m13;
		this->_m13 = tmp;

		//swap w value of z-axis and z value of translation row
		tmp = this->_m11;
		this->_m11 = this->_m14;
		this->_m14 = tmp;

		return *this;
	}

	Matrix Matrix::getT() const {
		//			---Transpose---
		//x-Axis becomes x values of other rows
		//y-Axis becomes y values of other rows
		//z-Axis becomes z values of other rows
		//translation row becomes w values of other rows

		//TODO refactor for RVO?

		//set transpose matrix
		Matrix mOut;

		//set xAxis
		mOut._m0 = this->_m0;
		mOut._m1 = this->_m4;
		mOut._m2 = this->_m8;
		mOut._m3 = this->_m12;

		//set yAxis
		mOut._m4 = this->_m1;
		mOut._m5 = this->_m5;
		mOut._m6 = this->_m9;
		mOut._m7 = this->_m13;

		//set zAxis
		mOut._m8  = this->_m2;
		mOut._m9  = this->_m6;
		mOut._m10 = this->_m10;
		mOut._m11 = this->_m14;

		//set translation direction row
		mOut._m12 = this->_m3;
		mOut._m13 = this->_m7;
		mOut._m14 = this->_m11;
		mOut._m15 = this->_m15;
		
		return mOut;
	}

	Matrix& Matrix::inv() {
		
		float tmp = 1.0f;
		tmp /= this->det();

		//Get Adjugate

		Matrix M = this->getAdjugate();
		M *= tmp;
		*this = M;

		return *this;
	}

	Matrix Matrix::getInv() const {
		
		float tmp = 1.0f;
		tmp /= this->det();

		//Get Adjugate
		Matrix M = this->getAdjugate();
		M *= tmp;
		
		return M;
	}

	Matrix Matrix::getAdjugate() const {
	
		Matrix M;

		//set xAxis
		M._m0 = (this->_m5 * this->_m10 * this->_m15) + (this->_m6 * this->_m11 * this->_m13) + (this->_m7 * this->_m9 * this->_m14)
			- (this->_m7 * this->_m10 * this->_m13) - (this->_m6 * this->_m9 * this->_m15) - (this->_m5 * this->_m11 * this->_m14);

		M._m1 = -(this->_m1 * this->_m10 * this->_m15) - (this->_m2 * this->_m11 * this->_m13) - (this->_m3 * this->_m9 * this->_m14)
			+ (this->_m3 * this->_m10 * this->_m13) + (this->_m2 * this->_m9 * this->_m15) + (this->_m1 * this->_m11 * this->_m14);

		M._m2 = (this->_m1 * this->_m6 * this->_m15) + (this->_m2 * this->_m7 * this->_m13) + (this->_m3 * this->_m5 * this->_m14)
			- (this->_m3 * this->_m6 * this->_m13) - (this->_m2 * this->_m5 * this->_m15) - (this->_m1 * this->_m7 * this->_m14);

		M._m3 = -(this->_m1 * this->_m6 * this->_m11) - (this->_m2 * this->_m7 * this->_m9) - (this->_m3 * this->_m5 * this->_m10)
			+ (this->_m3 * this->_m6 * this->_m9) + (this->_m2 * this->_m5 * this->_m11) + (this->_m1 * this->_m7 * this->_m10);

		//set yAxis
		M._m4 = -(this->_m4 * this->_m10 * this->_m15) - (this->_m6 * this->_m11 * this->_m12) - (this->_m7 * this->_m8 * this->_m14)
			+ (this->_m7 * this->_m10 * this->_m12) + (this->_m6 * this->_m8 * this->_m15) + (this->_m4 * this->_m11 * this->_m14);

		M._m5 = (this->_m0 * this->_m10 * this->_m15) + (this->_m2 * this->_m11 * this->_m12) + (this->_m3 * this->_m8 * this->_m14)
			- (this->_m3 * this->_m10 * this->_m12) - (this->_m2 * this->_m8 * this->_m15) - (this->_m0 * this->_m11 * this->_m14);

		M._m6 = -(this->_m0 * this->_m6 * this->_m15) - (this->_m2 * this->_m7 * this->_m12) - (this->_m3 * this->_m4 * this->_m14)
			+ (this->_m3 * this->_m6 * this->_m12) + (this->_m2 * this->_m4 * this->_m15) + (this->_m0 * this->_m7 * this->_m14);

		M._m7 = (this->_m0 * this->_m6 * this->_m11) + (this->_m2 * this->_m7 * this->_m8) + (this->_m3 * this->_m4 * this->_m10)
			- (this->_m3 * this->_m6 * this->_m8) - (this->_m2 * this->_m4 * this->_m11) - (this->_m0 * this->_m7 * this->_m10);

		//set zAxis
		M._m8 = (this->_m4 * this->_m9 * this->_m15) + (this->_m5 * this->_m11 * this->_m12) + (this->_m7 * this->_m8 * this->_m13)
			- (this->_m7 * this->_m9 * this->_m12) - (this->_m5 * this->_m8 * this->_m15) - (this->_m4 * this->_m11 * this->_m13);

		M._m9 = -(this->_m0 * this->_m9 * this->_m15) - (this->_m1 * this->_m11 * this->_m12) - (this->_m3 * this->_m8 * this->_m13)
			+ (this->_m3 * this->_m9 * this->_m12) + (this->_m1 * this->_m8 * this->_m15) + (this->_m0 * this->_m11 * this->_m13);

		M._m10 = (this->_m0 * this->_m5 * this->_m15) + (this->_m1 * this->_m7 * this->_m12) + (this->_m3 * this->_m4 * this->_m13)
			- (this->_m3 * this->_m5 * this->_m12) - (this->_m1 * this->_m4 * this->_m15) - (this->_m0 * this->_m7 * this->_m13);

		M._m11 = -(this->_m0 * this->_m5 * this->_m11) - (this->_m1 * this->_m7 * this->_m8) - (this->_m3 * this->_m4 * this->_m9)
			+ (this->_m3 * this->_m5 * this->_m8) + (this->_m1 * this->_m4 * this->_m11) + (this->_m0 * this->_m7 * this->_m9);

		//set translation direction row
		M._m12 = -(this->_m4 * this->_m9 * this->_m14) - (this->_m5 * this->_m10 * this->_m12) - (this->_m6 * this->_m8 * this->_m13)
			+ (this->_m6 * this->_m9 * this->_m12) + (this->_m5 * this->_m8 * this->_m14) + (this->_m4 * this->_m10 * this->_m13);

		M._m13 = (this->_m0 * this->_m9 * this->_m14) + (this->_m1 * this->_m10 * this->_m12) + (this->_m2 * this->_m8 * this->_m13)
			- (this->_m2 * this->_m9 * this->_m12) - (this->_m1 * this->_m8 * this->_m14) - (this->_m0 * this->_m10 * this->_m13);

		M._m14 = -(this->_m0 * this->_m5 * this->_m14) - (this->_m1 * this->_m6 * this->_m12) - (this->_m2 * this->_m4 * this->_m13)
			+ (this->_m2 * this->_m5 * this->_m12) + (this->_m1 * this->_m4 * this->_m14) + (this->_m0 * this->_m6 * this->_m13);

		M._m15 = (this->_m0 * this->_m5 * this->_m10) + (this->_m1 * this->_m6 * this->_m8) + (this->_m2 * this->_m4 * this->_m9)
			- (this->_m2 * this->_m5 * this->_m8) - (this->_m1 * this->_m4 * this->_m10) - (this->_m0 * this->_m6 * this->_m9);
	
		
		return M;
	}

	//---Checks---
	const bool Matrix::isIdentity(const float& ep) const {
	
		return  Util::isEqual(this->_m0,  1.0f, ep) &&
				Util::isEqual(this->_m1,  0.0f, ep) &&
				Util::isEqual(this->_m2,  0.0f, ep) &&
				Util::isEqual(this->_m3,  0.0f, ep) &&
				Util::isEqual(this->_m4,  0.0f, ep) &&
				Util::isEqual(this->_m5,  1.0f, ep) &&
				Util::isEqual(this->_m6,  0.0f, ep) &&
				Util::isEqual(this->_m7,  0.0f, ep) &&
				Util::isEqual(this->_m8,  0.0f, ep) &&
				Util::isEqual(this->_m9,  0.0f, ep) &&
				Util::isEqual(this->_m10, 1.0f, ep) &&
				Util::isEqual(this->_m11, 0.0f, ep) &&
				Util::isEqual(this->_m12, 0.0f, ep) &&
				Util::isEqual(this->_m13, 0.0f, ep) &&
				Util::isEqual(this->_m14, 0.0f, ep) &&
				Util::isEqual(this->_m15, 1.0f, ep);
	}

	const bool Matrix::isIdentity() const {

		return  Util::isEqual(this->_m0,  1.0f, MATH_TOLERANCE) &&
				Util::isEqual(this->_m1,  0.0f, MATH_TOLERANCE) &&
				Util::isEqual(this->_m2,  0.0f, MATH_TOLERANCE) &&
				Util::isEqual(this->_m3,  0.0f, MATH_TOLERANCE) &&
				Util::isEqual(this->_m4,  0.0f, MATH_TOLERANCE) &&
				Util::isEqual(this->_m5,  1.0f, MATH_TOLERANCE) &&
				Util::isEqual(this->_m6,  0.0f, MATH_TOLERANCE) &&
				Util::isEqual(this->_m7,  0.0f, MATH_TOLERANCE) &&
				Util::isEqual(this->_m8,  0.0f, MATH_TOLERANCE) &&
				Util::isEqual(this->_m9,  0.0f, MATH_TOLERANCE) &&
				Util::isEqual(this->_m10, 1.0f, MATH_TOLERANCE) &&
				Util::isEqual(this->_m11, 0.0f, MATH_TOLERANCE) &&
				Util::isEqual(this->_m12, 0.0f, MATH_TOLERANCE) &&
				Util::isEqual(this->_m13, 0.0f, MATH_TOLERANCE) &&
				Util::isEqual(this->_m14, 0.0f, MATH_TOLERANCE) &&
				Util::isEqual(this->_m15, 1.0f, MATH_TOLERANCE);
	}

	const bool Matrix::isEqual(const Matrix& t) const {

		return  Util::isEqual(this->_m0,  t._m0,  MATH_TOLERANCE) &&
				Util::isEqual(this->_m1,  t._m1,  MATH_TOLERANCE) &&
				Util::isEqual(this->_m2,  t._m2,  MATH_TOLERANCE) &&
				Util::isEqual(this->_m3,  t._m3,  MATH_TOLERANCE) &&
				Util::isEqual(this->_m4,  t._m4,  MATH_TOLERANCE) &&
				Util::isEqual(this->_m5,  t._m5,  MATH_TOLERANCE) &&
				Util::isEqual(this->_m6,  t._m6,  MATH_TOLERANCE) &&
				Util::isEqual(this->_m7,  t._m7,  MATH_TOLERANCE) &&
				Util::isEqual(this->_m8,  t._m8,  MATH_TOLERANCE) &&
				Util::isEqual(this->_m9,  t._m9,  MATH_TOLERANCE) &&
				Util::isEqual(this->_m10, t._m10, MATH_TOLERANCE) &&
				Util::isEqual(this->_m11, t._m11, MATH_TOLERANCE) &&
				Util::isEqual(this->_m12, t._m12, MATH_TOLERANCE) &&
				Util::isEqual(this->_m13, t._m13, MATH_TOLERANCE) &&
				Util::isEqual(this->_m14, t._m14, MATH_TOLERANCE) &&
				Util::isEqual(this->_m15, t._m15, MATH_TOLERANCE);
	}

	//---Setters---
	Matrix& Matrix::set(const Special InEnum) {
	
		this->SetSpecial(InEnum);
		return *this;
	}

	Matrix& Matrix::set(const Trans, const float& TransX, const float& TransY, const float& TransZ) {
		
		//set xAxis
		this->_m0 = 1.0f;
		this->_m1 = 0.0f;
		this->_m2 = 0.0f;
		this->_m3 = 0.0f;

		//set yAxis
		this->_m4 = 0.0f;
		this->_m5 = 1.0f;
		this->_m6 = 0.0f;
		this->_m7 = 0.0f;

		//set zAxis
		this->_m8 = 0.0f;
		this->_m9 = 0.0f;
		this->_m10 = 1.0f;
		this->_m11 = 0.0f;

		//set translation direction row
		this->_m12 = TransX;
		this->_m13 = TransY;
		this->_m14 = TransZ;
		this->_m15 = 1.0f;
	
		return *this;
	}

	Matrix& Matrix::set(const Trans, const Vect& TransV) {

		this->SetTrans(TransV);
		return *this;
	}

	Matrix& Matrix::set(const Scale, const float& ScaleX, const float& ScaleY, const float& ScaleZ) {

		//set xAxis
		this->_m0 = ScaleX;
		this->_m1 = 0.0f;
		this->_m2 = 0.0f;
		this->_m3 = 0.0f;

		//set yAxis
		this->_m4 = 0.0f;
		this->_m5 = ScaleY;
		this->_m6 = 0.0f;
		this->_m7 = 0.0f;

		//set zAxis
		this->_m8 = 0.0f;
		this->_m9 = 0.0f;
		this->_m10 = ScaleZ;
		this->_m11 = 0.0f;

		//set translation direction row
		this->_m12 = 0.0f;
		this->_m13 = 0.0f;
		this->_m14 = 0.0f;
		this->_m15 = 1.0f;

		return *this;
	}

	Matrix& Matrix::set(const Scale, const Vect& ScaleV) {

		this->SetScale(ScaleV);
		return *this;
	}

	Matrix& Matrix::set(const Rot1 InEnum, const float& RotAngle) {

		this->SetAxisRotation(InEnum, RotAngle);
		return *this;
	}

	Matrix& Matrix::set(const Row InEnum, const Vect& RowV) {
		
		switch (InEnum) {
		case Row::i0:
			this->_v0 = RowV;
			break;
		case Row::i1:
			this->_v1 = RowV;
			break;
		case Row::i2:
			this->_v2 = RowV;
			break;
		case Row::i3:
			this->_v3 = RowV;
			break;
		}

		return *this;
	}

	Matrix& Matrix::set(const Rot3, const float& RotX, const float& RotY, const float& RotZ) {

		this->SetRotation(RotX, RotY, RotZ);
		return *this;
	}

	Matrix& Matrix::set(const Rot, Vect& RotAxis, const float& RotAngle) {
	
		this->SetRotAxisAngle(RotAxis, RotAngle);
		return *this;
	}

	Matrix& Matrix::set(const Orient InEnum, const Vect& zAxisIn, const Vect& yAxisIn) {
	
		this->SetOrientation(InEnum, zAxisIn, yAxisIn);
		return *this;
	}

	Matrix& Matrix::set(const Vect& Row0, const Vect& Row1, const Vect& Row2, const Vect& Row3) {
		this->_v0 = Row0;
		this->_v1 = Row1;
		this->_v2 = Row2;
		this->_v3 = Row3;
		return *this;
	}

	//---Getters---
	Vect Matrix::get(const Row InEnum) const {

		Vect RowV;

		switch (InEnum) {
		case Row::i0:
			RowV = this->_v0;
			break;
		case Row::i1:
			RowV = this->_v1;
			break;
		case Row::i2:
			RowV = this->_v2;
			break;
		case Row::i3:
			RowV = this->_v3;
			break;
		}

		return RowV;
	}

	//---Overloaded Operators---

	Matrix Matrix::operator * (const Matrix& t) const
	{
		Matrix A;

		A._m0 = this->_m0 * t._m0 + this->_m1 * t._m4 + this->_m2 * t._m8 + this->_m3 * t._m12;
		A._m1 = this->_m0 * t._m1 + this->_m1 * t._m5 + this->_m2 * t._m9 + this->_m3 * t._m13;
		A._m2 = this->_m0 * t._m2 + this->_m1 * t._m6 + this->_m2 * t._m10 + this->_m3 * t._m14;
		A._m3 = this->_m0 * t._m3 + this->_m1 * t._m7 + this->_m2 * t._m11 + this->_m3 * t._m15;
		  					  					  					  
		A._m4 = this->_m4 * t._m0 + this->_m5 * t._m4 + this->_m6 * t._m8 + this->_m7 * t._m12;
		A._m5 = this->_m4 * t._m1 + this->_m5 * t._m5 + this->_m6 * t._m9 + this->_m7 * t._m13;
		A._m6 = this->_m4 * t._m2 + this->_m5 * t._m6 + this->_m6 * t._m10 + this->_m7 * t._m14;
		A._m7 = this->_m4 * t._m3 + this->_m5 * t._m7 + this->_m6 * t._m11 + this->_m7 * t._m15;
		  		
		A._m8 = this->_m8 * t._m0 + this->_m9 * t._m4 + this->_m10 * t._m8 + this->_m11 * t._m12;
		A._m9 = this->_m8 * t._m1 + this->_m9 * t._m5 + this->_m10 * t._m9 + this->_m11 * t._m13;
		A._m10 = this->_m8 * t._m2 + this->_m9 * t._m6 + this->_m10 * t._m10 + this->_m11 * t._m14;
		A._m11 = this->_m8 * t._m3 + this->_m9 * t._m7 + this->_m10 * t._m11 + this->_m11 * t._m15;
		  
		A._m12 = this->_m12 * t._m0 + this->_m13 * t._m4 + this->_m14 * t._m8 + this->_m15 * t._m12;
		A._m13 = this->_m12 * t._m1 + this->_m13 * t._m5 + this->_m14 * t._m9 + this->_m15 * t._m13;
		A._m14 = this->_m12 * t._m2 + this->_m13 * t._m6 + this->_m14 * t._m10 + this->_m15 * t._m14;
		A._m15 = this->_m12 * t._m3 + this->_m13 * t._m7 + this->_m14 * t._m11 + this->_m15 * t._m15;

		return A;
	}

	Matrix Matrix::operator * (const float& s) const {

		Matrix A = *this;

		//set xAxis
		A._m0 *= s;
		A._m1 *= s;
		A._m2 *= s;
		A._m3 *= s;

		//set yAxis
		A._m4 *= s;
		A._m5 *= s;
		A._m6 *= s;
		A._m7 *= s;

		//set zAxis
		A._m8 *= s;
		A._m9 *= s;
		A._m10 *= s;
		A._m11 *= s;

		//set translation direction row
		A._m12 *= s;
		A._m13 *= s;
		A._m14 *= s;
		A._m15 *= s;

		return A;

	}

	Matrix Matrix::operator + (const Matrix& t) const
	{
		Matrix A;

		A._m0 = this->_m0 + t._m0;
		A._m1 = this->_m1 + t._m1;
		A._m2 = this->_m2 + t._m2;
		A._m3 = this->_m3 + t._m3;
						  
		A._m4 = this->_m4 + t._m4;
		A._m5 = this->_m5 + t._m5;
		A._m6 = this->_m6 + t._m6;
		A._m7 = this->_m7 + t._m7;
						  
		A._m8 = this->_m8 + t._m8;
		A._m9 = this->_m9 + t._m9;
		A._m10 = this->_m10 + t._m10;
		A._m11 = this->_m11 + t._m11;

		A._m12 = this->_m12 + t._m12;
		A._m13 = this->_m13 + t._m13;
		A._m14 = this->_m14 + t._m14;
		A._m15 = this->_m15 + t._m15;

		return A;
	}

	Matrix Matrix::operator - (const Matrix& t) const
	{
		Matrix A;

		A._m0 = this->_m0 - t._m0;
		A._m1 = this->_m1 - t._m1;
		A._m2 = this->_m2 - t._m2;
		A._m3 = this->_m3 - t._m3;
						  
		A._m4 = this->_m4 - t._m4;
		A._m5 = this->_m5 - t._m5;
		A._m6 = this->_m6 - t._m6;
		A._m7 = this->_m7 - t._m7;
						  
		A._m8 = this->_m8 - t._m8;
		A._m9 = this->_m9 - t._m9;
		A._m10 = this->_m10 - t._m10;
		A._m11 = this->_m11 - t._m11;
							
		A._m12 = this->_m12 - t._m12;
		A._m13 = this->_m13 - t._m13;
		A._m14 = this->_m14 - t._m14;
		A._m15 = this->_m15 - t._m15;

		return A;
	}

	Matrix& Matrix::operator += (const Matrix& t)
	{
		
		this->_m0  += t._m0;
		this->_m1  += t._m1;
		this->_m2  += t._m2;
		this->_m3  += t._m3;

		this->_m4  += t._m4;
		this->_m5  += t._m5;
		this->_m6  += t._m6;
		this->_m7  += t._m7;

		this->_m8  += t._m8;
		this->_m9  += t._m9;
		this->_m10 += t._m10;
		this->_m11 += t._m11;

		this->_m12 += t._m12;
		this->_m13 += t._m13;
		this->_m14 += t._m14;
		this->_m15 += t._m15;

		return *this;
	}

	Matrix& Matrix::operator -= (const Matrix& t)
	{

		this->_m0  -= t._m0;
		this->_m1  -= t._m1;
		this->_m2  -= t._m2;
		this->_m3  -= t._m3;
				   
		this->_m4  -= t._m4;
		this->_m5  -= t._m5;
		this->_m6  -= t._m6;
		this->_m7  -= t._m7;
				   
		this->_m8  -= t._m8;
		this->_m9  -= t._m9;
		this->_m10 -= t._m10;
		this->_m11 -= t._m11;
				   
		this->_m12 -= t._m12;
		this->_m13 -= t._m13;
		this->_m14 -= t._m14;
		this->_m15 -= t._m15;

		return *this;
	}

	Matrix& Matrix::operator *= (const Matrix& t) {

		Matrix A;

		A._m0 = this->_m0 * t._m0 + this->_m1 * t._m4 + this->_m2 * t._m8 + this->_m3 * t._m12;
		A._m1 = this->_m0 * t._m1 + this->_m1 * t._m5 + this->_m2 * t._m9 + this->_m3 * t._m13;
		A._m2 = this->_m0 * t._m2 + this->_m1 * t._m6 + this->_m2 * t._m10 + this->_m3 * t._m14;
		A._m3 = this->_m0 * t._m3 + this->_m1 * t._m7 + this->_m2 * t._m11 + this->_m3 * t._m15;

		A._m4 = this->_m4 * t._m0 + this->_m5 * t._m4 + this->_m6 * t._m8 + this->_m7 * t._m12;
		A._m5 = this->_m4 * t._m1 + this->_m5 * t._m5 + this->_m6 * t._m9 + this->_m7 * t._m13;
		A._m6 = this->_m4 * t._m2 + this->_m5 * t._m6 + this->_m6 * t._m10 + this->_m7 * t._m14;
		A._m7 = this->_m4 * t._m3 + this->_m5 * t._m7 + this->_m6 * t._m11 + this->_m7 * t._m15;

		A._m8 = this->_m8 * t._m0 + this->_m9 * t._m4 + this->_m10 * t._m8 + this->_m11 * t._m12;
		A._m9 = this->_m8 * t._m1 + this->_m9 * t._m5 + this->_m10 * t._m9 + this->_m11 * t._m13;
		A._m10 = this->_m8 * t._m2 + this->_m9 * t._m6 + this->_m10 * t._m10 + this->_m11 * t._m14;
		A._m11 = this->_m8 * t._m3 + this->_m9 * t._m7 + this->_m10 * t._m11 + this->_m11 * t._m15;

		A._m12 = this->_m12 * t._m0 + this->_m13 * t._m4 + this->_m14 * t._m8 + this->_m15 * t._m12;
		A._m13 = this->_m12 * t._m1 + this->_m13 * t._m5 + this->_m14 * t._m9 + this->_m15 * t._m13;
		A._m14 = this->_m12 * t._m2 + this->_m13 * t._m6 + this->_m14 * t._m10 + this->_m15 * t._m14;
		A._m15 = this->_m12 * t._m3 + this->_m13 * t._m7 + this->_m14 * t._m11 + this->_m15 * t._m15;

		*this = A;
		return *this;
	}

	Matrix& Matrix::operator *= (const float& s) {
	
		//set xAxis
		this->_m0 *= s;
		this->_m1 *= s;
		this->_m2 *= s;
		this->_m3 *= s;

		//set yAxis
		this->_m4 *= s;
		this->_m5 *= s;
		this->_m6 *= s;
		this->_m7 *= s;

		//set zAxis
		this->_m8  *= s;
		this->_m9  *= s;
		this->_m10 *= s;
		this->_m11 *= s;

		//set translation direction row
		this->_m12 *= s;
		this->_m13 *= s;
		this->_m14 *= s;
		this->_m15 *= s;
	
		return *this;
	}

	Matrix Matrix::operator + () const {

		Matrix A = *this;
		return A;
	}

	Matrix Matrix::operator - () const {

		Matrix A = *this;
		A *= -1.0f;
		return A;
	}

	Matrix operator * (const float& s, const Matrix& t) {

		Matrix M = t;

		//set xAxis
		M.m0() *= s;
		M.m1() *= s;
		M.m2() *= s;
		M.m3() *= s;

		//set yAxis
		M.m4() *= s;
		M.m5() *= s;
		M.m6() *= s;
		M.m7() *= s;

		//set zAxis
		M.m8()  *= s;
		M.m9()  *= s;
		M.m10() *= s;
		M.m11() *= s;

		//set translation direction row
		M.m12() *= s;
		M.m13() *= s;
		M.m14() *= s;
		M.m15() *= s;

		return M;
	}

	float& Matrix::operator [] (const m0_enum  ) { return this->_m0;  }
	float& Matrix::operator [] (const m1_enum  ) { return this->_m1;  }
	float& Matrix::operator [] (const m2_enum  ) { return this->_m2;  }
	float& Matrix::operator [] (const m3_enum  ) { return this->_m3;  }
	float& Matrix::operator [] (const m4_enum  ) { return this->_m4;  }
	float& Matrix::operator [] (const m5_enum  ) { return this->_m5;  }
	float& Matrix::operator [] (const m6_enum  ) { return this->_m6;  }
	float& Matrix::operator [] (const m7_enum  ) { return this->_m7;  }
	float& Matrix::operator [] (const m8_enum  ) { return this->_m8;  }
	float& Matrix::operator [] (const m9_enum  ) { return this->_m9;  }
	float& Matrix::operator [] (const m10_enum ) { return this->_m10; }
	float& Matrix::operator [] (const m11_enum ) { return this->_m11; }
	float& Matrix::operator [] (const m12_enum ) { return this->_m12; }
	float& Matrix::operator [] (const m13_enum ) { return this->_m13; }
	float& Matrix::operator [] (const m14_enum ) { return this->_m14; }
	float& Matrix::operator [] (const m15_enum ) { return this->_m15; }

	const float Matrix::operator [] (const m0_enum ) const { return this->_m0;  }
	const float Matrix::operator [] (const m1_enum ) const { return this->_m1;  }
	const float Matrix::operator [] (const m2_enum ) const { return this->_m2;  }
	const float Matrix::operator [] (const m3_enum ) const { return this->_m3;  }
	const float Matrix::operator [] (const m4_enum ) const { return this->_m4;  }
	const float Matrix::operator [] (const m5_enum ) const { return this->_m5;  }
	const float Matrix::operator [] (const m6_enum ) const { return this->_m6;  }
	const float Matrix::operator [] (const m7_enum ) const { return this->_m7;  }
	const float Matrix::operator [] (const m8_enum ) const { return this->_m8;  }
	const float Matrix::operator [] (const m9_enum ) const { return this->_m9;  }
	const float Matrix::operator [] (const m10_enum) const { return this->_m10; }
	const float Matrix::operator [] (const m11_enum) const { return this->_m11; }
	const float Matrix::operator [] (const m12_enum) const { return this->_m12; }
	const float Matrix::operator [] (const m13_enum) const { return this->_m13; }
	const float Matrix::operator [] (const m14_enum) const { return this->_m14; }
	const float Matrix::operator [] (const m15_enum) const { return this->_m15; }
	
	//-----------------------------------
	//-------------data accessors--------
	//-----------------------------------

	float& Matrix::m0()  { return this->_m0; }
	float& Matrix::m1()  { return this->_m1; }
	float& Matrix::m2()  { return this->_m2; }
	float& Matrix::m3()  { return this->_m3; }
	float& Matrix::m4()  { return this->_m4; }
	float& Matrix::m5()  { return this->_m5; }
	float& Matrix::m6()  { return this->_m6; }
	float& Matrix::m7()  { return this->_m7; }
	float& Matrix::m8()  { return this->_m8; }
	float& Matrix::m9()  { return this->_m9; }
	float& Matrix::m10() { return this->_m10;}
	float& Matrix::m11() { return this->_m11;}
	float& Matrix::m12() { return this->_m12;}
	float& Matrix::m13() { return this->_m13;}
	float& Matrix::m14() { return this->_m14;}
	float& Matrix::m15() { return this->_m15;}

	float Matrix::m0()  const { return this->_m0; }
	float Matrix::m1()  const { return this->_m1; }
	float Matrix::m2()  const { return this->_m2; }
	float Matrix::m3()  const { return this->_m3; }
	float Matrix::m4()  const { return this->_m4; }
	float Matrix::m5()  const { return this->_m5; }
	float Matrix::m6()  const { return this->_m6; }
	float Matrix::m7()  const { return this->_m7; }
	float Matrix::m8()  const { return this->_m8; }
	float Matrix::m9()  const { return this->_m9; }
	float Matrix::m10() const { return this->_m10;}
	float Matrix::m11() const { return this->_m11;}
	float Matrix::m12() const { return this->_m12;}
	float Matrix::m13() const { return this->_m13;}
	float Matrix::m14() const { return this->_m14;}
	float Matrix::m15() const { return this->_m15;}

	//---Private Helpers... for duplicate code in specialized constructors and setters---
	void Matrix::SetRotAxisAngle(const Vect& RotAxis, const float& RotAngle) {
	
		// angle
			// axis;
		const float angle_a = 0.5f * RotAngle;
		
		const float cos_a = cosf(angle_a);
		const float sin_a = sinf(angle_a);

		Vect qV = RotAxis.getNorm();

		qV *= sin_a;

		Vect Q;
		Q[x] = qV[x];
		Q[y] = qV[y];
		Q[z] = qV[z];
		Q[w] = cos_a;

		// this function has been transposed
	
		// ADD test to make sure that quat is normalized

		const float x2 = Q[x] + Q[x];
		const float y2 = Q[y] + Q[y];
		const float z2 = Q[z] + Q[z];
		
		const float xx = Q[x] * x2;
		const float xy = Q[x] * y2;
		const float xz = Q[x] * z2;
		
		const float yy = Q[y] * y2;
		const float yz = Q[y] * z2;
		const float zz = Q[z] * z2;
		
		const float wx = Q[w] * x2;
		const float wy = Q[w] * y2;
		const float wz = Q[w] * z2;

		//set xAxis
		this->_m0 = 1.0f - (yy + zz);
		this->_m1 = xy + wz;
		this->_m2 = xz - wy;
		this->_m3 = 0.0f;

		//set yAxis
		this->_m4 = xy - wz;
		this->_m5 = 1.0f - (xx + zz);
		this->_m6 = yz + wx;
		this->_m7 = 0.0f;

		//set zAxis
		this->_m8 = xz + wy;
		this->_m9 = yz - wx;
		this->_m10 = 1.0f - (xx + yy);
		this->_m11 = 0.0f;

		//set translation direction row
		this->_m12 = 0.0f;
		this->_m13 = 0.0f;
		this->_m14 = 0.0f;
		this->_m15 = 1.0f;
	}
	void Matrix::SetOrientation(const Orient InEnum, const Vect& zAxisIn, const Vect& yAxisIn) {
	
		//Direction of flight is the z axis

		//Get x axis by getting cross product of y and z axis
		Vect xAxis = yAxisIn.cross(zAxisIn);

		//normalize
		xAxis.norm();
		Vect yAxis = yAxisIn.getNorm();
		Vect zAxis = zAxisIn.getNorm();

		if (InEnum == Orient::LocalToWorld) {
			//set matrix x, y, z axis
			this->_v0 = xAxis;
			this->_v1 = yAxis;
			this->_v2 = zAxis; // aka Direction Of Flight

			//set translation direction row
			this->_m12 = 0.0f;
			this->_m13 = 0.0f;
			this->_m14 = 0.0f;
			this->_m15 = 1.0f;

			//set w value of x, y, z axis to 0.0f
			this->_m3 = 0.0f;
			this->_m7 = 0.0f;
			this->_m11 = 0.0f;

		}
		else {
			//else inverse WorldToLocal
			//set transpose matrix

			//set xAxis
			this->_m0 = xAxis.x();
			this->_m1 = yAxis.x();
			this->_m2 = zAxis.x();
			this->_m3 = 0.0f;

			//set yAxis
			this->_m4 = xAxis.y();
			this->_m5 = yAxis.y();
			this->_m6 = zAxis.y();
			this->_m7 = 0.0f;

			//set zAxis
			this->_m8 = xAxis.z();
			this->_m9 = yAxis.z();
			this->_m10 = zAxis.z();
			this->_m11 = 0.0f;

			//set translation direction row
			this->_m12 = 0.0f;
			this->_m13 = 0.0f;
			this->_m14 = 0.0f;
			this->_m15 = 1.0f;
		}
	}
	void Matrix::SetSpecial(const Special InEnum) {
	
		if (InEnum == Special::Identity) {
			//set xAxis
			this->_m0 = 1.0f;
			this->_m1 = 0.0f;
			this->_m2 = 0.0f;
			this->_m3 = 0.0f;

			//set yAxis
			this->_m4 = 0.0f;
			this->_m5 = 1.0f;
			this->_m6 = 0.0f;
			this->_m7 = 0.0f;

			//set zAxis
			this->_m8 = 0.0f;
			this->_m9 = 0.0f;
			this->_m10 = 1.0f;
			this->_m11 = 0.0f;

			//set translation direction row
			this->_m12 = 0.0f;
			this->_m13 = 0.0f;
			this->_m14 = 0.0f;
			this->_m15 = 1.0f;
		}
		//else Zero
		else {
			//set xAxis
			this->_m0 = 0.0f;
			this->_m1 = 0.0f;
			this->_m2 = 0.0f;
			this->_m3 = 0.0f;

			//set yAxis
			this->_m4 = 0.0f;
			this->_m5 = 0.0f;
			this->_m6 = 0.0f;
			this->_m7 = 0.0f;

			//set zAxis
			this->_m8 = 0.0f;
			this->_m9 = 0.0f;
			this->_m10 = 0.0f;
			this->_m11 = 0.0f;

			//set translation direction row
			this->_m12 = 0.0f;
			this->_m13 = 0.0f;
			this->_m14 = 0.0f;
			this->_m15 = 0.0f;
		}
	}
	void Matrix::SetTrans(const Vect& TransV) {

		//set xAxis
		this->_m0 = 1.0f;
		this->_m1 = 0.0f;
		this->_m2 = 0.0f;
		this->_m3 = 0.0f;

		//set yAxis
		this->_m4 = 0.0f;
		this->_m5 = 1.0f;
		this->_m6 = 0.0f;
		this->_m7 = 0.0f;

		//set zAxis
		this->_m8 = 0.0f;
		this->_m9 = 0.0f;
		this->_m10 = 1.0f;
		this->_m11 = 0.0f;

		//set translation direction row
		this->_v3 = TransV;
	}
	void Matrix::SetScale(const Vect& ScaleV) {

		//set xAxis
		this->_m0 = ScaleV.x();
		this->_m1 = 0.0f;
		this->_m2 = 0.0f;
		this->_m3 = 0.0f;

		//set yAxis
		this->_m4 = 0.0f;
		this->_m5 = ScaleV.y();
		this->_m6 = 0.0f;
		this->_m7 = 0.0f;

		//set zAxis
		this->_m8 = 0.0f;
		this->_m9 = 0.0f;
		this->_m10 = ScaleV.z();
		this->_m11 = 0.0f;

		//set translation direction row
		this->_m12 = 0.0f;
		this->_m13 = 0.0f;
		this->_m14 = 0.0f;
		this->_m15 = 1.0f;
	}
	void Matrix::SetRotation(const float& RotX, const float& RotY, const float& RotZ) {

		Matrix m1(Rot1::X, RotX);
		Matrix m2(Rot1::Y, RotY);
		Matrix m3(Rot1::Z, RotZ);

		*this = m1 * m2 * m3;
	}
	void Matrix::SetAxisRotation(const Rot1 InEnum, const float& RotAngle) {

		float cos = cosf(RotAngle);
		float sin = sinf(RotAngle);

		switch (InEnum) {
		case Rot1::X:
			//set xAxis
			this->_m0 = 1.0f;
			this->_m1 = 0.0f;
			this->_m2 = 0.0f;
			this->_m3 = 0.0f;

			//set yAxis
			this->_m4 = 0.0f;
			this->_m5 = cos;
			this->_m6 = sin;
			this->_m7 = 0.0f;

			//set zAxis
			this->_m8 = 0.0f;
			this->_m9 = -sin;
			this->_m10 = cos;
			this->_m11 = 0.0f;
			break;

		case Rot1::Y:
			//set xAxis
			this->_m0 = cos;
			this->_m1 = 0.0f;
			this->_m2 = -sin;
			this->_m3 = 0.0f;

			//set yAxis
			this->_m4 = 0.0f;
			this->_m5 = 1.0f;
			this->_m6 = 0.0f;
			this->_m7 = 0.0f;

			//set zAxis
			this->_m8 = sin;
			this->_m9 = 0.0f;
			this->_m10 = cos;
			this->_m11 = 0.0f;
			break;

		case Rot1::Z:
			//set xAxis
			this->_m0 = cos;
			this->_m1 = sin;
			this->_m2 = 0.0f;
			this->_m3 = 0.0f;

			//set yAxis
			this->_m4 = -sin;
			this->_m5 = cos;
			this->_m6 = 0.0f;
			this->_m7 = 0.0f;

			//set zAxis
			this->_m8 = 0.0f;
			this->_m9 = 0.0f;
			this->_m10 = 1.0f;
			this->_m11 = 0.0f;
			break;
		}

		//set translation direction row
		this->_m12 = 0.0f;
		this->_m13 = 0.0f;
		this->_m14 = 0.0f;
		this->_m15 = 1.0f;
	}


}

// ---  End of File ---------------
