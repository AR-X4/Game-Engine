

#include "GraphicsObject.h"
#include "MathEngine.h"
#include "Models/Model.h"

namespace Azul
{

	GraphicsObject::GraphicsObject(const Model* const _pModel, const ShaderObject* const _pShaderObj)
		: pModel(_pModel),
		pShaderObj(_pShaderObj)
	{
		this->poWorld = new Matrix(Matrix::Special::Identity);
		assert(this->poWorld);
		assert(_pModel);
		assert(_pShaderObj);

	}

	GraphicsObject::~GraphicsObject()
	{
		delete this->poWorld;
	}

	void GraphicsObject::Render()
	{
		this->SetState();
		this->SetDataGPU();
		this->Draw();
		this->RestoreState();
	}

	const Model* GraphicsObject::GetModel() const
	{
		return this->pModel;
	}

	Matrix& GraphicsObject::GetWorld()
	{
		return *this->poWorld;
	}

	void GraphicsObject::SetWorld(Matrix& _world)
	{
		*this->poWorld = _world;
	}

}

// ---  End of File ---
