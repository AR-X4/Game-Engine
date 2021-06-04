#ifndef GRAPHICS_OBJECT_SKIN_TEXT_LIGHT_H
#define GRAPHICS_OBJECT_SKIN_TEXT_LIGHT_H

#include "GraphicsObject.h"
#include "Textures/Texture.h"
#include "Shaders/SSBO.h"
#include "Models/FBXSkinnedModel.h"

namespace Azul
{
	class GraphicsObject_SkinTextLight :public GraphicsObject
	{
	public:
		GraphicsObject_SkinTextLight(const FBXSkinnedModel *const pModel, const ShaderObject *const pShaderObj, Texture::Name _name, Vect& LightColor, Vect& LightPos);

		GraphicsObject_SkinTextLight() = delete;
		GraphicsObject_SkinTextLight(const GraphicsObject_SkinTextLight&) = delete;
		GraphicsObject_SkinTextLight&operator = (const GraphicsObject_SkinTextLight&) = delete;
		~GraphicsObject_SkinTextLight();

		// Rendermaterial contract
		virtual void SetState() override;
		virtual void SetDataGPU() override;
		virtual void Draw() override;
		virtual void RestoreState() override;

		// data:  place uniform instancing here
		Texture::Name textName;
		Vect* poLightColor;
		Vect* poLightPos;
		SSBO* pBoneWord_SBO;
	};
}

#endif

//--- End of File ---
