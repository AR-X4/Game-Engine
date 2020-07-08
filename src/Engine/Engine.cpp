//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Engine.h"

Engine* Engine::app = 0;

Engine::Engine(const char* pWindowName, const int Width, const int Height)
{
	this->window = nullptr;

	assert(pWindowName);
	strcpy_s(info.title, 128, pWindowName);
	info.windowWidth = Width;
	info.windowHeight = Height;

	info.majorVersion = 4;
	info.minorVersion = 3;

	info.samples = 0;
	info.flags.all = 0;
	info.flags.cursor = 1;

#ifdef _DEBUG
	info.flags.debug = 1;
#endif

}

Engine::~Engine()
{
}

//------------------------------------------------------------------
// Engine::Run()
//		This is the internal game loop that the engine runs on.
//------------------------------------------------------------------
void Engine::Run()
{
	this->InitGLFW();

	this->Initialize(); // virtual, calls init from derived game class

	this->CreateOpenGLWindow();// creates window, 

	this->InitGlew();

	this->setVsync(true);

	this->LoadContent(); // virtual, calls method from derived game class

	while ((glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) && (glfwWindowShouldClose(window) != GL_TRUE))
	{
		// move objects
		this->Update((float)glfwGetTime());

		// clear screen
		ClearBufferFunc();

		// render objects
		this->Draw();  // Virtual, calls method from derived game class

		glfwSwapBuffers(window);
		// Get and Handle user input events... should be at top?
		glfwPollEvents();
	}

	this->UnLoadContent(); // virtual, calls method from derived game class

	glfwDestroyWindow(window);
	glfwTerminate();
}

void Engine::InitGLFW()
{
	Engine::app = this;

	//init the GLFW library
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		glfwTerminate();
		return;
	}
}

void Engine::CreateOpenGLWindow()
{
	//WindowHint is for setting the attributes of the window prior to creation

	//Set the OpenGL Version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, info.majorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, info.minorVersion);

#ifndef _DEBUG
	if (info.flags.debug)
#endif /* _DEBUG */
	{
		//Set if in debug mode
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	}
	if (info.flags.robust)
	{
		glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_LOSE_CONTEXT_ON_RESET);
	}
	//Set No Backwards OpenGL Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Set Forward Compatability to true
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//Set MultiSampling
	glfwWindowHint(GLFW_SAMPLES, info.samples);
	//Set Stereoscopic Rendering
	glfwWindowHint(GLFW_STEREO, info.flags.stereo ? GL_TRUE : GL_FALSE);
	//        if (info.flags.fullscreen)
	//        {
	//            if (info.windowWidth == 0 || info.windowHeight == 0)
	//            {
	//                GLFWvidmode mode;
	//                glfwGetDesktopMode(&mode);
	//                info.windowWidth = mode.Width;
	//                info.windowHeight = mode.Height;
	//            }
	//
	//            glfwOpenWindow(info.windowWidth, info.windowHeight, 8, 8, 8, 0, 32, 0, GLFW_FULLSCREEN);
	//            glfwSwapInterval((int)info.flags.vsync);
	//        }
	//        else
	{
		//Create Window
		this->window = glfwCreateWindow(info.windowWidth, info.windowHeight, info.title, info.flags.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
		if (!this->window)
		{
			fprintf(stderr, "Failed to open window\n");
			return;
		}
	}
	//Set window as current and associate
	glfwMakeContextCurrent(this->window);
	glfwSetWindowSizeCallback(this->window, glfw_onResize);
	glfwSetKeyCallback(this->window, glfw_onKey);
	glfwSetMouseButtonCallback(this->window, glfw_onMouseButton);
	glfwSetCursorPosCallback(this->window, glfw_onMouseMove);
	glfwSetScrollCallback(this->window, glfw_onMouseWheel);
	if (!info.flags.cursor)
	{
		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	// info.flags.stereo = (glfwGetWindowParam(GLFW_STEREO) ? 1 : 0);

	
}

void Engine::InitGlew()
{
	//Init GLEW
	if (gl3wInit() == -1) {
		printf("GLEW Init Failed");
		glfwDestroyWindow(this->window);
		glfwTerminate();
		return;
	}


	////Get Buffer Size Information
	//int bufferWidth;
	//int bufferHeight;
	//glfwGetFramebufferSize(this->window, &bufferWidth, &bufferHeight);

	////Setup Viewport size
	//glViewport(0, 0, bufferWidth, bufferHeight);


#ifdef _DEBUG
	fprintf(stderr, "VENDOR: %s\n", (char*)glGetString(GL_VENDOR));
	fprintf(stderr, "VERSION: %s\n", (char*)glGetString(GL_VERSION));
	fprintf(stderr, "RENDERER: %s\n", (char*)glGetString(GL_RENDERER));
#endif

	if (info.flags.debug)
	{
		if (gl3wIsSupported(info.majorVersion, info.minorVersion))
		{
			glDebugMessageCallback((GLDEBUGPROC)debug_callback, this);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		}
		else if (sb6IsExtensionSupported("GL_ARB_debug_output"))
		{
			glDebugMessageCallbackARB((GLDEBUGPROC)debug_callback, this);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		}
	}

	// set state
	glEnable(GL_CULL_FACE);//enable face culling
	glFrontFace(GL_CW);// clockwise polys are front facing

	glEnable(GL_DEPTH_TEST);// enables depth comparisons and updates depth buffer
	glDepthFunc(GL_LEQUAL);// compares if incoming depth value is <= stored depth value when updating
}

void Engine::Initialize()
{
	strcpy_s(info.title, 128, "Game Engine");
	info.windowWidth = 800;
	info.windowHeight = 600;
#ifdef __APPLE__
	info.majorVersion = 3;
	info.minorVersion = 2;
#else
	info.majorVersion = 4;
	info.minorVersion = 3;
#endif
	info.samples = 0;
	info.flags.all = 0;
	info.flags.cursor = 1;
#ifdef _DEBUG
	info.flags.debug = 1;
#endif
}

void Engine::LoadContent()
{
}

void Engine::Draw()
{
}

void Engine::UnLoadContent()
{
}

//------------------------------------------------------------------
// Engine::ClearBufferFunc()
// Allows user to change the way the clear buffer function works
//------------------------------------------------------------------
void Engine::ClearBufferFunc()
{
	const GLfloat grey[] = { 0.250f, 0.25f, 0.25f, 1.0f };
	const GLfloat one = 1.0f;

	glViewport(0, 0, info.windowWidth, info.windowHeight);
	glClearBufferfv(GL_COLOR, 0, grey);
	glClearBufferfv(GL_DEPTH, 0, &one);
}

void APIENTRY Engine::debug_callback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	GLvoid* userParam)
{
	reinterpret_cast<Engine*>(userParam)->onDebugMessage(source, type, id, severity, length, message);
}

void Engine::setWindowTitle(const char* title)
{
	glfwSetWindowTitle(window, title);
}

void Engine::onDebugMessage(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message)
{
	AZUL_UNUSED_VAR(length);
	AZUL_UNUSED_VAR(severity);
	AZUL_UNUSED_VAR(id);
	AZUL_UNUSED_VAR(type);
	AZUL_UNUSED_VAR(source);

#ifdef _WIN32
	OutputDebugStringA(message);
	OutputDebugStringA("\n");
#endif /* _WIN32 */
}

void Engine::getMousePosition(int& x, int& y)
{
	double dx, dy;
	glfwGetCursorPos(window, &dx, &dy);

	x = static_cast<int>(floor(dx));
	y = static_cast<int>(floor(dy));
}

void Engine::onResize(int w, int h)
{
	info.windowWidth = w;
	info.windowHeight = h;
}

int Engine::GetKey(int key)
{
	return glfwGetKey(window, key);
}

int Engine::GetMouseButton(int button)
{
	return glfwGetMouseButton(window, button);
}

void Engine::GetCursorPos(float& xPos, float& yPos)
{
	double xPosDouble;
	double yPosDouble;

	glfwGetCursorPos(window, &xPosDouble, &yPosDouble);

	xPos = (float)xPosDouble;
	yPos = (float)yPosDouble;
}

void Engine::onKey(int key, int action)
{
	AZUL_UNUSED_VAR(key);
	AZUL_UNUSED_VAR(action);
}

void Engine::onMouseButton(int button, int action)
{
	AZUL_UNUSED_VAR(button);
	AZUL_UNUSED_VAR(action);
}

void Engine::onMouseMove(int x, int y)
{
	AZUL_UNUSED_VAR(x);
	AZUL_UNUSED_VAR(y);
}

void Engine::onMouseWheel(int pos)
{
	AZUL_UNUSED_VAR(pos);
}

void Engine::glfw_onResize(GLFWwindow* window, int w, int h)
{
	AZUL_UNUSED_VAR(window);

	app->onResize(w, h);
}

void Engine::glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	AZUL_UNUSED_VAR(mods);
	AZUL_UNUSED_VAR(scancode);
	AZUL_UNUSED_VAR(window);

	app->onKey(key, action);
}

void Engine::glfw_onMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	AZUL_UNUSED_VAR(mods);
	AZUL_UNUSED_VAR(window);

	app->onMouseButton(button, action);
}

void Engine::glfw_onMouseMove(GLFWwindow* window, double x, double y)
{
	AZUL_UNUSED_VAR(window);

	app->onMouseMove(static_cast<int>(x), static_cast<int>(y));
}

void Engine::glfw_onMouseWheel(GLFWwindow* window, double xoffset, double yoffset)
{
	AZUL_UNUSED_VAR(window);
	AZUL_UNUSED_VAR(xoffset);

	app->onMouseWheel(static_cast<int>(yoffset));
}

void Engine::setVsync(bool enable)
{
	info.flags.vsync = enable ? 1u : 0u;
	glfwSwapInterval((int)info.flags.vsync);
}

// --- End of File ---
