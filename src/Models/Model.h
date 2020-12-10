#ifndef MODEL_H
#define MODEL_H

#include "sb7.h"
#include "MathEngine.h"
#include "BoundingVolume/BoundingSphere.h"

namespace Azul
{
	#define WRITE_DATA_TO_FILE 1

	class Model
	{
	public:
		enum class Name
		{
			UNINITIALIZED,
			NULLMODEL,
			PYRAMID,
			SPHERE,
			CUBE,
			DIAMOND,
			SPACESHIP,
			BEAR,
			ASTROBOY,
			GUN,
			BUGGY,
			CAMERA
		
		};

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

	protected:
		Model(Name nameIn);

	public:
		Model(const Vert_xyzuvn* vertBuffer, const Tri_index* triBuffer, unsigned int _numVerts, unsigned int _numTris, Name nameIn = Name::UNINITIALIZED);// , Vect& CenterPoint, float radius);
		Model(Model& copyModel) = delete;
		Model& operator = (Model& copyModel) = delete;
		virtual ~Model();
		Model() = delete;
		
		// Data
		int numVerts;
		int numTris;    

		GLuint vao;
		GLuint vbo_0;   // xyzuvn
		GLuint vbo_1;   // trilist

		// Reference sphere... before any transformations are applied
		Sphere* poRefSphere;

	private:
		void privCreateVAO(const Vert_xyzuvn* vertBuffer, const Tri_index* triBuffer);

	//to do access protection 
	public:
		Model* next;
		Model* prev;
		Name ModelName;

	};

}

#endif

// --- End of File ---
