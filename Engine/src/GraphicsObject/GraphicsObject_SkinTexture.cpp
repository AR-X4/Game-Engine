

#include "GraphicsObject_SkinTexture.h"
#include "Textures/TextureManager.h"
#include "Models/Model.h"
#include "Camera/CameraManager.h"
#include "Models/FBXSkinnedModel.h"

namespace Azul
{
	GraphicsObject_SkinTexture::GraphicsObject_SkinTexture(const FBXSkinnedModel*const _pModel, const ShaderObject *const _pShaderObj, Texture::Name _name)
		: GraphicsObject(_pModel, _pShaderObj), textName(_name)
	{
		// check the base class
		assert(this->pModel);
		assert(this->pShaderObj);
		assert(this->poWorld);


		Matrix* pResultMatrix = new Matrix[_pModel->NumBones]();
		this->pBoneWord_SBO = new SSBO();
		this->pBoneWord_SBO->Set(_pModel->NumBones, sizeof(Matrix), pResultMatrix);
		delete[] pResultMatrix;
	}

	GraphicsObject_SkinTexture::~GraphicsObject_SkinTexture()
	{
		delete pBoneWord_SBO;
	}

	void GraphicsObject_SkinTexture::SetState()
	{
		glActiveTexture(GL_TEXTURE0);

		// Bind the texture
		Texture* tmp = TextureManager::Find(this->textName);
		GLuint textureID = tmp->textureID;
		glBindTexture(GL_TEXTURE_2D, textureID);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
	}

	void GraphicsObject_SkinTexture::SetDataGPU()
	{
		// use this shader
		this->pShaderObj->SetActive();

		// set the vao
		glBindVertexArray(this->GetModel()->vao);

		Camera *pCam = CameraManager::GetActiveCamera(Camera::Type::PERSPECTIVE_3D);
		assert(pCam);

		Matrix view = pCam->getViewMatrix();
		Matrix proj = pCam->getProjMatrix();

		// safe because can only be FBXSkinnedModel
		FBXSkinnedModel* pSkinnedModel = (FBXSkinnedModel*)this->GetModel();

		// Bine the SBOs
		pSkinnedModel->poInversePose_SBO->Bind(0);
		this->pBoneWord_SBO->Bind(1);
		
		// Send over Uniforms

		glUniformMatrix4fv(this->pShaderObj->GetLocation("proj_matrix"), 1, GL_FALSE, (float *)&proj);
		glUniformMatrix4fv(this->pShaderObj->GetLocation("view_matrix"), 1, GL_FALSE, (float *)&view);

		//glUniformMatrix4fv(this->pShaderObj->GetLocation("invPose"), (int)pSkinnedModel->NumBones, GL_FALSE, (float *)pSkinnedModel->poInversePose);
		//glUniformMatrix4fv(this->pShaderObj->GetLocation("boneWorld"), (int)pSkinnedModel->NumBones, GL_FALSE, (float *)pSkinnedModel->poBone);

		// Texture - linked to Texture 0
		glUniform1i(this->pShaderObj->GetLocation("tex_object"), 0);
	}

	void GraphicsObject_SkinTexture::Draw()
	{
		//The starting point of the IBO
		glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
	}

	void GraphicsObject_SkinTexture::RestoreState()
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
	}

}

//--- End of File ----

