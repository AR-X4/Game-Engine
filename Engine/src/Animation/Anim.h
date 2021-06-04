#ifndef ANIM_H
#define ANIM_H

#include "Time/AnimTimer.h"
#include "Models/FBXSkinnedModel.h"
#include "Clip.h"
#include "Skeleton.h"
#include "Shaders/SSBO.h"

namespace Azul
{ 
	class Anim
	{
	public:
		Anim(FBXSkinnedModel* pModelSkin, SSBO* pBoneWorld, Clip::Name ClipName);
		Anim() = delete;
		Anim(const Anim &) = delete;
		Anim &operator = (const Anim &) = delete;
		~Anim();

		void Animate(AnimTime tCurr);
		
		AnimTime FindMaxTime();

		Skeleton* GetSkel() const;
		int GetNumBones() const;

		void GpuComputeWorld(SSBO* Result);		

	public:
		Clip* pClip;

	private:
		
		Skeleton       *poSkeleton;

		ShaderObject* pWorldComputeShader;
		SSBO* pHierarchyTable;
		SSBO* pBoneWorld_SBO;
		unsigned int hierarchyDepth;

		int   numBones;
		
	};
}

#endif

// --- End of File ---
