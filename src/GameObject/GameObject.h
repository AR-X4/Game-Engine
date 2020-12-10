
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "MathEngine.h"
#include "GraphicsObject/GraphicsObject.h"
#include "PCSNode.h"
#include "Time/Time.h"

namespace Azul
{

	class GameObject : public PCSNode
	{
	public:
		GameObject(GraphicsObject* graphicsObject);

		// Big four
		GameObject() = delete;
		GameObject(const GameObject&) = delete;
		GameObject& operator=(GameObject&) = delete;
		virtual ~GameObject();

		virtual void Draw();
		virtual void Update(Time currentTime) = 0;

		GraphicsObject* GetGraphicsObject();
		Matrix* GetWorld();
		void SetWorld(Matrix* _pWorld);
		bool GetDrawEnable();
		void SetDrawEnable(bool);

		

	protected:
		Matrix* poWorld;
		//void baseUpdateBoundingSphere();

	protected:
		//Sphere* pSphere;
		GraphicsObject* poGraphicsObject;
		bool mDrawEnable;
		char pad[3];
		
	};
}

#endif



