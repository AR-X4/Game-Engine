#include "sb7.h"
#include "Game.h"
#include "MathEngine.h"
#include "Demo/Demo.h"
#include "Shaders/ShaderObject.h"
#include "Textures/TextureManager.h"
#include "Input/InputManager.h"
//#include "Input/InputTest.h"
#include "Camera/Camera.h"
#include "Camera/CameraManager.h"
#include "Camera/CameraModel.h"
#include "GameObject/GameObjectManager.h"
#include "Models/ModelManager.h"
#include "Shaders/ShaderManager.h"


#include "GUI/GUIManager.h"

#include "GUI/ViewportWindow.h"
#include "GUI/StatsWindow.h"

#include "GraphicsObject/GraphicsObject_WireframeConstantColor.h"
#include "GameObject/SimpleGameObject.h"

#include "Animation/ClipManager.h"
#include "Animation/SkeletonManager.h"
#include "Animation/AnimControllerManager.h"


namespace Azul
{

	//PerformanceTimer timer;

	CameraModel* pCameraModel;

	//-----------------------------------------------------------------------------
	//  Game::Game()
	//		Game Engine Constructor
	//-----------------------------------------------------------------------------
	Game::Game(const char* const pWindowName, const int Width, const int Height)
		:Engine(pWindowName, Width, Height)
	{
		assert(pWindowName);
		this->globalTimer.Tic();
		this->intervalTimer.Tic();
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
		GameObjectManager::Create();
		SkeletonManager::Create();
		ClipManager::Create();
		AnimControllerManager::Create();

		// Camera 1
		Camera* pCam01 = CameraManager::Add(Camera::Name::Cam1);
		pCam01->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
		pCam01->setPerspective(35.0f, float(pCam01->getScreenWidth()) / float(pCam01->getScreenHeight()), 1.0f, 20000.0f);
		pCam01->setOrientAndPosition(Vect(0.0f, 1.0f, 0.0f), Vect(0.0f, 50.0f, 0.0f), Vect(0.0f, 50.0f, 25.0f));
		pCam01->updateCamera();
		CameraManager::SetActiveCamera(pCam01);



		// Camera 2
		//Camera* pCam02 = CameraManager::Add(Camera::Name::Cam2);
		//pCam02->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
		////pCam02->setPerspective(1.0f, float(pCam02->getScreenWidth()) / float(pCam02->getScreenHeight()), 1.0f, 1000.0f);
		////pCam02->setOrientAndPosition(Vect(0.0f, 1.0f, 0.0f), Vect(0.0f, 0.0f, 0.0f), Vect(0.0f, 0.0f, -700.0f));

		//pCam02->setPerspective(35.0f, float(pCam02->getScreenWidth()) / float(pCam02->getScreenHeight()), 1.0f, 20000.0f);
		//pCam02->setOrientAndPosition(Vect(0.0f, 0.0f, 1.0f), Vect(0.0f, 0.0f, 0.0f), Vect(2000.0f, 0.0f, 0.0f));

		//pCam02->updateCamera();
		//CameraManager::SetActiveCamera(pCam02);


		// Create/Load Shader 
		ShaderManager::Add(ShaderObject::Name::COLOR_LIGHT, "Shaders/colorRender");
		ShaderManager::Add(ShaderObject::Name::TEXTURE_SIMPLE, "Shaders/textureRender");
		ShaderManager::Add(ShaderObject::Name::TEXTURE_POINT_LIGHT, "Shaders/texturePointLightDiff");
		ShaderManager::Add(ShaderObject::Name::CONST_COLOR, "Shaders/constcolorRender");
		ShaderManager::Add(ShaderObject::Name::COLOR_SINGLE, "Shaders/colorSingleRender");
		ShaderManager::Add(ShaderObject::Name::NULL_SHADER, "Shaders/nullRender");


		// Textures
		TextureManager::Add("Assets/HotPink.tga", TextureManager::Name::DEFAULT);
		TextureManager::Add("Assets/Rocks.tga", TextureManager::Name::ROCKS);
		TextureManager::Add("Assets/Stone.tga", TextureManager::Name::STONES);
		TextureManager::Add("Assets/RedBrick.tga", TextureManager::Name::RED_BRICK);
		TextureManager::Add("Assets/Duckweed.tga", TextureManager::Name::DUCKWEED);
		TextureManager::Add("Assets/polarbear.tga", TextureManager::Name::POLARBEAR);


		//Camera model
		/*pCameraModel = new CameraModel("cameraModel.azul");
		assert(pCameraModel);
		ModelManager::Add(pCameraModel);

		Vect White(1.0f, 1.0f, 1.0f, 1.0f);
		GraphicsObject_WireframeConstantColor* pGraphics_Wireframe = new GraphicsObject_WireframeConstantColor(pCameraModel, ShaderManager::Find(ShaderObject::Name::COLOR_SINGLE), White);
		SimpleGameObject* pGameObj = new SimpleGameObject(pGraphics_Wireframe);
		pGameObj->SetName("Camera");
		GameObjectManager::Add(pGameObj, GameObjectManager::GetRoot());*/

		//Load Demo
		
		LoadModelsDemo();
		LoadAnimDemo();



		//GUI stuff
		GUIManager::Create(this->window);

		GUIManager::Add(new ViewportWindow(GUIWindow::Name::VIEWPORT));
		GUIManager::Add(new StatsWindow(GUIWindow::Name::STATS));
	}


	//-----------------------------------------------------------------------------
	// Game::Update()
	//      Called once per frame, update data, tranformations, etc
	//      Use this function to control process order
	//      Input, AI, Physics, Animation, and Graphics
	//-----------------------------------------------------------------------------



	void Game::Update(float)
	{

		UpdateModelDemo();

		//pCameraModel->update();

		// Needs to be updated once a frame
		CameraManager::Update();

		AnimControllerManager::Update();

		// Update the game objects
		GameObjectManager::Update(this->globalTimer.Toc());

		
		

		GUIManager::Update();
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

		GUIManager::Draw();
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
		SkeletonManager::Destroy();
		ClipManager::Destroy();
		AnimControllerManager::Destroy();


		GUIManager::Destroy();
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
}

// --- End of File ---
