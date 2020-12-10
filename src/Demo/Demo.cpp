#include "Demo.h"

#include "GameObject/GameObject.h"
#include "GameObject/SimpleGameObject.h"
#include "BoundingVolume/BoundingSphereGameObject.h"

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
#include "GraphicsObject/GraphicsObject_WireframeConstantColor.h"

#include "GameObject/GameObjectManager.h"
#include "Models/ModelManager.h"
#include "Shaders/ShaderManager.h"

#include "Input/InputManager.h"
#include "AssetLoader/AssetLoader.h"
#include "Camera/CameraManager.h"

#include "Animation/AnimControllerManager.h"
#include "Animation/ClipManager.h"

using namespace Azul;

//hack input demo for now .. need to make input man
GraphicsObject_FlatTexture* pGraphicsObj_WARBEAR;
static bool textSwap = true;
static bool keypress = false;
static bool keypress2 = false;
static bool pause = false;
AnimController* controller;
AnimController* controller1;
AnimController* controller2;
AnimController* controller3;


void LoadAnimDemo()
{
	//bone model
	ModelManager::Add(new PyramidModel("pyramidModel.azul"));


	//---------------------------------------------------------------------------------------------------------------------------------

	AssetLoader::LoadAnim("../../Models/shotgun.flcn");

	Time delta2 = 0.2f * Time(Time::Duration::NTSC_30_FRAME);

	controller2 = new AnimController(delta2, SkeletonHierarchy::Name::HUMANOID_SHOTGUN, Clip::Name::HUMANOID_SHOTGUN);

	controller2->SetPos(4.0f, 46.0f, -5.0f);
	controller2->SetScale(0.005f, 0.005f, 0.005f);
	controller2->SetRot(-1.5f, 0.0f, 0.0f);


	controller2->Play();

	AnimControllerManager::Add(controller2);

	//------------------------------------------------------------------

	AssetLoader::LoadAnim("../../Models/humanoid_run.flcn");

	Time delta = 0.25f * Time(Time::Duration::NTSC_30_FRAME);
	controller = new AnimController(delta, SkeletonHierarchy::Name::HUMANOID_RUN, Clip::Name::HUMANOID_RUN);
	
	controller->SetPos(-1.0f, 46.0f, 0.0f);
	controller->SetScale(0.005f, 0.005f, 0.005f);
	controller->SetRot(-1.5f, 0.0f, 0.0f);


	controller->Play();

	AnimControllerManager::Add(controller);

	////---------------------------------------------------------------------------------------------------------------------------------

	AssetLoader::LoadAnim("../../Models/teddy.flcn");

	Time delta4 = 0.3f * Time(Time::Duration::NTSC_30_FRAME);

	controller1 = new AnimController(delta4, SkeletonHierarchy::Name::TEDDY, Clip::Name::TEDDY);
	
	controller1->SetPos(8.0f, 46.0f, 0.0f);
	controller1->SetRot(0.0f, 0.0f, 0.0f);
	controller1->SetScale(0.015f, 0.015f, 0.015f);


	controller1->Play();

	AnimControllerManager::Add(controller1);

	

	//---------------------------------------------------------------------------------------------------------------------------------

	AssetLoader::LoadAnim("../../Models/punch.flcn");

	Time delta3 = 0.5f * Time(Time::Duration::NTSC_30_FRAME);

	controller3 = new AnimController(delta3, SkeletonHierarchy::Name::PUNCH, Clip::Name::PUNCH);

	controller3->SetPos(-6.0f, 46.0f, 0.0f);
	controller3->SetScale(0.005f, 0.005f, 0.005f);
	controller3->SetRot(-1.5f, 0.0f, 0.0f);


	controller3->Play();

	AnimControllerManager::Add(controller3);

}

void UpdateAnimDemo()
{
}

void UpdateModelDemo()
{
	Keyboard* pKey = InputManager::GetKeyboard();
	/*Camera* pCam1 = CameraManager::Find(Camera::Name::Cam1);
	Camera* pCam2 = CameraManager::Find(Camera::Name::Cam2);


	if (pKey->GetKeyState(Keyboard::KEY_0))
	{
		CameraManager::SetActiveCamera(pCam1);
	}

	if (pKey->GetKeyState(Keyboard::KEY_1))
	{
		CameraManager::SetActiveCamera(pCam2);*/
	//}

	Camera* pActive = CameraManager::GetActiveCamera();


	bool keyTpress = pKey->GetKeyState(Keyboard::KEY_T);
	if (keyTpress == true && keypress == false)
	{
		if (textSwap == true) {
			pGraphicsObj_WARBEAR->textName = TextureManager::Name::POLARBEAR;
			textSwap = false;
		}
		else {
			pGraphicsObj_WARBEAR->textName = TextureManager::Name::WARBEAR;
			textSwap = true;
		}
	}
	keypress = keyTpress;

	bool keyPpress = pKey->GetKeyState(Keyboard::KEY_P);
	if (keyPpress == true && keypress2 == false)
	{
		if (pause == true) {
			controller->Play();
			controller1->Play();
			controller2->Play();
			controller3->Play();

			pause = false;
		}
		else {
			controller-> Pause();
			controller1->Pause();
			controller2->Pause();
			controller3->Pause();
			pause = true;
		}
	}
	keypress2 = keyPpress;


	if (pKey->GetKeyState(Keyboard::KEY_I))
	{

		Vect pos;
		Vect tar;
		Vect up;
		Vect upNorm;
		Vect forwardNorm;
		Vect rightNorm;

		pActive->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

		// OK, now 3 points... pos, tar, up
		//     now 3 normals... upNorm, forwardNorm, rightNorm
		//	tar = obj;

		pos = pos + 2.0f * forwardNorm;
		tar = tar + 2.0f * forwardNorm;


		pActive->SetHelper(up, tar, pos);
	}

	if (pKey->GetKeyState(Keyboard::KEY_O))
	{

		Vect pos;
		Vect tar;
		Vect up;
		Vect upNorm;
		Vect forwardNorm;
		Vect rightNorm;
		//Vect obj(0.0f, 50.0f, 0.0f);
		

		pActive->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

		// OK, now 3 points... pos, tar, up
		//     now 3 normals... upNorm, forwardNorm, rightNorm
		pos = pos - 2.0f * forwardNorm;
		tar = tar - 2.0f * forwardNorm;

		pActive->SetHelper(up, tar, pos);
	}

	if (pKey->GetKeyState(Keyboard::KEY_ARROW_RIGHT))
	{

		Vect pos;
		Vect tar;
		Vect up;
		Vect upNorm;
		Vect forwardNorm;
		Vect rightNorm;
		Vect obj(0.0f, 50.0f, 0.0f);
		

		pActive->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

		// OK, now 3 points... pos, tar, up
		//     now 3 normals... upNorm, forwardNorm, rightNorm
		tar = obj;

		Matrix Trans(Matrix::Trans::XYZ, tar);
		Matrix NegTrans(Matrix::Trans::XYZ, -tar);

		Matrix Rot;
		Rot.set(Matrix::Rot::AxisAngle, upNorm, +0.03f);

		Matrix M = NegTrans * Rot * Trans;

		up = up * M;
		pos = pos * M;
		tar = tar * M;

		pActive->SetHelper(up, tar, pos);
	}

	if (pKey->GetKeyState(Keyboard::KEY_ARROW_LEFT))
	{

		Vect pos;
		Vect tar;
		Vect up;
		Vect upNorm;
		Vect forwardNorm;
		Vect rightNorm;
		Vect obj(0.0f, 50.0f, 0.0f);


		pActive->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

		// OK, now 3 points... pos, tar, up
		//     now 3 normals... upNorm, forwardNorm, rightNorm

		tar = obj;

		Matrix Trans(Matrix::Trans::XYZ, tar);
		Matrix NegTrans(Matrix::Trans::XYZ, -tar);

		Matrix Rot;
		Rot.set(Matrix::Rot::AxisAngle, upNorm, -0.03f);

		Matrix M = NegTrans * Rot * Trans;

		up = up * M;
		pos = pos * M;
		tar = tar * M;

		pActive->SetHelper(up, tar, pos);
	}

	if (pKey->GetKeyState(Keyboard::KEY_ARROW_DOWN))
	{
		Vect pos;
		Vect tar;
		Vect up;
		Vect upNorm;
		Vect forwardNorm;
		Vect rightNorm;
		Vect obj(0.0f, 50.0f, 0.0f);

		pActive->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

		tar = obj;

		// OK, now 3 points... pos, tar, up
		//     now 3 normals... upNorm, forwardNorm, rightNorm

		Matrix Trans(Matrix::Trans::XYZ, tar);
		Matrix NegTrans(Matrix::Trans::XYZ, -tar);
		Matrix Rot;
		Rot.set(Matrix::Rot::AxisAngle, rightNorm, 0.03f);

		Matrix M = NegTrans * Rot * Trans;

		up = up * M;
		pos = pos * M;
		tar = tar * M;

		pActive->SetHelper(up, tar, pos);
	}

	if (pKey->GetKeyState(Keyboard::KEY_ARROW_UP))
	{
		Vect pos;
		Vect tar;
		Vect up;
		Vect upNorm;
		Vect forwardNorm;
		Vect rightNorm;
		Vect obj(0.0f, 50.0f, 0.0f);

		pActive->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

		// OK, now 3 points... pos, tar, up
		//     now 3 normals... upNorm, forwardNorm, rightNorm
		tar = obj;

		Matrix Trans(Matrix::Trans::XYZ, tar);
		Matrix NegTrans(Matrix::Trans::XYZ, -tar);
		Matrix Rot;
		Rot.set(Matrix::Rot::AxisAngle, rightNorm, -0.03f);

		Matrix M = NegTrans * Rot * Trans;

		up = up * M;
		pos = pos * M;
		tar = tar * M;

		pActive->SetHelper(up, tar, pos);
	}
}

void LoadModelsDemo()
{

	SphereModel* pSphereModel = new SphereModel("sphereModel.azul");
	assert(pSphereModel);
	ModelManager::Add(pSphereModel);


	//---------------------------------------------------------------------------------

	Model* SpaceShip = AssetLoader::LoadModel("../../Models/space_frigate.flcn");
	assert(SpaceShip);


	GraphicsObject* pGraphicsObj = new GraphicsObject_FlatTexture(SpaceShip, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::SPACE_SHIP);
	SimpleGameObject* pGameObj01 = new SimpleGameObject(pGraphicsObj);
	pGameObj01->SetPos(2.0f, 53.0f, 0.0f);
	pGameObj01->SetScale(.03f, .03f, .03f);

	GameObjectManager::Add(pGameObj01, GameObjectManager::GetRoot());

	//// ----- Bounding Sphere Model ------------

	Vect Yellow(1.0f, 1.0f, 0.0f, 1.0f);
	pGraphicsObj = new GraphicsObject_WireframeConstantColor(pSphereModel, ShaderManager::Find(ShaderObject::Name::COLOR_SINGLE), Yellow);
	BoundingSphereGameObject* pGameObjSphere;
	pGameObjSphere = new BoundingSphereGameObject(pGraphicsObj);
	pGameObjSphere->SetName("BoundingSphere_GREEN");

	pGameObjSphere->SetBoundingSphereReference(pGameObj01);
	GameObjectManager::Add(pGameObjSphere, GameObjectManager::GetRoot());

	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//Model* AstroBoy = AssetLoader::LoadModel("../../Models/astroboy.flcn");
	//assert(AstroBoy);

	//GraphicsObject* pGraphicsObj02 = new GraphicsObject_FlatTexture(AstroBoy, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::ASTROBOY);
	//SimpleGameObject* pGameObj02 = new SimpleGameObject(pGraphicsObj02);
	//pGameObj02->SetPos(10.0f, 53.0f, 0.0f);
	//pGameObj02->SetScale(.015f, .015f, .015f);
	//pGameObj02->poRot->set(-1.5f, 0.0f, 0.0f);

	//

	//GameObjectManager::Add(pGameObj02, GameObjectManager::GetRoot());

	//// ----- Bounding Sphere Model ------------

	
	/*pGraphicsObj02 = new GraphicsObject_WireframeConstantColor(pSphereModel, ShaderManager::Find(ShaderObject::Name::COLOR_SINGLE), Yellow);
	BoundingSphereGameObject* pGameObjSphere02;
	pGameObjSphere02 = new BoundingSphereGameObject(pGraphicsObj02);
	pGameObjSphere02->SetName("BoundingSphere_GREEN");

	pGameObjSphere02->SetBoundingSphereReference(pGameObj02);
	GameObjectManager::Add(pGameObjSphere02, GameObjectManager::GetRoot());*/

	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	Model* WarBear = AssetLoader::LoadModel("../../Models/warbear.flcn");
	assert(WarBear);

	pGraphicsObj_WARBEAR = new GraphicsObject_FlatTexture(WarBear, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::WARBEAR);
	SimpleGameObject* pGameObj03 = new SimpleGameObject(pGraphicsObj_WARBEAR);
	pGameObj03->SetPos(-5.5f, 52.5f, 0.0f);
	pGameObj03->SetScale(.015f, .015f, .015f);

	pGameObj03->poRot->set(-1.5f, 1.50f, 0.0f);

	GameObjectManager::Add(pGameObj03, GameObjectManager::GetRoot());

	//// ----- Bounding Sphere Model ------------


	GraphicsObject* pGraphicsObj03 = new GraphicsObject_WireframeConstantColor(pSphereModel, ShaderManager::Find(ShaderObject::Name::COLOR_SINGLE), Yellow);
	BoundingSphereGameObject* pGameObjSphere03;
	pGameObjSphere03 = new BoundingSphereGameObject(pGraphicsObj03);
	pGameObjSphere03->SetName("BoundingSphere_GREEN");

	pGameObjSphere03->SetBoundingSphereReference(pGameObj03);
	GameObjectManager::Add(pGameObjSphere03, GameObjectManager::GetRoot());

	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	Model* Buggy = AssetLoader::LoadModel("../../Models/buggy.flcn");
	assert(Buggy);

	GraphicsObject* pGraphicsObj04 = new GraphicsObject_FlatTexture(Buggy, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::BUGGY);
	SimpleGameObject* pGameObj04 = new SimpleGameObject(pGraphicsObj04);
	pGameObj04->SetPos(-10.0f, 53.0f, 0.0f);
	pGameObj04->SetScale(.006f, .006f, .006f);

	GameObjectManager::Add(pGameObj04, GameObjectManager::GetRoot());

	//// ----- Bounding Sphere Model ------------


	pGraphicsObj04 = new GraphicsObject_WireframeConstantColor(pSphereModel, ShaderManager::Find(ShaderObject::Name::COLOR_SINGLE), Yellow);
	BoundingSphereGameObject* pGameObjSphere04;
	pGameObjSphere04 = new BoundingSphereGameObject(pGraphicsObj04);
	pGameObjSphere04->SetName("BoundingSphere_GREEN");

	pGameObjSphere04->SetBoundingSphereReference(pGameObj04);
	GameObjectManager::Add(pGameObjSphere04, GameObjectManager::GetRoot());

	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	Model* Revolver = AssetLoader::LoadModel("../../Models/revolver.flcn");
	assert(Revolver);

	GraphicsObject* pGraphicsObj05 = new GraphicsObject_FlatTexture(Revolver, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::REVOLVER);
	SimpleGameObject* pGameObj05 = new SimpleGameObject(pGraphicsObj05);
	pGameObj05->SetPos(6.0f, 52.0f, 0.0f);
	pGameObj05->SetScale(.07f, .07f, .07f);

	GameObjectManager::Add(pGameObj05, GameObjectManager::GetRoot());

	//// ----- Bounding Sphere Model ------------


	pGraphicsObj05 = new GraphicsObject_WireframeConstantColor(pSphereModel, ShaderManager::Find(ShaderObject::Name::COLOR_SINGLE), Yellow);
	BoundingSphereGameObject* pGameObjSphere05;
	pGameObjSphere05 = new BoundingSphereGameObject(pGraphicsObj05);
	pGameObjSphere05->SetName("BoundingSphere_GREEN");

	pGameObjSphere05->SetBoundingSphereReference(pGameObj05);
	GameObjectManager::Add(pGameObjSphere05, GameObjectManager::GetRoot());

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	GraphicsObject* pGraphicsObj06 = new GraphicsObject_FlatTexture(Revolver, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::REVOLVER);
	SimpleGameObject* pGameObj06 = new SimpleGameObject(pGraphicsObj06);
	pGameObj06->SetPos(10.0f, 52.0f, 0.0f);
	pGameObj06->SetScale(.07f, .07f, .07f);

	GameObjectManager::Add(pGameObj06, GameObjectManager::GetRoot());

	//// ----- Bounding Sphere Model ------------


	pGraphicsObj06 = new GraphicsObject_WireframeConstantColor(pSphereModel, ShaderManager::Find(ShaderObject::Name::COLOR_SINGLE), Yellow);
	BoundingSphereGameObject* pGameObjSphere06;
	pGameObjSphere06 = new BoundingSphereGameObject(pGraphicsObj06);
	pGameObjSphere06->SetName("BoundingSphere_GREEN");

	pGameObjSphere06->SetBoundingSphereReference(pGameObj06);
	GameObjectManager::Add(pGameObjSphere06, GameObjectManager::GetRoot());

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	GraphicsObject* pGraphicsObj07 = new GraphicsObject_FlatTexture(SpaceShip, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE));
	SimpleGameObject* pGameObj07 = new SimpleGameObject(pGraphicsObj07);
	pGameObj07->SetPos(-2.0f, 53.0f, 0.0f);
	pGameObj07->SetScale(.03f, .03f, .03f);

	GameObjectManager::Add(pGameObj07, GameObjectManager::GetRoot());

	//// ----- Bounding Sphere Model ------------

	pGraphicsObj07 = new GraphicsObject_WireframeConstantColor(pSphereModel, ShaderManager::Find(ShaderObject::Name::COLOR_SINGLE), Yellow);
	BoundingSphereGameObject* pGameObjSphere07;
	pGameObjSphere07 = new BoundingSphereGameObject(pGraphicsObj07);
	pGameObjSphere07->SetName("BoundingSphere_GREEN");

	pGameObjSphere07->SetBoundingSphereReference(pGameObj07);
	GameObjectManager::Add(pGameObjSphere07, GameObjectManager::GetRoot());


}

void BoundingVolDemo()
{

	PyramidModel* pPyramidModel = new PyramidModel("PyramidModel.azul");
	assert(pPyramidModel);
	ModelManager::Add(pPyramidModel);

	SphereModel* pSphereModel = new SphereModel("sphereModel.azul");
	assert(pSphereModel);
	ModelManager::Add(pSphereModel);

	// Create GameObject - PYRAMID

	// Middle Row

	GraphicsObject* pGraphicsObj = new GraphicsObject_ColorByPosition(pPyramidModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	SimpleGameObject* pGameObj01 = new SimpleGameObject(pGraphicsObj);
	pGameObj01->SetPos(-2.0f, 0.0f, -2.0f);
	
	//pGameObj->SetDeltaRot(-0.07f);
	GameObjectManager::Add(pGameObj01, GameObjectManager::GetRoot());

	//// ----- Bounding Sphere Model ------------

	Vect Yellow(1.0f, 1.0f, 0.0f, 1.0f);
	pGraphicsObj = new GraphicsObject_WireframeConstantColor(pSphereModel, ShaderManager::Find(ShaderObject::Name::COLOR_SINGLE), Yellow);
	BoundingSphereGameObject* pGameObjSphere;
	pGameObjSphere = new BoundingSphereGameObject(pGraphicsObj);
	pGameObjSphere->SetName("BoundingSphere_GREEN");
	
	pGameObjSphere->SetBoundingSphereReference(pGameObj01);
	GameObjectManager::Add(pGameObjSphere, GameObjectManager::GetRoot());
}

void LoadPyramid()
{

	//PyramidModel* pPyramidModel = new PyramidModel("PyramidModel.azul");
	//assert(pPyramidModel);
	//ModelManager::Add(pPyramidModel);

	//// Create GameObject - PYRAMID

	//// Middle Row

	//GraphicsObject* pGraphicsObj = new GraphicsObject_ColorByPosition(pPyramidModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	//SimpleGameObject* pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(-2.0f, 0.0f, -2.0f);
	////pGameObj->SetDeltaRot(-0.07f);
	//GameObjectManager::Add(pGameObj);


	//pGraphicsObj = new GraphicsObject_Wireframe(pPyramidModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(0.0f, 0.0f, -2.0f);
	////pGameObj->SetDeltaRot(0.052f);
	//GameObjectManager::Add(pGameObj);

	//Vect color(1.50f, 1.50f, 1.50f, 1.0f);
	//Vect pos(1, 1, 1);

	//pGraphicsObj = new GraphicsObject_TextureLight(pPyramidModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_POINT_LIGHT), color, pos, TextureManager::Name::DUCKWEED);

	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(2.0f, 0.0f, -2.0f);
	////pGameObj->SetDeltaRot(-0.065f);
	//GameObjectManager::Add(pGameObj);

	//// Top Row

	//pGraphicsObj = new GraphicsObject_FlatTexture(pPyramidModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::RED_BRICK);
	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(-2.0f, 1.0f, -2.0f);
	////pGameObj->SetDeltaRot(0.1f);
	//GameObjectManager::Add(pGameObj);


	//pGraphicsObj = new GraphicsObject_FlatTexture(pPyramidModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::ROCKS);
	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(0.0f, 1.0f, -2.0f);
	////pGameObj->SetDeltaRot(-0.013f);
	//GameObjectManager::Add(pGameObj);


	//pGraphicsObj = new GraphicsObject_FlatTexture(pPyramidModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::STONES);
	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(2.0f, 1.0f, -2.0f);
	////pGameObj->SetDeltaRot(0.03f);
	//GameObjectManager::Add(pGameObj);

	//// bottom

	//color.set(1.25f, 1.25f, 1.25f, 1.0f);

	//pGraphicsObj = new GraphicsObject_ConstColor(pPyramidModel, ShaderManager::Find(ShaderObject::Name::CONST_COLOR), color, pos);


	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(-2.0f, -1.0f, -2.0f);
	////pGameObj->SetDeltaRot(0.002f);
	//GameObjectManager::Add(pGameObj);


	//pGraphicsObj = new GraphicsObject_ColorByPosition(pPyramidModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(0.0f, -1.0f, -2.0f);
	////pGameObj->SetDeltaRot(-0.007f);
	//pGameObj->SetScale(0.5f, 0.5f, 0.50f);
	//GameObjectManager::Add(pGameObj);


	//pGraphicsObj = new GraphicsObject_Wireframe(pPyramidModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(2.0f, -1.0f, -2.0f);
	////pGameObj->SetDeltaRot(0.082f);
	//pGameObj->SetScale(0.5f, 2.0f, 0.50f);
	//GameObjectManager::Add(pGameObj);

}

void LoadCube()
{

	//// Load the model
	//CubeModel* pCubeModel = new CubeModel("cubeModel.azul");
	//assert(pCubeModel);
	//ModelManager::Add(pCubeModel);

	//// Create GameObject - CUBE

	//// Middle Row
	//GraphicsObject*  pGraphicsObj = new GraphicsObject_ColorByPosition(pCubeModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	//SimpleGameObject* pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(-2.0f, 0.0f, 0.0f);
	////pGameObj->SetDeltaRot(-0.07f);
	//GameObjectManager::Add(pGameObj);

	//pGraphicsObj = new GraphicsObject_Wireframe(pCubeModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(0.0f, 0.0f, 0.0f);
	////pGameObj->SetDeltaRot(0.052f);
	//GameObjectManager::Add(pGameObj);

	//Vect color(1.50f, 1.50f, 1.50f, 1.0f);
	//Vect pos(1, 1, 1);
	//pGraphicsObj = new GraphicsObject_TextureLight(pCubeModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_POINT_LIGHT), color, pos, TextureManager::Name::DUCKWEED);

	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(2.0f, 0.0f, 0.0f);
	////pGameObj->SetDeltaRot(-0.065f);
	//GameObjectManager::Add(pGameObj);

	//// Top Row
	//pGraphicsObj = new GraphicsObject_FlatTexture(pCubeModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::RED_BRICK);
	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(-2.0f, 1.0f, 0.0f);
	////pGameObj->SetDeltaRot(0.1f);
	//GameObjectManager::Add(pGameObj);

	//pGraphicsObj = new GraphicsObject_FlatTexture(pCubeModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::ROCKS);
	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(0.0f, 1.0f, 0.0f);
	////pGameObj->SetDeltaRot(-0.013f);
	//GameObjectManager::Add(pGameObj);

	//pGraphicsObj = new GraphicsObject_FlatTexture(pCubeModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::STONES);
	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(2.0f, 1.0f, 0.0f);
	////pGameObj->SetDeltaRot(0.03f);
	//GameObjectManager::Add(pGameObj);

	//// bottom

	//color.set(1.25f, 1.25f, 1.25f, 1.0f);
	//pGraphicsObj = new GraphicsObject_ConstColor(pCubeModel, ShaderManager::Find(ShaderObject::Name::CONST_COLOR), color, pos);

	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(-2.0f, -1.0f, 0.0f);
	////pGameObj->SetDeltaRot(0.002f);
	//GameObjectManager::Add(pGameObj);

	//pGraphicsObj = new GraphicsObject_ColorByPosition(pCubeModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(0.0f, -1.0f, 0.0f);
	////pGameObj->SetDeltaRot(-0.007f);
	//pGameObj->SetScale(0.5f, 0.5f, 0.50f);
	//GameObjectManager::Add(pGameObj);

	//pGraphicsObj = new GraphicsObject_Wireframe(pCubeModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(2.0f, -1.0f, 0.0f);
	////pGameObj->SetDeltaRot(0.082f);
	//pGameObj->SetScale(0.5f, 2.0f, 0.50f);
	//GameObjectManager::Add(pGameObj);
}

void LoadSphere()
{

	//SphereModel* pSphereModel = new SphereModel("SphereModel.azul");
	//assert(pSphereModel);
	//ModelManager::Add(pSphereModel);

	//// Create GameObject - PYRAMID

	//// Middle Row

	//GraphicsObject* pGraphicsObj = new GraphicsObject_ColorByPosition(pSphereModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	//SimpleGameObject* pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(-2.0f, 0.0f, -4.0f);
	//pGameObj->SetScale(0.6f, 0.6f, 0.6f);
	////pGameObj->SetDeltaRot(-0.07f);
	//GameObjectManager::Add(pGameObj);


	//pGraphicsObj = new GraphicsObject_Wireframe(pSphereModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(0.0f, 0.0f, -4.0f);
	//pGameObj->SetScale(0.6f, 0.6f, 0.6f);
	////pGameObj->SetDeltaRot(0.052f);
	//GameObjectManager::Add(pGameObj);

	//Vect color(1.50f, 1.50f, 1.50f, 1.0f);
	//Vect pos(1, 1, 1);

	//pGraphicsObj = new GraphicsObject_TextureLight(pSphereModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_POINT_LIGHT), color, pos, TextureManager::Name::DUCKWEED);

	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(2.0f, 0.0f, -4.0f);
	//pGameObj->SetScale(0.6f, 0.6f, 0.6f);
	////pGameObj->SetDeltaRot(-0.065f);
	//GameObjectManager::Add(pGameObj);

	//// Top Row

	//pGraphicsObj = new GraphicsObject_FlatTexture(pSphereModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::RED_BRICK);
	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(-2.0f, 1.0f, -4.0f);
	//pGameObj->SetScale(0.6f, 0.6f, 0.6f);
	////pGameObj->SetDeltaRot(0.1f);
	//GameObjectManager::Add(pGameObj);


	//pGraphicsObj = new GraphicsObject_FlatTexture(pSphereModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::ROCKS);
	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(0.0f, 1.0f, -4.0f);
	//pGameObj->SetScale(0.6f, 0.6f, 0.6f);
	////pGameObj->SetDeltaRot(-0.013f);
	//GameObjectManager::Add(pGameObj);


	//pGraphicsObj = new GraphicsObject_FlatTexture(pSphereModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::STONES);
	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(2.0f, 1.0f, -4.0f);
	//pGameObj->SetScale(0.6f, 0.6f, 0.6f);
	////pGameObj->SetDeltaRot(0.03f);
	//GameObjectManager::Add(pGameObj);

	//// bottom

	//color.set(1.25f, 1.25f, 1.25f, 1.0f);

	//pGraphicsObj = new GraphicsObject_ConstColor(pSphereModel, ShaderManager::Find(ShaderObject::Name::CONST_COLOR), color, pos);


	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(-2.0f, -1.0f, -4.0f);
	//pGameObj->SetScale(0.6f, 0.6f, 0.6f);
	////pGameObj->SetDeltaRot(0.002f);
	//GameObjectManager::Add(pGameObj);


	//pGraphicsObj = new GraphicsObject_ColorByPosition(pSphereModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(0.0f, -1.0f, -4.0f);
	////pGameObj->SetDeltaRot(-0.007f);
	//pGameObj->SetScale(0.3f, 0.3f, 0.3f);
	//GameObjectManager::Add(pGameObj);


	//pGraphicsObj = new GraphicsObject_Wireframe(pSphereModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(2.0f, -1.0f, -4.0f);
	////pGameObj->SetDeltaRot(0.082f);
	//pGameObj->SetScale(0.3f, 1.0f, 0.25f);
	//GameObjectManager::Add(pGameObj);
}

void LoadDiamond()
{

	//DiamondModel* pDiamondModel = new DiamondModel("DiamondModel.azul");
	//assert(pDiamondModel);
	//ModelManager::Add(pDiamondModel);

	//// Create GameObject - PYRAMID

	//// Middle Row

	//GraphicsObject* pGraphicsObj = new GraphicsObject_ColorByPosition(pDiamondModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	//SimpleGameObject* pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(-2.0f, 0.0f, -6.0f);
	//pGameObj->SetScale(0.5f, 0.5f, 0.5f);
	////pGameObj->SetDeltaRot(-0.07f);
	//GameObjectManager::Add(pGameObj);

	//


	//pGraphicsObj = new GraphicsObject_Wireframe(pDiamondModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(0.0f, 0.0f, -6.0f);
	//pGameObj->SetScale(0.5f, 0.5f, 0.5f);
	////pGameObj->SetDeltaRot(0.052f);
	//GameObjectManager::Add(pGameObj);

	//Vect color(1.50f, 1.50f, 1.50f, 1.0f);
	//Vect pos(1, 1, 1);

	//pGraphicsObj = new GraphicsObject_TextureLight(pDiamondModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_POINT_LIGHT), color, pos, TextureManager::Name::DUCKWEED);

	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(2.0f, 0.0f, -6.0f);
	//pGameObj->SetScale(0.5f, 0.5f, 0.5f);
	////pGameObj->SetDeltaRot(-0.065f);
	//GameObjectManager::Add(pGameObj);

	//// Top Row

	//pGraphicsObj = new GraphicsObject_FlatTexture(pDiamondModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::RED_BRICK);
	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(-2.0f, 1.0f, -6.0f);
	//pGameObj->SetScale(0.5f, 0.5f, 0.5f);
	////pGameObj->SetDeltaRot(0.1f);
	//GameObjectManager::Add(pGameObj);


	//pGraphicsObj = new GraphicsObject_FlatTexture(pDiamondModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::ROCKS);
	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(0.0f, 1.0f, -6.0f);
	//pGameObj->SetScale(0.5f, 0.5f, 0.5f);
	////pGameObj->SetDeltaRot(-0.013f);
	//GameObjectManager::Add(pGameObj);


	//pGraphicsObj = new GraphicsObject_FlatTexture(pDiamondModel, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), TextureManager::Name::STONES);
	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(2.0f, 1.0f, -6.0f);
	//pGameObj->SetScale(0.5f, 0.5f, 0.5f);
	////pGameObj->SetDeltaRot(0.03f);
	//GameObjectManager::Add(pGameObj);

	//// bottom

	//color.set(1.25f, 1.25f, 1.25f, 1.0f);

	//pGraphicsObj = new GraphicsObject_ConstColor(pDiamondModel, ShaderManager::Find(ShaderObject::Name::CONST_COLOR), color, pos);


	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(-2.0f, -1.0f, -6.0f);
	//pGameObj->SetScale(0.5f, 0.5f, 0.5f);
	////pGameObj->SetDeltaRot(0.002f);
	//GameObjectManager::Add(pGameObj);


	//pGraphicsObj = new GraphicsObject_ColorByPosition(pDiamondModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(0.0f, -1.0f, -6.0f);
	////pGameObj->SetDeltaRot(-0.007f);
	//pGameObj->SetScale(0.25f, 0.25f, 0.250f);
	//GameObjectManager::Add(pGameObj);


	//pGraphicsObj = new GraphicsObject_Wireframe(pDiamondModel, ShaderManager::Find(ShaderObject::Name::COLOR_LIGHT));
	//pGameObj = new SimpleGameObject(pGraphicsObj);
	//pGameObj->SetPos(2.0f, -1.0f, -6.0f);
	////pGameObj->SetDeltaRot(0.082f);
	//pGameObj->SetScale(0.25f, 1.0f, 0.250f);
	//GameObjectManager::Add(pGameObj);
}
