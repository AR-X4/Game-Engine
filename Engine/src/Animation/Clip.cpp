#include "Clip.h"
#include "Time/AnimTime.h"
#include "Bone.h"
#include "Mixer.h"
#include "Shaders/ShaderManager.h"

namespace Azul
{

	Clip::Clip( int _numBones, FrameBucket* poHeadIn, Name _ClipName)
	: numBones(_numBones),
	numFrames(0),
	TotalTime(AnimTime::Duration::ZERO),
	poHead(poHeadIn),
	pNext(nullptr),
	pPrev(nullptr),
	ClipName(_ClipName)
	//pSSBO_Result(nullptr)
	{
		//this->privSetAnimationData();
		this->TotalTime = this->privFindMaxTime();
		this->numFrames = this->privFindNumFrames();

		this->pSSBO_Result = new SSBO();

		// Only done once per clip
		Bone* pBone = new Bone[(unsigned int)numBones]();
		assert(pBone);
		this->pSSBO_Result->Set((unsigned int)numBones, sizeof(Bone), pBone);
		delete[] pBone;
	}

	Clip::~Clip()
	{
		FrameBucket *pTmp = poHead;

		while (pTmp != nullptr)
		{
			FrameBucket *pDeleteMe = pTmp;
			pTmp = pTmp->nextBucket;
			delete pDeleteMe;
		}

		delete this->pSSBO_Result;
	}

	void Clip::AnimateBones(AnimTime tCurr)
	{
		// First one 
		FrameBucket* pTmp = this->poHead;

		// Find which key frames
		while (tCurr >= pTmp->KeyTime && pTmp->nextBucket != nullptr)
		{
			pTmp = pTmp->nextBucket;
		}

		// pTmp is the "B" key frame
		// pTmp->prev is the "A" key frame
		FrameBucket* pA = pTmp->prevBucket;
		FrameBucket* pB = pTmp;

		// find the "S" of the time
		float tS = (tCurr - pA->KeyTime) / (pB->KeyTime - pA->KeyTime);

		Mixer::Blend_GPU(pA->poKeyframe, pB->poKeyframe, this->pSSBO_Result, tS, numBones);

		// take pRseult and put into another SSBO
		// return SSBO
		// blend with result ssbo of another clip in Anim class


	}

	
	int Clip::privFindNumFrames()
	{
		int count = 0;
		FrameBucket *pTmp = this->poHead;

		while (pTmp->nextBucket != nullptr)
		{
			count++;
			pTmp = pTmp->nextBucket;
		}
		return count;
	}

	AnimTime Clip::privFindMaxTime()
	{
		AnimTime tMax;
		FrameBucket *pTmp = this->poHead;

		while (pTmp->nextBucket != nullptr)
		{
			pTmp = pTmp->nextBucket;
		}

		tMax = pTmp->KeyTime;

		return tMax;
	}

	AnimTime Clip::GetTotalTime()
	{
		return this->TotalTime;
	}

	
}

// --- End of File ---
