#ifndef SKELETON_H
#define SKELETON_H

#include "GameObject/GameObjectRigid.h"

namespace Azul
{
	class Skeleton
	{
	
	public:
		
		Skeleton();
		Skeleton(const Skeleton &) = delete;
		Skeleton &operator = (const Skeleton &) = delete;
		~Skeleton();

		void SetPos(float x, float y, float z);
		void SetRot(float x, float y, float z);
		void SetScale(float x, float y, float z);
		void SetName(const char* name);
		GameObjectRigid* GetRigidBody() const;

		
	private:
		void privSetAnimationHierarchy();
		
		GameObjectRigid* pRigidBody;
	
	};
}

#endif

// --- End of File ---
