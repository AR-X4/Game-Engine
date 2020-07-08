#ifndef MODEL_H
#define MODEL_H

#include "sb7.h"
#include "MathEngine.h"

namespace Azul
{
	#define WRITE_DATA_TO_FILE 1

	class Model
	{
	public:

		struct Vert_xyzuvn
		{
			float x;
			float y;
			float z;
			float u;
			float v;
			float nx;
			float ny;
			float nz;
		};

		struct Tri_index
		{
			unsigned int v0;
			unsigned int v1;
			unsigned int v2;
		}; 


		Model();
		Model(Model& copyModel) = delete;
		Model& operator = (Model& copyModel) = delete;
		virtual ~Model();


		// Data
		int numVerts;
		int numTris;    

		GLuint vao;
		GLuint vbo_0;   // xyzuvn
		GLuint vbo_1;   // trilist

	protected:


		// load VAO
		virtual void privCreateVAO(const char* const pModelFileName) = 0;

		Vect CalcNormalVect(const Vect& x, const Vect& y, const Vect& z);
		
	
	public:
		Model* next;
		Model* prev;

	};

}

#endif

// --- End of File ---
