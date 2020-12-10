

#ifndef ANIM_CONTROLLER_H
#define ANIM_CONTROLLER_H

#include "TimerController.h"
#include "GameObject/GameObjectAnim.h"
#include "Anim.h"

namespace Azul
{
	class AnimController
	{
	public:
		AnimController(Time delta, SkeletonHierarchy::Name SkelName, Clip::Name ClipName);
		AnimController(const AnimController &) = delete;
		AnimController &operator = (const AnimController &) = delete;
		~AnimController();

		void Update();
		void AddClip(Clip* ClipIn);

		void Play();
		void Pause();

		void SetPos(float x, float y, float z);
		void SetRot(float x, float y, float z);
		void SetScale(float x, float y, float z);
		

	private:
		TimerController *poTimerControl;
		Anim            *poAnim;

	public:
		bool			bPlay;
		AnimController* pNext;
		AnimController* pPrev;

	};
}

#endif

// --- End of File ---
