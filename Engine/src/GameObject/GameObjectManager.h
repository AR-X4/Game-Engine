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
		static void Add(GameObject* pObj, GameObject* pParent);
		static void Draw(void);
		static void Update(AnimTime currentTime);
		static GameObject* GetRoot(void);
		static PCSTree* GetPCSTree();

		static void Create();
		static void Destroy();
		~GameObjectManager();

	private:
		GameObjectManager();

		static GameObjectManager* privGetInstance();

		// data
		static GameObjectManager* poGameObjectManager;
		PCSTree* poRootTree;
	};

}

#endif


// --- End of File ---
