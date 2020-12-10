

#ifndef ANIM_H
#define ANIM_H

#include "Time/Timer.h"
#include "GameObject/GameObjectAnim.h"
#include "Clip.h"
#include "Skeleton.h"

namespace Azul
{ 
	
	#define BONE_WIDTH 0.1f

	class Anim
	{
	public:
		Anim(SkeletonHierarchy::Name SkelName, Clip::Name ClipName);
		Anim() = delete;
		Anim(const Anim &) = delete;
		Anim &operator = (const Anim &) = delete;
		~Anim();

		void Animate(Time tCurr);
		void PoseSkeleton(Time tCurr);
		Time FindMaxTime();

		Skeleton* GetSkel();

	private:
		void privSetBonePose(GameObjectAnim *node);

	public:
		Clip* pClip;//add accessor

	private:
		
		Bone           *poBoneResult;
		Skeleton       *poSkeleton;

		int   numBones;
		float boneWidth;
	};


}

#endif

// --- End of File ---
