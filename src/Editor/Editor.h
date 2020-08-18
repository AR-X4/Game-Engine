#ifndef EDITOR_H
#define EDITOR_H

#include "Game/Game.h"

class Editor : public Game
{
public:

	Editor(const char* windowName, const int Width, const int Height);
	Editor(const Editor&) = delete;
	Editor& operator = (const Editor&) = delete;
	~Editor();

private:

	virtual void Initialize() override;
	virtual void LoadContent() override;
	virtual void Update(float currentTime) override;
	virtual void Draw() override;
	virtual void UnLoadContent() override;


};

#endif