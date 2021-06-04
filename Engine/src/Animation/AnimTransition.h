#ifndef ANIM_TRANSITION_H
#define ANIM_TRANSITION_H

#include "TimerController.h"
#include "Anim.h"

namespace Azul
{
	class AnimTransition
	{
	public:
		AnimTransition(Anim* pAnim1In, Anim* pAnim2In);
		AnimTransition(const AnimTransition&) = delete;
		AnimTransition& operator = (const AnimTransition&) = delete;
		~AnimTransition();

		void Update();

		void StartTransition();
		void privTransitionTime();

	public:
		
		SSBO* poSSBO_Result;
		Anim* pAnim1;
		Anim* pAnim2;


		int numBones;
		float tS;
		bool StartEnable;
	};
}

#endif

// --- End of File ---

