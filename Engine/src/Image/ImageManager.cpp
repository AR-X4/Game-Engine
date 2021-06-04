#include "ImageManager.h"


namespace Azul
{

	ImageManager* ImageManager::poInstance = nullptr;

	ImageManager::ImageManager(int reserveNum, int reserveGrow)
	{
		this->BaseInitialize(reserveNum, reserveGrow);

		this->poNodeCompare = new Image();
		assert(this->poNodeCompare != nullptr);

	}

	ImageManager::~ImageManager()
	{
		delete this->poNodeCompare;
	}

	ImageManager* ImageManager::GetInstance()
	{
		assert(poInstance != nullptr);
		return poInstance;
	}


	void ImageManager::Create(int reserveNum, int reserveGrow)
	{
		assert(reserveNum > 0);
		assert(reserveGrow > 0);

		// initialize the singleton here
		assert(poInstance == nullptr);

		// Do the initialization
		if (poInstance == nullptr)
		{
			poInstance = new ImageManager(reserveNum, reserveGrow);
		}
	}

	void ImageManager::Destroy()
	{
		ImageManager* pMan = ImageManager::GetInstance();
		assert(pMan != nullptr);

		pMan->BaseDestroy();

		delete pMan->poInstance;
	}

	Image* ImageManager::Find(Image::Name NameIn)
	{
		// Get the instance to the manager
		ImageManager* pMan = ImageManager::GetInstance();
		assert(pMan != nullptr);

		pMan->poNodeCompare->SetName(NameIn);

		Image* pNode = (Image*)pMan->BaseFind(pMan->poNodeCompare);
		assert(pNode != nullptr);

		return pNode;
	}

	void ImageManager::Add(Image::Name imageNameIn, Texture::Name textName, Rect rect)
	{
		// Get the instance to the manager
		ImageManager* pShaderMan = ImageManager::GetInstance();
		assert(pShaderMan);


		Image* pNode = (Image*)pShaderMan->BaseAdd();


		pNode->Set(imageNameIn, textName, rect);

	}


	//-------------------------------------------------------
	//-------------------------------------------------------
	//-------------------------------------------------------

	DLink* ImageManager::DerivedCreateNode()
	{
		DLink* pNode = new Image();
		assert(pNode != nullptr);

		return pNode;
	}

	bool ImageManager::DerivedCompare(DLink* pLinkA, DLink* pLinkB)
	{
		// This is used in baseFind() 
		assert(pLinkA != nullptr);
		assert(pLinkB != nullptr);

		Image* pDataA = (Image*)pLinkA;
		Image* pDataB = (Image*)pLinkB;

		bool status = false;


		if (pDataA->imageName == pDataB->imageName)
		{
			status = true;
		}

		return status;
	}

	void ImageManager::DerivedWash(DLink* pLink)
	{
		assert(pLink != nullptr);
		Image* pNode = (Image*)pLink;
		pNode->Wash();
	}

	void ImageManager::DerivedDumpNode(DLink* pLink)
	{
		assert(pLink != nullptr);
		Image* pData = (Image*)pLink;
		pData->Dump();
	}

	void ImageManager::DerivedDestroyNode(DLink* pLink)
	{
		assert(pLink);
		Image* pData = (Image*)pLink;
		delete pData;
	}

}

