#include "ShaderManager.h"

namespace Azul
{
	
	ShaderManager* ShaderManager::poInstance = nullptr;

	ShaderManager::ShaderManager(int reserveNum, int reserveGrow)
	{
		this->BaseInitialize(reserveNum, reserveGrow);

		this->poNodeCompare = new ShaderObject();
		assert(this->poNodeCompare != nullptr);

	}

	ShaderManager::~ShaderManager()
	{
		delete this->poNodeCompare;
	}

	ShaderManager* ShaderManager::GetInstance()
	{
		assert(poInstance != nullptr);
		return poInstance;
	}


	void ShaderManager::Create(int reserveNum, int reserveGrow)
	{
		assert(reserveNum > 0);
		assert(reserveGrow > 0);

		// initialize the singleton here
		assert(poInstance == nullptr);

		// Do the initialization
		if (poInstance == nullptr)
		{
			poInstance = new ShaderManager(reserveNum, reserveGrow);
		}
	}

	void ShaderManager::Destroy()
	{
		ShaderManager* pMan = ShaderManager::GetInstance();
		assert(pMan != nullptr);

		pMan->BaseDestroy();

		delete pMan->poInstance;
	}

	ShaderObject* ShaderManager::Find(ShaderObject::Name NameIn)
	{
		// Get the instance to the manager
		ShaderManager* pMan = ShaderManager::GetInstance();
		assert(pMan != nullptr);

		pMan->poNodeCompare->SetName(NameIn);

		ShaderObject* pNode = (ShaderObject*)pMan->BaseFind(pMan->poNodeCompare);
		assert(pNode != nullptr);

		return pNode;
	}

	void ShaderManager::Add(ShaderObject::Name NameIn, const char* const _assetName, ShaderObject::Type shaderType)
	{
		// Get the instance to the manager
		ShaderManager* pShaderMan = ShaderManager::GetInstance();
		assert(pShaderMan);

	
		ShaderObject* pNode = (ShaderObject*)pShaderMan->BaseAdd();

		
		pNode->Set(NameIn, _assetName, shaderType);

	}


	//-------------------------------------------------------
	//-------------------------------------------------------
	//-------------------------------------------------------

	DLink* ShaderManager::DerivedCreateNode()
	{
		DLink* pNode = new ShaderObject();
		assert(pNode != nullptr);

		return pNode;
	}

	bool ShaderManager::DerivedCompare(DLink* pLinkA, DLink* pLinkB)
	{
		// This is used in baseFind() 
		assert(pLinkA != nullptr);
		assert(pLinkB != nullptr);

		ShaderObject* pDataA = (ShaderObject*)pLinkA;
		ShaderObject* pDataB = (ShaderObject*)pLinkB;

		bool status = false;


		if (pDataA->ShaderName == pDataB->ShaderName)
		{
			status = true;
		}

		return status;
	}

	void ShaderManager::DerivedWash(DLink* pLink)
	{
		assert(pLink != nullptr);
		ShaderObject* pNode = (ShaderObject*)pLink;
		pNode->Wash();
	}

	void ShaderManager::DerivedDumpNode(DLink* pLink)
	{
		assert(pLink != nullptr);
		ShaderObject* pData = (ShaderObject*)pLink;
		pData->Dump();
	}

	void ShaderManager::DerivedDestroyNode(DLink* pLink)
	{
		assert(pLink);
		ShaderObject* pData = (ShaderObject*)pLink;
		delete pData;
	}

}

// ---  End of File ---
