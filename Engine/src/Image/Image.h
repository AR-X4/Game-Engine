#ifndef IMAGE_H
#define IMAGE_H

#include "2D_Primitives/Rect.h"
#include "Textures/Texture.h"


namespace Azul
{
	class Image : public DLink
	{
	public:
		enum class Name
		{
			Alien_Blue,
			Alien_Green,
			Alien_Red,
			Stitch,
			UNINITIALIZED,
		};

	public:
		// public methods: -------------------------------------------------------------
		Image();
		Image(const Image&) = default;
		Image& operator = (const Image&) = default;
		virtual	~Image() = default;

		//Image(Image::Name imageName, Texture::Name textName, Rect rect);
		//Image(Image::Name imageName, Texture* pTexture, Rect rect);

		void Set(Image::Name imageName, Texture::Name textName, Rect rect);
		void SetName(Image::Name imageName);
		//void Set(Image::Name imageName, Texture* pTexture, Rect rect);

		void Dump();
		void Wash();

	public:
		// data: -----------------------------------------------------------------------
		Image::Name	    imageName;
		Texture*		pText;
		Rect			imageRect;
	};

}


#endif