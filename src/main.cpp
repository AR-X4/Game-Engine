#include "Game/GameManager.h"


using namespace Azul;


int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	
	GameManager::Create("Graphics_Engine", 1920, 1080);
	Game* pGame = GameManager::GetGame();
	pGame->Run();

	GameManager::Destroy();

	return 0;
}

// ---  End of File ----------
