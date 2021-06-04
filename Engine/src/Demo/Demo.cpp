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
#include "GraphicsObject/GraphicsObject_SkinTextLight.h"
#include "GraphicsObject/GraphicsObject_SkinColor.h"
#include "GraphicsObject/GraphicsObject_SkinTexture.h"


#include "GameObject/GameObjectManager.h"
#include "Models/ModelManager.h"
#include "Shaders/ShaderManager.h"

#include "Input/InputManager.h"
#include "AssetLoader/AssetLoader.h"
#include "Camera/CameraManager.h"

#include "Animation/AnimControllerManager.h"
#include "Animation/ClipManager.h"

#include "Image/ImageManager.h"
#include "Models/Sprite.h"
#include "GraphicsObject/GraphicsObject_Sprite.h"
#include "GameObject/GameObject2D.h"
#include "Models/NullModel.h"
#include "GraphicsObject/GraphicsObject_Null.h"
#include "GameObject/GameObjectTextBox.h"

#include "Models/FBXSkinnedModel.h"
#include "Animation/AnimTransition.h"

using namespace Azul;

static bool keypress = false;
static bool keypress2 = false;
static bool pause = false;

AnimController* controller1;
AnimController* controller2;
AnimTransition* pTrans;

GameObjectTextBox* textBox;
GameObjectTextBox* textBox2;
GameObjectTextBox* textBox3;

const char* str1 = "GAME";
const char* str2 = "ENGINE";
const char* tmp = str1;
static int i = 0;
static bool swap = false;

void LoadCompressDemo()
{

	//bone model
	ModelManager::Add(new PyramidModel("pyramidModel.azul"));

	Vect color(1.50f, 1.50f, 1.50f, 1.0f);
	Vect pos(0, 10, -5);

	//---------------------------------------------------------------------------------------------------------------------------------

	FBXSkinnedModel* pSkinModel0 = AssetLoader::LoadAnimModel("../../../Models/humanoid2.flcn");

	AssetLoader::LoadAnim("../../../Models/humanCOMP.flcn");


	GraphicsObject_SkinColor* pSkinGraphicsObject0 = new GraphicsObject_SkinColor(pSkinModel0, ShaderManager::Find(ShaderObject::Name::SKIN_CONST_COLOR), color, pos);

	GameObjectRigid* pSkinObject0 = new GameObjectRigid(pSkinGraphicsObject0);
	GameObjectManager::Add(pSkinObject0, GameObjectManager::GetRoot());
	pSkinObject0->SetName("Compressed Humanoid Skin");

	AnimTime delta2 = 0.2f * AnimTime(AnimTime::Duration::NTSC_30_FRAME);

	controller2 = new AnimController(delta2, false, pSkinModel0, pSkinGraphicsObject0->pBoneWord_SBO, Clip::Name::COMPRESSED_RUN);

	controller2->SetPos(-3.0f, 48.0f, -5.0f);
	controller2->SetScale(0.005f, 0.005f, 0.005f);
	controller2->SetRot(-1.5f, 0.0f, 0.0f);
	controller2->SetName("Compressed Humanoid Skel");

	controller2->Play();

	AnimControllerManager::Add(controller2);

	//---------------------------------------------------------------------------------------------------------------------------------

	GraphicsObject_SkinColor* pSkinGraphicsObject2 = new GraphicsObject_SkinColor(pSkinModel0, ShaderManager::Find(ShaderObject::Name::SKIN_CONST_COLOR), color, pos);

	GameObjectRigid* pSkinObject = new GameObjectRigid(pSkinGraphicsObject2);
	GameObjectManager::Add(pSkinObject, GameObjectManager::GetRoot());
	pSkinObject->SetName("Human Skin");


	AnimTime delta4 = 0.2f * AnimTime(AnimTime::Duration::NTSC_30_FRAME);

	controller1 = new AnimController(delta4, false, pSkinModel0, pSkinGraphicsObject2->pBoneWord_SBO, Clip::Name::HUMANOID);

	controller1->SetPos(3.0f, 48.0f, -5.0f);
	controller1->SetScale(0.005f, 0.005f, 0.005f);
	controller1->SetRot(-1.5f, 0.0f, 0.0f);
	controller1->SetName("Humanoid Skel");

	controller1->Play();

	AnimControllerManager::Add(controller1);
}

// TODO create anim trans manager
void DestroyAnimTransDemo()
{
	delete pTrans;
}

void UpdateAnimTransDemo()
{
	Keyboard* pKey = InputManager::GetKeyboard();

	bool keyTpress = pKey->GetKeyState(Keyboard::KEY_T);
	if (keyTpress == true && keypress == false)
	{
		pTrans->StartTransition();
	}
	keypress = keyTpress;


	pTrans->Update();
}

void LoadAnimTransDemo()
{
	//bone model
	ModelManager::Add(new PyramidModel("pyramidModel.azul"));

	Vect color(1.50f, 1.50f, 1.50f, 1.0f);
	Vect pos(0, 10, -5);

	//---------------------------------------------------------------------------------------------------------------------------------

	FBXSkinnedModel* pSkinModel0 = AssetLoader::LoadAnimModel("../../../Models/humanoid2.flcn");
	AssetLoader::LoadAnim("../../../Models/walk.flcn");


	GraphicsObject_SkinColor* pSkinGraphicsObject0 = new GraphicsObject_SkinColor(pSkinModel0, ShaderManager::Find(ShaderObject::Name::SKIN_CONST_COLOR), color, pos);

	GameObjectRigid* pSkinObject0 = new GameObjectRigid(pSkinGraphicsObject0);
	GameObjectManager::Add(pSkinObject0, GameObjectManager::GetRoot());
	pSkinObject0->SetName("Humanoid Skin Rigid");

	AnimTime delta = 0.2f * AnimTime(AnimTime::Duration::NTSC_30_FRAME);

	controller1 = new AnimController(delta, true, pSkinModel0, pSkinGraphicsObject0->pBoneWord_SBO, Clip::Name::WALK);

	controller1->SetPos(-3.0f, 46.0f, -5.0f);
	controller1->SetScale(0.005f, 0.005f, 0.005f);
	controller1->SetRot(-1.5f, -1.5f, 0.0f);
	controller1->SetName("Humanoid Skel");

	controller1->Play();

	AnimControllerManager::Add(controller1);

	//------------------------------------------------------------------------------------------------------------------------


	GraphicsObject_SkinColor* pSkinGraphicsObject1 = new GraphicsObject_SkinColor(pSkinModel0, ShaderManager::Find(ShaderObject::Name::SKIN_CONST_COLOR), color, pos);

	GameObjectRigid* pSkinObject1 = new GameObjectRigid(pSkinGraphicsObject1);
	GameObjectManager::Add(pSkinObject1, GameObjectManager::GetRoot());
	pSkinObject1->SetName("Humanoid Skin Rigid2");

	AnimTime delta2 = 0.2f * AnimTime(AnimTime::Duration::NTSC_30_FRAME);

	controller2 = new AnimController(delta2, true, pSkinModel0, pSkinGraphicsObject1->pBoneWord_SBO, Clip::Name::HUMANOID);

	controller2->SetPos(-3.0f, 46.0f, -5.0f);
	controller2->SetScale(0.005f, 0.005f, 0.005f);
	controller2->SetRot(-1.5f, 1.5f, 0.0f);
	controller2->SetName("Humanoid Skel2");

	controller2->Play();

	AnimControllerManager::Add(controller2);


	//---------------------------------------------------------------------------------------------------------------------------

	pTrans = new AnimTransition(controller1->GetAnim(), controller2->GetAnim());



}

void UpdateTextBoxDemo()
{
	textBox->UpdateMessage(tmp);
	textBox2->UpdateMessage("TEDDY");
	textBox3->UpdateMessage("HUMANOID");
	
	if (i > 100) {
		
		if (swap) {
			tmp = str2;
			swap = false;
		}
		else {
			tmp = str1;
			swap = true;
		}

		i = 0;
	}

	i++;
}

void LoadTextBoxDemo()
{
	NullModel* pNullModel = new NullModel(0);
	ModelManager::Add(pNullModel);

	GraphicsObject_Null* pGraphicsObjNull = new GraphicsObject_Null(pNullModel, ShaderManager::Find(ShaderObject::Name::NULL_SHADER));

	textBox = new GameObjectTextBox(pGraphicsObjNull, Glyph::FontType::Consolas36pt);
	textBox->SetName("TextBox1");
	GameObjectManager::Add(textBox, GameObjectManager::GetRoot());

	//textBox->x = 60;
	//textBox->y = 1000;
	textBox->SetPos(60.f, 1000.f, 0.0f);

	//----------------------------------------------------

	GraphicsObject_Null* pGraphicsObjNull2 = new GraphicsObject_Null(pNullModel, ShaderManager::Find(ShaderObject::Name::NULL_SHADER));

	textBox2 = new GameObjectTextBox(pGraphicsObjNull2, Glyph::FontType::Consolas36pt);
	textBox2->SetName("TextBox2");
	GameObjectManager::Add(textBox2, GameObjectManager::GetRoot());

	//textBox2->x = 1100;
	//textBox2->y = 200;
	textBox2->SetPos(1100.f, 200.f, 0.0f);

	//--------------------------------------------------------

	GraphicsObject_Null* pGraphicsObjNull3 = new GraphicsObject_Null(pNullModel, ShaderManager::Find(ShaderObject::Name::NULL_SHADER));

	textBox3 = new GameObjectTextBox(pGraphicsObjNull3, Glyph::FontType::Consolas36pt);
	textBox3->SetName("TextBox3");
	GameObjectManager::Add(textBox3, GameObjectManager::GetRoot());

	//textBox3->x = 600;
	//textBox3->y = 200;
	textBox3->SetPos(600.f, 200.f, 0.0f);
}

void LoadSpriteDemo()
{
	// Image
	ImageManager::Add(Image::Name::Alien_Green, Texture::Name::ALIENS_2D, Rect(136.0f, 64.0f, 85.0f, 63.0f));

	// Sprite
	Sprite* pSpriteModel = new Sprite(Model::Name::ALIEN);
	assert(pSpriteModel);
	ModelManager::Add(pSpriteModel);

	GraphicsObject_Sprite* pGraphics_Sprite = new GraphicsObject_Sprite(pSpriteModel, ShaderManager::Find(ShaderObject::Name::SPRITE), ImageManager::Find(Image::Name::Alien_Green), Rect(455, 155, 150, 150));
	GameObject2D* pA1 = new GameObject2D(pGraphics_Sprite);
	GameObjectManager::Add(pA1, GameObjectManager::GetRoot());
	//pA1->posX = 150.0f;
	//pA1->posY = 250.0f;
	pA1->SetPos(150.f, 250.f, 0.0f);

	pA1->SetName("ALIEN Sprite");
}

void LoadAnimDemo()
{
	//bone model
	ModelManager::Add(new PyramidModel("pyramidModel.azul"));

	Vect color(1.50f, 1.50f, 1.50f, 1.0f);
	Vect pos(0, 10, -5);

	//---------------------------------------------------------------------------------------------------------------------------------

	FBXSkinnedModel* pSkinModel0 = AssetLoader::LoadAnimModel("../../../Models/humanoid2.flcn");


	GraphicsObject_SkinColor* pSkinGraphicsObject0 = new GraphicsObject_SkinColor(pSkinModel0, ShaderManager::Find(ShaderObject::Name::SKIN_CONST_COLOR), color, pos );

	GameObjectRigid* pSkinObject0 = new GameObjectRigid(pSkinGraphicsObject0);
	GameObjectManager::Add(pSkinObject0, GameObjectManager::GetRoot());
	pSkinObject0->SetName("Humanoid Skin Rigid");

	AnimTime delta2 = 0.2f * AnimTime(AnimTime::Duration::NTSC_30_FRAME);

	controller2 = new AnimController(delta2, false, pSkinModel0, pSkinGraphicsObject0->pBoneWord_SBO, Clip::Name::HUMANOID);

	controller2->SetPos(-3.0f, 46.0f, -5.0f);
	controller2->SetScale(0.005f, 0.005f, 0.005f);
	controller2->SetRot(-1.5f, 0.0f, 0.0f);
	controller2->SetName("Humanoid Skel");

	controller2->Play();

	AnimControllerManager::Add(controller2);

	//------------------------------------------------------------------------------------------------------------------------

	//AssetLoader::LoadAnim("../../Models/teddy.flcn");
	FBXSkinnedModel* pSkinModel2 = AssetLoader::LoadAnimModel("../../../Models/teddySKIN2.flcn");

	GraphicsObject_SkinTexture* pSkinGraphicsObject = new GraphicsObject_SkinTexture(pSkinModel2, ShaderManager::Find(ShaderObject::Name::SKIN_TEXTURE), Texture::Name::TEDDY);

	GameObjectRigid* pSkinObject = new GameObjectRigid(pSkinGraphicsObject);
	GameObjectManager::Add(pSkinObject, GameObjectManager::GetRoot());
	pSkinObject->SetName("Teddy Skin Rigid");


	AnimTime delta4 = 0.3f * AnimTime(AnimTime::Duration::NTSC_30_FRAME);

	controller1 = new AnimController(delta4, false, pSkinModel2, pSkinGraphicsObject->pBoneWord_SBO, Clip::Name::TEDDY);
	
	controller1->SetPos(3.0f, 46.0f, 0.0f);
	controller1->SetRot(0.0f, 0.0f, 0.0f);
	controller1->SetScale(0.015f, 0.015f, 0.015f);
	controller1->SetName("Teddy Skel");


	controller1->Play();

	AnimControllerManager::Add(controller1);


}

void UpdateAnimDemo()
{
}

void UpdateModelDemo()
{
	Keyboard* pKey = InputManager::GetKeyboard();
	

	Camera* pActive = CameraManager::GetActiveCamera(Camera::Type::PERSPECTIVE_3D);

	bool keyPpress = pKey->GetKeyState(Keyboard::KEY_P);
	if (keyPpress == true && keypress2 == false)
	{
		if (pause == true) {
			
			controller1->Play();
			controller2->Play();

			pause = false;
		}
		else {
			
			controller1->Pause();
			controller2->Pause();
		
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
	Vect color(1.50f, 1.50f, 1.50f, 1.0f);
	Vect pos(0, 10, -5);

	SphereModel* pSphereModel = new SphereModel("sphereModel.azul");
	assert(pSphereModel);
	ModelManager::Add(pSphereModel);


	//---------------------------------------------------------------------------------

	Model* SpaceShip = AssetLoader::LoadModel("../../../Models/space_frigate.flcn");
	assert(SpaceShip);


	GraphicsObject* pGraphicsObj = new GraphicsObject_TextureLight(SpaceShip, ShaderManager::Find(ShaderObject::Name::TEXTURE_POINT_LIGHT), color, pos, Texture::Name::SPACE_SHIP);
	SimpleGameObject* pGameObj01 = new SimpleGameObject(pGraphicsObj);
	pGameObj01->SetName("Space Frigate");
	pGameObj01->SetPos(2.0f, 53.0f, 0.0f);
	pGameObj01->SetScale(.03f, .03f, .03f);

	GameObjectManager::Add(pGameObj01, GameObjectManager::GetRoot());

	//// ----- Bounding Sphere Model ------------

	Vect Yellow(1.0f, 1.0f, 0.0f, 1.0f);
	pGraphicsObj = new GraphicsObject_WireframeConstantColor(pSphereModel, ShaderManager::Find(ShaderObject::Name::COLOR_SINGLE), Yellow);
	BoundingSphereGameObject* pGameObjSphere;
	pGameObjSphere = new BoundingSphereGameObject(pGraphicsObj);
	pGameObjSphere->SetName("Bounding Sphere Space Frigate");

	pGameObjSphere->SetBoundingSphereReference(pGameObj01);
	GameObjectManager::Add(pGameObjSphere, pGameObj01);

	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	
	Model* WarBear = AssetLoader::LoadModel("../../../Models/warbear.flcn");
	assert(WarBear);

	GraphicsObject* spGraphicsObj_WARBEAR = new GraphicsObject_FlatTexture(WarBear, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), Texture::Name::WARBEAR);
	SimpleGameObject* pGameObj03 = new SimpleGameObject(spGraphicsObj_WARBEAR);
	pGameObj03->SetPos(-5.5f, 52.5f, 0.0f);
	pGameObj03->SetScale(.015f, .015f, .015f);

	pGameObj03->SetRot(-1.5f, 1.50f, 0.0f);
	pGameObj03->SetName("Warbear");

	GameObjectManager::Add(pGameObj03, GameObjectManager::GetRoot());

	//// ----- Bounding Sphere Model ------------


	GraphicsObject* pGraphicsObj03 = new GraphicsObject_WireframeConstantColor(pSphereModel, ShaderManager::Find(ShaderObject::Name::COLOR_SINGLE), Yellow);
	BoundingSphereGameObject* pGameObjSphere03;
	pGameObjSphere03 = new BoundingSphereGameObject(pGraphicsObj03);
	pGameObjSphere03->SetName("Bounding Sphere Warbear");

	pGameObjSphere03->SetBoundingSphereReference(pGameObj03);
	GameObjectManager::Add(pGameObjSphere03, pGameObj03);

	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	Model* Buggy = AssetLoader::LoadModel("../../../Models/buggy.flcn");
	assert(Buggy);

	GraphicsObject* pGraphicsObj04 = new GraphicsObject_FlatTexture(Buggy, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), Texture::Name::BUGGY);
	SimpleGameObject* pGameObj04 = new SimpleGameObject(pGraphicsObj04);
	pGameObj04->SetPos(-10.0f, 53.0f, 0.0f);
	pGameObj04->SetScale(.006f, .006f, .006f);
	pGameObj04->SetName("Buggy");

	GameObjectManager::Add(pGameObj04, GameObjectManager::GetRoot());

	//// ----- Bounding Sphere Model ------------


	pGraphicsObj04 = new GraphicsObject_WireframeConstantColor(pSphereModel, ShaderManager::Find(ShaderObject::Name::COLOR_SINGLE), Yellow);
	BoundingSphereGameObject* pGameObjSphere04;
	pGameObjSphere04 = new BoundingSphereGameObject(pGraphicsObj04);
	pGameObjSphere04->SetName("Bounding Sphere Buggy");

	pGameObjSphere04->SetBoundingSphereReference(pGameObj04);
	GameObjectManager::Add(pGameObjSphere04, pGameObj04);

	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	Model* Revolver = AssetLoader::LoadModel("../../../Models/revolver.flcn");
	assert(Revolver);

	GraphicsObject* pGraphicsObj05 = new GraphicsObject_FlatTexture(Revolver, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), Texture::Name::REVOLVER);
	SimpleGameObject* pGameObj05 = new SimpleGameObject(pGraphicsObj05);
	pGameObj05->SetPos(6.0f, 52.0f, 0.0f);
	pGameObj05->SetScale(.07f, .07f, .07f);
	pGameObj05->SetName("Revolver 1");

	GameObjectManager::Add(pGameObj05, GameObjectManager::GetRoot());

	//// ----- Bounding Sphere Model ------------


	pGraphicsObj05 = new GraphicsObject_WireframeConstantColor(pSphereModel, ShaderManager::Find(ShaderObject::Name::COLOR_SINGLE), Yellow);
	BoundingSphereGameObject* pGameObjSphere05;
	pGameObjSphere05 = new BoundingSphereGameObject(pGraphicsObj05);
	pGameObjSphere05->SetName("Bounding Sphere Revolver 1");

	pGameObjSphere05->SetBoundingSphereReference(pGameObj05);
	GameObjectManager::Add(pGameObjSphere05, pGameObj05);

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	GraphicsObject* pGraphicsObj06 = new GraphicsObject_FlatTexture(Revolver, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE), Texture::Name::REVOLVER);
	SimpleGameObject* pGameObj06 = new SimpleGameObject(pGraphicsObj06);
	pGameObj06->SetPos(10.0f, 52.0f, 0.0f);
	pGameObj06->SetScale(.07f, .07f, .07f);
	pGameObj06->SetName("Revolver 2");

	GameObjectManager::Add(pGameObj06, GameObjectManager::GetRoot());

	//// ----- Bounding Sphere Model ------------


	pGraphicsObj06 = new GraphicsObject_WireframeConstantColor(pSphereModel, ShaderManager::Find(ShaderObject::Name::COLOR_SINGLE), Yellow);
	BoundingSphereGameObject* pGameObjSphere06;
	pGameObjSphere06 = new BoundingSphereGameObject(pGraphicsObj06);
	pGameObjSphere06->SetName("Bounding Sphere Revolver 2");

	pGameObjSphere06->SetBoundingSphereReference(pGameObj06);
	GameObjectManager::Add(pGameObjSphere06, pGameObj06);

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	GraphicsObject* pGraphicsObj07 = new GraphicsObject_FlatTexture(SpaceShip, ShaderManager::Find(ShaderObject::Name::TEXTURE_SIMPLE));
	SimpleGameObject* pGameObj07 = new SimpleGameObject(pGraphicsObj07);
	pGameObj07->SetPos(-2.0f, 53.0f, 0.0f);
	pGameObj07->SetScale(.03f, .03f, .03f);
	pGameObj07->SetName("Space Frigate Null");

	GameObjectManager::Add(pGameObj07, GameObjectManager::GetRoot());

	//// ----- Bounding Sphere Model ------------

	pGraphicsObj07 = new GraphicsObject_WireframeConstantColor(pSphereModel, ShaderManager::Find(ShaderObject::Name::COLOR_SINGLE), Yellow);
	BoundingSphereGameObject* pGameObjSphere07;
	pGameObjSphere07 = new BoundingSphereGameObject(pGraphicsObj07);
	pGameObjSphere07->SetName("Bounding Sphere Space Frigate Null");

	pGameObjSphere07->SetBoundingSphereReference(pGameObj07);
	GameObjectManager::Add(pGameObjSphere07, pGameObj07);


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
