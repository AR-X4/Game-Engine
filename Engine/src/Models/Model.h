#ifndef MODEL_H
#define MODEL_H

#include "sb7.h"
#include "MathEngine.h"
#include "BoundingVolume/BoundingSphere.h"

namespace Azul
{
	

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
			CAMERA,
			ALIEN,
			TEXTBOX
		
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
		Model(unsigned int _numVerts, unsigned int _numTris, Name nameIn = Name::UNINITIALIZED);// , Vect& CenterPoint, float radius);
	public:
		Model(Model& copyModel) = delete;
		Model& operator = (Model& copyModel) = delete;
		virtual ~Model();
		Model() = delete;
		
		// Data
		int numVerts;
		int numTris;    

		GLuint vao;
		GLuint vbo_verts;   // xyzuvn
		GLuint vbo_tris;   // trilist

		// Reference sphere... before any transformations are applied
		Sphere* poRefSphere;


	//to do access protection 
	public:
		Model* next;
		Model* prev;
		Name ModelName;

	};

}

#endif

// --- End of File ---
