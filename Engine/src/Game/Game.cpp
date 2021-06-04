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
#include "GUI/SceneGraphWindow.h"

#include "GraphicsObject/GraphicsObject_WireframeConstantColor.h"
#include "GameObject/SimpleGameObject.h"

#include "Animation/ClipManager.h"

#include "Animation/AnimControllerManager.h"

#include "Image/ImageManager.h"
#include "Glyph/GlyphManager.h"


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
		//SkeletonManager::Create();
		ClipManager::Create();
		AnimControllerManager::Create();
		ImageManager::Create();
		GlyphManager::Create();

		// 3D Camera 1
		Camera* pCam3D = CameraManager::Add(Camera::Name::Cam1, Camera::Type::PERSPECTIVE_3D);
		pCam3D->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
		pCam3D->setPerspective(35.0f, float(pCam3D->getScreenWidth()) / float(pCam3D->getScreenHeight()), 1.0f, 20000.0f);
		pCam3D->setOrientAndPosition(Vect(0.0f, 1.0f, 0.0f), Vect(0.0f, 50.0f, 0.0f), Vect(0.0f, 50.0f, 25.0f));
		pCam3D->updateCamera();
		CameraManager::SetActiveCamera(Camera::Type::PERSPECTIVE_3D, pCam3D);

		// 2D Camera 1
		Camera* pCam2D = CameraManager::Add(Camera::Name::Cam2, Camera::Type::ORTHOGRAPHIC_2D);
		pCam2D->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
		pCam2D->setOrthographic((float)-pCam2D->getScreenWidth() / 2.0f, (float)pCam2D->getScreenWidth() / 2.0f,
			(float)-pCam2D->getScreenHeight() / 2.0f, (float)pCam2D->getScreenHeight() / 2.0f,
			1.0f, 1000.0f);
		pCam2D->setOrientAndPosition(Vect(0.0f, 1.0f, 0.0f), Vect(0.0f, 0.0f, -1.0f), Vect(0.0f, 0.0f, 2.0f));

		CameraManager::SetActiveCamera(Camera::Type::ORTHOGRAPHIC_2D, pCam2D);


		// Create/Load Shader 
		ShaderManager::Add(ShaderObject::Name::COLOR_LIGHT, "Shaders/colorRender", ShaderObject::Type::GRAPHICS);
		ShaderManager::Add(ShaderObject::Name::TEXTURE_SIMPLE, "Shaders/textureRender", ShaderObject::Type::GRAPHICS);
		ShaderManager::Add(ShaderObject::Name::TEXTURE_POINT_LIGHT, "Shaders/texturePointLightDiff", ShaderObject::Type::GRAPHICS);
		ShaderManager::Add(ShaderObject::Name::CONST_COLOR, "Shaders/constcolorRender", ShaderObject::Type::GRAPHICS);
		ShaderManager::Add(ShaderObject::Name::COLOR_SINGLE, "Shaders/colorSingleRender", ShaderObject::Type::GRAPHICS);
		ShaderManager::Add(ShaderObject::Name::NULL_SHADER, "Shaders/nullRender", ShaderObject::Type::GRAPHICS);
		ShaderManager::Add(ShaderObject::Name::SPRITE, "Shaders/spriteRender", ShaderObject::Type::GRAPHICS);
		ShaderManager::Add(ShaderObject::Name::SKIN_TEXTURE, "Shaders/skinTextureRender", ShaderObject::Type::GRAPHICS);
		ShaderManager::Add(ShaderObject::Name::SKIN_LIGHT_TEXTURE, "Shaders/skinTextLightRender", ShaderObject::Type::GRAPHICS);
		ShaderManager::Add(ShaderObject::Name::SKIN_CONST_COLOR, "Shaders/skinColorRender", ShaderObject::Type::GRAPHICS);

		ShaderManager::Add(ShaderObject::Name::COMPUTE_WORLD, "Shaders/computeWorld", ShaderObject::Type::COMPUTE);
		ShaderManager::Add(ShaderObject::Name::COMPUTE_MIXER, "Shaders/computeMixer", ShaderObject::Type::COMPUTE);

		// Textures
		TextureManager::Add("Assets/HotPink.tga", Texture::Name::DEFAULT);
		TextureManager::Add("Assets/Rocks.tga", Texture::Name::ROCKS);
		TextureManager::Add("Assets/Stone.tga", Texture::Name::STONES);
		TextureManager::Add("Assets/RedBrick.tga", Texture::Name::RED_BRICK);
		TextureManager::Add("Assets/Duckweed.tga", Texture::Name::DUCKWEED);
		TextureManager::Add("Assets/polarbear.tga", Texture::Name::POLARBEAR);
		//TextureManager::Add("Assets/SpaceInvaders.tga", Texture::Name::SPACEINVADERS_2D);
		TextureManager::Add("Assets/Aliens.tga", Texture::Name::ALIENS_2D);
		TextureManager::Add("Assets/Consolas36pt.tga", Texture::Name::CONSOLAS36_FONT);
		TextureManager::Add("Assets/TeddyTexNORLE.tga", Texture::Name::TEDDY);

		// glyph
		GlyphManager::ReadXML(Glyph::FontType::Consolas36pt, "Assets/Consolas36pt.xml", Texture::Name::CONSOLAS36_FONT);


		//Load Demo
		//LoadCompressDemo();
		//LoadAnimTransDemo();
		LoadTextBoxDemo();
		LoadSpriteDemo();
		LoadModelsDemo();
		LoadAnimDemo();


		//GUI stuff
		GUIManager::Create(this->window);

		GUIManager::Add(new ViewportWindow(GUIWindow::Name::VIEWPORT));
		GUIManager::Add(new StatsWindow(GUIWindow::Name::STATS));
		GUIManager::Add(new SceneGraphWindow(GUIWindow::Name::SCENE_GRAPH));
	}


	//-----------------------------------------------------------------------------
	// Game::Update()
	//      Called once per frame, update data, tranformations, etc
	//      Use this function to control process order
	//      Input, AI, Physics, Animation, and Graphics
	//-----------------------------------------------------------------------------



	void Game::Update(float)
	{
		UpdateTextBoxDemo();
		UpdateModelDemo();
		//UpdateAnimTransDemo();

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
		//DestroyAnimTransDemo();

		TextureManager::Destroy();
		InputManager::Destroy();
		CameraManager::Destroy();
		GameObjectManager::Destroy();
		ModelManager::Destroy();
		ShaderManager::Destroy();
	
		ClipManager::Destroy();
		AnimControllerManager::Destroy();
		ImageManager::Destroy();
		GlyphManager::Destroy();


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
