#include "PyramidModel.h"
#include "ModelFileHdr.h"
#include "File.h"

namespace Azul
{


	#if WRITE_DATA_TO_FILE

	#define TRILIST_NUM_TRIANGLES (sizeof(PyramidTriList)/sizeof(Tri_index))

		Model::Tri_index PyramidTriList[] =
		{
			/*{0,1,2},
			{3,4,5},
			{6,7,8},
			{9,10,11},
			{12,13,14},
			{15,16,17}*/

			{0,1,2},
			{2,3,0},
			{4,5,6},
			{5,7,6},
			{8,9,10},
			{9,11,10},
			{12,13,14},
			{13,15,14},
			{16,17,18},
			{18,19,16},
			{20,21,22},
			{22,23,20}

		};

	#define PYRAMID_DATA_NUM_VERTS (sizeof(PyramidData)/sizeof(Vert_xyzuvn))

		Model::Vert_xyzuvn  PyramidData[] =
		{

			//{ 0.0f, 0.25f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f},//A
			//{  0.25f, -0.25f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},//E
			//{  -0.25f, -0.25f, 0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},//D


			//{ 0.0f, 0.25f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f},//D
			//{  -0.25f, -0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},//E
			//{  0.25f, -0.25f, -0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},//C


			//{ 0.0f, 0.25f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f},//C
			//{  -0.25f, -0.25f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},//E
			//{  -0.25f, -0.25f, -0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},//B


			//{  0.0f, 0.25f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f},//B
			//{  0.25f, -0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},//E
			//{ 0.25f, -0.25f, 0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},//A


			//{ -0.25f, -0.25f, 0.25f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},//B
			//{ 0.25f, -0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},//A
			//{  -0.25f, -0.25f, -0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},//D

			//{ -0.25f, -0.25f, 0.25f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},//D
			//{ 0.25f, -0.25f, 0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},//A
			//{  0.25f, -0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},//B

			// Triangle 0
		{-0.25f,  0.0f, -0.25f, 0.0f, 0.0f, -0.6f,  0.6f, -0.6f  }, //0
		{-0.25f, -0.0f, -0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f  },// 1
		{ 0.25f, -0.0f, -0.25f, 1.0f, 1.0f,  0.6f, -0.6f, -0.6f  }, //2

		// Triangle 1

		{ 0.0f,   1.0f, -0.0f, 1.0f, 0.0f,  0.6f,  0.6f, -0.6f  },  //4  3


		// Triangle 2
		{ 0.25f, -0.0f, -0.25f, 0.0f, 1.0f, 0.6f, -0.6f, -0.6f  }, //6
		{ 0.25f, -0.0f,  0.25f, 1.0f, 1.0f, 0.6f, -0.6f,  0.6f  }, //7
		{ 0.0f,  1.0f, -0.0f, 0.0f, 0.0f, 0.6f,  0.6f, -0.6f  }, //8

		// Triangle 3

		{ 0.25f,  0.0f,  0.25f, 1.0f, 0.0f, 0.6f,  0.6f,  0.6f  }, //9 


		// Triangle 4
		{ 0.25f, -0.0f,  0.25f, 1.0f, 1.0f,  0.6f, -0.6f, 0.6f  },  // 12
		{-0.25f, -0.0f, 0.25f, 0.0f, 1.0f, -0.6f, -0.6f, 0.6f  },   // 13
		{ 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,  0.6f,  0.6f, 0.6f  },  // 14

		// Triangle 5

		{ -0.0f,  1.0f,  0.0f, 0.0f, 0.0f, -0.6f,  0.6f, 0.6f },  //15


		// Triangle 6
		{-0.25f, -0.0f,  0.25f, 0.0f, 0.0f, -0.6f, -0.6f,  0.6f }, //18
		{-0.25f, -0.0f, -0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f }, //19
		{-0.0f,  1.0f,  0.0f, 1.0f, 0.0f, -0.6f,  0.6f,  0.6f }, //20

		// Triangle 7

		{-0.0f,  1.0f, -0.0f, 1.0f, 1.0f, -0.6f,  0.6f, -0.6f }, // 21


		// Triangle 8
		{-0.25f, -0.0f,  0.25f, 0.0f, 0.0f, -0.6f, -0.6f,  0.6f }, //24
		{0.25f, -0.0f,   0.25f, 1.0f, 0.0f,  0.6f, -0.6f,  0.6f }, //25
		{0.25f, -0.0f,  -0.25f, 1.0f, 1.0f,  0.6f, -0.6f, -0.6f }, //26

		// Triangle 9

		{-0.25f, -0.0f, -0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f}, // 27


		// Triangle 10
		{-0.0f,  1.0f, -0.0f, 0.0f, 1.0f, -0.6f, 0.6f, -0.6f }, //30
		{0.0f,  1.0f,  -0.0f, 1.0f, 1.0f,  0.6f, 0.6f, -0.6f }, //31
		{0.0f,  1.0f,   0.0f, 1.0f, 0.0f,  0.6f, 0.6f,  0.6f }, //32

		// Triangle 11

		{-0.0f,  1.0f,  0.0f, 0.0f, 0.0f, -0.6f, 0.6f,  0.6f }, //33

		};							
		
#endif						



	void PyramidModel::privCreateVAO(const char* const pModelFileName)
	{


		AZUL_UNUSED_VAR(pModelFileName);

		this->numVerts = PYRAMID_DATA_NUM_VERTS;
		this->numTris = TRILIST_NUM_TRIANGLES;

		Matrix M(Matrix::Rot1::X, MATH_PI2);
		for (int i = 0; i < this->numVerts; i++)
		{
			Vect v(PyramidData[i].x, PyramidData[i].y, PyramidData[i].z);
			v = v * M;
			PyramidData[i].x = v[x];
			PyramidData[i].y = v[y];
			PyramidData[i].z = v[z];
		}


		// Create a VAO
		glGenVertexArrays(1, &this->vao);
		assert(this->vao != 0);
		glBindVertexArray(this->vao);

		// Create a VBO
		glGenBuffers(1, &this->vbo_0);
		assert(this->vbo_0 != 0);
		glGenBuffers(1, &this->vbo_1);
		assert(this->vbo_1 != 0);


		// Load the combined data: ---------------------------------------------------------

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_0);

		// load the data to the GPU
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyzuvn) * this->numVerts), PyramidData, GL_STATIC_DRAW);

		// Vertext data in location 0
		void* offsetVert = (void*)((unsigned int)&PyramidData[0].x - (unsigned int)PyramidData);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetVert);
		glEnableVertexAttribArray(0);

		// Texture data in location 1
		void* offsetTex = (void*)((unsigned int)&PyramidData[0].u - (unsigned int)PyramidData);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetTex);
		glEnableVertexAttribArray(1);

		// Normal data in location 2
		void* offsetNorm = (void*)((unsigned int)&PyramidData[0].nx - (unsigned int)PyramidData);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetNorm);
		glEnableVertexAttribArray(2);


		// Load the index data: ---------------------------------------------------------

			  /* Bind our 2nd VBO as being the active buffer and storing index ) */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_1);

		/* Copy the index data to our buffer */
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Tri_index) * this->numTris), PyramidTriList, GL_STATIC_DRAW);






		//// File stuff
		//File::Handle fh;
		//File::Error  ferror;

		//// create a header
		//ModelFileHdr modelHdr;


		//#if WRITE_DATA_TO_FILE

		//	// Fill the ModelFileHdr struct

		//	this->numVerts = PYRAMID_DATA_NUM_VERTS;
		//	this->numTris = TRILIST_NUM_TRIANGLES;

		//	// object name
		//	strncpy_s(modelHdr.objName, ModelFileHdr::OBJECT_NAME_SIZE, "Pyramid", _TRUNCATE);

		//	// vertex buffer
		//	modelHdr.numVerts = this->numVerts;
		//	modelHdr.vertBufferOffset = 0;

		//	// trilist index 
		//	modelHdr.numTriList = this->numTris;
		//	modelHdr.triListBufferOffset = 0;

		//	// Write to a data file

		//	ferror = File::Open(fh, pModelFileName, File::Mode::READ_WRITE);
		//	assert(ferror == File::Error::SUCCESS);

		//	// write the Header
		//	ferror = File::Write(fh, &modelHdr, sizeof(ModelFileHdr));
		//	assert(ferror == File::Error::SUCCESS);

		//	// write the vert buffer data
		//	ferror = File::Tell(fh, modelHdr.vertBufferOffset);
		//	assert(ferror == File::Error::SUCCESS);

		//	ferror = File::Write(fh, PyramidData, sizeof(Vert_xyzuvn) * this->numVerts);
		//	assert(ferror == File::Error::SUCCESS);

		//	// write the index buffer
		//	ferror = File::Tell(fh, modelHdr.triListBufferOffset);
		//	assert(ferror == File::Error::SUCCESS);

		//	ferror = File::Write(fh, PyramidTriList, sizeof(Tri_index) * this->numTris);
		//	assert(ferror == File::Error::SUCCESS);

		//	// Finish the write
		//	ferror = File::Flush(fh);
		//	assert(ferror == File::Error::SUCCESS);

		//	// Rewind and overwrite model hdr
		//	ferror = File::Seek(fh, File::Location::BEGIN, 0);
		//	assert(ferror == File::Error::SUCCESS);

		//	ferror = File::Write(fh, &modelHdr, sizeof(ModelFileHdr));
		//	assert(ferror == File::Error::SUCCESS);

		//	ferror = File::Flush(fh);
		//	assert(ferror == File::Error::SUCCESS);

		//	// VERIFY

		//	ModelFileHdr modelHdr2;

		//	ferror = File::Seek(fh, File::Location::BEGIN, 0);
		//	assert(ferror == File::Error::SUCCESS);

		//	ferror = File::Read(fh, &modelHdr2, sizeof(ModelFileHdr));
		//	assert(ferror == File::Error::SUCCESS);

		//	assert(memcmp(&modelHdr, &modelHdr2, sizeof(ModelFileHdr)) == 0);

		//	// CLOSE

		//	ferror = File::Close(fh);
		//	assert(ferror == File::Error::SUCCESS);

		//#endif



		//// READ the data from the file ONLY

		//ferror = File::Open(fh, pModelFileName, File::Mode::READ);
		//assert(ferror == File::Error::SUCCESS);

		//// Read the Hdr
		//ferror = File::Read(fh, &modelHdr, sizeof(ModelFileHdr));
		//assert(ferror == File::Error::SUCCESS);

		//// Using the hdr, allocate the space for the buffers
		//this->numVerts = modelHdr.numVerts;
		//this->numTris = modelHdr.numTriList;

		//// allocate buffers
		//Vert_xyzuvn* pCubeData = new Vert_xyzuvn[(unsigned int)modelHdr.numVerts];
		//Tri_index* pTriList = new Tri_index[(unsigned int)modelHdr.numTriList];

		//// Read verts
		//ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.vertBufferOffset);
		//assert(ferror == File::Error::SUCCESS);

		//ferror = File::Read(fh, pCubeData, sizeof(Vert_xyzuvn) * this->numVerts);
		//assert(ferror == File::Error::SUCCESS);

		//// Read trilist
		//ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.triListBufferOffset);
		//assert(ferror == File::Error::SUCCESS);

		//ferror = File::Read(fh, pTriList, sizeof(Tri_index) * this->numTris);
		//assert(ferror == File::Error::SUCCESS);

		//ferror = File::Close(fh);
		//assert(ferror == File::Error::SUCCESS);

		//// Data is in RAM...
		//// Configure and send data to GPU

		//// Create a VAO
		//glGenVertexArrays(1, &this->vao);
		//assert(this->vao != 0);
		//glBindVertexArray(this->vao);

		//// Create a VBO
		//glGenBuffers(1, &this->vbo_0);
		//assert(this->vbo_0 != 0);

		//glGenBuffers(1, &this->vbo_1);
		//assert(this->vbo_1 != 0);

		////----------------------------------------------------------------------------------

		////Find Bounding Volume - quick hack
		//	// MOVE this to converter!!!!!
		//Vect* pVerts = new Vect[(unsigned int)this->numVerts];
		//for (int i = 0; i < this->numVerts; i++)
		//{
		//	pVerts[i].set(pCubeData[i].x, pCubeData[i].y, pCubeData[i].z);
		//	Trace::out("%d:  %f %f %f\n", i, pVerts[i][x], pVerts[i][y], pVerts[i][z]);
		//}

		//assert(this->poRefSphere);
		//this->poRefSphere->RitterSphere(pVerts, this->numVerts);
		//Trace::out("Ritter: cntr:%f %f %f  rad: %f ", this->poRefSphere->cntr[x], this->poRefSphere->cntr[y], this->poRefSphere->cntr[z], this->poRefSphere->rad);

		//delete[] pVerts;

		//// Load the combined data: ---------------------------------------------------------

		//glBindBuffer(GL_ARRAY_BUFFER, this->vbo_0);

		//// load the data to the GPU
		//glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyzuvn) * this->numVerts), pCubeData, GL_STATIC_DRAW);

		//// Define an array of generic vertex attribute data

		//// todo make a table or enum - locations needs enums...

		//// Vert data is location: 0  (used in vertex shader)
		//void* offsetVert = (void*)((unsigned int)&pCubeData[0].x - (unsigned int)pCubeData);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetVert);
		//glEnableVertexAttribArray(0);

		//// Texture data is location: 1  (used in vertex shader)
		//void* offsetTex = (void*)((unsigned int)&pCubeData[0].u - (unsigned int)pCubeData);
		//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetTex);
		//glEnableVertexAttribArray(1);

		//// normals data in location 2 (used in vertex shader
		//void* offsetNorm = (void*)((unsigned int)&pCubeData[0].nx - (unsigned int)pCubeData);
		//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetNorm);
		//glEnableVertexAttribArray(2);

		//delete pCubeData;

		//// Load the index data: ---------------------------------------------------------

		//	/* Bind our 2nd VBO as being the active buffer and storing index ) */
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_1);

		///* Copy the index data to our buffer */
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Tri_index) * this->numTris), pTriList, GL_STATIC_DRAW);
		//delete pTriList;
	}

	void PyramidModel::CreateNormalData()
	{
		
		for (int i = 0; i < PYRAMID_DATA_NUM_VERTS ; i++) {


			if (i % 3 == 0) {
				Vect NormalVect = this->CalcNormalVect(Vect(PyramidData[i].x, PyramidData[i].y, PyramidData[i].z),
														Vect(PyramidData[i + 1].x, PyramidData[i + 1].y, PyramidData[i + 1].z),
														Vect(PyramidData[i + 2].x, PyramidData[i + 2].y, PyramidData[i + 2].z));

				PyramidData[i].nx = NormalVect.x() *-1;
				PyramidData[i].ny = NormalVect.y() *-1;
				PyramidData[i].nz = NormalVect.z() *-1;

				PyramidData[i+1].nx = NormalVect.x() * -1;
				PyramidData[i+1].ny = NormalVect.y() * -1;
				PyramidData[i+1].nz = NormalVect.z() * -1;

				PyramidData[i+2].nx = NormalVect.x() * -1;
				PyramidData[i+2].ny = NormalVect.y() * -1;
				PyramidData[i+2].nz = NormalVect.z() * -1;
			}
		}

		

	}

	PyramidModel::PyramidModel(const char* const pModelFileName)
		: PrimitiveModel(Name::PYRAMID)
	{

		//this->CreateNormalData();

		assert(pModelFileName);
		this->privCreateVAO(pModelFileName);


		
	}

	PyramidModel::~PyramidModel()
	{
		// remove anything dynamic here
	}
}

// --- End of File ---
