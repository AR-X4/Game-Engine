#ifndef GLYPH_H
#define GLYPH_H

#include "2D_Primitives/Rect.h"
#include "Textures/Texture.h"

namespace Azul
{
	class Glyph : public DLink
	{

	public:
		enum class FontType
		{
			Consolas36pt,
			UNINITIALIZED

		};

		// public methods: -------------------------------------------------------------
		Glyph();
		Glyph(const Glyph&) = default;
		Glyph& operator = (const Glyph&) = default;
		virtual	~Glyph() = default;

		void Set(FontType FontName, Texture::Name textName, Rect rect, int _key);
		void Set(FontType FontName, int _key);

		void Dump();
		void Wash();

	//private: TODO
		// data
		FontType FontName;
		Texture* pTexture;
		Rect	glyphRect;
		int key;

	};
}

#endif
