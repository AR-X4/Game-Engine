#ifndef SPRITE_MODEL_H
#define SPRITE_MODEL_H

#include "Model.h"

namespace Azul
{
	class Sprite : public Model
	{
	public:
		struct Vert_xyzuv
		{
			float x;
			float y;
			float z;
			float u;
			float v;
		};

		Sprite(Name Namein);
		Sprite() = delete;
		virtual ~Sprite() = default;

		// tells the compiler do not create or allow it to be used, c++11
		Sprite(const Sprite&) = delete;
		Sprite& operator=(const Sprite& other) = delete;

	private:
		void privCreateVAO();

	};
}

#endif