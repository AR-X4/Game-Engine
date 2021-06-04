#ifndef GRAPHICS_OBJECT_GLYPH_H
#define GRAPHICS_OBJECT_GLYPH_H

#include "Textures/Texture.h"
#include "Glyph/Glyph.h"
#include "GraphicsObject/GraphicsObject.h"
#include "Shaders/ShaderObject.h"

namespace Azul
{
	class GraphicsObject_Glyph : public GraphicsObject
	{
	public:


		GraphicsObject_Glyph(Model* model, ShaderObject* pShaderObj);
		GraphicsObject_Glyph() = delete;
		GraphicsObject_Glyph(const GraphicsObject_Glyph&) = delete;
		GraphicsObject_Glyph& operator = (const GraphicsObject_Glyph&) = delete;
		virtual ~GraphicsObject_Glyph();

		void Swap(Glyph* image, Rect rect);

		// Rendermaterial contract
		virtual void SetState() override;
		virtual void SetDataGPU() override;
		virtual void Draw() override;
		virtual void RestoreState() override;

		// data:  place uniform instancing here

		Glyph* pGlyph;
		Matrix* poMatrix_uv;
		Matrix* poMatrix_orig;

		float origWidth;
		float origHeight;

		float origPosX;
		float origPosY;
	};

}

#endif