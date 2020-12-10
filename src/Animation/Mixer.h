

#ifndef MIXER_H
#define MIXER_H

#include "Time/Timer.h"
#include "Bone.h"

namespace Azul
{
	class Mixer
	{
	public:
		static void Blend(	Bone *pResult,
							const Bone *pA,
							const Bone *pB,
							const float tS,
							const int numBones);
	};
}

#endif

// --- End of File ---
