#ifndef SKELETON_HIERARCHY_H
#define SKELETON_HIERARCHY_H

class SkeletonHierarchy
{
public:

	enum class Name
	{
		HUMANOID_RUN,
		HUMANOID_SHOTGUN,
		PUNCH,
		TEDDY

	};

	struct BoneData
	{
		char name[64];
		int  index;
		int  parentIndex;
	};

	SkeletonHierarchy(BoneData* pBoneData, int numBones, Name _skelName);
	~SkeletonHierarchy();
	SkeletonHierarchy() = delete;
	SkeletonHierarchy(const SkeletonHierarchy&) = delete;
	SkeletonHierarchy& operator = (const SkeletonHierarchy&) = delete;


	SkeletonHierarchy* pNext;
	SkeletonHierarchy* pPrev;
	BoneData* pBones;
	int             numBones;
	Name SkelName;


};

#endif