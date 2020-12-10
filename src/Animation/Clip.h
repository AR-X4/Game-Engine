

#ifndef CLIP_H
#define CLIP_H

#include "Bone.h"
#include "Time/Time.h"

namespace Azul
{
	class Clip
	{
	public:

		enum class Name
		{
			HUMANOID_RUN,
			HUMANOID_SHOTGUN,
			PUNCH,
			TEDDY
		};

		struct FrameBucket
		{
			~FrameBucket()
			{
				delete[] poBone;
			}

			FrameBucket *nextBucket;
			FrameBucket *prevBucket;
			Time        KeyTime;
			Bone        *poBone;
			char        pad[4];
		};

	public:
		Clip(int _numFrames, FrameBucket* poHeadIn, Name _ClipName);

		Clip() = delete;
		Clip(const Clip &) = delete;
		Clip &operator = (const Clip &) = delete;
		~Clip();

		Time GetTotalTime();
		void AnimateBones(Time tCurr, Bone *pResult);

	private:
		//void privSetAnimationData();
		Time privFindMaxTime();
		int  privFindNumFrames();

	private:
		int          numBones;
		int          numFrames;
		Time         TotalTime;
		FrameBucket *poHead;

	public:
		Clip* pNext;
		Clip* pPrev;
		Name ClipName;

	};
}

#endif

//--- End of File ---
