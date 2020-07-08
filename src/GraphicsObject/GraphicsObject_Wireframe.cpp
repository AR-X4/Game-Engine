

#include "Models/Model.h"
#include "GraphicsObject/GraphicsObject_Wireframe.h"
#include "Shaders/ShaderObject.h"
#include "Camera/CameraManager.h"



namespace Azul
{

	GraphicsObject_Wireframe::GraphicsObject_Wireframe(const Model* const _pModel, const ShaderObject* const _pShaderObj)
		: GraphicsObject(_pModel, _pShaderObj)
	{
		assert(pModel);
		assert(pShaderObj);
		assert(poWorld);
	}

	void GraphicsObject_Wireframe::SetState()
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
	}

	void GraphicsObject_Wireframe::SetDataGPU()
	{
		// use this shader
		this->pShaderObj->SetActive();

		// Draw Me: -----------------------------------------

		// set the vao
		glBindVertexArray(this->GetModel()->vao);

		// Set state
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);

		Matrix world = this->GetWorld();
		Matrix view = CameraManager::GetActiveCamera()->getViewMatrix();
		Matrix proj = CameraManager::GetActiveCamera()->getProjMatrix();

		glUniformMatrix4fv(this->pShaderObj->GetLocation("proj_matrix"), 1, GL_FALSE, (float*)&proj);
		glUniformMatrix4fv(this->pShaderObj->GetLocation("view_matrix"), 1, GL_FALSE, (float*)&view);
		glUniformMatrix4fv(this->pShaderObj->GetLocation("world_matrix"), 1, GL_FALSE, (float*)&world);
	}

	void GraphicsObject_Wireframe::Draw()
	{
		// draw
		//glDrawArrays(GL_TRIANGLES, 0, (3 * this->GetModel()->numVerts));

		//The starting point of the IBO
		glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
	}

	void GraphicsObject_Wireframe::RestoreState()
	{
		// Restore state
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

}

// ---  End of File ---
