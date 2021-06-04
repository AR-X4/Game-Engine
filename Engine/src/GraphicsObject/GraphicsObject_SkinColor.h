
#ifndef GRAPHICS_OBJECT_SKIN_COLOR_H
#define GRAPHICS_OBJECT_SKIN_COLOR_H

#include "GraphicsObject.h"
#include "Textures/Texture.h"
#include "Shaders/SSBO.h"
#include "Models/FBXSkinnedModel.h"

namespace Azul
{
	class GraphicsObject_SkinColor :public GraphicsObject
	{
	public:
		GraphicsObject_SkinColor(const FBXSkinnedModel*const pModel, const ShaderObject *const pShaderObj, Vect& LightColor, Vect& LightPos);

		GraphicsObject_SkinColor() = delete;
		GraphicsObject_SkinColor(const GraphicsObject_SkinColor&) = delete;
		GraphicsObject_SkinColor&operator = (const GraphicsObject_SkinColor&) = delete;
		~GraphicsObject_SkinColor();

		// Rendermaterial contract
		virtual void SetState() override;
		virtual void SetDataGPU() override;
		virtual void Draw() override;
		virtual void RestoreState() override;

		// data:  place uniform instancing here
		Vect* poLightColor;
		Vect* poLightPos;

		SSBO* pBoneWord_SBO;
	};
}

#endif

//--- End of File ---
