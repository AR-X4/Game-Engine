

#include "Game/Game.h"
#include "Keyboard.h"

#include "Editor/Editor.h"

//extern Game* pGame;
extern Editor* pEditor;

namespace Azul
{

	// Use this to read keyboard
	bool Keyboard::GetKeyState(Keyboard::Key key)
	{
		bool value;


		if (pEditor->GetKey(key) == GLFW_PRESS)
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
