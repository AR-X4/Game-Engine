#include "MathEngine.h"

namespace Azul
{

	// Do your magic here
	void Debug::Print(const char* pName, const Vect& r)
	{
		Trace::out("%s:(Vect) \n", pName);
		Trace::out(" %9.4g %9.4g %9.4g %9.4g \n", r.x(), r.y(), r.z(), r.w());
		Trace::out("\n");
	}

	void Debug::Print(const char* pName, const Quat& r)
	{
		Trace::out("%s:(Quat) \n", pName);
		Trace::out(" %9.4g %9.4g %9.4g %9.4g \n", r.qx(), r.qy(), r.qz(), r.real());
		Trace::out("\n");
	}

	void Debug::Print(const char* pName, const Matrix& r)
	{
		Trace::out("%s:(Matrix) \n", pName);
		Trace::out(" %9.4g %9.4g %9.4g %9.4g \n", r.m0(), r.m1(), r.m2(), r.m3());
		Trace::out(" %9.4g %9.4g %9.4g %9.4g \n", r.m4(), r.m5(), r.m6(), r.m7());
		Trace::out(" %9.4g %9.4g %9.4g %9.4g \n", r.m8(), r.m9(), r.m10(), r.m11());
		Trace::out(" %9.4g %9.4g %9.4g %9.4g \n", r.m12(), r.m13(), r.m14(), r.m15());
		Trace::out("\n");
	}

}