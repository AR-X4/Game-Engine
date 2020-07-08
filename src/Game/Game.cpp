
#include "sb7.h"
#include "Game.h"
#include "MathEngine.h"
#include "Demo/Demo.h"
#include "Shaders/ShaderObject.h"
#include "Textures/TextureManager.h"
#include "Input/InputManager.h"
#include "Input/InputTest.h"
#include "Camera/Camera.h"
#include "Camera/CameraManager.h"
#include "GameObject/GameObjectManager.h"
#include "Models/ModelManager.h"
#include "Shaders/ShaderManager.h"

using namespace Azul;

// TO DO: ----------------------------------
//Camera* pCam = nullptr;

// Demo
Vect Orig_up;
Vect Orig_tar;
Vect Orig_pos;
Vect Orig_upNorm;
Vect Orig_forwardNorm;
Vect Orig_RightNorm;

Vect obj(0.0f, 0.0f, 0.0f);

//-----------------------------------------------------------------------------
//  Game::Game()
//		Game Engine Constructor
//-----------------------------------------------------------------------------
Game::Game(const char* const pWindowName, const int Width, const int Height)
	:Engine(pWindowName, Width, Height)
{
	assert(pWindowName);
}

//-----------------------------------------------------------------------------
//  Game::~Game()
//		Game Engine destructor
//-----------------------------------------------------------------------------
Game::~Game()
{
}

//-----------------------------------------------------------------------------
// Game::Initialize()
//		Allows the engine to perform any initialization it needs to before 
//      starting to run.  This is where it can query for any required services 
//      and load any non-graphic related content. 
//-----------------------------------------------------------------------------
void Game::Initialize()
{

}


//-----------------------------------------------------------------------------
// Game::LoadContent()
//		Allows you to load all content needed for your engine,
//	    such as objects, graphics, etc.
//-----------------------------------------------------------------------------
void Game::LoadContent()
{
	// Start the managers
	InputManager::Create();
	CameraManager::Create();
	ModelManager::Create();
	ShaderManager::Create();
	TextureManager::Create();

	// Camera setup
	Camera* pCam = CameraManager::Add(Camera::Name::Cam1);
	pCam->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
	pCam->setPerspective(50.0f, float(pCam->getScreenWidth()) / float(pCam->getScreenHeight()), 0.10f, 1000.0f);
	pCam->setOrientAndPosition(Vect(0.0f, 1.0f, 0.0f), Vect(0.0f, 0.0f, 0.0f), Vect(0.0f, 0.0f, 4.5f));
	pCam->updateCamera();
	CameraManager::SetActiveCamera(pCam);
	

	// Create/Load Shader 
	ShaderManager::Add(ShaderObject::Name::COLOR_LIGHT, "Shaders/colorRender");
	ShaderManager::Add(ShaderObject::Name::TEXTURE_SIMPLE, "Shaders/textureRender");
	ShaderManager::Add(ShaderObject::Name::TEXTURE_POINT_LIGHT, "Shaders/texturePointLightDiff");
	ShaderManager::Add(ShaderObject::Name::CONST_COLOR, "Shaders/constcolorRender");

	// Textures
	TextureManager::Add("Assets/Rocks.tga", TextureManager::Name::ROCKS);
	TextureManager::Add("Assets/Stone.tga", TextureManager::Name::STONES);
	TextureManager::Add("Assets/RedBrick.tga", TextureManager::Name::RED_BRICK);
	TextureManager::Add("Assets/Duckweed.tga", TextureManager::Name::DUCKWEED);

	//Load Models
	LoadPyramid();
	LoadCube();
	LoadSphere();
	LoadDiamond();


	// squirrel for reset
	pCam->updateCamera();
	pCam->GetHelper(Orig_up, Orig_tar, Orig_pos, Orig_upNorm, Orig_forwardNorm, Orig_RightNorm);

	obj.set(0, 0, -3);
	pCam->SetHelper(Orig_up, obj, Orig_pos);

	Trace::out("Z, I,O, \n");
}


//-----------------------------------------------------------------------------
// Game::Update()
//      Called once per frame, update data, tranformations, etc
//      Use this function to control process order
//      Input, AI, Physics, Animation, and Graphics
//-----------------------------------------------------------------------------



void Game::Update(float currentTime)
{
	Keyboard* pKey = InputManager::GetKeyboard();

	Camera* pCam = CameraManager::Find(Camera::Name::Cam1);

	if (pKey->GetKeyState(Keyboard::KEY_Z))
	{
		// Reset	
		obj.set(0, 0, -3);
		pCam->SetHelper(Orig_up, Orig_tar, Orig_pos);
	}

	if (pKey->GetKeyState(Keyboard::KEY_I))
	{

		Vect pos;
		Vect tar;
		Vect up;
		Vect upNorm;
		Vect forwardNorm;
		Vect rightNorm;

		pCam->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

		// OK, now 3 points... pos, tar, up
		//     now 3 normals... upNorm, forwardNorm, rightNorm
		//	tar = obj;

		pos = pos + 0.03f * forwardNorm;
		tar = tar + 0.03f * forwardNorm;


		pCam->SetHelper(up, tar, pos);
	}

	if (pKey->GetKeyState(Keyboard::KEY_O))
	{

		Vect pos;
		Vect tar;
		Vect up;
		Vect upNorm;
		Vect forwardNorm;
		Vect rightNorm;

		pCam->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

		// OK, now 3 points... pos, tar, up
		//     now 3 normals... upNorm, forwardNorm, rightNorm
		tar = obj;

		pos = pos - 0.03f * forwardNorm;

		pCam->SetHelper(up, tar, pos);
	}

	if (pKey->GetKeyState(Keyboard::KEY_ARROW_RIGHT))
	{

		Vect pos;
		Vect tar;
		Vect up;
		Vect upNorm;
		Vect forwardNorm;
		Vect rightNorm;

		pCam->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

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

		pCam->SetHelper(up, tar, pos);
	}

	if (pKey->GetKeyState(Keyboard::KEY_ARROW_LEFT))
	{

		Vect pos;
		Vect tar;
		Vect up;
		Vect upNorm;
		Vect forwardNorm;
		Vect rightNorm;

		pCam->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

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

		pCam->SetHelper(up, tar, pos);
	}

	if (pKey->GetKeyState(Keyboard::KEY_ARROW_DOWN))
	{
		Vect pos;
		Vect tar;
		Vect up;
		Vect upNorm;
		Vect forwardNorm;
		Vect rightNorm;

		pCam->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

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

		pCam->SetHelper(up, tar, pos);
	}

	if (pKey->GetKeyState(Keyboard::KEY_ARROW_UP))
	{
		Vect pos;
		Vect tar;
		Vect up;
		Vect upNorm;
		Vect forwardNorm;
		Vect rightNorm;

		pCam->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

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

		pCam->SetHelper(up, tar, pos);
	}


	// Needs to be updated once a frame
	CameraManager::Update();

	// Update the game objects
	GameObjectManager::Update(currentTime);
}


//-----------------------------------------------------------------------------
// Game::Draw()
//		This function is called once per frame
//	    Use this for draw graphics to the screen.
//      Only do rendering here
//-----------------------------------------------------------------------------
void Game::Draw()
{
	GameObjectManager::Draw();
}


//-----------------------------------------------------------------------------
// Game::UnLoadContent()
//       unload content (resources loaded above)
//       unload all content that was loaded before the Engine Loop started
//-----------------------------------------------------------------------------
void Game::UnLoadContent()
{
	// Todo fix this - verify OpenGL removal

	//  glDeleteVertexArrays(1, &vao);
	//  glDeleteProgram(pShaderObject->programObject);
	//  glDeleteBuffers(1, &buffer);

	TextureManager::Destroy();
	InputManager::Destroy();
	CameraManager::Destroy();
	GameObjectManager::Destroy();
	ModelManager::Destroy();
	ShaderManager::Destroy();

}

//------------------------------------------------------------------
// Game::ClearBufferFunc()
// Allows user to change the way the clear buffer function works
//------------------------------------------------------------------
void Game::ClearBufferFunc()
{
	const GLfloat grey[] = { 0.7f, 0.7f, 0.8f, 1.0f };
	const GLfloat one = 1.0f;

	glViewport(0, 0, info.windowWidth, info.windowHeight);
	glClearBufferfv(GL_COLOR, 0, grey);
	glClearBufferfv(GL_DEPTH, 0, &one);
}


// --- End of File ---
