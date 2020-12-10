
#include "Models/NullModel.h"
#include "GraphicsObject/GraphicsObject_Null.h"

#include "GameObject/GameObject.h"
#include "GameObject/GameObjectRigid.h"
#include "GameObject/GameObjectManager.h"
#include "PCSTreeForwardIterator.h"

#include "Models/ModelManager.h"
#include "Shaders/ShaderManager.h"

namespace Azul
{

	GameObjectManager* GameObjectManager::poGameObjectManager = nullptr;

	void GameObjectManager::Add(GameObject* pObj, GameObject* pParent)
	{
		assert(pObj != 0);
		assert(pParent != 0);

		// Get singleton
		GameObjectManager* pGOM = GameObjectManager::privGetInstance();

		// insert object to root
		pGOM->poRootTree->Insert(pObj, pParent);
	}


	void GameObjectManager::Create()
	{

		if (GameObjectManager::poGameObjectManager == nullptr) {
		
			GameObjectManager::poGameObjectManager = new GameObjectManager();
			assert(poGameObjectManager);
		}
		else {
			assert(false);
		}
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

		delete pGOM->poGameObjectManager;
	}

	void GameObjectManager::Update(Time currentTime)
	{
		GameObjectManager* pGOM = GameObjectManager::privGetInstance();
		assert(pGOM);

		PCSNode* pRootNode = pGOM->poRootTree->GetRoot();
		assert(pRootNode);

		PCSTreeForwardIterator pForwardIter(pRootNode->GetChild());
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

			if (pGameObj->GetDrawEnable())
			{
				pGameObj->Draw();
			}
			else
			{
				//assert(0);
			}

			pNode = pForwardIter.Next();
		}
	}

	GameObjectManager::GameObjectManager()
	{
		// Create the root node (null object)
		NullModel* pModel = new NullModel(nullptr);
		assert(pModel);
		ModelManager::Add(pModel);

		
		ShaderManager::Add(ShaderObject::Name::NULL_SHADER, "Shaders/nullRender");

		GraphicsObject_Null* pGraphicsObject = new GraphicsObject_Null(pModel, ShaderManager::Find(ShaderObject::Name::NULL_SHADER));

		GameObjectRigid* pGameRoot = new GameObjectRigid(pGraphicsObject);
		pGameRoot->SetName("GameObject_Root");

		// Create the tree
		this->poRootTree = new PCSTree();
		assert(this->poRootTree);

		// Attach the root node
		this->poRootTree->Insert(pGameRoot, this->poRootTree->GetRoot());
	}


	PCSTree* GameObjectManager::GetPCSTree()
	{
		// Get singleton
		GameObjectManager* pGOM = GameObjectManager::privGetInstance();
		assert(pGOM);

		// Get root node
		return pGOM->poRootTree;
	}

	GameObject* GameObjectManager::GetRoot()
	{
		// Get singleton
		GameObjectManager* pGOM = GameObjectManager::privGetInstance();
		assert(pGOM);

		GameObject* pGameObj = (GameObject*)pGOM->poRootTree->GetRoot();
		assert(pGameObj);

		return pGameObj;
	}

	GameObjectManager::~GameObjectManager()
	{
		delete this->poRootTree;
	}

	GameObjectManager* GameObjectManager::privGetInstance()
	{
		assert(poGameObjectManager != nullptr);
		return poGameObjectManager;
	}

}

// --- End of File ---
