#include "AnimControllerManager.h"


namespace Azul {

	AnimControllerManager::AnimControllerManager()
		:poHead(nullptr)
	{}

	void AnimControllerManager::Add(AnimController* pAnimControl)
	{
		// Get the instance to the manager
		AnimControllerManager* pAnimControllerManager = AnimControllerManager::privGetInstance();
		assert(pAnimControllerManager);

		// Create a Model
		assert(pAnimControl);

		// Now add it to the manager
		pAnimControllerManager->privAddToFront(pAnimControl);
	}

	void AnimControllerManager::Create()
	{
		AnimControllerManager* pAnimControllerManager = AnimControllerManager::privGetInstance();
		assert(pAnimControllerManager);
	}

	void AnimControllerManager::Destroy()
	{
		AnimControllerManager* pAnimControllerManager = AnimControllerManager::privGetInstance();
		assert(pAnimControllerManager);

		AnimController* pLink = pAnimControllerManager->poHead;

		while (pLink != nullptr)
		{
			AnimController* pTmp = pLink;
			pLink = pLink->pNext;
			pAnimControllerManager->privRemove(pTmp);
			delete pTmp;
		}
	}

	void AnimControllerManager::Update()
	{
		// Get the instance to the manager
		AnimControllerManager* pAnimControllerManager = AnimControllerManager::privGetInstance();
		assert(pAnimControllerManager);

		AnimController* pLink = pAnimControllerManager->poHead;

		while (pLink != nullptr)
		{
			if (pLink->bPlay == true) {
				pLink->Update();
			}
			pLink = pLink->pNext;
		}
	}

	

	AnimControllerManager* AnimControllerManager::privGetInstance()
	{
		static AnimControllerManager animControlMan;
		return &animControlMan;
	}

	void AnimControllerManager::privAddToFront(AnimController* pNode)
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

	void AnimControllerManager::privRemove(AnimController* pNode)
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



