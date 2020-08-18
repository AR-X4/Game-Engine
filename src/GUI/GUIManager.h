#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include"Manager/DLink.h"
#include "GUIWindow.h"

#include "GL/gl3w.h"
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class GUIManager
{
private:  // methods

	GUIManager();
	GUIManager(const GUIManager&) = delete;
	GUIManager& operator = (const GUIManager&) = delete;
	~GUIManager();

	static GUIManager* GetInstance();

	void InitImGui(GLFWwindow* window);
	void DestroyImGui();
	void UpdateDockSpace();


public:
	static void Add(GUIWindow* pGuiWindow);
	static void Create(GLFWwindow* window);
	static void Destroy();
	static GUIWindow* Find(GUIWindow::Name NameIn);
	static void Update();
	static void Draw();

private:

	//DATA
	static GUIManager* poInstance;
	
	DLink* poHead;


};

#endif