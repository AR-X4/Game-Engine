
#include "ModelManager.h"

namespace Azul
{
	ModelManager::ModelManager()
	{
		this->poHead = nullptr;
	}

	ModelManager* ModelManager::privGetInstance()
	{
		// This is where its actually stored (BSS section)
		static ModelManager shaderMan;
		return &shaderMan;
	}

	void ModelManager::Create()
	{
		ModelManager* pModelManager;
		pModelManager = ModelManager::privGetInstance();
		assert(pModelManager);
	}

	void ModelManager::Destroy()
	{
		ModelManager* pModelManager = ModelManager::privGetInstance();
		assert(pModelManager);

		Model* pLink = pModelManager->poHead;

		while (pLink != nullptr)
		{
			Model* pTmp = pLink;
			pLink = (Model*)pLink->next;
			pModelManager->privRemove(pTmp, pModelManager->poHead);
			delete pTmp;
		}
	}

	Model* ModelManager::Find(Model::Name _name)
	{
		ModelManager* pModelManager = ModelManager::privGetInstance();
		assert(pModelManager);

		Model* pLink = pModelManager->poHead;

		while (pLink != nullptr)
		{
			if (pLink->ModelName == _name) {
				return pLink;
			}

			pLink = pLink->next;
		}

		assert(false);
		return nullptr;





	}

	void ModelManager::Add(Model* pModel)
	{
		// Get the instance to the manager
		ModelManager* pModelManager = ModelManager::privGetInstance();
		assert(pModelManager);

		// Create a Model
		assert(pModel);

		// Now add it to the manager
		pModelManager->privAddToFront(pModel, pModelManager->poHead);
	}

	void ModelManager::privAddToFront(Model* node, Model*& head)
	{
		assert(node != 0);

		if (head == 0)
		{
			head = node;
			node->next = 0;
			node->prev = 0;
		}
		else
		{
			node->next = head;
			head->prev = node;
			head = node;
		}
	}

	void ModelManager::privRemove(Model* pNode, Model*& poHead_)
	{
		assert(pNode);

		if (pNode->prev != nullptr)
		{	// middle or last node
			pNode->prev->next = pNode->next;
		}
		else
		{  // first
			poHead_ = (Model*)pNode->next;
		}

		if (pNode->next != nullptr)
		{	// middle node
			pNode->next->prev = pNode->prev;
		}

	}

}

// ---  End of File ---
