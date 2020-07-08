#include "Demo.h"

#include "GameObject/GameObject.h"

#include "Models/CubeModel.h"
#include "Models/PyramidModel.h"
#include "Models/SphereModel.h"
#include "Models/DiamondModel.h"
#include "GraphicsObject/GraphicsObject.h"
#include "GraphicsObject/GraphicsObject_ColorByPosition.h"
#include "GraphicsObject/GraphicsObject_Wireframe.h"
#include "GraphicsObject/GraphicsObject_FlatTexture.h"
#include "GraphicsObject/GraphicsObject_TextureLight.h"
#include "GraphicsObject/GraphicsObject_ConstColor.h"

#include "GameObject/GameObjectManager.h"
#include "Models/ModelManager.h"
#include "Shaders/ShaderManager.h"

using namespace Azul;

void LoadPyramid()
{

	PyramidModel* pPyramidModel = new PyramidModel("PyramidModel.azul");
	assert(pPyramidModel);
	ModelManager::Add(pPyramidModel);

	// Create GameObject - PYRAMID

	// Middle Row

	GraphicsObject* pGraphicsObj = new GraphicsObject_ColorByPosition(pPyramidModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	GameObject* pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(-2.0f, 0.0f, -2.0f));
	pGameObj->SetDeltaRot(-0.07f);
	GameObjectManager::Add(pGameObj);


	pGraphicsObj = new GraphicsObject_Wireframe(pPyramidModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(0.0f, 0.0f, -2.0f));
	pGameObj->SetDeltaRot(0.052f);
	GameObjectManager::Add(pGameObj);

	Vect color(1.50f, 1.50f, 1.50f, 1.0f);
	Vect pos(1, 1, 1);

	pGraphicsObj = new GraphicsObject_TextureLight(pPyramidModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_POINT_LIGHT), TextureManager::Name::DUCKWEED, color, pos);

	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(2.0f, 0.0f, -2.0f));
	pGameObj->SetDeltaRot(-0.065f);
	GameObjectManager::Add(pGameObj);

	// Top Row

	pGraphicsObj = new GraphicsObject_FlatTexture(pPyramidModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::RED_BRICK);
	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(-2.0f, 1.0f, -2.0f));
	pGameObj->SetDeltaRot(0.1f);
	GameObjectManager::Add(pGameObj);


	pGraphicsObj = new GraphicsObject_FlatTexture(pPyramidModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::ROCKS);
	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(0.0f, 1.0f, -2.0f));
	pGameObj->SetDeltaRot(-0.013f);
	GameObjectManager::Add(pGameObj);


	pGraphicsObj = new GraphicsObject_FlatTexture(pPyramidModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::STONES);
	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(2.0f, 1.0f, -2.0f));
	pGameObj->SetDeltaRot(0.03f);
	GameObjectManager::Add(pGameObj);

	// bottom

	color.set(1.25f, 1.25f, 1.25f, 1.0f);

	pGraphicsObj = new GraphicsObject_ConstColor(pPyramidModel, ShaderManager::Find(ShaderObject::Name::CONST_COLOR), color, pos);


	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(-2.0f, -1.0f, -2.0f));
	pGameObj->SetDeltaRot(0.002f);
	GameObjectManager::Add(pGameObj);


	pGraphicsObj = new GraphicsObject_ColorByPosition(pPyramidModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(0.0f, -1.0f, -2.0f));
	pGameObj->SetDeltaRot(-0.007f);
	pGameObj->SetScale(Vect(0.5f, 0.5f, 0.50f));
	GameObjectManager::Add(pGameObj);


	pGraphicsObj = new GraphicsObject_Wireframe(pPyramidModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(2.0f, -1.0f, -2.0f));
	pGameObj->SetDeltaRot(0.082f);
	pGameObj->SetScale(Vect(0.5f, 2.0f, 0.50f));
	GameObjectManager::Add(pGameObj);

}

void LoadCube()
{

	// Load the model
	CubeModel* pCubeModel = new CubeModel("cubeModel.azul");
	assert(pCubeModel);
	ModelManager::Add(pCubeModel);

	// Create GameObject - CUBE

	// Middle Row
	GraphicsObject*  pGraphicsObj = new GraphicsObject_ColorByPosition(pCubeModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	GameObject* pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(-2.0f, 0.0f, 0.0f));
	pGameObj->SetDeltaRot(-0.07f);
	GameObjectManager::Add(pGameObj);

	pGraphicsObj = new GraphicsObject_Wireframe(pCubeModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(0.0f, 0.0f, 0.0f));
	pGameObj->SetDeltaRot(0.052f);
	GameObjectManager::Add(pGameObj);

	Vect color(1.50f, 1.50f, 1.50f, 1.0f);
	Vect pos(1, 1, 1);
	pGraphicsObj = new GraphicsObject_TextureLight(pCubeModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_POINT_LIGHT), TextureManager::Name::DUCKWEED, color, pos);

	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(2.0f, 0.0f, 0.0f));
	pGameObj->SetDeltaRot(-0.065f);
	GameObjectManager::Add(pGameObj);

	// Top Row
	pGraphicsObj = new GraphicsObject_FlatTexture(pCubeModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::RED_BRICK);
	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(-2.0f, 1.0f, 0.0f));
	pGameObj->SetDeltaRot(0.1f);
	GameObjectManager::Add(pGameObj);

	pGraphicsObj = new GraphicsObject_FlatTexture(pCubeModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::ROCKS);
	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(0.0f, 1.0f, 0.0f));
	pGameObj->SetDeltaRot(-0.013f);
	GameObjectManager::Add(pGameObj);

	pGraphicsObj = new GraphicsObject_FlatTexture(pCubeModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::STONES);
	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(2.0f, 1.0f, 0.0f));
	pGameObj->SetDeltaRot(0.03f);
	GameObjectManager::Add(pGameObj);

	// bottom

	color.set(1.25f, 1.25f, 1.25f, 1.0f);
	pGraphicsObj = new GraphicsObject_ConstColor(pCubeModel, ShaderManager::Find(ShaderObject::Name::CONST_COLOR), color, pos);

	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(-2.0f, -1.0f, 0.0f));
	pGameObj->SetDeltaRot(0.002f);
	GameObjectManager::Add(pGameObj);

	pGraphicsObj = new GraphicsObject_ColorByPosition(pCubeModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(0.0f, -1.0f, 0.0f));
	pGameObj->SetDeltaRot(-0.007f);
	pGameObj->SetScale(Vect(0.5f, 0.5f, 0.50f));
	GameObjectManager::Add(pGameObj);

	pGraphicsObj = new GraphicsObject_Wireframe(pCubeModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(2.0f, -1.0f, 0.0f));
	pGameObj->SetDeltaRot(0.082f);
	pGameObj->SetScale(Vect(0.5f, 2.0f, 0.50f));
	GameObjectManager::Add(pGameObj);
}

void LoadSphere()
{

	SphereModel* pSphereModel = new SphereModel("SphereModel.azul");
	assert(pSphereModel);
	ModelManager::Add(pSphereModel);

	// Create GameObject - PYRAMID

	// Middle Row

	GraphicsObject* pGraphicsObj = new GraphicsObject_ColorByPosition(pSphereModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	GameObject* pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(-2.0f, 0.0f, -4.0f));
	pGameObj->SetScale(Vect(0.6f, 0.6f, 0.6f));
	pGameObj->SetDeltaRot(-0.07f);
	GameObjectManager::Add(pGameObj);


	pGraphicsObj = new GraphicsObject_Wireframe(pSphereModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(0.0f, 0.0f, -4.0f));
	pGameObj->SetScale(Vect(0.6f, 0.6f, 0.6f));
	pGameObj->SetDeltaRot(0.052f);
	GameObjectManager::Add(pGameObj);

	Vect color(1.50f, 1.50f, 1.50f, 1.0f);
	Vect pos(1, 1, 1);

	pGraphicsObj = new GraphicsObject_TextureLight(pSphereModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_POINT_LIGHT), TextureManager::Name::DUCKWEED, color, pos);

	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(2.0f, 0.0f, -4.0f));
	pGameObj->SetScale(Vect(0.6f, 0.6f, 0.6f));
	pGameObj->SetDeltaRot(-0.065f);
	GameObjectManager::Add(pGameObj);

	// Top Row

	pGraphicsObj = new GraphicsObject_FlatTexture(pSphereModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::RED_BRICK);
	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(-2.0f, 1.0f, -4.0f));
	pGameObj->SetScale(Vect(0.6f, 0.6f, 0.6f));
	pGameObj->SetDeltaRot(0.1f);
	GameObjectManager::Add(pGameObj);


	pGraphicsObj = new GraphicsObject_FlatTexture(pSphereModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::ROCKS);
	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(0.0f, 1.0f, -4.0f));
	pGameObj->SetScale(Vect(0.6f, 0.6f, 0.6f));
	pGameObj->SetDeltaRot(-0.013f);
	GameObjectManager::Add(pGameObj);


	pGraphicsObj = new GraphicsObject_FlatTexture(pSphereModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::STONES);
	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(2.0f, 1.0f, -4.0f));
	pGameObj->SetScale(Vect(0.6f, 0.6f, 0.6f));
	pGameObj->SetDeltaRot(0.03f);
	GameObjectManager::Add(pGameObj);

	// bottom

	color.set(1.25f, 1.25f, 1.25f, 1.0f);

	pGraphicsObj = new GraphicsObject_ConstColor(pSphereModel, ShaderManager::Find(ShaderObject::Name::CONST_COLOR), color, pos);


	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(-2.0f, -1.0f, -4.0f));
	pGameObj->SetScale(Vect(0.6f, 0.6f, 0.6f));
	pGameObj->SetDeltaRot(0.002f);
	GameObjectManager::Add(pGameObj);


	pGraphicsObj = new GraphicsObject_ColorByPosition(pSphereModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(0.0f, -1.0f, -4.0f));
	pGameObj->SetDeltaRot(-0.007f);
	pGameObj->SetScale(Vect(0.3f, 0.3f, 0.3f));
	GameObjectManager::Add(pGameObj);


	pGraphicsObj = new GraphicsObject_Wireframe(pSphereModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(2.0f, -1.0f, -4.0f));
	pGameObj->SetDeltaRot(0.082f);
	pGameObj->SetScale(Vect(0.3f, 1.0f, 0.25f));
	GameObjectManager::Add(pGameObj);
}

void LoadDiamond()
{

	DiamondModel* pDiamondModel = new DiamondModel("DiamondModel.azul");
	assert(pDiamondModel);
	ModelManager::Add(pDiamondModel);

	// Create GameObject - PYRAMID

	// Middle Row

	GraphicsObject* pGraphicsObj = new GraphicsObject_ColorByPosition(pDiamondModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	GameObject* pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(-2.0f, 0.0f, -6.0f));
	pGameObj->SetScale(Vect(0.5f, 0.5f, 0.5f));
	pGameObj->SetDeltaRot(-0.07f);
	GameObjectManager::Add(pGameObj);


	pGraphicsObj = new GraphicsObject_Wireframe(pDiamondModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(0.0f, 0.0f, -6.0f));
	pGameObj->SetScale(Vect(0.5f, 0.5f, 0.5f));
	pGameObj->SetDeltaRot(0.052f);
	GameObjectManager::Add(pGameObj);

	Vect color(1.50f, 1.50f, 1.50f, 1.0f);
	Vect pos(1, 1, 1);

	pGraphicsObj = new GraphicsObject_TextureLight(pDiamondModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_POINT_LIGHT), TextureManager::Name::DUCKWEED, color, pos);

	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(2.0f, 0.0f, -6.0f));
	pGameObj->SetScale(Vect(0.5f, 0.5f, 0.5f));
	pGameObj->SetDeltaRot(-0.065f);
	GameObjectManager::Add(pGameObj);

	// Top Row

	pGraphicsObj = new GraphicsObject_FlatTexture(pDiamondModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::RED_BRICK);
	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(-2.0f, 1.0f, -6.0f));
	pGameObj->SetScale(Vect(0.5f, 0.5f, 0.5f));
	pGameObj->SetDeltaRot(0.1f);
	GameObjectManager::Add(pGameObj);


	pGraphicsObj = new GraphicsObject_FlatTexture(pDiamondModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::ROCKS);
	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(0.0f, 1.0f, -6.0f));
	pGameObj->SetScale(Vect(0.5f, 0.5f, 0.5f));
	pGameObj->SetDeltaRot(-0.013f);
	GameObjectManager::Add(pGameObj);


	pGraphicsObj = new GraphicsObject_FlatTexture(pDiamondModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::STONES);
	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(2.0f, 1.0f, -6.0f));
	pGameObj->SetScale(Vect(0.5f, 0.5f, 0.5f));
	pGameObj->SetDeltaRot(0.03f);
	GameObjectManager::Add(pGameObj);

	// bottom

	color.set(1.25f, 1.25f, 1.25f, 1.0f);

	pGraphicsObj = new GraphicsObject_ConstColor(pDiamondModel, ShaderManager::Find(ShaderObject::Name::CONST_COLOR), color, pos);


	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(-2.0f, -1.0f, -6.0f));
	pGameObj->SetScale(Vect(0.5f, 0.5f, 0.5f));
	pGameObj->SetDeltaRot(0.002f);
	GameObjectManager::Add(pGameObj);


	pGraphicsObj = new GraphicsObject_ColorByPosition(pDiamondModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(0.0f, -1.0f, -6.0f));
	pGameObj->SetDeltaRot(-0.007f);
	pGameObj->SetScale(Vect(0.25f, 0.25f, 0.250f));
	GameObjectManager::Add(pGameObj);


	pGraphicsObj = new GraphicsObject_Wireframe(pDiamondModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	pGameObj = new GameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(2.0f, -1.0f, -6.0f));
	pGameObj->SetDeltaRot(0.082f);
	pGameObj->SetScale(Vect(0.25f, 1.0f, 0.250f));
	GameObjectManager::Add(pGameObj);
}
