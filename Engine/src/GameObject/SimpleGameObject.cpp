#include "SimpleGameObject.h"
#include "Camera/CameraManager.h"

namespace Azul
{
	SimpleGameObject::SimpleGameObject(GraphicsObject *pGraphicsObject)
		: GameObject(pGraphicsObject)
	{
		assert(pGraphicsObject);

		/*this->poDof = new Vect(0.0f, 0.0f, 0.0f);
		assert(poDof);

		this->poUp = new Vect(0.0f, 1.0f, 0.0f);
		assert(poUp);*/

		

		this->poParent = new Matrix(Matrix::Special::Identity);
		assert(poParent);

		this->poLocal = new Matrix(Matrix::Special::Identity);
		assert(poLocal);
	}

	SimpleGameObject::~SimpleGameObject()
	{
		//delete this->poDof;
		//delete this->poUp;
		

		delete this->poParent;
		delete this->poLocal;
	}

	/*void SimpleGameObject::setMove(MoveDir move)
	{
		this->moveDir = move;
	};*/

	void SimpleGameObject::privUpdate(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		// Goal: update the world matrix

		//this->rotY += deltaRotY;
		//this->rotZ += deltaRotZ;

		this->pRot->y() += .01f;

		Matrix T = Matrix(Matrix::Trans::XYZ, this->pPos->x(), this->pPos->y(), this->pPos->z());
		Matrix S = Matrix(Matrix::Scale::XYZ, this->pScale->x(), this->pScale->y(), this->pScale->z());
		Quat   Q = Quat(Quat::Rot3::XYZ, this->pRot->x(), this->pRot->y(), this->pRot->z());
		
		

		// Isn't it awesome that we can multiply Quat with matrices!
		Matrix M = S * Q * T;
		*this->poLocal = M;
		
		// Goal: update the world matrix
		*this->poWorld = *this->poLocal;// *ParentWorld;
	}

	void SimpleGameObject::Update(AnimTime currentTime)
	{
		Camera* pCam = CameraManager::GetActiveCamera(Camera::Type::PERSPECTIVE_3D);
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

	/*void SimpleGameObject::SetScale(float sx, float sy, float sz)
	{
		this->pScale->set(sx, sy, sz);
	}

	void SimpleGameObject::SetPos(float x, float y, float z)
	{
		this->pPos->set(x, y, z);
	}*/

}

// --- End of File ---
