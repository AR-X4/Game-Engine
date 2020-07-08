
#include "Models/NullModel.h"
#include "GraphicsObject/GraphicsObject_Null.h"

#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "PCSTreeForwardIterator.h"

#include "Models/ModelManager.h"
#include "Shaders/ShaderManager.h"

namespace Azul
{
	void GameObjectManager::Add(GameObject* pObj)
	{
		assert(pObj != 0);

		// Get singleton
		GameObjectManager* pGOM = GameObjectManager::privGetInstance();

		// Get root node
		PCSNode* pRootNode = pGOM->poRootTree->GetRoot();

		// insert object to root
		pGOM->poRootTree->Insert(pObj, pRootNode);
	}


	void GameObjectManager::Destroy()
	{
		// Get singleton
		GameObjectManager* pGOM = GameObjectManager::privGetInstance();
		assert(pGOM);

		PCSTree* pTree = pGOM->poRootTree;
		PCSNode* pNode = nullptr;

		PCSTreeForwardIterator pForIter(pTree->GetRoot());
		pNode = pForIter.First();
		PCSNode* pTmp = nullptr;
		while (!pForIter.IsDone())
		{
			pTmp = pForIter.CurrentItem();

			pNode = pForIter.Next();
			delete pTmp;

		}


	}

	void GameObjectManager::Update(float currentTime)
	{
		GameObjectManager* pGOM = GameObjectManager::privGetInstance();
		assert(pGOM);

		PCSNode* pRootNode = pGOM->poRootTree->GetRoot();
		assert(pRootNode);

		PCSTreeForwardIterator pForwardIter(pRootNode);
		PCSNode* pNode = pForwardIter.First();

		GameObject* pGameObj = 0;

		while (!pForwardIter.IsDone())
		{
			assert(pNode);
			// Update the game object
			pGameObj = (GameObject*)pNode;
			pGameObj->Update(currentTime);

			pNode = pForwardIter.Next();
		}
	}

	void GameObjectManager::Draw()
	{
		GameObjectManager* pGOM = GameObjectManager::privGetInstance();
		assert(pGOM);

		PCSNode* pRootNode = pGOM->poRootTree->GetRoot();
		assert(pRootNode);

		PCSTreeForwardIterator pForwardIter(pRootNode);
		PCSNode* pNode = pForwardIter.First();

		GameObject* pGameObj = 0;

		while (!pForwardIter.IsDone())
		{
			assert(pNode);
			// Update the game object
			pGameObj = (GameObject*)pNode;
			pGameObj->Draw();

			pNode = pForwardIter.Next();
		}
	}

	GameObjectManager::GameObjectManager()
	{
		// Create the root node (null object)
		NullModel* pModel = new NullModel(nullptr);
		assert(pModel);
		ModelManager::Add(pModel);

		
		ShaderManager::Add(ShaderObject::Name::NULL_SHADER, "nullRender");

		GraphicsObject_Null* pGraphicsObject = new GraphicsObject_Null(pModel, ShaderManager::Find(ShaderObject::Name::NULL_SHADER));
		GameObject* pGameRoot = new GameObject(pGraphicsObject);
		pGameRoot->SetName("GameObject_Root");

		// Create the tree
		this->poRootTree = new PCSTree();
		assert(this->poRootTree);

		// Attach the root node
		this->poRootTree->Insert(pGameRoot, this->poRootTree->GetRoot());
	}

	GameObjectManager::~GameObjectManager()
	{
		delete this->poRootTree;
	}

	GameObjectManager* GameObjectManager::privGetInstance(void)
	{
		// This is where its actually stored (BSS section)
		static GameObjectManager gom;
		return &gom;
	}

}

// --- End of File ---
