#include "Textures/TextureManager.h"
#include "Image.h"
#include "2D_Primitives/Rect.h"

namespace Azul
{
	// public methods: -------------------------------------------------------------

	Image::Image()
	{
		this->imageName = Image::Name::UNINITIALIZED;
		this->imageRect.Clear();
		this->pText = nullptr;
	}

	/*Image::Image(Image::Name _imageName, Texture* _pText, Rect _rect)
	{
		this->imageName = _imageName;
		this->pText = _pText;
		this->imageRect = _rect;
	}

	Image::Image(Image::Name _imageName, Texture::Name _textName, Rect _rect)
	{
		this->imageName = _imageName;
		this->pText = TextureManager::Find(_textName);
		assert(this->pText);
		this->imageRect = _rect;
	}

	void Image::Set(Image::Name _imageName, Texture* _pText, Rect _rect)
	{
		this->imageName = _imageName;
		this->pText = _pText;
		this->imageRect = _rect;
	}*/

	void Image::Dump()
	{
	}

	void Image::Wash()
	{
		this->imageName = Image::Name::UNINITIALIZED;
		this->imageRect.Clear();
		this->pText = nullptr;
	}

	void Image::Set(Image::Name _imageName, Texture::Name _textName, Rect _rect)
	{
		this->imageName = _imageName;
		this->pText = TextureManager::Find(_textName);
		assert(this->pText);
		this->imageRect = _rect;
	}
	void Image::SetName(Image::Name imageNameIn)
	{
		this->imageName = imageNameIn;
	}
}