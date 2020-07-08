
#include <math.h>
#include "MathEngine.h"
#include "GameObject.h"
#include "Game/Game.h"

// ToDo:  refactor
extern Game* pGame;

namespace Azul
{

	GameObject::GameObject(GraphicsObject* pGraphicsObject)
		: poGraphicsObject(pGraphicsObject)
	{
		this->poWorld = new Matrix(Matrix::Special::Identity);//default matrix
		this->poTrans = new Vect(0.0f, 0.0f, 0.0f);			  //default location
		this->poScale = new Vect(1.0f, 1.0f, 1.0f);			  //default scale
		assert(poWorld);
		assert(poTrans);
		assert(poScale);
		assert(pGraphicsObject);

		this->rotY = 0.0f;
		this->rotZ = 0.0f;
		this->deltaRot = 0.0f;
	}

	GameObject::~GameObject()
	{
		delete this->poWorld;
		delete this->poTrans;
		delete this->poScale;
		delete this->poGraphicsObject;
	}

	Matrix* GameObject::GetWorld()
	{
		return this->poWorld;
	}

	GraphicsObject* GameObject::GetGraphicsObject()
	{
		return this->poGraphicsObject;
	}

	void GameObject::privUpdate(float currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		// Goal: update the world matrix
		this->rotY += deltaRot;
		this->rotZ += 0.01f;

		Matrix Scale(Matrix::Scale::XYZ, *this->poScale);
		Matrix TransA(Matrix::Trans::XYZ, *this->poTrans);
		Matrix RotY(Matrix::Rot1::Y, this->rotY);
		Matrix RotZ(Matrix::Rot1::Z, this->rotZ);

		// world matrix
		*this->poWorld = Scale * RotY * RotZ * TransA;
	}

	void GameObject::Update(float currentTime)
	{
		// Goal: update the world matrix
		this->privUpdate(currentTime);

		// push to graphics object
		this->poGraphicsObject->SetWorld(*this->poWorld);
	}

	void GameObject::SetWorld(Matrix* pWorld)
	{
		assert(pWorld);
		*this->poWorld = *pWorld;
	}

	void GameObject::SetPos(const Vect& pos)
	{
		*this->poTrans = pos;
	}

	void GameObject::SetScale(const Vect& inScale)
	{
		// deep copy
		*this->poScale = inScale;
	}

	void GameObject::SetDeltaRot(float delta)
	{
		this->deltaRot = delta;
	}

	void GameObject::Draw()
	{
		GraphicsObject* pGraphicsObj = this->GetGraphicsObject();
		assert(pGraphicsObj);

		pGraphicsObj->Render();
	}

}

// --- End of File ---
