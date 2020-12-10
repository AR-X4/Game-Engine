#ifndef SIMPLE_GAME_OBJECT_H
#define SIMPLE_GAME_OBJECT_H

#include "GameObject.h"
#include "MathEngine.h"

namespace Azul
{
	class SimpleGameObject : public GameObject
	{
	public:
		enum MoveDir
		{
			Move_X,
			Move_Y,
			Move_Z,
			Move_None
		};

	public:
		SimpleGameObject(GraphicsObject *graphicsObject);

		// Big four
		SimpleGameObject() = delete;
		SimpleGameObject(const SimpleGameObject &) = delete;
		SimpleGameObject &operator=(SimpleGameObject &) = delete;
		virtual ~SimpleGameObject();

		virtual void Update(Time t);

		void SetScale(float sx, float sy, float sz);
		void SetPos(float x, float y, float z);

		//void setMove(MoveDir move);

	private:
		void privUpdate(Time t);

	public:   // add accessors later
		Vect* poScale;
		Vect* poPos;
		Vect* poRot;

		Vect* poDof;
		Vect* poUp;

		Matrix* poParent;
		Matrix* poLocal;
		
		//MoveDir moveDir;

	};

}

#endif

// --- End of File ---
