

#include "Game/GameManager.h"
#include "Keyboard.h"


namespace Azul
{

	// Use this to read keyboard
	bool Keyboard::GetKeyState(Keyboard::Key key)
	{
		bool value;

		if (GameManager::GetGame()->GetKey(key) == GLFW_PRESS)
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
