#include "CameraManager.h"

namespace Azul
{

	CameraManager* CameraManager::poInstance = nullptr;

	CameraManager::CameraManager(int reserveNum, int reserveGrow)
	{
		this->BaseInitialize(reserveNum, reserveGrow);

		this->poNodeCompare = new Camera();
		assert(this->poNodeCompare != nullptr);

	}

	CameraManager::~CameraManager()
	{
		delete this->poNodeCompare;
	}

	CameraManager* CameraManager::GetInstance()
	{
		assert(poInstance != nullptr);
		return poInstance;
	}

	void CameraManager::Create(int reserveNum, int reserveGrow)
	{
		assert(reserveNum > 0);
		assert(reserveGrow > 0);

		// initialize the singleton here
		assert(poInstance == nullptr);

		// Do the initialization
		if (poInstance == nullptr)
		{
			poInstance = new CameraManager(reserveNum, reserveGrow);
		}
	}

	void CameraManager::Destroy()
	{
		CameraManager* pMan = CameraManager::GetInstance();
		assert(pMan != nullptr);

		pMan->BaseDestroy();

		delete pMan->poInstance;
	}

	void CameraManager::Update()
	{
		CameraManager* pCameraManager = CameraManager::GetInstance();
		assert(pCameraManager);

		/*Camera* pCurrent = (Camera*)pCameraManager->BaseGetActiveHead();
		while (pCurrent != nullptr) {
		
			pCurrent->updateCamera();
			pCurrent = (Camera*)pCurrent->pNext;
		}*/

		pCameraManager->pActiveCamera->updateCamera();

	}

	Camera* CameraManager::Find(Camera::Name NameIn)
	{
		CameraManager* pCameraManager = CameraManager::GetInstance();
		assert(pCameraManager);

		pCameraManager->poNodeCompare->SetName(NameIn);

		Camera* pNode = (Camera*)pCameraManager->BaseFind(pCameraManager->poNodeCompare);
		assert(pNode != nullptr);

		return pNode;
	}

	void CameraManager::SetActiveCamera(Camera* pCamera)
	{
		CameraManager* pCameraManager = CameraManager::GetInstance();
		assert(pCameraManager);
		assert(pCamera);

		pCameraManager->pActiveCamera = pCamera;
	}

	Camera* CameraManager::GetActiveCamera()
	{
		CameraManager* pCameraManager = CameraManager::GetInstance();
		assert(pCameraManager);
		assert(pCameraManager->pActiveCamera);

		return pCameraManager->pActiveCamera;
	}

	Camera* CameraManager::Add(Camera::Name NameIn)
	{
		
		CameraManager* pCameraManager = CameraManager::GetInstance();
		assert(pCameraManager);

		Camera* pNode = (Camera*)pCameraManager->BaseAdd();
		assert(pNode != nullptr);

		pNode->SetName(NameIn);

		return pNode;
	}

	//-------------------------------------------------------
	//-------------------------------------------------------
	//-------------------------------------------------------

	DLink* CameraManager::DerivedCreateNode()
	{
		DLink* pNode = new Camera();
		assert(pNode != nullptr);

		return pNode;
	}

	bool CameraManager::DerivedCompare(DLink* pLinkA, DLink* pLinkB)
	{
		// This is used in baseFind() 
		assert(pLinkA != nullptr);
		assert(pLinkB != nullptr);

		Camera* pDataA = (Camera*)pLinkA;
		Camera* pDataB = (Camera*)pLinkB;

		bool status = false;


		if (pDataA->CamName == pDataB->CamName)
		{
			status = true;
		}

		return status;
	}

	void CameraManager::DerivedWash(DLink* pLink)
	{
		assert(pLink != nullptr);
		Camera* pNode = (Camera*)pLink;
		pNode->Wash();
	}

	void CameraManager::DerivedDumpNode(DLink* pLink)
	{
		assert(pLink != nullptr);
		Camera* pData = (Camera*)pLink;
		pData->Dump();
	}

	void CameraManager::DerivedDestroyNode(DLink* pLink)
	{
		assert(pLink);
		Camera* pData = (Camera*)pLink;
		delete pData;
	}

}

 //---  End of File ---
