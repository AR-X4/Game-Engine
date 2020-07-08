//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Game/Game.h"
#include "Keyboard.h"

extern Game* pGame;

namespace Azul
{

	// Use this to read keyboard
	bool Keyboard::GetKeyState(Keyboard::Key key)
	{
		bool value;


		if (pGame->GetKey(key) == GLFW_PRESS)
		{
			value = true;
		}
		else
		{
			value = false;
		}

		return value;
	}
}
// --- End of File ---
