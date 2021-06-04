#include "GraphicsObject_Glyph.h"
#include "Camera/CameraManager.h"

namespace Azul 
{



	GraphicsObject_Glyph::GraphicsObject_Glyph(Model* model, ShaderObject* pShaderObj)
		:GraphicsObject(model, pShaderObj)
	{
		assert(model);
		assert(pShaderObj);


		// Create the UV matrix
		this->poMatrix_uv = new Matrix();
		assert(this->poMatrix_uv);

		// Create the orig Matrix 
		// The initial sprite setup, after this point standard world will adjust
		this->poMatrix_orig = new Matrix();
		assert(this->poMatrix_orig);

	}

	GraphicsObject_Glyph::~GraphicsObject_Glyph()
	{
		delete this->poMatrix_uv;
		delete this->poMatrix_orig;
	}

	void GraphicsObject_Glyph::Swap(Glyph* glyph, Rect rect)
	{
		this->pGlyph = glyph;// unnecessary?

		this->origWidth = rect.width;
		this->origHeight = rect.height;

		this->origPosX = rect.x;
		this->origPosY = rect.y;

		float w = pGlyph->glyphRect.width / (float)pGlyph->pTexture->width;
		float h = pGlyph->glyphRect.height / (float)pGlyph->pTexture->height;
		float u = pGlyph->glyphRect.x / (float)pGlyph->pTexture->width;
		float v = pGlyph->glyphRect.y / (float)pGlyph->pTexture->height;
		Matrix ScaleUV(Matrix::Scale::XYZ, w, h, 1.0f);
		Matrix TransUV(Matrix::Trans::XYZ, u, v, 0.0f);
		*this->poMatrix_uv = ScaleUV * TransUV;

		Matrix Scale(Matrix::Scale::XYZ, this->origWidth, this->origHeight, 1.0f);
		*this->poMatrix_orig = Scale;

	}

	void GraphicsObject_Glyph::SetState()
	{
		glActiveTexture(GL_TEXTURE0);

		// Bind the texture
		GLuint textureID = this->pGlyph->pTexture->textureID;
		glBindTexture(GL_TEXTURE_2D, textureID);

		// comment out one
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glFrontFace(GL_CW);

		// Blends for sprites
		glEnable(GL_BLEND);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	}

	void GraphicsObject_Glyph::SetDataGPU()
	{
		// Get camera
		Camera* pCam = CameraManager::GetActiveCamera(Camera::Type::ORTHOGRAPHIC_2D);
		assert(pCam);

		// set the vao
		glBindVertexArray(this->GetModel()->vao);

		// use this shader
		this->pShaderObj->SetActive();

		// World matrix ----------------------------
		Matrix TransToOriginLowerLeft(Matrix::Trans::XYZ, (float)-pCam->getScreenWidth() / 2.0f, (float)-pCam->getScreenHeight() / 2.0f, 0.0f);
		Matrix tmpMatrix = *this->poMatrix_orig * this->GetWorld() * TransToOriginLowerLeft;

		glUniformMatrix4fv(this->pShaderObj->GetLocation("proj_matrix"), 1, GL_FALSE, (float*)&pCam->getProjMatrix());
		glUniformMatrix4fv(this->pShaderObj->GetLocation("view_matrix"), 1, GL_FALSE, (float*)&pCam->getViewMatrix());
		glUniformMatrix4fv(this->pShaderObj->GetLocation("world_matrix"), 1, GL_FALSE, (float*)&tmpMatrix);
		glUniformMatrix4fv(this->pShaderObj->GetLocation("uv_matrix"), 1, GL_FALSE, (float*)this->poMatrix_uv);

	}

	void GraphicsObject_Glyph::Draw()
	{
		// draw
		glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);   //The starting point of the IBO
	}

	void GraphicsObject_Glyph::RestoreState()
	{
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
	}

};
