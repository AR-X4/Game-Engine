

#include "MathEngine.h"
#include "GameObjectRigid.h"


namespace Azul
{
	GameObjectRigid::GameObjectRigid(GraphicsObject *pGraphicsObject)
		: GameObject(pGraphicsObject)
	{
		assert(pGraphicsObject != 0);

		this->pScale = new Vect(1.0f, 1.0f, 1.0f);
		assert(this->pScale);

		this->pPos = new Vect(0.0f, 0.0f, 0.0f);
		assert(this->pPos);

		this->pRot = new Vect(0.0f, 0.0f, 0.0f);
		assert(this->pRot);

		this->angle = 0.0f;

		
	}

	GameObjectRigid::~GameObjectRigid()
	{
		delete this->pPos;
		delete this->pScale;
		delete this->pRot;
	}

	void GameObjectRigid::privUpdate(Time)
	{
		// for Humanoid
		/*Vect Trans = Vect(-280.172638f, 1288.917847f - 500, 526.212402f);
		Matrix T = Matrix(Matrix::Trans::XYZ, -Trans);
		Matrix M = T;
		Matrix R = Matrix(Matrix::Rot1::Z, 0);
		Matrix S = Matrix(Matrix::Scale::XYZ, 0.5f, 0.5f, 0.5f);

		*this->poWorld = S * R * M;*/


		// Special transform for Teddy
		//Matrix Rx = Matrix(Matrix::Rot1::X, MATH_PI2);
		//Matrix Rz = Matrix(Matrix::Rot1::Z, 1.7f);
		//Matrix M = Rx * Rz;

		//*this->poWorld = M;

		Matrix T = Matrix(Matrix::Trans::XYZ, this->pPos->x(), this->pPos->y(), this->pPos->z());
		Matrix S = Matrix(Matrix::Scale::XYZ, this->pScale->x(), this->pScale->y(), this->pScale->z());
		Quat   Q = Quat(Quat::Rot3::XYZ, this->pRot->x(), this->pRot->y(), this->pRot->z());

		*this->poWorld = S * Q *T;

		
	}

	void GameObjectRigid::Update(Time currentTime)
	{
		// Goal: update the world matrix
		this->privUpdate(currentTime);

		// push to graphics object
		this->poGraphicsObject->SetWorld(*this->poWorld);
	}
	void GameObjectRigid::SetPos(float x, float y, float z)
	{

		this->pPos->set(x, y, z);

	}
	void GameObjectRigid::SetRot(float x, float y, float z)
	{
		this->pRot->set(x, y, z);
	}
	void GameObjectRigid::SetScale(float x, float y, float z)
	{
		this->pScale->set(x, y, z);
	}
}

// --- End of File ---
