

#ifndef SSBO_H
#define SSBO_H

#include "sb7.h"

namespace Azul
{
	// Shader Storage Buffer Object
	class SSBO
	{
	public:
		enum class Access
		{
			READ_ONLY = GL_READ_ONLY,
			WRITE_ONLY = GL_WRITE_ONLY,
			READ_WRITE = GL_READ_WRITE
		};

	public:
		//Big 4
		SSBO();
		SSBO(const SSBO &) = delete;
		const SSBO &operator = (const SSBO &) = delete;
		~SSBO();



		void Set(unsigned int elementCount, unsigned int size, void *inData);

		unsigned int GetLocation() const;
		void Bind(unsigned bindLocation);
		void *Map(SSBO::Access accessType);
		void Unmap();

	private:
		GLuint ssbo;
		unsigned int elementCount;
		unsigned int elementSize;
	};
}

#endif

//--- End of File ---

