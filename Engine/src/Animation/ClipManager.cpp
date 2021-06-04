#include "ClipManager.h"



namespace Azul {

	ClipManager::ClipManager()
		:poHead(nullptr)
	{}

	void ClipManager::Add(Clip* pClip)
	{
		// Get the instance to the manager
		ClipManager* ClipManager = ClipManager::privGetInstance();
		assert(ClipManager);

		// Create a Model
		assert(pClip);

		// Now add it to the manager
		ClipManager->privAddToFront(pClip);
	}

	void ClipManager::Create()
	{
		ClipManager* pClipManager = ClipManager::privGetInstance();
		assert(pClipManager);
	}

	void ClipManager::Destroy()
	{
		ClipManager* pClipManager = ClipManager::privGetInstance();
		assert(pClipManager);

		Clip* pLink = pClipManager->poHead;

		while (pLink != nullptr)
		{
			Clip* pTmp = pLink;
			pLink = pLink->pNext;
			pClipManager->privRemove(pTmp);
			delete pTmp;
		}
	}

	Clip* ClipManager::Find(Clip::Name _name)
	{
		ClipManager* pClipManager = ClipManager::privGetInstance();
		assert(pClipManager);

		Clip* pLink = pClipManager->poHead;

		while (pLink != nullptr)
		{
			if (pLink->ClipName == _name) {
				return pLink;
			}

			pLink = pLink->pNext;
		}

		assert(false);
		return nullptr;
	}

	ClipManager* ClipManager::privGetInstance()
	{
		static ClipManager clipMan;
		return &clipMan;
	}

	void ClipManager::privAddToFront(Clip* pNode)
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

	void ClipManager::privRemove(Clip* pNode)
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