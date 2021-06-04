#ifndef GLYPH_SPRITE_H
#define GLYPH_SPRITE_H

#include "MathEngine.h"
#include "GameObject.h"
#include "GraphicsObject/GraphicsObject_Glyph.h"

namespace Azul
{
	// TODO: inheret from GameObject2D
	class GlyphSprite : public GameObject
	{
	public:

		GlyphSprite(GraphicsObject_Glyph* graphicsObject);

		// Big four
		GlyphSprite() = delete;
		GlyphSprite(const GlyphSprite&) = delete;
		GlyphSprite& operator=(GlyphSprite&) = delete;
		virtual ~GlyphSprite() override;


		virtual void Update(AnimTime currentTime) override;


	public:
		float scaleX;
		float scaleY;
		float posX;
		float posY;
		float angle;
	};
}

#endif

// --- End of File ---
