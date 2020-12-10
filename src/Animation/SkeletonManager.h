#ifndef SKELETON_MANAGER_H
#define SKELETON_MANAGER_H

#include "SkeletonHierarchy.h"

namespace Azul {

	class SkeletonManager
	{
	public:



		static void Add(SkeletonHierarchy* pSkel);
		static void Create();
		static void Destroy();
		static SkeletonHierarchy* Find(SkeletonHierarchy::Name _name);

	private:  

		static SkeletonManager* privGetInstance();
		SkeletonManager();

		void privAddToFront(SkeletonHierarchy* pNode);
		void privRemove(SkeletonHierarchy* pNode);

	public:

		SkeletonHierarchy* poHead;


	};
}
#endif