#include "SimpleGameObject.h"
#include "Camera/CameraManager.h"

namespace Azul
{
	SimpleGameObject::SimpleGameObject(GraphicsObject *pGraphicsObject)
		: GameObject(pGraphicsObject)
	{
		assert(pGraphicsObject);

		this->poDof = new Vect(0.0f, 0.0f, 0.0f);
		assert(poDof);

		this->poUp = new Vect(0.0f, 1.0f, 0.0f);
		assert(poUp);

		this->poScale = new Vect(1.0f, 1.0f, 1.0f);
		assert(poScale);

		this->poPos = new Vect(0.0f, 0.0f, 0.0f);
		assert(poPos);

		this->poRot = new Vect(0.0f, 0.0f, 0.0f);
		assert(poRot);

		this->poParent = new Matrix(Matrix::Special::Identity);
		assert(poParent);

		this->poLocal = new Matrix(Matrix::Special::Identity);
		assert(poLocal);
	}

	SimpleGameObject::~SimpleGameObject()
	{
		delete this->poDof;
		delete this->poUp;
		delete this->poScale;
		delete this->poPos;

		delete this->poRot;

		delete this->poParent;
		delete this->poLocal;
	}

	/*void SimpleGameObject::setMove(MoveDir move)
	{
		this->moveDir = move;
	};*/

	void SimpleGameObject::privUpdate(Time currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		// Goal: update the world matrix

		//this->rotY += deltaRotY;
		//this->rotZ += deltaRotZ;

		this->poRot->y() += .01f;

		Matrix T = Matrix(Matrix::Trans::XYZ, this->poPos->x(), this->poPos->y(), this->poPos->z());
		Matrix S = Matrix(Matrix::Scale::XYZ, this->poScale->x(), this->poScale->y(), this->poScale->z());
		Quat   Q = Quat(Quat::Rot3::XYZ, this->poRot->x(), this->poRot->y(), this->poRot->z());
		
		

		// Isn't it awesome that we can multiply Quat with matrices!
		Matrix M = S * Q * T;
		*this->poLocal = M;
		
		// Goal: update the world matrix
		*this->poWorld = *this->poLocal;// *ParentWorld;
	}

	void SimpleGameObject::Update(Time currentTime)
	{
		Camera* pCam = CameraManager::GetActiveCamera();
		Sphere* pCurrSphere = this->poGraphicsObject->GetBoundingSphereForCollision();

		if (pCam->CullTest(*pCurrSphere) == Camera::CullResult::CULL_OUTSIDE)
		{
			this->mDrawEnable = false;
		}
		else
		{
			this->mDrawEnable = true;
		}

		// Goal: update the world matrix
		this->privUpdate(currentTime);

		// push to graphics object
		this->poGraphicsObject->SetWorld(*this->poWorld);
	}

	void SimpleGameObject::SetScale(float sx, float sy, float sz)
	{
		this->poScale->set(sx, sy, sz);
	}

	void SimpleGameObject::SetPos(float x, float y, float z)
	{
		this->poPos->set(x, y, z);
	}

}

// --- End of File ---
