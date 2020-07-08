//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Game/Game.h"

Game* pGame = 0;

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	pGame = new Game("Graphics_Engine", 1280, 720);
	assert(pGame);

	pGame->Run();
	delete pGame;

	return 0;
}

// ---  End of File ----------
