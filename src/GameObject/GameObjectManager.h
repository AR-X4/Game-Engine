

#ifndef GAME_OBJECT_MAN_H
#define GAME_OBJECT_MAN_H

#include "GameObject.h"
#include "PCSTree.h"

namespace Azul
{

	// Singleton
	class GameObjectManager
	{
	public:
		static void Add(GameObject* pObj);
		static void Draw(void);
		static void Update(float currentTime);

		static void Destroy();
		~GameObjectManager();

	private:
		GameObjectManager();

		static GameObjectManager* privGetInstance();

		// data
		PCSTree* poRootTree;
	};

}

#endif


// --- End of File ---
