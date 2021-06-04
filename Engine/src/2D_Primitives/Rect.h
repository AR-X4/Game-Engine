
#ifndef RECT_H
#define RECT_H

namespace Azul
{
	struct Rect
	{
		Rect(float x, float y, float width, float height);
		Rect();
		Rect(const Rect &) = default;
		virtual ~Rect() = default;

		void Clear();

	// Data:
		float x;
		float y;
		float width;
		float height;
	};
}

#endif

// --- End of File ---
