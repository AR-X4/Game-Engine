#ifndef GRAPHICS_OBJECT_SKIN_TEXTURE_H
#define GRAPHICS_OBJECT_SKIN_TEXTURE_H

#include "GraphicsObject.h"
#include "Textures/Texture.h"
#include "Shaders/SSBO.h"
#include "Models/FBXSkinnedModel.h"

namespace Azul
{
	class GraphicsObject_SkinTexture :public GraphicsObject
	{
	public:
		GraphicsObject_SkinTexture(const FBXSkinnedModel *const pModel, const ShaderObject *const pShaderObj, Texture::Name _name);

		GraphicsObject_SkinTexture() = delete;
		GraphicsObject_SkinTexture(const GraphicsObject_SkinTexture &) = delete;
		GraphicsObject_SkinTexture &operator = (const GraphicsObject_SkinTexture &) = delete;
		~GraphicsObject_SkinTexture();

		// Rendermaterial contract
		virtual void SetState() override;
		virtual void SetDataGPU() override;
		virtual void Draw() override;
		virtual void RestoreState() override;

		// data:  place uniform instancing here
		Texture::Name textName;
		SSBO* pBoneWord_SBO;
	};
}

#endif

//--- End of File ---
