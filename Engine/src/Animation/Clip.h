

#ifndef CLIP_H
#define CLIP_H

#include "Bone.h"
#include "Time/AnimTime.h"
#include "Shaders/SSBO.h"

namespace Azul
{
	class Clip
	{
	public:

		enum class Name
		{
			HUMANOID_RUN,
			COMPRESSED_RUN,
			WALK,
			PUNCH,
			TEDDY,
			HUMANOID,
			UNINITIALIZED
		};

		struct FrameBucket
		{
			~FrameBucket()
			{
				delete poKeyframe;
			}

			FrameBucket *nextBucket;
			FrameBucket *prevBucket;
			AnimTime        KeyTime;
			//Bone        *poBone;
			//char        pad[4];
			SSBO* poKeyframe;
		};

	public:
		Clip(int _numFrames, FrameBucket* poHeadIn, Name _ClipName);

		Clip() = delete;
		Clip(const Clip &) = delete;
		Clip &operator = (const Clip &) = delete;
		~Clip();

		AnimTime GetTotalTime();
		void AnimateBones(AnimTime tCurr);

	private:
		
		AnimTime privFindMaxTime();
		int  privFindNumFrames();

	private:
		int          numBones;
		int          numFrames;
		AnimTime         TotalTime;
		FrameBucket *poHead;

	public:
		Clip* pNext;
		Clip* pPrev;
		Name ClipName;

		SSBO* pSSBO_Result;

	};
}

#endif

//--- End of File ---
