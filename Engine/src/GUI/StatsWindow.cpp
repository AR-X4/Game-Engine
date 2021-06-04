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
	ImGui::Begin("Engine Stats");                          
	ImGui::Text("GPU Time: %.3f ms (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}
