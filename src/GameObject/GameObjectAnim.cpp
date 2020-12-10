

#include "Game/Game.h"
#include "MathEngine.h"
#include "GameObjectAnim.h"
#include "GameObject.h"
#include "GraphicsObject/GraphicsObject.h"
#include "Models/Model.h"
#include "Animation/Anim.h"

namespace Azul
{
	GameObjectAnim::GameObjectAnim(GraphicsObject *pGraphicsObject, Bone *_pBoneResult)
		: GameObject(pGraphicsObject),
		pBoneResult(_pBoneResult)
	{
		assert(pGraphicsObject);
		assert(pBoneResult);

		this->poDof = new Vect(0.0f, 0.0f, 0.0f);
		assert(poDof);

		this->poUp = new Vect(0.0f, 1.0f, 0.0f);
		assert(poUp);

		this->poScale = new Vect(1.0f, 1.0f, 1.0f);
		assert(poScale);

		this->poPos = new Vect(0.0f, 0.0f, 0.0f);
		assert(poPos);

		this->indexBoneArray = 0;

		this->poLocal = new Matrix(Matrix::Special::Identity);
		assert(poLocal);

		this->poBoneOrientation = new Matrix(Matrix::Special::Identity);
		assert(poBoneOrientation);
	}

	GameObjectAnim::~GameObjectAnim()
	{
		delete this->poDof;   
		delete this->poUp;    
		delete this->poScale; 
		delete this->poPos;   

		delete this->poLocal; 
		delete this->poBoneOrientation;
	}

	void GameObjectAnim::privUpdate(Time currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		GameObject *pBoneParent = (GameObject *)this->GetParent();
		assert(pBoneParent);

		Matrix ParentWorld = *pBoneParent->GetWorld();

		// REMEMBER this is for Animation and hierachy, you need to handle models differently
		// Get the result bone array, from there make the matrix
		Matrix T = Matrix(Matrix::Trans::XYZ, this->pBoneResult[indexBoneArray].T);
		Matrix S = Matrix(Matrix::Scale::XYZ, this->pBoneResult[indexBoneArray].S);
		Quat   Q = this->pBoneResult[indexBoneArray].Q;

		// Isn't it awesome that we can multiply Quat with matrices!
		Matrix M = S * Q * T;
		*this->poLocal = M;

		// Goal: update the world matrix
		*this->poWorld = *this->poLocal * ParentWorld;

	}

	void GameObjectAnim::Update(Time currentTime)
	{
		// Goal: update the world matrix
		this->privUpdate(currentTime);

		// update the bounding volume based on world matrix
		//this->baseUpdateBoundingSphere();

		// push to graphics object
		Matrix mTmp = *this->poBoneOrientation;

		// push to graphics object
		this->poGraphicsObject->SetWorld(mTmp);
	}

	void GameObjectAnim::SetIndex(int val)
	{
		this->indexBoneArray = val;
	}

	void GameObjectAnim::SetBoneOrientation(const Matrix &tmp)
	{
		*this->poBoneOrientation = tmp;
	}

	Matrix GameObjectAnim::GetBoneOrientation(void) const
	{
		return Matrix(*this->poBoneOrientation);
	}
}

// --- End of File ---
