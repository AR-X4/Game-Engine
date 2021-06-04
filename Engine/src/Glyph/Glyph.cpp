#include "Glyph.h"
#include "Textures/TextureManager.h"

namespace Azul {

	Glyph::Glyph()
		:FontName(FontType::UNINITIALIZED),
		pTexture(nullptr),
		glyphRect(),
		key(0)
	{
		this->glyphRect.Clear();
	}

	void Glyph::Set(FontType _FontName, Texture::Name _textName, Rect rect, int _key)
	{
		this->FontName = _FontName;
		this->pTexture = TextureManager::Find(_textName);
		assert(this->pTexture);
		this->glyphRect = rect;
		this->key = _key;
	}

	void Glyph::Set(FontType _FontName, int _key)
	{
		this->FontName = _FontName;
		this->key = _key;
	}
	

	void Glyph::Dump()
	{
	}

	void Glyph::Wash()
	{
		this->FontName = FontType::UNINITIALIZED;
		this->pTexture = nullptr;
		this->glyphRect.Clear();
		this->key = 0;
	}


}