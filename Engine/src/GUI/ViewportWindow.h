#ifndef VIEWPORT_WINDOW_H
#define VIEWPORT_WINDOW_H

#include "GUIWindow.h"

#include "GL/gl3w.h"

class ViewportWindow : public GUIWindow
{

public:
	ViewportWindow() = delete;
	ViewportWindow(const ViewportWindow&) = delete;
	ViewportWindow& operator = (ViewportWindow&) = delete;

	ViewportWindow(GUIWindow::Name NameIn);
	~ViewportWindow();


	void Update();

private:

	//data
	uint32_t RenderID = 0;
	uint32_t ColorAttachment = 0;
	uint32_t DepthAttachment = 0;


};

#endif