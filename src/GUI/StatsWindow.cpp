#include "StatsWindow.h"

StatsWindow::StatsWindow(GUIWindow::Name NameIn)
	:GUIWindow(NameIn)
{
}

StatsWindow::~StatsWindow()
{
}

void StatsWindow::Update()
{
	ImGui::Begin("Engine Stats");                          // Create a window called "Hello, world!" and append into it.
	ImGui::Text("FPS %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}
