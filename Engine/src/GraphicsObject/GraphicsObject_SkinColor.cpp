

#include "GraphicsObject_SkinColor.h"
#include "Textures/TextureManager.h"
#include "Models/Model.h"
#include "Camera/CameraManager.h"
#include "Models/FBXSkinnedModel.h"

namespace Azul
{
	GraphicsObject_SkinColor::GraphicsObject_SkinColor(const FBXSkinnedModel*const _pModel, const ShaderObject *const _pShaderObj, Vect& LightColor, Vect& LightPos)
		: GraphicsObject(_pModel, _pShaderObj)
	{
		// check the base class
		assert(this->pModel);
		assert(this->pShaderObj);
		assert(this->poWorld);

		this->poLightColor = new Vect(LightColor);
		this->poLightPos = new Vect(LightPos);

		Matrix* pResultMatrix = new Matrix[_pModel->NumBones]();
		this->pBoneWord_SBO = new SSBO();
		this->pBoneWord_SBO->Set(_pModel->NumBones, sizeof(Matrix), pResultMatrix);
		delete[] pResultMatrix;
	}

	GraphicsObject_SkinColor::~GraphicsObject_SkinColor()
	{
		delete poLightColor;
		delete poLightPos;
		delete pBoneWord_SBO;
	}

	void GraphicsObject_SkinColor::SetState()
	{

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
	}

	void GraphicsObject_SkinColor::SetDataGPU()
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
		//glUniform1i(this->pShaderObj->GetLocation("tex_object"), 0);

		Vect light = *this->poLightPos;
		Vect color = *this->poLightColor;
		glUniform3fv(this->pShaderObj->GetLocation("vLightPos"), 1, (float*)&light);
		glUniform4fv(this->pShaderObj->GetLocation("vColor"), 1, (float*)&color);
	}

	void GraphicsObject_SkinColor::Draw()
	{
		//The starting point of the IBO
		glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
	}

	void GraphicsObject_SkinColor::RestoreState()
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
	}

}

//--- End of File ----

