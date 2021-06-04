
#include "AnimTransition.h"
#include "Clip.h"
#include "Mixer.h"

#include "MathApp.h"

namespace Azul
{

	AnimTransition::AnimTransition(Anim* pAnim1In, Anim* pAnim2In)
		: 
		poSSBO_Result(nullptr),
		pAnim1(pAnim1In),
		pAnim2(pAnim2In),
		numBones(-1),
		tS(0.0f),
		StartEnable(false)
	{
		assert(pAnim1);
		assert(pAnim2);


		assert(pAnim1->GetNumBones() == pAnim2->GetNumBones());
		numBones = pAnim1->GetNumBones();

		this->poSSBO_Result = new SSBO();
		Bone* pBone = new Bone[(unsigned int)numBones]();
		this->poSSBO_Result->Set((unsigned int)numBones, sizeof(Bone), pBone);
		delete[] pBone;

		
	}

	AnimTransition::~AnimTransition()
	{
		delete this->poSSBO_Result;
	}

	void AnimTransition::StartTransition()
	{
		this->StartEnable = true;
	}

	void AnimTransition::privTransitionTime()
	{
		if (this->StartEnable)
		{
			float t_delta = 0.03f;
			float b = 1.0f;
			float a = 0.0f;
			static float t = 0.0f;
			t += t_delta;
			if (t > 1.0f)
			{
				t = 1.0f;
			}

			this->tS = a + t * (b - a);
		}
	}

	void AnimTransition::Update()
	{
		this->privTransitionTime();

		Mixer::Blend_GPU(pAnim1->pClip->pSSBO_Result, pAnim2->pClip->pSSBO_Result, this->poSSBO_Result, tS, numBones);

		pAnim1->GpuComputeWorld(this->poSSBO_Result);
	}

}
