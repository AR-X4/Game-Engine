

#include "GameObject.h"
#include "Game/Game.h"

namespace Azul
{

	GameObject::GameObject(GraphicsObject* pGraphicsObject)
		: poGraphicsObject(pGraphicsObject)
	{
		this->poWorld = new Matrix(Matrix::Special::Identity);
		assert(poWorld);

		assert(pGraphicsObject != 0);

		this->mDrawEnable = true;

		this->pScale = new Vect(1.0f, 1.0f, 1.0f);
		assert(this->pScale);

		this->pPos = new Vect(0.0f, 0.0f, 0.0f);
		assert(this->pPos);

		this->pRot = new Vect(0.0f, 0.0f, 0.0f);
		assert(this->pRot);
	}

	GameObject::~GameObject()
	{
		delete this->poWorld;
		delete this->poGraphicsObject;

		delete this->pPos;
		delete this->pScale;
		delete this->pRot;
	}

	//void GameObject::baseUpdateBoundingSphere()
	//{
	//	// Get the ref Sphere
	//	Sphere* pRefSphere = poGraphicsObject->GetModel()->poRefSphere;
	//	assert(pRefSphere);

	//	// Get the curr Sphere
	//	Sphere* pCurrSphere = poGraphicsObject->poCurrSphere;
	//	assert(pCurrSphere);

	//	// Update... cntr process through the world
	//	// Assuming a uniform scaling
	//	Vect A = pRefSphere->cntr;
	//	Vect radius(1.0f, 0.0f, 0.0f);
	//	Vect B = A + radius;
	//	Vect A_out = A * (*this->poWorld);
	//	Vect B_out = B * (*this->poWorld);

	//	pCurrSphere->rad = (B_out[x] - A_out[x]) * pRefSphere->rad;
	//	pCurrSphere->cntr = A_out;
	//}

	Matrix* GameObject::GetWorld()
	{
		return this->poWorld;
	}

	GraphicsObject* GameObject::GetGraphicsObject()
	{
		return this->poGraphicsObject;
	}

	/*void GameObject::SetWorld(Matrix* _pWorld)
	{
		assert(_pWorld);
		*this->poWorld = *_pWorld;
	}*/

	void GameObject::SetPos(float x, float y, float z)
	{
		this->pPos->set(x, y, z);
	}
	void GameObject::SetRot(float x, float y, float z)
	{
		this->pRot->set(x, y, z);
	}
	void GameObject::SetScale(float x, float y, float z)
	{
		this->pScale->set(x, y, z);
	}

	Vect* GameObject::GetPos() const
	{
		assert(pPos);
		return pPos;
	}

	Vect* GameObject::GetRot() const
	{
		assert(pRot);
		return pRot;
	}

	Vect* GameObject::GetScale() const
	{
		assert(pScale);
		return pScale;
	}

	bool GameObject::GetDrawEnable()
	{
		return this->mDrawEnable;
	}

	void GameObject::SetDrawEnable(bool val)
	{
		this->mDrawEnable = val;
	}

	void GameObject::Draw()
	{
		if (mDrawEnable == true) {

			GraphicsObject* pGraphicsObj = this->GetGraphicsObject();
			assert(pGraphicsObj);

			pGraphicsObj->Render();
		}
	}

}

// --- End of File ---
