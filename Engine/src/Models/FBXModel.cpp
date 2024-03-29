#include "FBXModel.h"
namespace Azul
{
	FBXModel::FBXModel(const Vert_xyzuvn* vertBuffer, const Tri_index* triBuffer, unsigned int _numVerts, unsigned int _numTris, Name nameIn)
		:Model(_numVerts, _numTris, nameIn)
	{
		this->privCreateVAO(vertBuffer, triBuffer);
	}

	void Azul::FBXModel::privCreateVAO(const Vert_xyzuvn* vertBuffer, const Tri_index* triBuffer)
	{
		// Create a VAO
		glGenVertexArrays(1, &this->vao);
		assert(this->vao != 0);
		glBindVertexArray(this->vao);

		// Create a VBO
		glGenBuffers(1, &this->vbo_verts);
		assert(this->vbo_verts != 0);

		glGenBuffers(1, &this->vbo_tris);
		assert(this->vbo_tris != 0);

		//----------------------------------------------------------------------------------

		//Find Bounding Volume - quick hack
			// MOVE this to converter!!!!!
		Vect* pVerts = new Vect[(unsigned int)this->numVerts];
		for (int i = 0; i < this->numVerts; i++)
		{
			pVerts[i].set(vertBuffer[i].x, vertBuffer[i].y, vertBuffer[i].z);
			//Trace::out("%d:  %f %f %f\n", i, pVerts[i][x], pVerts[i][y], pVerts[i][z]);
		}

		assert(this->poRefSphere);
		this->poRefSphere->RitterSphere(pVerts, this->numVerts);
		//Trace::out("Ritter: cntr:%f %f %f  rad: %f ", this->poRefSphere->cntr[x], this->poRefSphere->cntr[y], this->poRefSphere->cntr[z], this->poRefSphere->rad);

		delete[] pVerts;

		// Load the combined data: ---------------------------------------------------------

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_verts);

		// load the data to the GPU
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyzuvn) * this->numVerts), vertBuffer, GL_STATIC_DRAW);

		// Define an array of generic vertex attribute data

		// todo make a table or enum - locations needs enums...

		// Vert data is location: 0  (used in vertex shader)
		void* offsetVert = (void*)((unsigned int)&vertBuffer[0].x - (unsigned int)vertBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetVert);
		glEnableVertexAttribArray(0);

		// Texture data is location: 1  (used in vertex shader)
		void* offsetTex = (void*)((unsigned int)&vertBuffer[0].u - (unsigned int)vertBuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetTex);
		glEnableVertexAttribArray(1);

		// normals data in location 2 (used in vertex shader
		void* offsetNorm = (void*)((unsigned int)&vertBuffer[0].nx - (unsigned int)vertBuffer);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetNorm);
		glEnableVertexAttribArray(2);

		delete[] vertBuffer;

		// Load the index data: ---------------------------------------------------------

		/* Bind our 2nd VBO as being the active buffer and storing index ) */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_tris);

		/* Copy the index data to our buffer */
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Tri_index) * this->numTris), triBuffer, GL_STATIC_DRAW);
		delete[] triBuffer;
	}
}