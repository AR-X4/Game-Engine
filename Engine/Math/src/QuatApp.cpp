#include "MathEngine.h"
#include "MathApp.h"

//----------------------------------------------------------------------------- 
// Mutates result to be a slerp between the source and target by the
// specified factor.
// For a factor of 0.0, result == source.
// For a factor of 1.0, result == target.
//----------------------------------------------------------------------------- 

namespace Azul
{
	void QuatApp::Slerp(Quat &result, const Quat &src, const Quat &tar, const float slerpFactor)
	{
		float cosTmp = src.dot(tar);
		if (cosTmp >= 1.0f)
		{
			result = src;
		}
		else
		{
			Quat target(tar);

			if (cosTmp < 0.0f)
			{
				cosTmp *= -1.0f;
				target *= -1.0f;
			}
		
			float tarFactor = slerpFactor;
			float srcFactor = 1.0f;
			srcFactor -= slerpFactor;

			if (cosTmp < (1.0f))
			{
				const float f1 = Trig::acos(cosTmp);
				float f2 = 1.0f;
				f2 /= Trig::sin(f1);

				srcFactor = Trig::sin(srcFactor * f1);
				srcFactor *= f2;

				tarFactor = Trig::sin(tarFactor * f1);
				tarFactor *= f2;
			}

			Quat source(src);
			source *= srcFactor;
			target *= tarFactor;
			result = source;
			result += target;
		}
	};

	void QuatApp::SlerpArray(Quat *out, const Quat *source, const Quat *target, const float slerpFactor, const int numQuats)
	{
		for (int i = 0; i < numQuats; i++)
		{
			Slerp(out[i], source[i], target[i], slerpFactor);
		}
	};
}

// ---  End of File ---------------
