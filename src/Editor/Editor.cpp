#include "Editor.h"
#include "GUI/GUIManager.h"

#include "GUI/ViewportWindow.h"
#include "GUI/StatsWindow.h"

//PerformanceTimer timer;

Editor::Editor(const char* windowName, const int Width, const int Height)
	:Game(windowName, Width, Height)
{
	assert(windowName);
}

Editor::~Editor()
{
}

void Editor::Initialize()
{
	Game::Initialize();
}

void Editor::LoadContent()
{
	Game::LoadContent();


	
	GUIManager::Create(this->window);

	GUIManager::Add(new ViewportWindow(GUIWindow::Name::VIEWPORT));
	GUIManager::Add(new StatsWindow(GUIWindow::Name::STATS));
}

void Editor::Update(float currentTime)
{
	
	Game::Update(currentTime);
	
	//timer.Tic();
	GUIManager::Update();
	//timer.Toc();
	

	//Trace::out("\n---Time: %f ms---\n", timer.TimeInSeconds() * 1000.0f);

	
}

void Editor::Draw()
{
	Game::Draw();

	//timer.Tic();
	GUIManager::Draw();
	//timer.Toc();

	//Trace::out("\n---Time: %f ms---\n", timer.TimeInSeconds() * 1000.0f);
}

void Editor::UnLoadContent()
{
	Game::UnLoadContent();

	GUIManager::Destroy();

}
