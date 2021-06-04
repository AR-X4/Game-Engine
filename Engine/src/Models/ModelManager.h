
#ifndef MODEL_MANAGER_H
#define MODEL_MANAGER_H

#include "Model.h"

namespace Azul
{
	class ModelManager
	{
	public:
		static void Add(Model* pModel);
		static void Create();
		static void Destroy();
		static Model* Find(Model::Name _name);

	private:  // methods

		static ModelManager* privGetInstance();
		ModelManager();

		void privAddToFront(Model* node, Model*& head);
		void privRemove(Model* node, Model*& head);

	private:  // add

		Model* poHead;

	};

}

#endif

// ---  End of File ---
