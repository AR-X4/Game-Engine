#ifndef ANIM_CONTROLLER_H
#define ANIM_CONTROLLER_H

#include "TimerController.h"
#include "Anim.h"
#include "Models/FBXSkinnedModel.h"
#include "Shaders/SSBO.h"

namespace Azul
{
	class AnimController
	{
	public:
		AnimController(AnimTime delta, bool transIn, FBXSkinnedModel* pModelSkin, SSBO* pBoneWorld, Clip::Name ClipNameA);
		AnimController(const AnimController &) = delete;
		AnimController &operator = (const AnimController &) = delete;
		~AnimController();

		void Update();

		void Play();
		void Pause();

		void SetPos(float x, float y, float z);
		void SetRot(float x, float y, float z);
		void SetScale(float x, float y, float z);
		void SetName(const char* name);

		Anim* GetAnim() const;
		

	private:
		TimerController *poTimerControl;
		Anim* poAnim;
		bool trans;

	public:
		bool			bPlay;
		AnimController* pNext;
		AnimController* pPrev;

	};
}

#endif

// --- End of File ---
