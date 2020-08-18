

#include "MathEngine.h"

namespace Azul
{
	//---Big Four---
	Vect::Vect() 
		:_vx(0.0f), _vy(0.0f), _vz(0.0f), _vw(1.0f)
	{}

	Vect::Vect(const Vect& t) {
		
		this->_vx = t._vx;
		this->_vy = t._vy;
		this->_vz = t._vz;
		this->_vw = t._vw;
	}

	Vect& Vect::operator = (const Vect& t) {
	
		this->_vx = t._vx;
		this->_vy = t._vy;
		this->_vz = t._vz;
		this->_vw = t._vw;

		return *this;
	}

	//---Specialized Constructors---
    Vect::Vect(const float& tx, const float& ty, const float& tz, const float& tw)
        : _vx(tx), _vy(ty), _vz(tz), _vw(tw)
    {
    }

	//---Overloaded Operators---
	Vect Vect::operator + (const Vect& t) const
	{
		return Vect(this->_vx + t._vx, this->_vy + t._vy, this->_vz + t._vz);
	}

	Vect Vect::operator - (const Vect& t) const
	{
		return Vect(this->_vx - t._vx, this->_vy - t._vy, this->_vz - t._vz);
	}

	Vect Vect::operator * (const float &s) const
	{
		return Vect(this->_vx * s, this->_vy * s, this->_vz * s);
	}

	Vect Vect::operator * (const Vect& t) const
	{
		return Vect(this->_vx * t._vx, this->_vy * t._vy, this->_vz * t._vz);
	}

	Vect operator * (const float& s, const Vect& t) {

		return Vect(s * t.x(), s * t.y(), s * t.z());
	}

	Vect Vect::operator * (const Matrix& m) const
	{
		/*Vect C;

		C._vx = this->_vx * m.m0() + this->_vy * m.m4() + this->_vz * m.m8()  + this->_vw * m.m12();
		C._vy = this->_vx * m.m1() + this->_vy * m.m5() + this->_vz * m.m9()  + this->_vw * m.m13();
		C._vz = this->_vx * m.m2() + this->_vy * m.m6() + this->_vz * m.m10() + this->_vw * m.m14();
		C._vw = this->_vx * m.m3() + this->_vy * m.m7() + this->_vz * m.m11() + this->_vw * m.m15();

		return C;*/

		return Vect(this->_vx * m.m0() + this->_vy * m.m4() + this->_vz * m.m8() + this->_vw * m.m12(),
			this->_vx * m.m1() + this->_vy * m.m5() + this->_vz * m.m9() + this->_vw * m.m13(),
			this->_vx * m.m2() + this->_vy * m.m6() + this->_vz * m.m10() + this->_vw * m.m14(),
			this->_vx * m.m3() + this->_vy * m.m7() + this->_vz * m.m11() + this->_vw * m.m15());
	};

	Vect& Vect::operator *= (const Matrix& m) {
		
		Vect V;

		V._vx = this->_vx * m.m0() + this->_vy * m.m4() + this->_vz * m.m8()  + this->_vw * m.m12();
		V._vy = this->_vx * m.m1() + this->_vy * m.m5() + this->_vz * m.m9()  + this->_vw * m.m13();
		V._vz = this->_vx * m.m2() + this->_vy * m.m6() + this->_vz * m.m10() + this->_vw * m.m14();
		V._vw = this->_vx * m.m3() + this->_vy * m.m7() + this->_vz * m.m11() + this->_vw * m.m15();

		*this = V;
		return *this; 
	}

	Vect& Vect::operator *= (const float& s)
	{
		this->_vx *= s;
		this->_vy *= s;
		this->_vz *= s;
		this->_vw = 1.0f;

		return *this;
	}

	Vect& Vect::operator += (const Vect& v)
	{
		this->_vx += v._vx;
		this->_vy += v._vy;
		this->_vz += v._vz;
		this->_vw = 1.0f;

		return *this;
	}

	Vect& Vect::operator -= (const Vect& v)
	{
		this->_vx -= v._vx;
		this->_vy -= v._vy;
		this->_vz -= v._vz;
		this->_vw = 1.0f;

		return *this;
	}

	Vect Vect::operator + () const {
	
		Vect V;
		V._vx = this->_vx;
		V._vy = this->_vy;
		V._vz = this->_vz;
		V._vw = 1.0f;
		return V;
	}

	Vect Vect::operator - () const {
		
		Vect V = *this;
		V._vx *= -1.0f;
		V._vy *= -1.0f;
		V._vz *= -1.0f;
		V._vw  =  1.0f;

		return V;
	}

	float& Vect::operator [] (const x_enum) { return this->_vx; }
	float& Vect::operator [] (const y_enum) { return this->_vy; }
	float& Vect::operator [] (const z_enum) { return this->_vz; }
	float& Vect::operator [] (const w_enum) { return this->_vw; }

	const float Vect::operator [] (const x_enum) const { return this->_vx; }
	const float Vect::operator [] (const y_enum) const { return this->_vy; }
	const float Vect::operator [] (const z_enum) const { return this->_vz; }
	const float Vect::operator [] (const w_enum) const { return this->_vw; }

	//---Math Functions---
	Vect Vect::cross(const Vect& t) const {

		//Cross product equation
		//(ay*bz - az*by, -(ax*bz - az*bx), ax*by - ay*bx, 1)
		return Vect(this->_vy * t._vz - this->_vz * t._vy,
				  -(this->_vx * t._vz - this->_vz * t._vx),
					this->_vx * t._vy - this->_vy * t._vx);
	}

	const float Vect::dot(const Vect& t) const {
		
		//Dot Product Equation
		//ax*bx + ay*by + az*bz

		return this->_vx * t._vx + this->_vy * t._vy + this->_vz * t._vz;
	}

	Vect& Vect::norm() {

		const float mag = this->mag();

		this->_vx /= mag;
		this->_vy /= mag;
		this->_vz /= mag;
		this->_vw = 1.0f;

		return *this;
	}

	Vect Vect::getNorm() const {

		const float mag = this->mag();
		return Vect(this->_vx / mag, this->_vy / mag, this->_vz / mag);
	}

	const float Vect::mag() const {
	
		return sqrtf(this->dot(*this));
	}

	const float Vect::magSqr() const {

		return this->dot(*this);
	}

	const float Vect::getAngle(const Vect& v) const
	{
		return acosf(this->dot(v) / (this->mag() * v.mag()));
	}

	//---Checks---
	const bool Vect::isEqual(const Vect& v, const float& ep) const {
	
		return	Util::isEqual(this->_vx, v._vx, ep) &&
				Util::isEqual(this->_vy, v._vy, ep) &&
				Util::isEqual(this->_vz, v._vz, ep) &&
				Util::isEqual(this->_vw, v._vw, ep);
	}

	const bool Vect::isEqual(const Vect& v) const {

		return	Util::isEqual(this->_vx, v._vx, MATH_TOLERANCE) &&
				Util::isEqual(this->_vy, v._vy, MATH_TOLERANCE) &&
				Util::isEqual(this->_vz, v._vz, MATH_TOLERANCE) &&
				Util::isEqual(this->_vw, v._vw, MATH_TOLERANCE);
	}

	const bool Vect::isZero(const float& ep) const {

		return	Util::isEqual(this->_vx, 0.0f, ep) &&
				Util::isEqual(this->_vy, 0.0f, ep) &&
				Util::isEqual(this->_vz, 0.0f, ep) &&
				Util::isEqual(this->_vw, 1.0f, ep);
	}

	const bool Vect::isZero() const {

		return	Util::isEqual(this->_vx, 0.0f, MATH_TOLERANCE) &&
				Util::isEqual(this->_vy, 0.0f, MATH_TOLERANCE) &&
				Util::isEqual(this->_vz, 0.0f, MATH_TOLERANCE) &&
				Util::isEqual(this->_vw, 1.0f, MATH_TOLERANCE);
	}

	//---Setters---
	Vect& Vect::set(const float& x, const float& y, const float& z) {
	
		this->_vx = x;
		this->_vy = y;
		this->_vz = z;
		this->_vw = 1.0f;

		return *this;
	}

	Vect& Vect::set(const float& x, const float& y, const float& z, const float& w) {

		this->_vx = x;
		this->_vy = y;
		this->_vz = z;
		this->_vw = w;

		return *this;
	}

	Vect& Vect::set(const Vect& v) {

		this->_vx = v._vx;
		this->_vy = v._vy;
		this->_vz = v._vz;
		this->_vw = v._vw;

		return *this;
	}

	//---Accessors---
	const float Vect::x() const { return this->_vx; }
	const float Vect::y() const { return this->_vy; }
	const float Vect::z() const { return this->_vz; }
	const float Vect::w() const { return this->_vw; }

	float& Vect::x() { return this->_vx; }
	float& Vect::y() { return this->_vy; }
	float& Vect::z() { return this->_vz; }
	float& Vect::w() { return this->_vw; }

}

// ---  End of File ---------------
