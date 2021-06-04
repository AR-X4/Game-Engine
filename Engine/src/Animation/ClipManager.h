#ifndef CLIP_MANAGER_H
#define CLIP_MANAGER_H

#include "Clip.h"

namespace Azul {

	class ClipManager
	{

	public:
		static void Add(Clip* pClip);
		static void Create();
		static void Destroy();
		static Clip* Find(Clip::Name _name);

	private:

		static ClipManager* privGetInstance();
		ClipManager();

		void privAddToFront(Clip* pNode);
		void privRemove(Clip* pNode);

	public:

		Clip* poHead;


	};

}
#endif