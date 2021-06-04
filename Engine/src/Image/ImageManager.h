#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include "Image.h"
#include "Manager/Manager.h"


namespace Azul
{
	class ImageManager : Manager
	{

	private:  // methods

		ImageManager(int reserveNum = 3, int reserveGrow = 1);
		ImageManager() = delete;
		ImageManager(const ImageManager&) = delete;
		ImageManager& operator = (const ImageManager&) = delete;
		~ImageManager();

		static ImageManager* GetInstance();



	public:
		static void Add(Image::Name imageNameIn, Texture::Name textName, Rect rect);
		static void Create(int reserveNum = 3, int reserveGrow = 1);
		static void Destroy();
		static Image* Find(Image::Name NameIn);

	protected:
		virtual DLink* DerivedCreateNode() override;
		virtual bool DerivedCompare(DLink* pLinkA, DLink* pLinkB) override;
		virtual void DerivedWash(DLink* pLink) override;
		virtual void DerivedDumpNode(DLink* pLink) override;
		virtual void DerivedDestroyNode(DLink* pLink) override;

	private:

		//DATA
		static ImageManager* poInstance;
		Image* poNodeCompare;
	};

}

#endif