#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include "Manager/DLink.h"
#include "imgui.h"

class GUIWindow : public DLink
{

public:
	enum class Name
	{
		UNINITIALIZED,
		VIEWPORT,
		SCENE_GRAPH,
		STATS
	};



	GUIWindow() = delete;
	GUIWindow(const GUIWindow&) = delete;
	GUIWindow& operator = (GUIWindow&) = delete;

	GUIWindow(GUIWindow::Name WindowName);
	virtual ~GUIWindow() = 0;


	virtual void Update() = 0;

	

private:

	//Data
	Name WindowName;

};

#endif