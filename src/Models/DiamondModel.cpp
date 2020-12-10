#include "DiamondModel.h"
#include "ModelFileHdr.h"
#include "File.h"

namespace Azul
{


#if WRITE_DATA_TO_FILE

#define TRILIST_NUM_TRIANGLES (sizeof(DiamondTriList)/sizeof(Tri_index))

	Model::Tri_index DiamondTriList[] =
	{
		{0,1,2},
		{3,4,5},
		{6,7,8},
		{9,10,11},
		{12,13,14},
		{15,16,17},
		{18,19,20},
		{21,22,23}

	};

#define DIAMOND_DATA_NUM_VERTS (sizeof(DiamondData)/sizeof(Vert_xyzuvn))

	Model::Vert_xyzuvn  DiamondData[] =
	{
		//Top Front
		{ 0.0f, 0.25f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f},
		{  0.25f, -0.25f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
		{  -0.25f, -0.25f, 0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},

		//Top Back
		{ 0.0f, 0.25f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f},
		{  -0.25f, -0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
		{  0.25f, -0.25f, -0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},


		{ 0.0f, 0.25f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f},
		{  -0.25f, -0.25f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
		{  -0.25f, -0.25f, -0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},


		{  0.0f, 0.25f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f},
		{  0.25f, -0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
		{ 0.25f, -0.25f, 0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},


		

		{ 0.0f, -0.75f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f},
		{  -0.25f, -0.25f, 0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
		{  0.25f, -0.25f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},

		{ 0.0f, -0.75f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f},
		{  0.25f, -0.25f, -0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
		{  -0.25f, -0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},

		{ 0.0f, -0.75f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f},
		{  -0.25f, -0.25f, -0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
		{  -0.25f, -0.25f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},

		{ 0.0f, -0.75f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f},
		{ 0.25f, -0.25f, 0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
		{  0.25f, -0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f}

	};

#endif						



	void DiamondModel::privCreateVAO(const char* const pModelFileName)
	{
		// File stuff
		File::Handle fh;
		File::Error  ferror;

		// create a header
		ModelFileHdr modelHdr;


#if WRITE_DATA_TO_FILE

		// Fill the ModelFileHdr struct

		this->numVerts = DIAMOND_DATA_NUM_VERTS;
		this->numTris = TRILIST_NUM_TRIANGLES;

		// object name
		strncpy_s(modelHdr.objName, ModelFileHdr::OBJECT_NAME_SIZE, "Diamond", _TRUNCATE);

		// vertex buffer
		modelHdr.numVerts = this->numVerts;
		modelHdr.vertBufferOffset = 0;

		// trilist index 
		modelHdr.numTriList = this->numTris;
		modelHdr.triListBufferOffset = 0;

		// Write to a data file

		ferror = File::Open(fh, pModelFileName, File::Mode::READ_WRITE);
		assert(ferror == File::Error::SUCCESS);

		// write the Header
		ferror = File::Write(fh, &modelHdr, sizeof(ModelFileHdr));
		assert(ferror == File::Error::SUCCESS);

		// write the vert buffer data
		ferror = File::Tell(fh, modelHdr.vertBufferOffset);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Write(fh, DiamondData, sizeof(Vert_xyzuvn) * this->numVerts);
		assert(ferror == File::Error::SUCCESS);

		// write the index buffer
		ferror = File::Tell(fh, modelHdr.triListBufferOffset);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Write(fh, DiamondTriList, sizeof(Tri_index) * this->numTris);
		assert(ferror == File::Error::SUCCESS);

		// Finish the write
		ferror = File::Flush(fh);
		assert(ferror == File::Error::SUCCESS);

		// Rewind and overwrite model hdr
		ferror = File::Seek(fh, File::Location::BEGIN, 0);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Write(fh, &modelHdr, sizeof(ModelFileHdr));
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Flush(fh);
		assert(ferror == File::Error::SUCCESS);

		// VERIFY

		ModelFileHdr modelHdr2;

		ferror = File::Seek(fh, File::Location::BEGIN, 0);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Read(fh, &modelHdr2, sizeof(ModelFileHdr));
		assert(ferror == File::Error::SUCCESS);

		assert(memcmp(&modelHdr, &modelHdr2, sizeof(ModelFileHdr)) == 0);

		// CLOSE

		ferror = File::Close(fh);
		assert(ferror == File::Error::SUCCESS);

#endif



		// READ the data from the file ONLY

		ferror = File::Open(fh, pModelFileName, File::Mode::READ);
		assert(ferror == File::Error::SUCCESS);

		// Read the Hdr
		ferror = File::Read(fh, &modelHdr, sizeof(ModelFileHdr));
		assert(ferror == File::Error::SUCCESS);

		// Using the hdr, allocate the space for the buffers
		this->numVerts = modelHdr.numVerts;
		this->numTris = modelHdr.numTriList;

		// allocate buffers
		Vert_xyzuvn* pCubeData = new Vert_xyzuvn[(unsigned int)modelHdr.numVerts];
		Tri_index* pTriList = new Tri_index[(unsigned int)modelHdr.numTriList];

		// Read verts
		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.vertBufferOffset);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Read(fh, pCubeData, sizeof(Vert_xyzuvn) * this->numVerts);
		assert(ferror == File::Error::SUCCESS);

		// Read trilist
		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.triListBufferOffset);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Read(fh, pTriList, sizeof(Tri_index) * this->numTris);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Close(fh);
		assert(ferror == File::Error::SUCCESS);

		// Data is in RAM...
		// Configure and send data to GPU

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
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyzuvn) * this->numVerts), pCubeData, GL_STATIC_DRAW);

		// Define an array of generic vertex attribute data

		// todo make a table or enum - locations needs enums...

		// Vert data is location: 0  (used in vertex shader)
		void* offsetVert = (void*)((unsigned int)&pCubeData[0].x - (unsigned int)pCubeData);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetVert);
		glEnableVertexAttribArray(0);

		// Texture data is location: 1  (used in vertex shader)
		void* offsetTex = (void*)((unsigned int)&pCubeData[0].u - (unsigned int)pCubeData);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetTex);
		glEnableVertexAttribArray(1);

		// normals data in location 2 (used in vertex shader
		void* offsetNorm = (void*)((unsigned int)&pCubeData[0].nx - (unsigned int)pCubeData);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetNorm);
		glEnableVertexAttribArray(2);

		delete pCubeData;

		// Load the index data: ---------------------------------------------------------

			/* Bind our 2nd VBO as being the active buffer and storing index ) */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_1);

		/* Copy the index data to our buffer */
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Tri_index) * this->numTris), pTriList, GL_STATIC_DRAW);
		delete pTriList;
	}

	void DiamondModel::CreateNormalData()
	{
		int i;
		for (i = 0; i < DIAMOND_DATA_NUM_VERTS; i++) {


			if (i % 3 == 0) {
				Vect NormalVect = this->CalcNormalVect(Vect(DiamondData[i].x, DiamondData[i].y, DiamondData[i].z),
					Vect(DiamondData[i + 1].x, DiamondData[i + 1].y, DiamondData[i + 1].z),
					Vect(DiamondData[i + 2].x, DiamondData[i + 2].y, DiamondData[i + 2].z));

				DiamondData[i].nx = NormalVect.x() * -1;
				DiamondData[i].ny = NormalVect.y() * -1;
				DiamondData[i].nz = NormalVect.z() * -1;

				DiamondData[i+1].nx = NormalVect.x() * -1;
				DiamondData[i+1].ny = NormalVect.y() * -1;
				DiamondData[i+1].nz = NormalVect.z() * -1;

				DiamondData[i+2].nx = NormalVect.x() * -1;
				DiamondData[i+2].ny = NormalVect.y() * -1;
				DiamondData[i+2].nz = NormalVect.z() * -1;

				//Trace::out("Index %d: nx = %f, ny = %f, nz = %f\n", i, DiamondData[i].nx, DiamondData[i].ny, DiamondData[i].nz);
			}
		}
		

	}

	DiamondModel::DiamondModel(const char* const pModelFileName)
		: PrimitiveModel(Name::DIAMOND)
	{

		this->CreateNormalData();

		assert(pModelFileName);
		this->privCreateVAO(pModelFileName);



	}

	DiamondModel::~DiamondModel()
	{
		// remove anything dynamic here
	}
}

// --- End of File ---
