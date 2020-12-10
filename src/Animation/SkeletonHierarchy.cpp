#include "SkeletonHierarchy.h"

SkeletonHierarchy::SkeletonHierarchy(BoneData* pBoneData, int _numBones, Name _skelName)
	
	:pNext(nullptr),
	pPrev(nullptr),
	pBones(pBoneData),
	numBones(_numBones),
	SkelName(_skelName)
{
}

SkeletonHierarchy::~SkeletonHierarchy()
{
	delete[] pBones;
}
