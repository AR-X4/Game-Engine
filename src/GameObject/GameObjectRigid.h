#ifndef GAME_OBJECT_RIGID_H
#define GAME_OBJECT_RIGID_H

#include "GameObject.h"
#include "MathEngine.h"
#include "GraphicsObject/GraphicsObject.h"

namespace Azul
{
	
	class GameObjectRigid : public GameObject
	{
	public:
		GameObjectRigid(GraphicsObject *graphicsObject);

		// Big four
		GameObjectRigid() = delete;
		GameObjectRigid(const GameObjectRigid &) = delete;
		GameObjectRigid &operator=(GameObjectRigid &) = delete;
		virtual ~GameObjectRigid() override;

		virtual void Update(Time currentTime);

		void SetPos(float x, float y, float z);
		void SetRot(float x, float y, float z);
		void SetScale(float x, float y, float z);

	private:
		void privUpdate(Time currentTime);

	public:
		Vect *pScale;
		Vect *pPos;
		Vect* pRot;
		float angle;

	};
}

#endif

// --- End of File ---
