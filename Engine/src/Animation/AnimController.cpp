#include "AnimController.h"
#include "Anim.h"

namespace Azul
{
	AnimController::AnimController(AnimTime delta, bool transIn, FBXSkinnedModel* pModelSkin, SSBO* pBoneWorld, Clip::Name ClipNameA)
		: poAnim(nullptr), trans(transIn), bPlay(false), pNext(nullptr), pPrev(nullptr)
	{
		// Animation object
		this->poAnim = new Anim(pModelSkin, pBoneWorld, ClipNameA);
		assert(this->poAnim);

		// TimerController
		AnimTime maxTime = poAnim->FindMaxTime();
		AnimTime minTime = AnimTime(AnimTime::Duration::ZERO);
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

		//find cleaner way
		if(!trans)
			poAnim->GpuComputeWorld(poAnim->pClip->pSSBO_Result);
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
	void AnimController::SetName(const char* name)
	{
		this->poAnim->GetSkel()->SetName(name);
	}
	Anim* AnimController::GetAnim() const
	{
		assert(poAnim);
		return poAnim;
	}
}

// --- End of File ---
