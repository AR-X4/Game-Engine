

#include "MathEngine.h"

namespace Azul
{ 

	//--------------
	//---Big Four---
	//--------------
	Quat::Quat()
		:_qx(0),_qy(0.0f),_qz(0.0f),_qw(1.0f)
	{
	}

	Quat::~Quat()
	{
	}

	Quat::Quat(const Quat& other)
		:_mq(other._mq)
	{
	}

	Quat& Quat::operator=(const Quat& other)
	{
		if (this != &other)
		{
			this->_mq = other._mq;

			return *this;
		}
		return *this;
	}

	//------------------------------
	//---Specialized Constructors---
	//------------------------------
	Quat::Quat(const float& qx, const float& qy, const float& qz, const float& qw)
		:_qx(qx), _qy(qy), _qz(qz), _qw(qw)
	{
	}

	Quat::Quat(const Vect& qv, const float& qs)
		:_qx(qv.x()), _qy(qv.y()), _qz(qv.z()), _qw(qs)
	{
	}

	Quat::Quat(const Matrix& RotMatrix)
	{

		this->set(RotMatrix);
	}

	Quat::Quat(const Special type)
		:_qx(0.0f), _qy(0.0f), _qz(0.0f), _qw(1.0f)
	{
		if (type == Special::Zero) { this->_qw = 0.0f; }
	}

	Quat::Quat(const Rot1 axis, const float& rotAngle)
	{
		this->set(axis, rotAngle);
	}

	Quat::Quat(const Rot3, const float& xRot, const float& yRot, const float& zRot)
	{
		this->set(Rot3::XYZ, xRot, yRot, zRot);
	}

	Quat::Quat(const Rot, const Vect& rotAxis, const float& rotAngle)
	{
		this->set(Rot::AxisAngle, rotAxis, rotAngle);
	}

	Quat::Quat(const Orient orient, const Vect& forward, const Vect& up)
	{
		this->set(orient, forward, up);
	}

	//--------------------------------
	//------------Accessors-----------
	//--------------------------------

	//Setters
	Quat& Quat::set(const Matrix& RotMatrix)
	{
		float tr = RotMatrix.m0() + RotMatrix.m5() + RotMatrix.m10();
		float S;
		if (tr > 0.0f)
		{
			S = 2 * Trig::sqrt(tr + 1.0f);
			this->_qw = 0.25f * S;
			this->_qx = -(RotMatrix.m9() - RotMatrix.m6()) / S;
			this->_qy = -(RotMatrix.m2() - RotMatrix.m8()) / S;
			this->_qz = -(RotMatrix.m4() - RotMatrix.m1()) / S;
		}
		else if ((RotMatrix.m0() > RotMatrix.m5()) && (RotMatrix.m0() > RotMatrix.m10()))
		{
			S = 2 * Trig::sqrt(1.0f + RotMatrix.m0() - RotMatrix.m5() - RotMatrix.m10());
			this->_qw = -(RotMatrix.m9() - RotMatrix.m6()) / S;
			this->_qx = 0.25f * S;
			this->_qy = (RotMatrix.m1() + RotMatrix.m4()) / S;
			this->_qz = (RotMatrix.m2() + RotMatrix.m8()) / S;
		}
		else if (RotMatrix.m5() > RotMatrix.m10())
		{
			S = 2 * Trig::sqrt(1.0f + RotMatrix.m5() - RotMatrix.m0() - RotMatrix.m10());
			this->_qw = -(RotMatrix.m2() - RotMatrix.m8()) / S;
			this->_qx = (RotMatrix.m1() + RotMatrix.m4()) / S;
			this->_qy = 0.25f * S;
			this->_qz = (RotMatrix.m6() + RotMatrix.m9()) / S;
		}
		else
		{
			S = 2 * Trig::sqrt(1.0f + RotMatrix.m10() - RotMatrix.m0() - RotMatrix.m5());
			this->_qw = -(RotMatrix.m4() - RotMatrix.m1()) / S;
			this->_qx = (RotMatrix.m2() + RotMatrix.m8()) / S;
			this->_qy = (RotMatrix.m6() + RotMatrix.m9()) / S;
			this->_qz = 0.25f * S;
		}

		return *this;
	}

	Quat& Quat::set(const float& qx, const float& qy, const float& qz, const float& qw)
	{
		this->_qx = qx;
		this->_qy = qy;
		this->_qz = qz;
		this->_qw = qw;

		return *this;
	}

	Quat& Quat::set(const Rot1 axis, const float& rotAngle)
	{
		const float cos = Trig::cos(rotAngle * 0.5f);
		const float sin = Trig::sin(rotAngle * 0.5f);

		switch (axis) {
		
		case Rot1::X: 

			this->_qx = sin;
			this->_qy = 0.0f;
			this->_qz = 0.0f;
			this->_qw = cos;

			break;
		case Rot1::Y:

			this->_qx = 0.0f;
			this->_qy = sin;
			this->_qz = 0.0f;
			this->_qw = cos;

			break;
		case Rot1::Z:

			this->_qx = 0.0f;
			this->_qy = 0.0f;
			this->_qz = sin;
			this->_qw = cos;

			break;
		}

		return *this;
	}

	Quat& Quat::set(const Rot3, const float& xRot, const float& yRot, const float& zRot)
	{
		Quat qx(Rot1::X, xRot);
		Quat qy(Rot1::Y, yRot);
		Quat qz(Rot1::Z, zRot);

		return *this = qx * qy * qz;
	}

	Quat& Quat::set(const Vect& qv, const float& qs)
	{
		this->_qx = qv.x();
		this->_qy = qv.y();
		this->_qz = qv.z();
		this->_qw = qs;

		return *this;
	}

	Quat& Quat::set(const Rot, const Vect& rotAxis, const float& rotAngle)
	{
		const Vect axis = rotAxis.getNorm();

		const float angle = rotAngle * 0.5f;
		const float sin = Trig::sin(angle);
		const float cos = Trig::cos(angle);

		this->_qx = axis.x() * sin;
		this->_qy = axis.y() * sin;
		this->_qz = axis.z() * sin;
		this->_qw = cos;

		return *this;
	}

	Quat& Quat::set(const Orient orient, const Vect& forward, const Vect& up)
	{
		//find better way?
		this->set(Matrix((Matrix::Orient)orient, forward, up));
		return *this;
	}

	Quat& Quat::set(const Special type)
	{
		this->_qx = 0.0f;
		this->_qy = 0.0f;
		this->_qz = 0.0f;

		if (type == Special::Zero) { 
			this->_qw = 0.0f; 
		}
		else { 
			this->_qw = 1.0f;
		}

		return *this;
	}

	Quat& Quat::setVect(const Vect& v)
	{
		this->qx() = v.x();
		this->qy() = v.y();
		this->qz() = v.z();
		this->real() = 0.0f;

		return *this;
	}

	float& Quat::qx()   { return this->_qx; }
	float& Quat::qy()   { return this->_qy; }
	float& Quat::qz()   { return this->_qz; }
	float& Quat::real() { return this->_qw; }

	float& Quat::operator[](const x_enum) { return this->_qx; }
	float& Quat::operator[](const y_enum) { return this->_qy; }
	float& Quat::operator[](const z_enum) { return this->_qz; }
	float& Quat::operator[](const w_enum) { return this->_qw; }

	void Quat::getVect(Vect& v) const
	{
		v.x() = this->qx();
		v.y() = this->qy();
		v.z() = this->qz();
		v.w() = 1.0f;
	}

	void Quat::getAxis(Vect& v) const
	{
		v.x() = this->qx();
		v.y() = this->qy();
		v.z() = this->qz();
		v.w() = 1.0f;

		//find angle and make sure isnt 0
		//const float angle = this->getAngle();
		//tmp /= Trig::sin(angle / 2);

		//v = tmp;

		//OR...
		//v /= Trig::sin(Trig::acos(this->_qw));
	}

	const float Quat::getAngle() const
	{
		return 2.0f * Trig::acos(this->_qw);
	}

	//Getters
	const float Quat::operator[](const x_enum) const { return this->_qx; }
	const float Quat::operator[](const y_enum) const { return this->_qy; }
	const float Quat::operator[](const z_enum) const { return this->_qz; }
	const float Quat::operator[](const w_enum) const { return this->_qw; }

	const float Quat::qx() const { return this->_qx; }
	const float Quat::qy() const { return this->_qy; }
	const float Quat::qz() const { return this->_qz; }
	const float Quat::real() const { return this->_qw; }

	//---------------------
	//---Unary Operators---
	//---------------------
	const Quat Quat::operator+() const
	{
		return *this;
	}

	const Quat Quat::operator+(const Quat& other) const
	{
		return Quat(this->_qx + other._qx, 
					this->_qy + other._qy, 
					this->_qz + other._qz,
					this->_qw + other._qw);
	}

	const Quat Quat::operator+(const float scalar) const
	{
		return Quat(this->_qx + scalar,
					this->_qy + scalar,
					this->_qz + scalar,
					this->_qw + scalar);
	}

	Quat& Quat::operator+=(const Quat& other)
	{
		this->_qx += other._qx;
		this->_qy += other._qy;
		this->_qz += other._qz;
		this->_qw += other._qw;

		return *this;
	}

	Quat& Quat::operator+=(const float scalar)
	{
		this->_qx += scalar;
		this->_qy += scalar;
		this->_qz += scalar;
		this->_qw += scalar;

		return *this;
	}

	const Quat Quat::operator-()
	{
		this->_qx *= -1.0f;
		this->_qy *= -1.0f;
		this->_qz *= -1.0f;
		this->_qw *= -1.0f;

		return *this;
	}

	const Quat Quat::operator-(const Quat& other) const
	{
		return Quat(this->_qx - other._qx,
					this->_qy - other._qy,
					this->_qz - other._qz,
					this->_qw - other._qw);
	}

	const Quat Quat::operator-(const float scalar) const
	{
		return Quat(this->_qx - scalar,
					this->_qy - scalar,
					this->_qz - scalar,
					this->_qw - scalar);
	}

	Quat& Quat::operator-=(const Quat& other)
	{
		this->_qx -= other._qx;
		this->_qy -= other._qy;
		this->_qz -= other._qz;
		this->_qw -= other._qw;

		return *this;
	}

	Quat& Quat::operator-=(const float scalar)
	{
		this->_qx -= scalar;
		this->_qy -= scalar;
		this->_qz -= scalar;
		this->_qw -= scalar;

		return *this;
	}

	const Quat Quat::operator*(const Quat& other) const
	{		
		const Vect pV(*this);
		const Vect qV(other);

		const float pw = this->_qw;
		const float qw = other._qw;

		return Quat(qV.cross(pV) + (qw * pV) + (pw * qV), (pw*qw)-pV.dot(qV));
	}

	const Quat Quat::operator*(const float scalar) const
	{
		return Quat(this->_qx * scalar,
					this->_qy * scalar,
					this->_qz * scalar,
					this->_qw * scalar);
	}

	const Matrix Quat::operator*(const Matrix& other) const
	{
		Matrix tmp(*this);

		return tmp * other;
	}

	Quat& Quat::operator*=(const Quat& other)
	{
		const Vect pV(*this);
		const Vect qV(other);

		const float pw = this->_qw;
		const float qw = other._qw;

		this->set(qV.cross(pV) + (qw * pV) + (pw * qV), (pw * qw) - pV.dot(qV));
		return *this;
	}

	Quat& Quat::operator*=(const float scalar)
	{
		this->_qx *= scalar;
		this->_qy *= scalar;
		this->_qz *= scalar;
		this->_qw *= scalar;

		return *this;
	}

	const Quat Quat::operator/(const Quat& other) const
	{
		return Quat(this->_qx / other._qx,
					this->_qy / other._qy,
					this->_qz / other._qz,
					this->_qw / other._qw);
	}

	const Quat Quat::operator/(const float scalar) const
	{
		return Quat(this->_qx / scalar,
					this->_qy / scalar,
					this->_qz / scalar,
					this->_qw / scalar);
	}

	Quat& Quat::operator/=(const Quat& other)
	{
		this->_qx /= other._qx;
		this->_qy /= other._qy;
		this->_qz /= other._qz;
		this->_qw /= other._qw;

		return *this;
	}

	Quat& Quat::operator/=(const float scalar)
	{
		this->_qx /= scalar;
		this->_qy /= scalar;
		this->_qz /= scalar;
		this->_qw /= scalar;

		return *this;
	}

	const Quat Quat::multByElement(const Quat& other)
	{
		return Quat(this->_qx * other._qx,
					this->_qy * other._qy,
					this->_qz * other._qz,
					this->_qw * other._qw);
	}

	const bool Quat::isEqual(const Quat& q, const float& tolerance) const
	{
		return Util::isEqual(this->_qx, q.qx(), tolerance) &&
			   Util::isEqual(this->_qy, q.qy(), tolerance) &&
			   Util::isEqual(this->_qz, q.qz(), tolerance) &&
			   Util::isEqual(this->_qw, q.real(), tolerance);
	}

	const bool Quat::isNegEqual(const Quat& q, const float& tolerance) const
	{
		return Util::isEqual(-this->_qx, q.qx(), tolerance) &&
			   Util::isEqual(-this->_qy, q.qy(), tolerance) &&
			   Util::isEqual(-this->_qz, q.qz(), tolerance) &&
			   Util::isEqual(-this->_qw, q.real(), tolerance);
	}

	const bool Quat::isEquivalent(const Quat& q, const float& tolerance) const
	{
		return isEqual(q, tolerance) || isNegEqual(q, tolerance);
	}

	const bool Quat::isConjugateEqual(const Quat& q, const float& tolerance) const
	{
		return Util::isEqual(-this->_qx, q.qx(), tolerance) &&
			   Util::isEqual(-this->_qy, q.qy(), tolerance) &&
			   Util::isEqual(-this->_qz, q.qz(), tolerance) &&
			   Util::isEqual(this->_qw, q.real(), tolerance);
	}

	const bool Quat::isIdentity(const float& tolerance) const
	{
		return Util::isZero(this->_qx, tolerance) &&
			   Util::isZero(this->_qy, tolerance) &&
			   Util::isZero(this->_qz, tolerance) &&
			   Util::isOne(this->_qw, tolerance);
	}

	const bool Quat::isZero(const float& tolerance) const
	{
		return Util::isZero(this->_qx, tolerance) &&
			   Util::isZero(this->_qy, tolerance) &&
			   Util::isZero(this->_qz, tolerance) &&
			   Util::isZero(this->_qw, tolerance);
	}

	const bool Quat::isNormalized(const float& tolerance) const
	{
		return(Util::isEqual(1.0f, this->dot(*this), tolerance));
	}

	const Quat Quat::getConj() const
	{
		return(Quat(-this->_qx, -this->_qy, -this->_qz, this->_qw));
	}

	Quat& Quat::conj()
	{
		this->_qx *= -1.0f;
		this->_qy *= -1.0f;
		this->_qz *= -1.0f;

		return *this;
	}

	const Quat Quat::getT() const
	{
		return(Quat(-this->_qx, -this->_qy, -this->_qz, this->_qw));
	}

	Quat& Quat::T()
	{

		this->_qx *= -1.0f;
		this->_qy *= -1.0f;
		this->_qz *= -1.0f;

		return *this;
	}

	const float Quat::mag() const
	{
		return Trig::sqrt(this->dot(*this));
	}

	const float Quat::magSquared() const
	{
		return this->dot(*this);
	}

	const float Quat::invMag() const
	{
		return 1.0f / this->mag();
	}

	Quat& Quat::norm()
	{
		return *this /= this->mag();
	}

	const Quat Quat::getNorm() const
	{
		return Quat(*this) /= this->mag();
	}

	Quat& Quat::inv()
	{
		return this->conj() /= this->magSquared();
	}

	const Quat Quat::getInv() const
	{
		return Quat(this->getConj() / this->magSquared());
	}

	void Quat::Lqcvq(const Vect& vIn, Vect& vOut) const
	{

		const Vect qv(*this);
		vOut = (2.0f * this->_qw) * (qv.cross(vIn)) + ((this->_qw * this->_qw - qv.dot(qv)) * vIn) + (2.0f * qv.dot(vIn)) * qv;
	}

	void Quat::Lqvqc(const Vect& vIn, Vect& vOut) const
	{

		const Vect qv(*this);
		vOut = (2.0f * this->_qw) * (vIn.cross(qv)) + ((this->_qw * this->_qw - qv.dot(qv)) * vIn) + (2.0f * qv.dot(vIn)) * qv;
	}

	const float Quat::dot(const Quat& other) const
	{
		return (this->_qx * other.qx() + this->_qy * other._qy + this->_qz * other._qz + this->_qw * other._qw);
	}

	Quat operator+(const float& scalar, const Quat& other)
	{
		return Quat(scalar + other.qx(),
					scalar + other.qy(),
					scalar + other.qz(),
					scalar + other.real());
	}

	Quat operator-(const float& scalar, const Quat& other)
	{
		return Quat(scalar - other.qx(),
					scalar - other.qy(),
					scalar - other.qz(),
					scalar - other.real());
	}

	Quat operator/(const float& scalar, const Quat& other)
	{
		return Quat(scalar / other.qx(),
					scalar / other.qy(),
					scalar / other.qz(),
					scalar / other.real());
	}

	Quat operator*(const float& scalar, const Quat& other)
	{
		return Quat(scalar * other.qx(),
					scalar * other.qy(),
					scalar * other.qz(),
					scalar * other.real());
	}

}

// ---  End of File ---------------
