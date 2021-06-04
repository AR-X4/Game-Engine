#include "GlyphSprite.h"


namespace Azul {

	GlyphSprite::GlyphSprite(GraphicsObject_Glyph* graphicsObject)
		:GameObject(graphicsObject)
	{
		assert(graphicsObject);

		this->angle = 0.0f;
		this->posX = graphicsObject->origPosX;
		this->posY = graphicsObject->origPosY;
		this->scaleX = 1.0f;
		this->scaleY = 1.0f;
	}

	GlyphSprite::~GlyphSprite()
	{
	}

	void GlyphSprite::Update(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		// Goal: update the world matrix
		Matrix Scale(Matrix::Scale::XYZ, this->scaleX, this->scaleY, 1.0f);
		Matrix Rot(Matrix::Rot1::Z, this->angle);
		Matrix Trans(Matrix::Trans::XYZ, this->posX, this->posY, 0.0f);

		*this->poWorld = Scale * Rot * Trans;

		// push to graphics object
		this->poGraphicsObject->SetWorld(*this->poWorld);
	}


}