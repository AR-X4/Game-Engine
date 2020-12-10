

#include "AnimController.h"
#include "Anim.h"

namespace Azul
{
	AnimController::AnimController(Time delta, SkeletonHierarchy::Name SkelName, Clip::Name ClipName)
		: bPlay(false), pNext(nullptr), pPrev(nullptr)
	{
		// Animation object
		this->poAnim = new Anim(SkelName, ClipName);
		assert(this->poAnim);

		// TimerController
		Time maxTime = poAnim->FindMaxTime();
		Time minTime = Time(Time::Duration::ZERO);
		this->poTimerControl = new TimerController(delta, minTime, maxTime);
		assert(this->poTimerControl);

	}

	AnimController::~AnimController()
	{
		delete poAnim;
		delete poTimerControl;
	}

	void AnimController::Update()
	{
		this->poTimerControl->Update();

		// interpolate all bones
		this->poAnim->Animate(this->poTimerControl->GetCurrTime());

		// pose skeleton
		this->poAnim->PoseSkeleton(this->poTimerControl->GetCurrTime());
	}
	void AnimController::AddClip(Clip* ClipIn)
	{
		assert(ClipIn);
		this->poAnim->pClip = ClipIn;

	}
	void AnimController::Play()
	{
		this->bPlay = true;
	}
	void AnimController::Pause()
	{
		this->bPlay = false;
	}
	void AnimController::SetPos(float x, float y, float z)
	{
		this->poAnim->GetSkel()->SetPos(x, y, z);
	}
	void AnimController::SetRot(float x, float y, float z)
	{
		this->poAnim->GetSkel()->SetRot(x, y, z);
	}
	void AnimController::SetScale(float x, float y, float z)
	{
		this->poAnim->GetSkel()->SetScale(x, y, z);
	}
}

// --- End of File ---
