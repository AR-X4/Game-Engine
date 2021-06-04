
#include "BoundingSphereGameObject.h"
#include "GraphicsObject/GraphicsObject_WireframeConstantColor.h"
#include "Camera/CameraManager.h"

namespace Azul
{
	BoundingSphereGameObject::BoundingSphereGameObject(GraphicsObject *pGraphicsObject)
		: GameObject(pGraphicsObject)
	{
		this->pRefGameObj = nullptr;
		this->poScale = new Vect();
		this->poTrans = new Vect();

		assert(pGraphicsObject);
		assert(this->poScale);
		assert(this->poTrans);
	}

	BoundingSphereGameObject::~BoundingSphereGameObject()
	{
		delete this->poScale;
		delete this->poTrans;
	}

	void BoundingSphereGameObject::SetBoundingSphereReference(GameObject *pGameObj)
	{
		this->pRefGameObj = pGameObj;

		// Correct the center and size of the model
		Sphere* pSphereTmp;
		pSphereTmp = pGameObj->GetGraphicsObject()->GetModel()->poRefSphere;

		this->poScale->set(pSphereTmp->rad, pSphereTmp->rad, pSphereTmp->rad);
		this->poTrans->set(pSphereTmp->cntr);
	}

	void BoundingSphereGameObject::privUpdate(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		// Goal: update the world matrix

		Matrix Scale(Matrix::Scale::XYZ, *this->poScale * 2);
		Matrix TransA(Matrix::Trans::XYZ, *this->poTrans);

		// world matrix
		//               (adjust unit sphere) * world
		*this->poWorld = Scale * TransA * *this->pRefGameObj->GetWorld();


		//-----------------------------------------------------
		// Normally the collision test goes in the graphics 
		//    object for culling, only here for visualization
		//-----------------------------------------------------

		//this is unnecessary...
		// Flip the color based on camera_1
		Camera* pCam = CameraManager::GetActiveCamera(Camera::Type::PERSPECTIVE_3D);
		Sphere *pCurrSphere = this->poGraphicsObject->GetBoundingSphereForCollision();

		// Architecture issue... this doesn't feel good.
		GraphicsObject_WireframeConstantColor *pGObj = (GraphicsObject_WireframeConstantColor *)this->poGraphicsObject;
		if (pCam->CullTest(*pCurrSphere) == Camera::CullResult::CULL_OUTSIDE)
		{
			pGObj->poWireColor->set(1.0f, 0.0f, 0.0f, 1.0f);
			//this->mDrawEnable = false;
		}
		else
		{
			pGObj->poWireColor->set(0.0f, 1.0f, 0.0f, 1.0f);
			//this->mDrawEnable = true;
		}
	}

	//void BoundingSphereGameObject::privUpdateBoundingSphereReference(Time currentTime)
	//{
	//	// incase the reference object wasn't updated first
	//	// update it now.

	//	this->pRefGameObj->Update(currentTime);// redundant?

	//	// set the center and scale of bound box based on ref game object's world coord
	//	Sphere *pRefCurrSphere = this->pRefGameObj->GetGraphicsObject()->poCurrSphere;
	//	this->poScale->set(pRefCurrSphere->rad*2.0f, pRefCurrSphere->rad * 2.0f, pRefCurrSphere->rad * 2.0f);//hack fix scaling
	//	this->poTrans->set(pRefCurrSphere->cntr);

	//	Sphere *pCurrSphere = this->GetGraphicsObject()->poCurrSphere;
	//	*pCurrSphere = *pRefCurrSphere;
	//}

	void BoundingSphereGameObject::Update(AnimTime currentTime)
	{
		// update the reference's world
		this->pRefGameObj->Update(currentTime);

		// Goal: update the world matrix
		this->privUpdate(currentTime);

		// push to graphics object
		this->poGraphicsObject->SetWorld(*this->poWorld);
	}

}

// --- End of File ---

