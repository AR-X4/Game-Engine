

#include "Models/PyramidModel.h"
#include "GraphicsObject/GraphicsObject_TextureLight.h"
#include "GameObject/GameObjectManager.h"
#include "Models/ModelManager.h"
#include "Shaders/ShaderManager.h"

#include "GraphicsObject/GraphicsObject_Null.h"
#include "Models/NullModel.h"
#include "Skeleton.h"
#include "PCSTreeForwardIterator.h"
#include "GameObject/GameObjectManager.h"



namespace Azul
{
	// Needs to move to binary file
	/*Skeleton::Data skel[]
	{
		{ "Hips", 0, -1 },
		{ "Spine_Dummy", 1, 0 },
		{ "Spine", 2, 1 },
		{ "Spine1", 3, 2 },
		{ "Spine2", 4, 3 },
		{ "Neck", 5, 4 },
		{ "Head", 6, 5 },
		{ "Head_End", 7, 6 },
		{ "LeftShoulder_Dummy", 8, 4 },
		{ "LeftShoulder", 9, 8 },
		{ "LeftArm_Dummy", 10, 9 },
		{ "LeftArm", 11, 10 },
		{ "LeftArmRoll", 12, 11 },
		{ "LeftForeArm", 13, 12 },
		{ "LeftForeArmRoll", 14, 13 },
		{ "LeftHand", 15, 14 },
		{ "LeftThumb1", 16, 15 },
		{ "LeftThumb2", 17, 16 },
		{ "LeftThumb3", 18, 17 },
		{ "LeftThumb_End", 19, 18 },
		{ "LeftIndex1", 20, 15 },
		{ "LeftIndex2", 21, 20 },
		{ "LeftIndex3", 22, 21 },
		{ "LeftIndex_End", 23, 22 },
		{ "LeftMiddle1", 24, 15 },
		{ "LeftMiddle2", 25, 24 },
		{ "LeftMiddle3", 26, 25 },
		{ "LeftMiddle_End", 27, 26 },
		{ "LeftRing1", 28, 15 },
		{ "LeftRing2", 29, 28 },
		{ "LeftRing3", 30, 29 },
		{ "LeftRing_End", 31, 30 },
		{ "LeftPinky1", 32, 15 },
		{ "LeftPinky2", 33, 32 },
		{ "LeftPinky3", 34, 33 },
		{ "LeftPinky_End", 35, 34 },
		{ "RightShoulder_Dummy", 36, 4 },
		{ "RightShoulder", 37, 36 },
		{ "RightArm_Dummy", 38, 37 },
		{ "RightArm", 39, 38 },
		{ "RightArmRoll", 40, 39 },
		{ "RightForeArm", 41, 40 },
		{ "RightForeArmRoll", 42, 41 },
		{ "RightHand", 43, 42 },
		{ "RightThumb1", 44, 43 },
		{ "RightThumb2", 45, 44 },
		{ "RightThumb3", 46, 45 },
		{ "RightThumb_End", 47, 46 },
		{ "RightIndex1", 48, 43 },
		{ "RightIndex2", 49, 48 },
		{ "RightIndex3", 50, 49 },
		{ "RightIndex_End", 51, 50 },
		{ "RightMiddle1", 52, 43 },
		{ "RightMiddle2", 53, 52 },
		{ "RightMiddle3", 54, 53 },
		{ "RightMiddle_End", 55, 54 },
		{ "RightRing1", 56, 43 },
		{ "RightRing2", 57, 56 },
		{ "RightRing3", 58, 57 },
		{ "RightRing_End", 59, 58 },
		{ "RightPinky1", 60, 43 },
		{ "RightPinky2", 61, 60 },
		{ "RightPinky3", 62, 61 },
		{ "RightPinky_End", 63, 62 },
		{ "LeftHips_Dummy", 64, 0 },
		{ "LeftUpLeg", 65, 64 },
		{ "LeftUpLegRoll", 66, 65 },
		{ "LeftLeg", 67, 66 },
		{ "LeftLegRoll", 68, 67 },
		{ "LeftFoot", 69, 68 },
		{ "LeftToes", 70, 69 },
		{ "LeftToes_End", 71, 70 },
		{ "RightHips_Dummy", 72, 0 },
		{ "RightUpLeg", 73, 72 },
		{ "RightUpLegRoll", 74, 73 },
		{ "RightLeg", 75, 74 },
		{ "RightLegRoll", 76, 75 },
		{ "RightFoot", 77, 76 },
		{ "RightToes", 78, 77 },
		{ "RightToes_End", 79, 78 }
	};*/


Skeleton::Skeleton(Bone* pBoneResultBuff, SkeletonHierarchy* pSkelIn)
	:pFirstBone(nullptr), pRigidBody(nullptr)
{

	this->SkelHierarchy = pSkelIn;

	this->privSetAnimationHierarchy(pBoneResultBuff);
	assert(pFirstBone != nullptr);

}

Skeleton::~Skeleton()
	{
		
	}

	GameObjectAnim *Skeleton::GetFirstBone()
	{
		assert(pFirstBone);
		return this->pFirstBone;
	}

	void Skeleton::SetPos(float x, float y, float z)
	{
		this->pRigidBody->SetPos(x, y, z);
	}

	void Skeleton::SetRot(float x, float y, float z)
	{
		this->pRigidBody->SetRot(x, y, z);
	}

	void Skeleton::SetScale(float x, float y, float z)
	{
		this->pRigidBody->SetScale(x, y, z);
	}

	GameObjectAnim *Skeleton::privFindBoneByIndex(int index)
	{
		GameObjectAnim *pFound = nullptr;

		GameObjectAnim *pObj = this->GetFirstBone();

		// TODO - add test bed for an interator of a tree with only one node
		if (pObj->indexBoneArray == index)
		{
			pFound = pObj;
		}
		else
		{
			PCSTreeForwardIterator pIter(pObj);
			PCSNode *pNode = pIter.First();
			GameObjectAnim *pGameObj = nullptr;

			// walks the anim node does the pose for everything that
			while (pNode != nullptr)
			{
				assert(pNode);
				// Update the game object
				pGameObj = (GameObjectAnim *)pNode;
				if (pGameObj->indexBoneArray == index)
				{
					pFound = pGameObj;
					break;
				}
				pNode = pIter.Next();
			}
		}

		return pFound;
	}

	void Skeleton::privSetAnimationHierarchy(Bone *pBone)
	{
		//AZUL_UNUSED_VAR(pBone);

		 //Load the model
		//Change this to find
		PyramidModel* pPyramidModel = (PyramidModel*)ModelManager::Find(Model::Name::PYRAMID);
		assert(pPyramidModel);
		

		 //Create GameObject
		Vect color;
		Vect pos(1, 1, 1);

		// Null object, Rigid body
		NullModel *pNullModel = new NullModel(0);
		ModelManager::Add(pNullModel);
		GraphicsObject_Null *pGraphicsObjNull = new GraphicsObject_Null(pNullModel, ShaderManager::Find(ShaderObject::Name::NULL_SHADER));
		GameObjectRigid *pGameRigid = new GameObjectRigid(pGraphicsObjNull);
		pGameRigid->SetName("Rigid");
		GameObjectManager::Add(pGameRigid, GameObjectManager::GetRoot());
		this->pRigidBody = pGameRigid;

		 //Human from txt file
		color.set(1.0f, 0.0f, 0.0f, 1.0f);

		ShaderObject* pLightShader = ShaderManager::Find(ShaderObject::Name::TEXTURE_POINT_LIGHT);

		//first bone
		GraphicsObject_TextureLight* pGraphics_TextureLight = new GraphicsObject_TextureLight(pPyramidModel, pLightShader, color, pos, TextureManager::Name::DUCKWEED);
		GameObjectAnim *pObj = new GameObjectAnim(pGraphics_TextureLight, pBone);
		pObj->SetIndex(SkelHierarchy->pBones[0].index);
		pObj->SetName(SkelHierarchy->pBones[0].name);
		GameObjectManager::Add(pObj, pGameRigid);
		this->pFirstBone = pObj;
		//Trace::out2("\n{ %s %d %d }\n", SkelHierarchy->pBones[0].name, SkelHierarchy->pBones[0].index, SkelHierarchy->pBones[0].parentIndex);


		for (int i = 1; i < SkelHierarchy->numBones; i++)
		{
			pGraphics_TextureLight = new GraphicsObject_TextureLight(pPyramidModel, pLightShader, color, pos, TextureManager::Name::DUCKWEED);
			pObj = new GameObjectAnim(pGraphics_TextureLight, pBone);
			pObj->SetIndex(SkelHierarchy->pBones[i].index);
			pObj->SetName(SkelHierarchy->pBones[i].name);

			GameObjectAnim *pParent = this->privFindBoneByIndex(SkelHierarchy->pBones[i].parentIndex);
			assert(pParent);
			GameObjectManager::Add(pObj, pParent);

			//Trace::out2("{ %s %d %d }\n", SkelHierarchy->pBones[i].name, SkelHierarchy->pBones[i].index, SkelHierarchy->pBones[i].parentIndex);

		}

	}
}

// --- End of File ---
