//-----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef ENGINE_MATH_DEBUG_H
#define ENGINE_MATH_DEBUG_H

namespace Azul
{
	// Forward declarations:
	class Vect; 
	class Matrix;

	class Debug
	{
	public:

		static void Print(const char* pName, const Vect &r);
		static void Print(const char* pName, const Matrix& r);

	};
}

#endif

// ---  End of File ---------------
