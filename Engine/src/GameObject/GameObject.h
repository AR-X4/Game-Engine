
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "MathEngine.h"
#include "GraphicsObject/GraphicsObject.h"
#include "PCSNode.h"
#include "Time/AnimTime.h"

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
		virtual void Update(AnimTime currentTime) = 0;

		GraphicsObject* GetGraphicsObject();
		Matrix* GetWorld();
		//void SetWorld(Matrix* _pWorld);
		bool GetDrawEnable();
		void SetDrawEnable(bool);

		void SetPos(float x, float y, float z);
		void SetRot(float x, float y, float z);
		void SetScale(float x, float y, float z);

		Vect* GetPos() const;
		Vect* GetRot() const;
		Vect* GetScale() const;

	protected:
		Matrix* poWorld;

		Vect* pScale;
		Vect* pPos;
		Vect* pRot;
		//void baseUpdateBoundingSphere();

	
		//Sphere* pSphere;
		GraphicsObject* poGraphicsObject;
		bool mDrawEnable;
		char pad[3];
		
	};
}

#endif



