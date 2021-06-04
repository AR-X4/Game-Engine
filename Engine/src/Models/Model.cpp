#include "Model.h"

namespace Azul
{
	Model::Model(Name nameIn)
		: numVerts(0),
		numTris(0),    
		vao(0),
		vbo_verts(0),
		vbo_tris(0),
		poRefSphere(new Sphere()),
		next(nullptr),
		prev(nullptr),
		ModelName(nameIn)
	{
		

	}

	Model::Model(
				unsigned int _numVerts, 
				unsigned int _numTris,
				Name nameIn)//, 
				//Vect& CenterPoint, 
				//float radius)

		:numVerts((int)_numVerts),
		numTris((int)_numTris),
		vao(0),
		vbo_verts(0),
		vbo_tris(0),
		poRefSphere(new Sphere()), 
		next(nullptr),
		prev(nullptr),
		ModelName(nameIn)
	{
		
		
	}

	Model::~Model()
	{
		delete this->poRefSphere;
	}

	

	
	
}
// --- End of File ---
