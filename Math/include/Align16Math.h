
//
// Framework 
//
//    One header to rule them all, 
//        one header to find them, 
//        one header to bring them all 
//        and in the Linker bind them.   <thank you Tolkien>
//
//    Don't try to understand it... its very complicated
//
//    Systems:
//        Memory Tracking - dynamic memory leak tracking
//        C++ Unit Testing - unit testing with teardown
//        Trace - debug printing to visual studio output window
//        Performance timer - hardware cycle count micro timer for performance tracking
//        File logging - dumping data to an external logging file
//        Memory Alignment Base class - keeping alignment to 16 bytes for SIMD
//    
// ---------------------------------------------------------------------------
//    v.2.5.0 
//    v.2.6   - Baseline
//    v.2.6.1 - fixed warning TestRegistry
//    v.2.7   - Baseline
//    v.2.8   - warning 5039 windows.h
//    v.2.9   - fence
//    v.2.10  - msc_ver 1914, win 10.0.17
//    v.2.11  - warning fixes 
//    v.2.12  - mutex on all trace::out
//    v.2.13  - fixed warnings on mutex and default copy const / assignment
//    v.2.14  - FileIO remove new... so you can overload new/delete
//    v.2.15  - number tests pass reformat
//    v.2.16  - fixing static analysis issues
//    v.2.18  - teardown issue
//    v.2.19  - WinSDK, Compiler, Framework version reporting
//    v.2.20  - WinSDK, Compiler, Framework version reporting in Release mode
//    v.2.21  - x64 info header
//    v.2.22  - Timer renamed to PerformanceTimer 
//    v.2.23  - Mem Tracking now works with placement and alignment new
//    v.2.24  - new tracking with file names and line numbers
//    v.2.25  - fixing collision with unused_var() macros in align16
//    v.2.26  - any order for the link, removed FileIO.cpp and PerformanceTimer.cpp
//    v.2.27  - atomics in Timer 
//    v.2.28  - remove dynamic cast, warnings on atomic
//    v.2.29  - placement new on align16
//    v.2.30  - HUGE rework of UnitTest.h removed UnitTest.cpp
//    v.3.00  - Flatten into 2 files
//    v.3.01  - reduce includes
//    v.3.02  - removed framework main... std::call_once() now only one include <period>
//    v.3.03  - MemoryTracking reporting improvements
//    v.3.04  - Clean up of framework
//    v.3.05  - consolidate warnings
//    v.3.06  - Tweaked several settings on configurations 8 in total {debug/release,x86/64, mem_enable/disabled}
//    v.3.07  - nothrow is a problem
//    v.3.08  - application vs external memory tracking
//    v.3.09  - added log file
//    v.3.10  - modes printing
//    v.3.11  - banner before leak print out
//    v.3.12  - project settings
//    v.3.13  - UnitStats, mkdir only it doesn't exist
//    v.3.14  - file open errors... need a cleaner way to deal with them
//    v.3.15  - log directory - fixed for Memory: todo FileIO
//    v.3.16  - added default if MEMORY_LOGS_DIR is missing
//    v.3.17  - copyright
//    v.3.18  - warnings around string include
//    v.3.19  - removed string class include (it was system leaking)
//    v.3.20  - MR_FAST mode display string
//    v.3.21  - Trace::out2() dump to debug and stdio printf
//    v.3.22  - Added which test was runned
//    v.3.23  - MemTrace::LeakCount()
//    v.3.24  - placement new
//    v.3.25  - REPLACE_ME_STUB()
//    v.3.26  - bogus compiler warning in compiler C6011 
//    v.3.27  - INSERT_CODE_HERE
//    v.3.28  - code analysis warning
//    v.1.00  - Branched from Framework 3.28 to create Align16Math.h
//----------------------------------------------------------------------------- 

#ifndef ALIGN_16_MATH_H
#define ALIGN_16_MATH_H

#define ALIGN_16_MATH_VER "1.00"

// -----------------------------------------------------------------------------
// Warning levels turned on for Wall... that Should be supressed globally
//     Set here to avoid surpressing them in the settings
// -----------------------------------------------------------------------------

#pragma warning( disable : 4127 ) // conditional expression is constant
#pragma warning( disable : 4201 ) // nonstandard extension used: nameless struct/union
#pragma warning( disable : 4355 ) // 'this' : used in base member initializer list
#pragma warning( disable : 4514 ) // 'function': unreferenced inline function has been removed
#pragma warning( disable : 4571 ) // Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
#pragma warning( disable : 4625 ) // 'derived class' : copy constructor was implicitly defined as deleted because a base class copy constructor is inaccessible or deleted
#pragma warning( disable : 4626 ) // 'derived class' : assignment operator was implicitly defined as deleted because a base class assignment operator is inaccessible or deleted
#pragma warning( disable : 4668 ) // 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directive'
#pragma warning( disable : 4587 ) // 'anonymous_structure': behavior change: constructor is no longer implicitly called
#pragma warning( disable : 4588 ) // 'anonymous_structure': behavior change: destructor is no longer implicitly called
#pragma warning( disable : 4710 ) // 'function': function not inlined
#pragma warning( disable : 4711 ) // function 'function' selected for automatic inline expansion
#pragma warning( disable : 4820 ) // 'bytes' bytes padding added after construct 'member_name'
#pragma warning( disable : 4291 ) // 'declaration' : no matching operator delete found; memory will not be freed if initialization throws an exception
#pragma warning( disable : 5025 ) // 'type': move assignment operator was implicitly defined as deleted
#pragma warning( disable : 5026 ) // 'type': move constructor was implicitly defined as deleted
#pragma warning( disable : 5027 ) // 'type': move assignment operator was implicitly defined as deleted
#pragma warning( disable : 5039 ) // function': pointer or reference to potentially throwing function passed to extern C function under -EHc. Undefined behavior may occur if this function throws an exception.
#pragma warning( disable : 5045 ) // Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
#pragma warning( disable : 6011 ) // dereferencing NULL pointer <name>

// --------------------------------------
// General includes
// --------------------------------------

#include <assert.h>
#include <stdio.h>
#include <cmath>
#include <stdlib.h>  
#include <crtdbg.h>  

// -----------------------------------------------------
// Many standard includes have serious warnings 
//     need to wrap for Wall warnings
//
// Todo: reverify with major compiler releases... 
//       Come on Microsoft.. clean up your code
// -----------------------------------------------------

#pragma warning( push )
	#pragma warning( disable : 4820 ) // 'bytes' bytes padding added after construct 'member_name'
	#define NOMINMAX
	#include <Windows.h>
#pragma warning( pop ) 


// -----------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//
// Align16
//      Needed a clean way to allocate aligned data for Matrix, Quat, and Vector
//      Data is align by inheriting this class
//      
//
//      Used to be stand alone files... now all one include
//
// -----------------------------------------------------------------------

#define ALIGN_16_MATH_UNUSED_VAR(x) (void(x))

#pragma push_macro("new")
#undef new

	class Align16Math
	{
	public:

		// Placement new for align16
		void* operator new(size_t, void *p) noexcept
		{
			ALIGN_16_MATH_UNUSED_VAR(p);
			return p;
		}

		void *operator new(size_t size) noexcept
		{
			void *p = _aligned_malloc(size, 16);
			return p;
		}

		void operator delete(void *p)
		{
			_aligned_free(p);
		}

		void *operator new[](size_t size) noexcept
		{
			void *p = _aligned_malloc(size, 16);
			return p;
		}

		void operator delete[](void *p)
		{
			_aligned_free(p);
		}

		void *operator new(size_t      size,
							int         _BlockUse,
							char const* _FileName,
							int         _LineNumber) noexcept
		{
			ALIGN_16_MATH_UNUSED_VAR(_BlockUse);
			ALIGN_16_MATH_UNUSED_VAR(_FileName);
			ALIGN_16_MATH_UNUSED_VAR(_LineNumber);

			void *p = _aligned_malloc_dbg(size, 16, _FileName, _LineNumber);
			return p;
		}

		void operator delete(void        *p,
								int         _BlockUse,
								char const* _FileName,
								int         _LineNumber)
		{
			ALIGN_16_MATH_UNUSED_VAR(_BlockUse);
			ALIGN_16_MATH_UNUSED_VAR(_FileName);
			ALIGN_16_MATH_UNUSED_VAR(_LineNumber);
			_aligned_free_dbg(p);
		}

		void *operator new[](size_t        size,
								int         _BlockUse,
								char const* _FileName,
								int         _LineNumber)  noexcept
		{
			ALIGN_16_MATH_UNUSED_VAR(_BlockUse);
			ALIGN_16_MATH_UNUSED_VAR(_FileName);
			ALIGN_16_MATH_UNUSED_VAR(_LineNumber);

			void *p = _aligned_malloc_dbg(size, 16, _FileName, _LineNumber);
			return p;
		}

		void operator delete[](void      *p,
								int         _BlockUse,
								char const* _FileName,
								int         _LineNumber)
		{
			ALIGN_16_MATH_UNUSED_VAR(_BlockUse);
			ALIGN_16_MATH_UNUSED_VAR(_FileName);
			ALIGN_16_MATH_UNUSED_VAR(_LineNumber);
			_aligned_free_dbg(p);
		}

	};

#pragma pop_macro("new")


#endif //ALIGN_16_MATH_H

// ---  End of File ---------------
