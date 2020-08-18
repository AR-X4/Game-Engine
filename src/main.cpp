//#include "Game/Game.h"
#include "Editor/Editor.h"

//Game* pGame = 0;
Editor* pEditor = 0;

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	/*pGame = new Game("Graphics_Engine", 1920, 1080);
	assert(pGame);

	pGame->Run();
	delete pGame;*/

	pEditor = new Editor("Graphics_Engine", 1920, 1080);
	assert(pEditor);

	pEditor->Run();
	delete pEditor;

	return 0;
}

// ---  End of File ----------
