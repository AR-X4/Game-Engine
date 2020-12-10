#ifndef SKELETON_H
#define SKELETON_H

#include "GameObject/GameObjectAnim.h"
#include "Bone.h"
#include "SkeletonHierarchy.h"
#include "GameObject/GameObjectRigid.h"

namespace Azul
{
	class Skeleton
	{
	
	public:
		
		Skeleton(Bone* pBoneResultBuff, SkeletonHierarchy* pSkelIn);

		Skeleton() = delete;
		Skeleton(const Skeleton &) = delete;
		Skeleton &operator = (const Skeleton &) = delete;
		~Skeleton();

		GameObjectAnim *GetFirstBone();
		void SetPos(float x, float y, float z);
		void SetRot(float x, float y, float z);
		void SetScale(float x, float y, float z);

		
	private:
		void privSetAnimationHierarchy(Bone *pBone);
		GameObjectAnim *privFindBoneByIndex(int index);

	private:
		GameObjectAnim* pFirstBone;
		SkeletonHierarchy* SkelHierarchy;
		GameObjectRigid* pRigidBody;
		
	};
}

#endif

// --- End of File ---
