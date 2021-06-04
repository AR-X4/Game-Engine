
#ifndef GRAPHICS_OBJECT_SPRITE_H
#define GRAPHICS_OBJECT_SPRITE_H

#include "Textures/Texture.h"
#include "Image/Image.h"
#include "GraphicsObject/GraphicsObject.h"
#include "Shaders/ShaderObject.h"

namespace Azul
{
	class GraphicsObject_Sprite : public GraphicsObject
	{
	public:
		GraphicsObject_Sprite(Model *pModel, ShaderObject *pShaderObj, Image *image, Rect rect);

		GraphicsObject_Sprite() = delete;
		GraphicsObject_Sprite(const GraphicsObject_Sprite &) = delete;
		GraphicsObject_Sprite & operator = (const GraphicsObject_Sprite &) = delete;
		virtual ~GraphicsObject_Sprite();

		// Rendermaterial contract
		virtual void SetState() override;
		virtual void SetDataGPU() override;
		virtual void Draw() override;
		virtual void RestoreState() override;

		// data:  place uniform instancing here

		Image *pImage;
		Matrix *poMatrix_uv;
		Matrix *poMatrix_orig;

		float origWidth;
		float origHeight;

		float origPosX;
		float origPosY;
	};
}

#endif

//--- End of File ---

