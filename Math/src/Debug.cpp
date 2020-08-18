

#include "Debug.h"
#include "MathEngine.h"

namespace Azul
{

	// Do your magic here

	void Debug::Print(const char* pName, const Vect& r)
	{
		printf("%s:(Vect) \n", pName);
		printf(" %9.4g %9.4g %9.4g %9.4g \n", r.x(), r.y(), r.z(), r.w());
		printf("\n");
	}

	void Debug::Print(const char* pName, const Matrix& r)
	{
		printf("%s:(Matrix) \n", pName);
		printf(" %9.4g %9.4g %9.4g %9.4g \n", r.m0(), r.m1(), r.m2(), r.m3());
		printf(" %9.4g %9.4g %9.4g %9.4g \n", r.m4(), r.m5(), r.m6(), r.m7());
		printf(" %9.4g %9.4g %9.4g %9.4g \n", r.m8(), r.m9(), r.m10(), r.m11());
		printf(" %9.4g %9.4g %9.4g %9.4g \n", r.m12(), r.m13(), r.m14(), r.m15());		
		printf("\n");
	}

}

// ---  End of File ---------------
