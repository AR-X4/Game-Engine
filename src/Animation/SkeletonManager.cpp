#include "SkeletonManager.h"


namespace Azul {

	SkeletonManager::SkeletonManager() 
		:poHead(nullptr)
	{}

	void SkeletonManager::Add(SkeletonHierarchy* pSkel)
	{
		// Get the instance to the manager
		SkeletonManager* pSkeletonManager = SkeletonManager::privGetInstance();
		assert(pSkeletonManager);

		// Create a Model
		assert(pSkel);

		// Now add it to the manager
		pSkeletonManager->privAddToFront(pSkel);
	}

	void SkeletonManager::Create()
	{
		SkeletonManager* pSkeletonManager = SkeletonManager::privGetInstance();
		assert(pSkeletonManager);
	}

	void SkeletonManager::Destroy()
	{
		SkeletonManager* pSkeletonManager = SkeletonManager::privGetInstance();
		assert(pSkeletonManager);

		SkeletonHierarchy* pLink = pSkeletonManager->poHead;

		while (pLink != nullptr)
		{
			SkeletonHierarchy* pTmp = pLink;
			pLink = pLink->pNext;
			pSkeletonManager->privRemove(pTmp);
			delete pTmp;
		}
	}

	SkeletonHierarchy* SkeletonManager::Find(SkeletonHierarchy::Name _name)
	{
		SkeletonManager* pSkeletonManager = SkeletonManager::privGetInstance();
		assert(pSkeletonManager);

		SkeletonHierarchy* pLink = pSkeletonManager->poHead;

		while (pLink != nullptr)
		{
			if (pLink->SkelName == _name) {
				return pLink;
			}

			pLink = pLink->pNext;
		}

		assert(false);
		return nullptr;
	}

	SkeletonManager* SkeletonManager::privGetInstance()
	{
		static SkeletonManager skelMan;
		return &skelMan;
	}

	void SkeletonManager::privAddToFront(SkeletonHierarchy* pNode)
	{
		assert(pNode != 0);

		if (this->poHead == 0)
		{
			this->poHead = pNode;
			pNode->pNext = 0;
			pNode->pPrev = 0;
		}
		else
		{
			pNode->pNext = this->poHead;
			this->poHead->pPrev = pNode;
			this->poHead = pNode;
		}
	}

	void SkeletonManager::privRemove(SkeletonHierarchy* pNode)
	{
		assert(pNode);

		if (pNode->pPrev != nullptr)
		{	// middle or last node
			pNode->pPrev->pNext = pNode->pNext;
		}
		else
		{  // first
			poHead = pNode->pNext;
		}

		if (pNode->pNext != nullptr)
		{	// middle node
			pNode->pNext->pPrev = pNode->pPrev;
		}
	}
}