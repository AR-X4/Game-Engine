//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_H
#define GAME_H

#include "sb7.h"
#include "Engine/Engine.h"

class Game : public Engine
{
public:
	// constructor
	Game(const char* windowName, const int Width, const int Height);
	~Game();

private:
	virtual void Initialize() override;
	virtual void LoadContent() override;
	virtual void Update(float currentTime) override;
	virtual void Draw() override;
	virtual void UnLoadContent() override;
	virtual void ClearBufferFunc() override;

	//virtual void onResize(int w, int h) override;

public:

	// Nice and Clean

};

#endif

// --- End of File ---

