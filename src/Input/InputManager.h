#ifndef INPUT_MAN_H
#define INPUT_MAN_H

#include "Keyboard.h"
#include "Mouse.h"

namespace Azul
{
	class InputManager
	{
	public:
		InputManager(const InputManager&) = delete;
		InputManager& operator = (const InputManager&) = delete;
		~InputManager();

		static void Create();
		static void Destroy();

		static Keyboard* GetKeyboard();
		static Mouse* GetMouse();

	private:
		InputManager();

		static InputManager* privInstance();
		static void privCreate();

		// Data
		static InputManager* poInputMan;
		Keyboard* poKeyboard;
		Mouse* poMouse;

	};

}

#endif

// --- End of File ---
