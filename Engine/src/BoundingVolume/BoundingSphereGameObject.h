#ifndef BOUNDING_SPHERE_GAME_OBJECT_H
#define BOUNDING_SPHERE_GAME_OBJECT_H

#include "MathEngine.h"
#include "GameObject/GameObject.h"


namespace Azul
{
	class BoundingSphereGameObject : public GameObject
	{
	public:
		BoundingSphereGameObject(GraphicsObject *graphicsObject);

		// Big four
		BoundingSphereGameObject() = delete;
		BoundingSphereGameObject(const BoundingSphereGameObject &) = delete;
		BoundingSphereGameObject &operator=(BoundingSphereGameObject &) = delete;
		virtual ~BoundingSphereGameObject();

		void SetBoundingSphereReference(GameObject *pGameObj);

		virtual void Update(AnimTime t) override;

	private:
		void privUpdate(AnimTime t);
		//void privUpdateBoundingSphereReference(Time currentTime);

		// Data: ------------------
		GameObject *pRefGameObj;

	public:
		Vect *poScale;
		Vect *poTrans;
	};

}

#endif

// --- End of File ---
