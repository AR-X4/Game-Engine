#ifndef ANIM_CONTROLLER_MANAGER_H
#define ANIM_CONTROLLER_MANAGER_H

#include "AnimController.h"

namespace Azul {

	class AnimControllerManager
	{
	public:
		static void Add(AnimController* pController);
		static void Create();
		static void Destroy();
		static void Update();

	private:

		static AnimControllerManager* privGetInstance();
		AnimControllerManager();

		void privAddToFront(AnimController* pNode);
		void privRemove(AnimController* pNode);

	public:

		AnimController* poHead;
	};
}
#endif