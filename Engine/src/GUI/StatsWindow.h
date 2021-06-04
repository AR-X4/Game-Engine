#ifndef STATS_WINDOW_H
#define STATS_WINDOW_H

#include "GUIWindow.h"

class StatsWindow : public GUIWindow
{
public:
	StatsWindow() = delete;
	StatsWindow(const StatsWindow&) = delete;
	StatsWindow& operator = (StatsWindow&) = delete;

	StatsWindow(GUIWindow::Name NameIn);
	~StatsWindow();


	void Update();

};

#endif