//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Game/Game.h"
#include "Mouse.h"

extern Game* pGame;

namespace Azul
{

	Mouse::Mouse()
	{

	}

	// Use this to read mouse buttons
	bool Mouse::GetKeyState(Button button)
	{
		bool value;

		if (pGame->GetMouseButton(button) == GLFW_PRESS)
		{
			value = true;
		}
		else
		{
			value = false;
		}
		return value;
	}

	void Mouse::GetCursor(float& xPos, float& yPos)
	{
		// get mouse position
		pGame->GetCursorPos(xPos, yPos);

	}

}

// --- End of File ---