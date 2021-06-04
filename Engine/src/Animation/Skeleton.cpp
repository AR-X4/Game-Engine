

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
	


Skeleton::Skeleton()
	:pRigidBody(nullptr)
{

	//this->SkelHierarchy = pSkelIn;

	this->privSetAnimationHierarchy();
	//assert(pFirstBone != nullptr);

}

Skeleton::~Skeleton()
	{
		
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

	void Skeleton::SetName(const char* name)
	{
		assert(pRigidBody);
		pRigidBody->SetName(name);
	}

	GameObjectRigid* Skeleton::GetRigidBody() const
	{
		assert(pRigidBody);
		return pRigidBody;
	}

	

	void Skeleton::privSetAnimationHierarchy()
	{
		//AZUL_UNUSED_VAR(pBone);

		 //Load the model
		//Change this to find
		PyramidModel* pPyramidModel = (PyramidModel*)ModelManager::Find(Model::Name::PYRAMID);
		assert(pPyramidModel);
		

		 //Create GameObject
		//Vect color;
		//Vect pos(1, 1, 1);

		// Null object, Rigid body
		NullModel *pNullModel = new NullModel(0);
		ModelManager::Add(pNullModel);
		GraphicsObject_Null *pGraphicsObjNull = new GraphicsObject_Null(pNullModel, ShaderManager::Find(ShaderObject::Name::NULL_SHADER));
		GameObjectRigid *pGameRigid = new GameObjectRigid(pGraphicsObjNull);
		pGameRigid->SetDrawEnable(false);
		//pGameRigid->SetName("Skeleton Rigid");
		GameObjectManager::Add(pGameRigid, GameObjectManager::GetRoot());
		this->pRigidBody = pGameRigid;

		 //Human from txt file
		//color.set(1.0f, 0.0f, 0.0f, 1.0f);

		//ShaderObject* pLightShader = ShaderManager::Find(ShaderObject::Name::TEXTURE_POINT_LIGHT);

		////first bone
		//GraphicsObject_TextureLight* pGraphics_TextureLight = new GraphicsObject_TextureLight(pPyramidModel, pLightShader, color, pos, Texture::Name::DUCKWEED);
		//GameObjectAnim *pObj = new GameObjectAnim(pGraphics_TextureLight, pBone, pModelSkin);
		//pObj->SetIndex(SkelHierarchy->pBones[0].index);
		//pObj->SetName(SkelHierarchy->pBones[0].name);
		//GameObjectManager::Add(pObj, pGameRigid);
		//this->pFirstBone = pObj;
		////Trace::out2("\n{ %s %d %d }\n", SkelHierarchy->pBones[0].name, SkelHierarchy->pBones[0].index, SkelHierarchy->pBones[0].parentIndex);


		//for (int i = 1; i < SkelHierarchy->numBones; i++)
		//{
		//	pGraphics_TextureLight = new GraphicsObject_TextureLight(pPyramidModel, pLightShader, color, pos, Texture::Name::DUCKWEED);
		//	pObj = new GameObjectAnim(pGraphics_TextureLight, pBone, pModelSkin);
		//	pObj->SetDrawEnable(false);
		//	pObj->SetIndex(SkelHierarchy->pBones[i].index);
		//	pObj->SetName(SkelHierarchy->pBones[i].name);

		//	GameObjectAnim *pParent = this->privFindBoneByIndex(SkelHierarchy->pBones[i].parentIndex);
		//	assert(pParent);
		//	GameObjectManager::Add(pObj, pParent);

		//	//Trace::out2("{ %s %d %d }\n", SkelHierarchy->pBones[i].name, SkelHierarchy->pBones[i].index, SkelHierarchy->pBones[i].parentIndex);

		//}

	}
}

// --- End of File ---
