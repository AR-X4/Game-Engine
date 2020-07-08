

#include "Model.h"

namespace Azul
{
	Model::Model()
		: numVerts(0),
		numTris(0),    
		vao(0),
		vbo_0(0),
		vbo_1(0)
	{
		this->next = nullptr;
		this->prev = nullptr;

	}

	Model::~Model()
	{
	}

	Vect Model::CalcNormalVect(const Vect& x, const Vect& y, const Vect& z)
	{
		Vect vect1 = y - x;
		Vect vect2 = z - x;
		return vect1.cross(vect2).getNorm();
	}
	
}
// --- End of File ---
