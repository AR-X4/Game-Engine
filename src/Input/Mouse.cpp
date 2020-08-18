
#include "Game/Game.h"
#include "Mouse.h"

#include "Editor/Editor.h"

//extern Game* pGame;
extern Editor* pEditor;

namespace Azul
{

	Mouse::Mouse()
	{

	}

	// Use this to read mouse buttons
	bool Mouse::GetKeyState(Button button)
	{
		bool value;

		if (pEditor->GetMouseButton(button) == GLFW_PRESS)
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
		pEditor->GetCursorPos(xPos, yPos);

	}

}

// --- End of File ---