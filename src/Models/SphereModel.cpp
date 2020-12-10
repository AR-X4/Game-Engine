#include "SphereModel.h"

#include "ModelFileHdr.h"
#include "File.h"

namespace Azul
{


#if WRITE_DATA_TO_FILE

#define TRILIST_NUM_TRIANGLES (sizeof(SphereTriList)/sizeof(Tri_index))

	Model::Tri_index SphereTriList[200];
	
		


#define SPHERE_DATA_NUM_VERTS (sizeof(SphereData)/sizeof(Vert_xyzuvn))

	Model::Vert_xyzuvn  SphereData[121];

#endif						



	void SphereModel::privCreateVAO(const char* const pModelFileName)
	{
		// File stuff
		File::Handle fh;
		File::Error  ferror;

		// create a header
		ModelFileHdr modelHdr;


#if WRITE_DATA_TO_FILE

		// Fill the ModelFileHdr struct

		this->numVerts = SPHERE_DATA_NUM_VERTS;
		this->numTris = TRILIST_NUM_TRIANGLES;

		// object name
		strncpy_s(modelHdr.objName, ModelFileHdr::OBJECT_NAME_SIZE, "Pyramid", _TRUNCATE);

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

		ferror = File::Write(fh, SphereData, sizeof(Vert_xyzuvn) * this->numVerts);
		assert(ferror == File::Error::SUCCESS);

		// write the index buffer
		ferror = File::Tell(fh, modelHdr.triListBufferOffset);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Write(fh, SphereTriList, sizeof(Tri_index) * this->numTris);
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


		/*----------------------------------------------------------------------------------

		Find Bounding Volume - quick hack
			 MOVE this to converter!!!!!
			Vect * pVerts = new Vect[(unsigned int)this->numVerts];
		for (int i = 0; i < this->numVerts; i++)
		{
			pVerts[i].set(pCubeData[i].x, pCubeData[i].y, pCubeData[i].z);
			Trace::out("%d:  %f %f %f\n", i, pVerts[i][x], pVerts[i][y], pVerts[i][z]);
		}

		assert(this->poRefSphere);
		this->poRefSphere->RitterSphere(pVerts, this->numVerts);
		Trace::out("Ritter: cntr:%f %f %f  rad: %f ", this->poRefSphere->cntr[x], this->poRefSphere->cntr[y], this->poRefSphere->cntr[z], this->poRefSphere->rad);

		delete[] pVerts;*/

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

	void SphereModel::CreateSphere()
	{
		//Gross hack... removing this next quarter anyway and loading .fbx files instead
		//just needed sphere for demo

		SphereTriList[0].v0 = 0;
		SphereTriList[0].v1 = 1;
		SphereTriList[0].v2 = 2;

		SphereTriList[1].v0 = 1;
		SphereTriList[1].v1 = 3;
		SphereTriList[1].v2 = 2;

		SphereTriList[2].v0 = 2;
		SphereTriList[2].v1 = 3;
		SphereTriList[2].v2 = 4;

		SphereTriList[3].v0 = 3;
		SphereTriList[3].v1 = 5;
		SphereTriList[3].v2 = 4;

		SphereTriList[4].v0 = 4;
		SphereTriList[4].v1 = 5;
		SphereTriList[4].v2 = 6;

		SphereTriList[5].v0 = 5;
		SphereTriList[5].v1 = 7;
		SphereTriList[5].v2 = 6;

		SphereTriList[6].v0 = 6;
		SphereTriList[6].v1 = 7;
		SphereTriList[6].v2 = 8;

		SphereTriList[7].v0 = 7;
		SphereTriList[7].v1 = 9;
		SphereTriList[7].v2 = 8;

		SphereTriList[8].v0 = 8;
		SphereTriList[8].v1 = 9;
		SphereTriList[8].v2 = 10;

		SphereTriList[9].v0 = 9;
		SphereTriList[9].v1 = 11;
		SphereTriList[9].v2 = 10;

		SphereTriList[10].v0 = 10;
		SphereTriList[10].v1 = 11;
		SphereTriList[10].v2 = 12;

		SphereTriList[11].v0 = 11;
		SphereTriList[11].v1 = 13;
		SphereTriList[11].v2 = 12;

		SphereTriList[12].v0 = 12;
		SphereTriList[12].v1 = 13;
		SphereTriList[12].v2 = 14;

		SphereTriList[13].v0 = 13;
		SphereTriList[13].v1 = 15;
		SphereTriList[13].v2 = 14;

		SphereTriList[14].v0 = 14;
		SphereTriList[14].v1 = 15;
		SphereTriList[14].v2 = 16;

		SphereTriList[15].v0 = 15;
		SphereTriList[15].v1 = 17;
		SphereTriList[15].v2 = 16;

		SphereTriList[16].v0 = 16;
		SphereTriList[16].v1 = 17;
		SphereTriList[16].v2 = 18;

		SphereTriList[17].v0 = 17;
		SphereTriList[17].v1 = 19;
		SphereTriList[17].v2 = 18;

		SphereTriList[18].v0 = 18;
		SphereTriList[18].v1 = 19;
		SphereTriList[18].v2 = 20;

		SphereTriList[19].v0 = 19;
		SphereTriList[19].v1 = 21;
		SphereTriList[19].v2 = 20;

		SphereTriList[20].v0 = 1;
		SphereTriList[20].v1 = 22;
		SphereTriList[20].v2 = 3;

		SphereTriList[21].v0 = 22;
		SphereTriList[21].v1 = 23;
		SphereTriList[21].v2 = 3;

		SphereTriList[22].v0 = 3;
		SphereTriList[22].v1 = 23;
		SphereTriList[22].v2 = 5;

		SphereTriList[23].v0 = 23;
		SphereTriList[23].v1 = 24;
		SphereTriList[23].v2 = 5;

		SphereTriList[24].v0 = 5;
		SphereTriList[24].v1 = 24;
		SphereTriList[24].v2 = 7;

		SphereTriList[25].v0 = 24;
		SphereTriList[25].v1 = 25;
		SphereTriList[25].v2 = 7;

		SphereTriList[26].v0 = 7;
		SphereTriList[26].v1 = 25;
		SphereTriList[26].v2 = 9;

		SphereTriList[27].v0 = 25;
		SphereTriList[27].v1 = 26;
		SphereTriList[27].v2 = 9;

		SphereTriList[28].v0 = 9;
		SphereTriList[28].v1 = 26;
		SphereTriList[28].v2 = 11;

		SphereTriList[29].v0 = 26;
		SphereTriList[29].v1 = 27;
		SphereTriList[29].v2 = 11;

		SphereTriList[30].v0 = 11;
		SphereTriList[30].v1 = 27;
		SphereTriList[30].v2 = 13;

		SphereTriList[31].v0 = 27;
		SphereTriList[31].v1 = 28;
		SphereTriList[31].v2 = 13;

		SphereTriList[32].v0 = 13;
		SphereTriList[32].v1 = 28;
		SphereTriList[32].v2 = 15;

		SphereTriList[33].v0 = 28;
		SphereTriList[33].v1 = 29;
		SphereTriList[33].v2 = 15;

		SphereTriList[34].v0 = 15;
		SphereTriList[34].v1 = 29;
		SphereTriList[34].v2 = 17;

		SphereTriList[35].v0 = 29;
		SphereTriList[35].v1 = 30;
		SphereTriList[35].v2 = 17;

		SphereTriList[36].v0 = 17;
		SphereTriList[36].v1 = 30;
		SphereTriList[36].v2 = 19;

		SphereTriList[37].v0 = 30;
		SphereTriList[37].v1 = 31;
		SphereTriList[37].v2 = 19;

		SphereTriList[38].v0 = 19;
		SphereTriList[38].v1 = 31;
		SphereTriList[38].v2 = 21;

		SphereTriList[39].v0 = 31;
		SphereTriList[39].v1 = 32;
		SphereTriList[39].v2 = 21;

		SphereTriList[40].v0 = 22;
		SphereTriList[40].v1 = 33;
		SphereTriList[40].v2 = 23;

		SphereTriList[41].v0 = 33;
		SphereTriList[41].v1 = 34;
		SphereTriList[41].v2 = 23;

		SphereTriList[42].v0 = 23;
		SphereTriList[42].v1 = 34;
		SphereTriList[42].v2 = 24;

		SphereTriList[43].v0 = 34;
		SphereTriList[43].v1 = 35;
		SphereTriList[43].v2 = 24;

		SphereTriList[44].v0 = 24;
		SphereTriList[44].v1 = 35;
		SphereTriList[44].v2 = 25;

		SphereTriList[45].v0 = 35;
		SphereTriList[45].v1 = 36;
		SphereTriList[45].v2 = 25;

		SphereTriList[46].v0 = 25;
		SphereTriList[46].v1 = 36;
		SphereTriList[46].v2 = 26;

		SphereTriList[47].v0 = 36;
		SphereTriList[47].v1 = 37;
		SphereTriList[47].v2 = 26;

		SphereTriList[48].v0 = 26;
		SphereTriList[48].v1 = 37;
		SphereTriList[48].v2 = 27;

		SphereTriList[49].v0 = 37;
		SphereTriList[49].v1 = 38;
		SphereTriList[49].v2 = 27;

		SphereTriList[50].v0 = 27;
		SphereTriList[50].v1 = 38;
		SphereTriList[50].v2 = 28;

		SphereTriList[51].v0 = 38;
		SphereTriList[51].v1 = 39;
		SphereTriList[51].v2 = 28;

		SphereTriList[52].v0 = 28;
		SphereTriList[52].v1 = 39;
		SphereTriList[52].v2 = 29;

		SphereTriList[53].v0 = 39;
		SphereTriList[53].v1 = 40;
		SphereTriList[53].v2 = 29;

		SphereTriList[54].v0 = 29;
		SphereTriList[54].v1 = 40;
		SphereTriList[54].v2 = 30;

		SphereTriList[55].v0 = 40;
		SphereTriList[55].v1 = 41;
		SphereTriList[55].v2 = 30;

		SphereTriList[56].v0 = 30;
		SphereTriList[56].v1 = 41;
		SphereTriList[56].v2 = 31;

		SphereTriList[57].v0 = 41;
		SphereTriList[57].v1 = 42;
		SphereTriList[57].v2 = 31;

		SphereTriList[58].v0 = 31;
		SphereTriList[58].v1 = 42;
		SphereTriList[58].v2 = 32;

		SphereTriList[59].v0 = 42;
		SphereTriList[59].v1 = 43;
		SphereTriList[59].v2 = 32;

		SphereTriList[60].v0 = 33;
		SphereTriList[60].v1 = 44;
		SphereTriList[60].v2 = 34;

		SphereTriList[61].v0 = 44;
		SphereTriList[61].v1 = 45;
		SphereTriList[61].v2 = 34;

		SphereTriList[62].v0 = 34;
		SphereTriList[62].v1 = 45;
		SphereTriList[62].v2 = 35;

		SphereTriList[63].v0 = 45;
		SphereTriList[63].v1 = 46;
		SphereTriList[63].v2 = 35;

		SphereTriList[64].v0 = 35;
		SphereTriList[64].v1 = 46;
		SphereTriList[64].v2 = 36;

		SphereTriList[65].v0 = 46;
		SphereTriList[65].v1 = 47;
		SphereTriList[65].v2 = 36;

		SphereTriList[66].v0 = 36;
		SphereTriList[66].v1 = 47;
		SphereTriList[66].v2 = 37;

		SphereTriList[67].v0 = 47;
		SphereTriList[67].v1 = 48;
		SphereTriList[67].v2 = 37;

		SphereTriList[68].v0 = 37;
		SphereTriList[68].v1 = 48;
		SphereTriList[68].v2 = 38;

		SphereTriList[69].v0 = 48;
		SphereTriList[69].v1 = 49;
		SphereTriList[69].v2 = 38;

		SphereTriList[70].v0 = 38;
		SphereTriList[70].v1 = 49;
		SphereTriList[70].v2 = 39;

		SphereTriList[71].v0 = 49;
		SphereTriList[71].v1 = 50;
		SphereTriList[71].v2 = 39;

		SphereTriList[72].v0 = 39;
		SphereTriList[72].v1 = 50;
		SphereTriList[72].v2 = 40;

		SphereTriList[73].v0 = 50;
		SphereTriList[73].v1 = 51;
		SphereTriList[73].v2 = 40;

		SphereTriList[74].v0 = 40;
		SphereTriList[74].v1 = 51;
		SphereTriList[74].v2 = 41;

		SphereTriList[75].v0 = 51;
		SphereTriList[75].v1 = 52;
		SphereTriList[75].v2 = 41;

		SphereTriList[76].v0 = 41;
		SphereTriList[76].v1 = 52;
		SphereTriList[76].v2 = 42;

		SphereTriList[77].v0 = 52;
		SphereTriList[77].v1 = 53;
		SphereTriList[77].v2 = 42;

		SphereTriList[78].v0 = 42;
		SphereTriList[78].v1 = 53;
		SphereTriList[78].v2 = 43;

		SphereTriList[79].v0 = 53;
		SphereTriList[79].v1 = 54;
		SphereTriList[79].v2 = 43;

		SphereTriList[80].v0 = 44;
		SphereTriList[80].v1 = 55;
		SphereTriList[80].v2 = 45;

		SphereTriList[81].v0 = 55;
		SphereTriList[81].v1 = 56;
		SphereTriList[81].v2 = 45;

		SphereTriList[82].v0 = 45;
		SphereTriList[82].v1 = 56;
		SphereTriList[82].v2 = 46;

		SphereTriList[83].v0 = 56;
		SphereTriList[83].v1 = 57;
		SphereTriList[83].v2 = 46;

		SphereTriList[84].v0 = 46;
		SphereTriList[84].v1 = 57;
		SphereTriList[84].v2 = 47;

		SphereTriList[85].v0 = 57;
		SphereTriList[85].v1 = 58;
		SphereTriList[85].v2 = 47;

		SphereTriList[86].v0 = 47;
		SphereTriList[86].v1 = 58;
		SphereTriList[86].v2 = 48;

		SphereTriList[87].v0 = 58;
		SphereTriList[87].v1 = 59;
		SphereTriList[87].v2 = 48;

		SphereTriList[88].v0 = 48;
		SphereTriList[88].v1 = 59;
		SphereTriList[88].v2 = 49;

		SphereTriList[89].v0 = 59;
		SphereTriList[89].v1 = 60;
		SphereTriList[89].v2 = 49;

		SphereTriList[90].v0 = 49;
		SphereTriList[90].v1 = 60;
		SphereTriList[90].v2 = 50;

		SphereTriList[91].v0 = 60;
		SphereTriList[91].v1 = 61;
		SphereTriList[91].v2 = 50;

		SphereTriList[92].v0 = 50;
		SphereTriList[92].v1 = 61;
		SphereTriList[92].v2 = 51;

		SphereTriList[93].v0 = 61;
		SphereTriList[93].v1 = 62;
		SphereTriList[93].v2 = 51;

		SphereTriList[94].v0 = 51;
		SphereTriList[94].v1 = 62;
		SphereTriList[94].v2 = 52;

		SphereTriList[95].v0 = 62;
		SphereTriList[95].v1 = 63;
		SphereTriList[95].v2 = 52;

		SphereTriList[96].v0 = 52;
		SphereTriList[96].v1 = 63;
		SphereTriList[96].v2 = 53;

		SphereTriList[97].v0 = 63;
		SphereTriList[97].v1 = 64;
		SphereTriList[97].v2 = 53;

		SphereTriList[98].v0 = 53;
		SphereTriList[98].v1 = 64;
		SphereTriList[98].v2 = 54;

		SphereTriList[99].v0 = 64;
		SphereTriList[99].v1 = 65;
		SphereTriList[99].v2 = 54;

		SphereTriList[100].v0 = 55;
		SphereTriList[100].v1 = 66;
		SphereTriList[100].v2 = 56;

		SphereTriList[101].v0 = 66;
		SphereTriList[101].v1 = 67;
		SphereTriList[101].v2 = 56;

		SphereTriList[102].v0 = 56;
		SphereTriList[102].v1 = 67;
		SphereTriList[102].v2 = 57;

		SphereTriList[103].v0 = 67;
		SphereTriList[103].v1 = 68;
		SphereTriList[103].v2 = 57;

		SphereTriList[104].v0 = 57;
		SphereTriList[104].v1 = 68;
		SphereTriList[104].v2 = 58;

		SphereTriList[105].v0 = 68;
		SphereTriList[105].v1 = 69;
		SphereTriList[105].v2 = 58;

		SphereTriList[106].v0 = 58;
		SphereTriList[106].v1 = 69;
		SphereTriList[106].v2 = 59;

		SphereTriList[107].v0 = 69;
		SphereTriList[107].v1 = 70;
		SphereTriList[107].v2 = 59;

		SphereTriList[108].v0 = 59;
		SphereTriList[108].v1 = 70;
		SphereTriList[108].v2 = 60;

		SphereTriList[109].v0 = 70;
		SphereTriList[109].v1 = 71;
		SphereTriList[109].v2 = 60;

		SphereTriList[110].v0 = 60;
		SphereTriList[110].v1 = 71;
		SphereTriList[110].v2 = 61;

		SphereTriList[111].v0 = 71;
		SphereTriList[111].v1 = 72;
		SphereTriList[111].v2 = 61;

		SphereTriList[112].v0 = 61;
		SphereTriList[112].v1 = 72;
		SphereTriList[112].v2 = 62;

		SphereTriList[113].v0 = 72;
		SphereTriList[113].v1 = 73;
		SphereTriList[113].v2 = 62;

		SphereTriList[114].v0 = 62;
		SphereTriList[114].v1 = 73;
		SphereTriList[114].v2 = 63;

		SphereTriList[115].v0 = 73;
		SphereTriList[115].v1 = 74;
		SphereTriList[115].v2 = 63;

		SphereTriList[116].v0 = 63;
		SphereTriList[116].v1 = 74;
		SphereTriList[116].v2 = 64;

		SphereTriList[117].v0 = 74;
		SphereTriList[117].v1 = 75;
		SphereTriList[117].v2 = 64;

		SphereTriList[118].v0 = 64;
		SphereTriList[118].v1 = 75;
		SphereTriList[118].v2 = 65;

		SphereTriList[119].v0 = 75;
		SphereTriList[119].v1 = 76;
		SphereTriList[119].v2 = 65;

		SphereTriList[120].v0 = 66;
		SphereTriList[120].v1 = 77;
		SphereTriList[120].v2 = 67;

		SphereTriList[121].v0 = 77;
		SphereTriList[121].v1 = 78;
		SphereTriList[121].v2 = 67;

		SphereTriList[122].v0 = 67;
		SphereTriList[122].v1 = 78;
		SphereTriList[122].v2 = 68;

		SphereTriList[123].v0 = 78;
		SphereTriList[123].v1 = 79;
		SphereTriList[123].v2 = 68;

		SphereTriList[124].v0 = 68;
		SphereTriList[124].v1 = 79;
		SphereTriList[124].v2 = 69;

		SphereTriList[125].v0 = 79;
		SphereTriList[125].v1 = 80;
		SphereTriList[125].v2 = 69;

		SphereTriList[126].v0 = 69;
		SphereTriList[126].v1 = 80;
		SphereTriList[126].v2 = 70;

		SphereTriList[127].v0 = 80;
		SphereTriList[127].v1 = 81;
		SphereTriList[127].v2 = 70;

		SphereTriList[128].v0 = 70;
		SphereTriList[128].v1 = 81;
		SphereTriList[128].v2 = 71;

		SphereTriList[129].v0 = 81;
		SphereTriList[129].v1 = 82;
		SphereTriList[129].v2 = 71;

		SphereTriList[130].v0 = 71;
		SphereTriList[130].v1 = 82;
		SphereTriList[130].v2 = 72;

		SphereTriList[131].v0 = 82;
		SphereTriList[131].v1 = 83;
		SphereTriList[131].v2 = 72;

		SphereTriList[132].v0 = 72;
		SphereTriList[132].v1 = 83;
		SphereTriList[132].v2 = 73;

		SphereTriList[133].v0 = 83;
		SphereTriList[133].v1 = 84;
		SphereTriList[133].v2 = 73;

		SphereTriList[134].v0 = 73;
		SphereTriList[134].v1 = 84;
		SphereTriList[134].v2 = 74;

		SphereTriList[135].v0 = 84;
		SphereTriList[135].v1 = 85;
		SphereTriList[135].v2 = 74;

		SphereTriList[136].v0 = 74;
		SphereTriList[136].v1 = 85;
		SphereTriList[136].v2 = 75;

		SphereTriList[137].v0 = 85;
		SphereTriList[137].v1 = 86;
		SphereTriList[137].v2 = 75;

		SphereTriList[138].v0 = 75;
		SphereTriList[138].v1 = 86;
		SphereTriList[138].v2 = 76;

		SphereTriList[139].v0 = 86;
		SphereTriList[139].v1 = 87;
		SphereTriList[139].v2 = 76;

		SphereTriList[140].v0 = 77;
		SphereTriList[140].v1 = 88;
		SphereTriList[140].v2 = 78;

		SphereTriList[141].v0 = 88;
		SphereTriList[141].v1 = 89;
		SphereTriList[141].v2 = 78;

		SphereTriList[142].v0 = 78;
		SphereTriList[142].v1 = 89;
		SphereTriList[142].v2 = 79;

		SphereTriList[143].v0 = 89;
		SphereTriList[143].v1 = 90;
		SphereTriList[143].v2 = 79;

		SphereTriList[144].v0 = 79;
		SphereTriList[144].v1 = 90;
		SphereTriList[144].v2 = 80;

		SphereTriList[145].v0 = 90;
		SphereTriList[145].v1 = 91;
		SphereTriList[145].v2 = 80;

		SphereTriList[146].v0 = 80;
		SphereTriList[146].v1 = 91;
		SphereTriList[146].v2 = 81;

		SphereTriList[147].v0 = 91;
		SphereTriList[147].v1 = 92;
		SphereTriList[147].v2 = 81;

		SphereTriList[148].v0 = 81;
		SphereTriList[148].v1 = 92;
		SphereTriList[148].v2 = 82;

		SphereTriList[149].v0 = 92;
		SphereTriList[149].v1 = 93;
		SphereTriList[149].v2 = 82;

		SphereTriList[150].v0 = 82;
		SphereTriList[150].v1 = 93;
		SphereTriList[150].v2 = 83;

		SphereTriList[151].v0 = 93;
		SphereTriList[151].v1 = 94;
		SphereTriList[151].v2 = 83;

		SphereTriList[152].v0 = 83;
		SphereTriList[152].v1 = 94;
		SphereTriList[152].v2 = 84;

		SphereTriList[153].v0 = 94;
		SphereTriList[153].v1 = 95;
		SphereTriList[153].v2 = 84;

		SphereTriList[154].v0 = 84;
		SphereTriList[154].v1 = 95;
		SphereTriList[154].v2 = 85;

		SphereTriList[155].v0 = 95;
		SphereTriList[155].v1 = 96;
		SphereTriList[155].v2 = 85;

		SphereTriList[156].v0 = 85;
		SphereTriList[156].v1 = 96;
		SphereTriList[156].v2 = 86;

		SphereTriList[157].v0 = 96;
		SphereTriList[157].v1 = 97;
		SphereTriList[157].v2 = 86;

		SphereTriList[158].v0 = 86;
		SphereTriList[158].v1 = 97;
		SphereTriList[158].v2 = 87;

		SphereTriList[159].v0 = 97;
		SphereTriList[159].v1 = 98;
		SphereTriList[159].v2 = 87;

		SphereTriList[160].v0 = 88;
		SphereTriList[160].v1 = 99;
		SphereTriList[160].v2 = 89;

		SphereTriList[161].v0 = 99;
		SphereTriList[161].v1 = 100;
		SphereTriList[161].v2 = 89;

		SphereTriList[162].v0 = 89;
		SphereTriList[162].v1 = 100;
		SphereTriList[162].v2 = 90;

		SphereTriList[163].v0 = 100;
		SphereTriList[163].v1 = 101;
		SphereTriList[163].v2 = 90;

		SphereTriList[164].v0 = 90;
		SphereTriList[164].v1 = 101;
		SphereTriList[164].v2 = 91;

		SphereTriList[165].v0 = 101;
		SphereTriList[165].v1 = 102;
		SphereTriList[165].v2 = 91;

		SphereTriList[166].v0 = 91;
		SphereTriList[166].v1 = 102;
		SphereTriList[166].v2 = 92;

		SphereTriList[167].v0 = 102;
		SphereTriList[167].v1 = 103;
		SphereTriList[167].v2 = 92;

		SphereTriList[168].v0 = 92;
		SphereTriList[168].v1 = 103;
		SphereTriList[168].v2 = 93;

		SphereTriList[169].v0 = 103;
		SphereTriList[169].v1 = 104;
		SphereTriList[169].v2 = 93;

		SphereTriList[170].v0 = 93;
		SphereTriList[170].v1 = 104;
		SphereTriList[170].v2 = 94;

		SphereTriList[171].v0 = 104;
		SphereTriList[171].v1 = 105;
		SphereTriList[171].v2 = 94;

		SphereTriList[172].v0 = 94;
		SphereTriList[172].v1 = 105;
		SphereTriList[172].v2 = 95;

		SphereTriList[173].v0 = 105;
		SphereTriList[173].v1 = 106;
		SphereTriList[173].v2 = 95;

		SphereTriList[174].v0 = 95;
		SphereTriList[174].v1 = 106;
		SphereTriList[174].v2 = 96;

		SphereTriList[175].v0 = 106;
		SphereTriList[175].v1 = 107;
		SphereTriList[175].v2 = 96;

		SphereTriList[176].v0 = 96;
		SphereTriList[176].v1 = 107;
		SphereTriList[176].v2 = 97;

		SphereTriList[177].v0 = 107;
		SphereTriList[177].v1 = 108;
		SphereTriList[177].v2 = 97;

		SphereTriList[178].v0 = 97;
		SphereTriList[178].v1 = 108;
		SphereTriList[178].v2 = 98;

		SphereTriList[179].v0 = 108;
		SphereTriList[179].v1 = 109;
		SphereTriList[179].v2 = 98;

		SphereTriList[180].v0 = 99;
		SphereTriList[180].v1 = 110;
		SphereTriList[180].v2 = 100;

		SphereTriList[181].v0 = 110;
		SphereTriList[181].v1 = 111;
		SphereTriList[181].v2 = 100;

		SphereTriList[182].v0 = 100;
		SphereTriList[182].v1 = 111;
		SphereTriList[182].v2 = 101;

		SphereTriList[183].v0 = 111;
		SphereTriList[183].v1 = 112;
		SphereTriList[183].v2 = 101;

		SphereTriList[184].v0 = 101;
		SphereTriList[184].v1 = 112;
		SphereTriList[184].v2 = 102;

		SphereTriList[185].v0 = 112;
		SphereTriList[185].v1 = 113;
		SphereTriList[185].v2 = 102;

		SphereTriList[186].v0 = 102;
		SphereTriList[186].v1 = 113;
		SphereTriList[186].v2 = 103;

		SphereTriList[187].v0 = 113;
		SphereTriList[187].v1 = 114;
		SphereTriList[187].v2 = 103;

		SphereTriList[188].v0 = 103;
		SphereTriList[188].v1 = 114;
		SphereTriList[188].v2 = 104;

		SphereTriList[189].v0 = 114;
		SphereTriList[189].v1 = 115;
		SphereTriList[189].v2 = 104;

		SphereTriList[190].v0 = 104;
		SphereTriList[190].v1 = 115;
		SphereTriList[190].v2 = 105;

		SphereTriList[191].v0 = 115;
		SphereTriList[191].v1 = 116;
		SphereTriList[191].v2 = 105;

		SphereTriList[192].v0 = 105;
		SphereTriList[192].v1 = 116;
		SphereTriList[192].v2 = 106;

		SphereTriList[193].v0 = 116;
		SphereTriList[193].v1 = 117;
		SphereTriList[193].v2 = 106;

		SphereTriList[194].v0 = 106;
		SphereTriList[194].v1 = 117;
		SphereTriList[194].v2 = 107;

		SphereTriList[195].v0 = 117;
		SphereTriList[195].v1 = 118;
		SphereTriList[195].v2 = 107;

		SphereTriList[196].v0 = 107;
		SphereTriList[196].v1 = 118;
		SphereTriList[196].v2 = 108;

		SphereTriList[197].v0 = 118;
		SphereTriList[197].v1 = 119;
		SphereTriList[197].v2 = 108;

		SphereTriList[198].v0 = 108;
		SphereTriList[198].v1 = 119;
		SphereTriList[198].v2 = 109;

		SphereTriList[199].v0 = 119;
		SphereTriList[199].v1 = 120;
		SphereTriList[199].v2 = 109;

		//--------------------------------------------------
		//--------------------------------------------------
		//--------------------------------------------------

		SphereData[0].x =  -0.000000f;
		SphereData[0].y =  0.000000f;
		SphereData[0].z =  0.500000f;
		SphereData[0].nx = -1.0f * -0.000000f;
		SphereData[0].ny = -1.0f * 0.000000f;
		SphereData[0].nz = -1.0f * 1.000000f;
		SphereData[0].u = 0.000000f;
		SphereData[0].v = 1.000000f;

		SphereData[1].x =  -0.000000f;
		SphereData[1].y =  0.154509f;
		SphereData[1].z =  0.475528f;
		SphereData[1].nx = -1.0f * -0.000000f;
		SphereData[1].ny = -1.0f * 0.309017f;
		SphereData[1].nz = -1.0f * 0.951057f;
		SphereData[1].u = 0.000000f;
		SphereData[1].v = 0.900000f;

		SphereData[2].x =  -0.000000f;
		SphereData[2].y =  0.000000f;
		SphereData[2].z =  0.500000f;
		SphereData[2].nx = -1.0f * -0.000000f;
		SphereData[2].ny = -1.0f * 0.000000f;
		SphereData[2].nz = -1.0f * 1.000000f;
		SphereData[2].u = 0.100000f;
		SphereData[2].v = 1.000000f;

		SphereData[3].x =  -0.090818f;
		SphereData[3].y =  0.125000f;
		SphereData[3].z =  0.475528f;
		SphereData[3].nx = -1.0f * -0.181636f;
		SphereData[3].ny = -1.0f * 0.250000f;
		SphereData[3].nz = -1.0f * 0.951057f;
		SphereData[3].u = 0.100000f;
		SphereData[3].v = 0.900000f;

		SphereData[4].x =  -0.000000f;
		SphereData[4].y =  0.000000f;
		SphereData[4].z =  0.500000f;
		SphereData[4].nx = -1.0f * -0.000000f;
		SphereData[4].ny = -1.0f * 0.000000f;
		SphereData[4].nz = -1.0f * 1.000000f;
		SphereData[4].u = 0.200000f;
		SphereData[4].v = 1.000000f;

		SphereData[5].x =  -0.146946f;
		SphereData[5].y =  0.047746f;
		SphereData[5].z =  0.475528f;
		SphereData[5].nx = -1.0f * -0.293893f;
		SphereData[5].ny = -1.0f * 0.095491f;
		SphereData[5].nz = -1.0f * 0.951057f;
		SphereData[5].u = 0.200000f;
		SphereData[5].v = 0.900000f;

		SphereData[6].x =  -0.000000f;
		SphereData[6].y =  -0.000000f;
		SphereData[6].z =  0.500000f;
		SphereData[6].nx = -1.0f * -0.000000f;
		SphereData[6].ny = -1.0f * -0.000000f;
		SphereData[6].nz = -1.0f * 1.000000f;
		SphereData[6].u = 0.300000f;
		SphereData[6].v = 1.000000f;

		SphereData[7].x =  -0.146946f;
		SphereData[7].y =  -0.047746f;
		SphereData[7].z =  0.475528f;
		SphereData[7].nx = -1.0f * -0.293893f;
		SphereData[7].ny = -1.0f * -0.095492f;
		SphereData[7].nz = -1.0f * 0.951057f;
		SphereData[7].u = 0.300000f;
		SphereData[7].v = 0.900000f;

		SphereData[8].x =  -0.000000f;
		SphereData[8].y =  -0.000000f;
		SphereData[8].z =  0.500000f;
		SphereData[8].nx = -1.0f * -0.000000f;
		SphereData[8].ny = -1.0f * -0.000000f;
		SphereData[8].nz = -1.0f * 1.000000f;
		SphereData[8].u = 0.400000f;
		SphereData[8].v = 1.000000f;

		SphereData[9].x =  -0.090818f;
		SphereData[9].y =  -0.125000f;
		SphereData[9].z =  0.475528f;
		SphereData[9].nx = -1.0f * -0.181636f;
		SphereData[9].ny = -1.0f * -0.250000f;
		SphereData[9].nz = -1.0f * 0.951057f;
		SphereData[9].u = 0.400000f;
		SphereData[9].v = 0.900000f;

		SphereData[10].x =  0.000000f;
		SphereData[10].y =  -0.000000f;
		SphereData[10].z =  0.500000f;
		SphereData[10].nx = -1.0f * 0.000000f;
		SphereData[10].ny = -1.0f * -0.000000f;
		SphereData[10].nz = -1.0f * 1.000000f;
		SphereData[10].u = 0.500000f;
		SphereData[10].v = 1.000000f;

		SphereData[11].x =  0.000000f;
		SphereData[11].y =  -0.154509f;
		SphereData[11].z =  0.475528f;
		SphereData[11].nx = -1.0f * 0.000000f;
		SphereData[11].ny = -1.0f * -0.309017f;
		SphereData[11].nz = -1.0f * 0.951057f;
		SphereData[11].u = 0.500000f;
		SphereData[11].v = 0.900000f;

		SphereData[12].x =  0.000000f;
		SphereData[12].y =  -0.000000f;
		SphereData[12].z =  0.500000f;
		SphereData[12].nx = -1.0f * 0.000000f;
		SphereData[12].ny = -1.0f * -0.000000f;
		SphereData[12].nz = -1.0f * 1.000000f;
		SphereData[12].u = 0.600000f;
		SphereData[12].v = 1.000000f;

		SphereData[13].x =  0.090818f;
		SphereData[13].y =  -0.125000f;
		SphereData[13].z =  0.475528f;
		SphereData[13].nx = -1.0f * 0.181636f;
		SphereData[13].ny = -1.0f * -0.250000f;
		SphereData[13].nz = -1.0f * 0.951057f;
		SphereData[13].u = 0.600000f;
		SphereData[13].v = 0.900000f;

		SphereData[14].x =  0.000000f;
		SphereData[14].y =  -0.000000f;
		SphereData[14].z =  0.500000f;
		SphereData[14].nx = -1.0f * 0.000000f;
		SphereData[14].ny = -1.0f * -0.000000f;
		SphereData[14].nz = -1.0f * 1.000000f;
		SphereData[14].u = 0.700000f;
		SphereData[14].v = 1.000000f;

		SphereData[15].x =  0.146946f;
		SphereData[15].y =  -0.047746f;
		SphereData[15].z =  0.475528f;
		SphereData[15].nx = -1.0f * 0.293893f;
		SphereData[15].ny = -1.0f * -0.095492f;
		SphereData[15].nz = -1.0f * 0.951057f;
		SphereData[15].u = 0.700000f;
		SphereData[15].v = 0.900000f;

		SphereData[16].x =  0.000000f;
		SphereData[16].y =  0.000000f;
		SphereData[16].z =  0.500000f;
		SphereData[16].nx = -1.0f * 0.000000f;
		SphereData[16].ny = -1.0f * 0.000000f;
		SphereData[16].nz = -1.0f * 1.000000f;
		SphereData[16].u = 0.800000f;
		SphereData[16].v = 1.000000f;

		SphereData[17].x =  0.146946f;
		SphereData[17].y =  0.047746f;
		SphereData[17].z =  0.475528f;
		SphereData[17].nx = -1.0f * 0.293893f;
		SphereData[17].ny = -1.0f * 0.095492f;
		SphereData[17].nz = -1.0f * 0.951057f;
		SphereData[17].u = 0.800000f;
		SphereData[17].v = 0.900000f;

		SphereData[18].x =  0.000000f;
		SphereData[18].y =  0.000000f;
		SphereData[18].z =  0.500000f;
		SphereData[18].nx = -1.0f * 0.000000f;
		SphereData[18].ny = -1.0f * 0.000000f;
		SphereData[18].nz = -1.0f * 1.000000f;
		SphereData[18].u = 0.900000f;
		SphereData[18].v = 1.000000f;

		SphereData[19].x =  0.090818f;
		SphereData[19].y =  0.125000f;
		SphereData[19].z =  0.475528f;
		SphereData[19].nx = -1.0f * 0.181636f;
		SphereData[19].ny = -1.0f * 0.250000f;
		SphereData[19].nz = -1.0f * 0.951057f;
		SphereData[19].u = 0.900000f;
		SphereData[19].v = 0.900000f;

		SphereData[20].x =  -0.000000f;
		SphereData[20].y =  0.000000f;
		SphereData[20].z =  0.500000f;
		SphereData[20].nx = -1.0f * -0.000000f;
		SphereData[20].ny = -1.0f * 0.000000f;
		SphereData[20].nz = -1.0f * 1.000000f;
		SphereData[20].u = 1.000000f;
		SphereData[20].v = 1.000000f;

		SphereData[21].x =  -0.000000f;
		SphereData[21].y =  0.154509f;
		SphereData[21].z =  0.475528f;
		SphereData[21].nx = -1.0f * -0.000000f;
		SphereData[21].ny = -1.0f * 0.309017f;
		SphereData[21].nz = -1.0f * 0.951057f;
		SphereData[21].u = 1.000000f;
		SphereData[21].v = 0.900000f;

		SphereData[22].x =  -0.000000f;
		SphereData[22].y =  0.293893f;
		SphereData[22].z =  0.404509f;
		SphereData[22].nx = -1.0f * -0.000000f;
		SphereData[22].ny = -1.0f * 0.587785f;
		SphereData[22].nz = -1.0f * 0.809017f;
		SphereData[22].u = 0.000000f;
		SphereData[22].v = 0.800000f;

		SphereData[23].x =  -0.172746f;
		SphereData[23].y =  0.237764f;
		SphereData[23].z =  0.404509f;
		SphereData[23].nx = -1.0f * -0.345491f;
		SphereData[23].ny = -1.0f * 0.475528f;
		SphereData[23].nz = -1.0f * 0.809017f;
		SphereData[23].u = 0.100000f;
		SphereData[23].v = 0.800000f;

		SphereData[24].x =  -0.279509f;
		SphereData[24].y =  0.090818f;
		SphereData[24].z =  0.404509f;
		SphereData[24].nx = -1.0f * -0.559017f;
		SphereData[24].ny = -1.0f * 0.181636f;
		SphereData[24].nz = -1.0f * 0.809017f;
		SphereData[24].u = 0.200000f;
		SphereData[24].v = 0.800000f;

		SphereData[25].x =  -0.279508f;
		SphereData[25].y =  -0.090818f;
		SphereData[25].z =  0.404509f;
		SphereData[25].nx = -1.0f * -0.559017f;
		SphereData[25].ny = -1.0f * -0.181636f;
		SphereData[25].nz = -1.0f * 0.809017f;
		SphereData[25].u = 0.300000f;
		SphereData[25].v = 0.800000f;

		SphereData[26].x =  -0.172746f;
		SphereData[26].y =  -0.237764f;
		SphereData[26].z =  0.404509f;
		SphereData[26].nx = -1.0f * -0.345491f;
		SphereData[26].ny = -1.0f * -0.475528f;
		SphereData[26].nz = -1.0f * 0.809017f;
		SphereData[26].u = 0.400000f;
		SphereData[26].v = 0.800000f;

		SphereData[27].x =  0.000000f;
		SphereData[27].y =  -0.293893f;
		SphereData[27].z =  0.404509f;
		SphereData[27].nx = -1.0f * 0.000000f;
		SphereData[27].ny = -1.0f * -0.587785f;
		SphereData[27].nz = -1.0f * 0.809017f;
		SphereData[27].u = 0.500000f;
		SphereData[27].v = 0.800000f;

		SphereData[28].x =  0.172746f;
		SphereData[28].y =  -0.237764f;
		SphereData[28].z =  0.404509f;
		SphereData[28].nx = -1.0f * 0.345492f;
		SphereData[28].ny = -1.0f * -0.475528f;
		SphereData[28].nz = -1.0f * 0.809017f;
		SphereData[28].u = 0.600000f;
		SphereData[28].v = 0.800000f;

		SphereData[29].x =  0.279508f;
		SphereData[29].y =  -0.090818f;
		SphereData[29].z =  0.404509f;
		SphereData[29].nx = -1.0f * 0.559017f;
		SphereData[29].ny = -1.0f * -0.181636f;
		SphereData[29].nz = -1.0f * 0.809017f;
		SphereData[29].u = 0.700000f;
		SphereData[29].v = 0.800000f;

		SphereData[30].x =  0.279508f;
		SphereData[30].y =  0.090818f;
		SphereData[30].z =  0.404509f;
		SphereData[30].nx = -1.0f * 0.559017f;
		SphereData[30].ny = -1.0f * 0.181636f;
		SphereData[30].nz = -1.0f * 0.809017f;
		SphereData[30].u = 0.800000f;
		SphereData[30].v = 0.800000f;

		SphereData[31].x =  0.172746f;
		SphereData[31].y =  0.237764f;
		SphereData[31].z =  0.404509f;
		SphereData[31].nx = -1.0f * 0.345491f;
		SphereData[31].ny = -1.0f * 0.475528f;
		SphereData[31].nz = -1.0f * 0.809017f;
		SphereData[31].u = 0.900000f;
		SphereData[31].v = 0.800000f;

		SphereData[32].x =  -0.000000f;
		SphereData[32].y =  0.293893f;
		SphereData[32].z =  0.404509f;
		SphereData[32].nx = -1.0f * -0.000000f;
		SphereData[32].ny = -1.0f * 0.587785f;
		SphereData[32].nz = -1.0f * 0.809017f;
		SphereData[32].u = 1.000000f;
		SphereData[32].v = 0.800000f;

		SphereData[33].x =  -0.000000f;
		SphereData[33].y =  0.404509f;
		SphereData[33].z =  0.293893f;
		SphereData[33].nx = -1.0f * -0.000000f;
		SphereData[33].ny = -1.0f * 0.809017f;
		SphereData[33].nz = -1.0f * 0.587785f;
		SphereData[33].u = 0.000000f;
		SphereData[33].v = 0.700000f;

		SphereData[34].x =  -0.237764f;
		SphereData[34].y =  0.327254f;
		SphereData[34].z =  0.293893f;
		SphereData[34].nx = -1.0f * -0.475528f;
		SphereData[34].ny = -1.0f * 0.654509f;
		SphereData[34].nz = -1.0f * 0.587785f;
		SphereData[34].u = 0.100000f;
		SphereData[34].v = 0.700000f;

		SphereData[35].x =  -0.384710f;
		SphereData[35].y =  0.125000f;
		SphereData[35].z =  0.293893f;
		SphereData[35].nx = -1.0f * -0.769421f;
		SphereData[35].ny = -1.0f * 0.250000f;
		SphereData[35].nz = -1.0f * 0.587785f;
		SphereData[35].u = 0.200000f;
		SphereData[35].v = 0.700000f;

		SphereData[36].x =  -0.384710f;
		SphereData[36].y =  -0.125000f;
		SphereData[36].z =  0.293893f;
		SphereData[36].nx = -1.0f * -0.769421f;
		SphereData[36].ny = -1.0f * -0.250000f;
		SphereData[36].nz = -1.0f * 0.587785f;
		SphereData[36].u = 0.300000f;
		SphereData[36].v = 0.700000f;

		SphereData[37].x =  -0.237764f;
		SphereData[37].y =  -0.327254f;
		SphereData[37].z =  0.293893f;
		SphereData[37].nx = -1.0f * -0.475528f;
		SphereData[37].ny = -1.0f * -0.654509f;
		SphereData[37].nz = -1.0f * 0.587785f;
		SphereData[37].u = 0.400000f;
		SphereData[37].v = 0.700000f;

		SphereData[38].x =  0.000000f;
		SphereData[38].y =  -0.404509f;
		SphereData[38].z =  0.293893f;
		SphereData[38].nx = -1.0f * 0.000000f;
		SphereData[38].ny = -1.0f * -0.809017f;
		SphereData[38].nz = -1.0f * 0.587785f;
		SphereData[38].u = 0.500000f;
		SphereData[38].v = 0.700000f;

		SphereData[39].x =  0.237764f;
		SphereData[39].y =  -0.327254f;
		SphereData[39].z =  0.293893f;
		SphereData[39].nx = -1.0f * 0.475528f;
		SphereData[39].ny = -1.0f * -0.654508f;
		SphereData[39].nz = -1.0f * 0.587785f;
		SphereData[39].u = 0.600000f;
		SphereData[39].v = 0.700000f;

		SphereData[40].x =  0.384710f;
		SphereData[40].y =  -0.125000f;
		SphereData[40].z =  0.293893f;
		SphereData[40].nx = -1.0f * 0.769421f;
		SphereData[40].ny = -1.0f * -0.250000f;
		SphereData[40].nz = -1.0f * 0.587785f;
		SphereData[40].u = 0.700000f;
		SphereData[40].v = 0.700000f;

		SphereData[41].x =  0.384710f;
		SphereData[41].y =  0.125000f;
		SphereData[41].z =  0.293893f;
		SphereData[41].nx = -1.0f * 0.769421f;
		SphereData[41].ny = -1.0f * 0.250000f;
		SphereData[41].nz = -1.0f * 0.587785f;
		SphereData[41].u = 0.800000f;
		SphereData[41].v = 0.700000f;

		SphereData[42].x =  0.237764f;
		SphereData[42].y =  0.327254f;
		SphereData[42].z =  0.293893f;
		SphereData[42].nx = -1.0f * 0.475528f;
		SphereData[42].ny = -1.0f * 0.654509f;
		SphereData[42].nz = -1.0f * 0.587785f;
		SphereData[42].u = 0.900000f;
		SphereData[42].v = 0.700000f;

		SphereData[43].x =  -0.000000f;
		SphereData[43].y =  0.404509f;
		SphereData[43].z =  0.293893f;
		SphereData[43].nx = -1.0f * -0.000000f;
		SphereData[43].ny = -1.0f * 0.809017f;
		SphereData[43].nz = -1.0f * 0.587785f;
		SphereData[43].u = 1.000000f;
		SphereData[43].v = 0.700000f;

		SphereData[44].x =  -0.000000f;
		SphereData[44].y =  0.475528f;
		SphereData[44].z =  0.154508f;
		SphereData[44].nx = -1.0f * -0.000000f;
		SphereData[44].ny = -1.0f * 0.951057f;
		SphereData[44].nz = -1.0f * 0.309017f;
		SphereData[44].u = 0.000000f;
		SphereData[44].v = 0.600000f;

		SphereData[45].x =  -0.279509f;
		SphereData[45].y =  0.384710f;
		SphereData[45].z =  0.154508f;
		SphereData[45].nx = -1.0f * -0.559017f;
		SphereData[45].ny = -1.0f * 0.769421f;
		SphereData[45].nz = -1.0f * 0.309017f;
		SphereData[45].u = 0.100000f;
		SphereData[45].v = 0.600000f;

		SphereData[46].x =  -0.452254f;
		SphereData[46].y =  0.146946f;
		SphereData[46].z =  0.154508f;
		SphereData[46].nx = -1.0f * -0.904509f;
		SphereData[46].ny = -1.0f * 0.293893f;
		SphereData[46].nz = -1.0f * 0.309017f;
		SphereData[46].u = 0.200000f;
		SphereData[46].v = 0.600000f;

		SphereData[47].x =  -0.452254f;
		SphereData[47].y =  -0.146946f;
		SphereData[47].z =  0.154508f;
		SphereData[47].nx = -1.0f * -0.904508f;
		SphereData[47].ny = -1.0f * -0.293893f;
		SphereData[47].nz = -1.0f * 0.309017f;
		SphereData[47].u = 0.300000f;
		SphereData[47].v = 0.600000f;

		SphereData[48].x =  -0.279508f;
		SphereData[48].y =  -0.384710f;
		SphereData[48].z =  0.154508f;
		SphereData[48].nx = -1.0f * -0.559017f;
		SphereData[48].ny = -1.0f * -0.769421f;
		SphereData[48].nz = -1.0f * 0.309017f;
		SphereData[48].u = 0.400000f;
		SphereData[48].v = 0.600000f;

		SphereData[49].x =  0.000000f;
		SphereData[49].y =  -0.475528f;
		SphereData[49].z =  0.154508f;
		SphereData[49].nx = -1.0f * 0.000000f;
		SphereData[49].ny = -1.0f * -0.951057f;
		SphereData[49].nz = -1.0f * 0.309017f;
		SphereData[49].u = 0.500000f;
		SphereData[49].v = 0.600000f;

		SphereData[50].x =  0.279509f;
		SphereData[50].y =  -0.384710f;
		SphereData[50].z =  0.154508f;
		SphereData[50].nx = -1.0f * 0.559017f;
		SphereData[50].ny = -1.0f * -0.769421f;
		SphereData[50].nz = -1.0f * 0.309017f;
		SphereData[50].u = 0.600000f;
		SphereData[50].v = 0.600000f;

		SphereData[51].x =  0.452254f;
		SphereData[51].y =  -0.146946f;
		SphereData[51].z =  0.154508f;
		SphereData[51].nx = -1.0f * 0.904508f;
		SphereData[51].ny = -1.0f * -0.293893f;
		SphereData[51].nz = -1.0f * 0.309017f;
		SphereData[51].u = 0.700000f;
		SphereData[51].v = 0.600000f;

		SphereData[52].x =  0.452254f;
		SphereData[52].y =  0.146946f;
		SphereData[52].z =  0.154508f;
		SphereData[52].nx = -1.0f * 0.904508f;
		SphereData[52].ny = -1.0f * 0.293893f;
		SphereData[52].nz = -1.0f * 0.309017f;
		SphereData[52].u = 0.800000f;
		SphereData[52].v = 0.600000f;

		SphereData[53].x =  0.279508f;
		SphereData[53].y =  0.384711f;
		SphereData[53].z =  0.154508f;
		SphereData[53].nx = -1.0f * 0.559017f;
		SphereData[53].ny = -1.0f * 0.769421f;
		SphereData[53].nz = -1.0f * 0.309017f;
		SphereData[53].u = 0.900000f;
		SphereData[53].v = 0.600000f;

		SphereData[54].x =  -0.000000f;
		SphereData[54].y =  0.475528f;
		SphereData[54].z =  0.154508f;
		SphereData[54].nx = -1.0f * -0.000000f;
		SphereData[54].ny = -1.0f * 0.951057f;
		SphereData[54].nz = -1.0f * 0.309017f;
		SphereData[54].u = 1.000000f;
		SphereData[54].v = 0.600000f;

		SphereData[55].x =  -0.000000f;
		SphereData[55].y =  0.500000f;
		SphereData[55].z =  -0.000000f;
		SphereData[55].nx = -1.0f * -0.000000f;
		SphereData[55].ny = -1.0f * 1.000000f;
		SphereData[55].nz = -1.0f * -0.000000f;
		SphereData[55].u = 0.000000f;
		SphereData[55].v = 0.500000f;

		SphereData[56].x =  -0.293893f;
		SphereData[56].y =  0.404509f;
		SphereData[56].z =  -0.000000f;
		SphereData[56].nx = -1.0f * -0.587785f;
		SphereData[56].ny = -1.0f * 0.809017f;
		SphereData[56].nz = -1.0f * -0.000000f;
		SphereData[56].u = 0.100000f;
		SphereData[56].v = 0.500000f;

		SphereData[57].x =  -0.475528f;
		SphereData[57].y =  0.154508f;
		SphereData[57].z =  -0.000000f;
		SphereData[57].nx = -1.0f * -0.951057f;
		SphereData[57].ny = -1.0f * 0.309017f;
		SphereData[57].nz = -1.0f * -0.000000f;
		SphereData[57].u = 0.200000f;
		SphereData[57].v = 0.500000f;

		SphereData[58].x =  -0.475528f;
		SphereData[58].y =  -0.154509f;
		SphereData[58].z =  -0.000000f;
		SphereData[58].nx = -1.0f * -0.951057f;
		SphereData[58].ny = -1.0f * -0.309017f;
		SphereData[58].nz = -1.0f * -0.000000f;
		SphereData[58].u = 0.300000f;
		SphereData[58].v = 0.500000f;

		SphereData[59].x =  -0.293893f;
		SphereData[59].y =  -0.404509f;
		SphereData[59].z =  -0.000000f;
		SphereData[59].nx = -1.0f * -0.587785f;
		SphereData[59].ny = -1.0f * -0.809017f;
		SphereData[59].nz = -1.0f * -0.000000f;
		SphereData[59].u = 0.400000f;
		SphereData[59].v = 0.500000f;

		SphereData[60].x =  0.000000f;
		SphereData[60].y =  -0.500000f;
		SphereData[60].z =  -0.000000f;
		SphereData[60].nx = -1.0f * 0.000000f;
		SphereData[60].ny = -1.0f * -1.000000f;
		SphereData[60].nz = -1.0f * -0.000000f;
		SphereData[60].u = 0.500000f;
		SphereData[60].v = 0.500000f;

		SphereData[61].x =  0.293893f;
		SphereData[61].y =  -0.404508f;
		SphereData[61].z =  -0.000000f;
		SphereData[61].nx = -1.0f * 0.587785f;
		SphereData[61].ny = -1.0f * -0.809017f;
		SphereData[61].nz = -1.0f * -0.000000f;
		SphereData[61].u = 0.600000f;
		SphereData[61].v = 0.500000f;

		SphereData[62].x =  0.475528f;
		SphereData[62].y =  -0.154509f;
		SphereData[62].z =  -0.000000f;
		SphereData[62].nx = -1.0f * 0.951056f;
		SphereData[62].ny = -1.0f * -0.309017f;
		SphereData[62].nz = -1.0f * -0.000000f;
		SphereData[62].u = 0.700000f;
		SphereData[62].v = 0.500000f;

		SphereData[63].x =  0.475528f;
		SphereData[63].y =  0.154509f;
		SphereData[63].z =  -0.000000f;
		SphereData[63].nx = -1.0f * 0.951056f;
		SphereData[63].ny = -1.0f * 0.309017f;
		SphereData[63].nz = -1.0f * -0.000000f;
		SphereData[63].u = 0.800000f;
		SphereData[63].v = 0.500000f;

		SphereData[64].x =  0.293892f;
		SphereData[64].y =  0.404509f;
		SphereData[64].z =  -0.000000f;
		SphereData[64].nx = -1.0f * 0.587785f;
		SphereData[64].ny = -1.0f * 0.809017f;
		SphereData[64].nz = -1.0f * -0.000000f;
		SphereData[64].u = 0.900000f;
		SphereData[64].v = 0.500000f;

		SphereData[65].x =  -0.000000f;
		SphereData[65].y =  0.500000f;
		SphereData[65].z =  -0.000000f;
		SphereData[65].nx = -1.0f * -0.000000f;
		SphereData[65].ny = -1.0f * 1.000000f;
		SphereData[65].nz = -1.0f * -0.000000f;
		SphereData[65].u = 1.000000f;
		SphereData[65].v = 0.500000f;

		SphereData[66].x =  -0.000000f;
		SphereData[66].y =  0.475528f;
		SphereData[66].z =  -0.154509f;
		SphereData[66].nx = -1.0f * -0.000000f;
		SphereData[66].ny = -1.0f * 0.951057f;
		SphereData[66].nz = -1.0f * -0.309017f;
		SphereData[66].u = 0.000000f;
		SphereData[66].v = 0.400000f;

		SphereData[67].x =  -0.279508f;
		SphereData[67].y =  0.384710f;
		SphereData[67].z =  -0.154509f;
		SphereData[67].nx = -1.0f * -0.559017f;
		SphereData[67].ny = -1.0f * 0.769421f;
		SphereData[67].nz = -1.0f * -0.309017f;
		SphereData[67].u = 0.100000f;
		SphereData[67].v = 0.400000f;

		SphereData[68].x =  -0.452254f;
		SphereData[68].y =  0.146946f;
		SphereData[68].z =  -0.154509f;
		SphereData[68].nx = -1.0f * -0.904509f;
		SphereData[68].ny = -1.0f * 0.293893f;
		SphereData[68].nz = -1.0f * -0.309017f;
		SphereData[68].u = 0.200000f;
		SphereData[68].v = 0.400000f;

		SphereData[69].x =  -0.452254f;
		SphereData[69].y =  -0.146946f;
		SphereData[69].z =  -0.154509f;
		SphereData[69].nx = -1.0f * -0.904509f;
		SphereData[69].ny = -1.0f * -0.293893f;
		SphereData[69].nz = -1.0f * -0.309017f;
		SphereData[69].u = 0.300000f;
		SphereData[69].v = 0.400000f;

		SphereData[70].x =  -0.279508f;
		SphereData[70].y =  -0.384710f;
		SphereData[70].z =  -0.154509f;
		SphereData[70].nx = -1.0f * -0.559017f;
		SphereData[70].ny = -1.0f * -0.769421f;
		SphereData[70].nz = -1.0f * -0.309017f;
		SphereData[70].u = 0.400000f;
		SphereData[70].v = 0.400000f;

		SphereData[71].x =  0.000000f;
		SphereData[71].y =  -0.475528f;
		SphereData[71].z =  -0.154509f;
		SphereData[71].nx = -1.0f * 0.000000f;
		SphereData[71].ny = -1.0f * -0.951057f;
		SphereData[71].nz = -1.0f * -0.309017f;
		SphereData[71].u = 0.500000f;
		SphereData[71].v = 0.400000f;

		SphereData[72].x =  0.279509f;
		SphereData[72].y =  -0.384710f;
		SphereData[72].z =  -0.154509f;
		SphereData[72].nx = -1.0f * 0.559017f;
		SphereData[72].ny = -1.0f * -0.769421f;
		SphereData[72].nz = -1.0f * -0.309017f;
		SphereData[72].u = 0.600000f;
		SphereData[72].v = 0.400000f;

		SphereData[73].x =  0.452254f;
		SphereData[73].y =  -0.146946f;
		SphereData[73].z =  -0.154509f;
		SphereData[73].nx = -1.0f * 0.904509f;
		SphereData[73].ny = -1.0f * -0.293893f;
		SphereData[73].nz = -1.0f * -0.309017f;
		SphereData[73].u = 0.700000f;
		SphereData[73].v = 0.400000f;

		SphereData[74].x =  0.452254f;
		SphereData[74].y =  0.146946f;
		SphereData[74].z =  -0.154509f;
		SphereData[74].nx = -1.0f * 0.904509f;
		SphereData[74].ny = -1.0f * 0.293893f;
		SphereData[74].nz = -1.0f * -0.309017f;
		SphereData[74].u = 0.800000f;
		SphereData[74].v = 0.400000f;

		SphereData[75].x =  0.279508f;
		SphereData[75].y =  0.384711f;
		SphereData[75].z =  -0.154509f;
		SphereData[75].nx = -1.0f * 0.559017f;
		SphereData[75].ny = -1.0f * 0.769421f;
		SphereData[75].nz = -1.0f * -0.309017f;
		SphereData[75].u = 0.900000f;
		SphereData[75].v = 0.400000f;

		SphereData[76].x =  -0.000000f;
		SphereData[76].y =  0.475528f;
		SphereData[76].z =  -0.154509f;
		SphereData[76].nx = -1.0f * -0.000000f;
		SphereData[76].ny = -1.0f * 0.951057f;
		SphereData[76].nz = -1.0f * -0.309017f;
		SphereData[76].u = 1.000000f;
		SphereData[76].v = 0.400000f;

		SphereData[77].x =  -0.000000f;
		SphereData[77].y =  0.404509f;
		SphereData[77].z =  -0.293893f;
		SphereData[77].nx = -1.0f * -0.000000f;
		SphereData[77].ny = -1.0f * 0.809017f;
		SphereData[77].nz = -1.0f * -0.587785f;
		SphereData[77].u = 0.000000f;
		SphereData[77].v = 0.300000f;

		SphereData[78].x =  -0.237764f;
		SphereData[78].y =  0.327254f;
		SphereData[78].z =  -0.293893f;
		SphereData[78].nx = -1.0f * -0.475528f;
		SphereData[78].ny = -1.0f * 0.654509f;
		SphereData[78].nz = -1.0f * -0.587785f;
		SphereData[78].u = 0.100000f;
		SphereData[78].v = 0.300000f;

		SphereData[79].x =  -0.384710f;
		SphereData[79].y =  0.125000f;
		SphereData[79].z =  -0.293893f;
		SphereData[79].nx = -1.0f * -0.769421f;
		SphereData[79].ny = -1.0f * 0.250000f;
		SphereData[79].nz = -1.0f * -0.587785f;
		SphereData[79].u = 0.200000f;
		SphereData[79].v = 0.300000f;

		SphereData[80].x =  -0.384710f;
		SphereData[80].y =  -0.125000f;
		SphereData[80].z =  -0.293893f;
		SphereData[80].nx = -1.0f * -0.769421f;
		SphereData[80].ny = -1.0f * -0.250000f;
		SphereData[80].nz = -1.0f * -0.587785f;
		SphereData[80].u = 0.300000f;
		SphereData[80].v = 0.300000f;

		SphereData[81].x =  -0.237764f;
		SphereData[81].y =  -0.327254f;
		SphereData[81].z =  -0.293893f;
		SphereData[81].nx = -1.0f * -0.475528f;
		SphereData[81].ny = -1.0f * -0.654509f;
		SphereData[81].nz = -1.0f * -0.587785f;
		SphereData[81].u = 0.400000f;
		SphereData[81].v = 0.300000f;

		SphereData[82].x =  0.000000f;
		SphereData[82].y =  -0.404509f;
		SphereData[82].z =  -0.293893f;
		SphereData[82].nx = -1.0f * 0.000000f;
		SphereData[82].ny = -1.0f * -0.809017f;
		SphereData[82].nz = -1.0f * -0.587785f;
		SphereData[82].u = 0.500000f;
		SphereData[82].v = 0.300000f;

		SphereData[83].x =  0.237764f;
		SphereData[83].y =  -0.327254f;
		SphereData[83].z =  -0.293893f;
		SphereData[83].nx = -1.0f * 0.475528f;
		SphereData[83].ny = -1.0f * -0.654508f;
		SphereData[83].nz = -1.0f * -0.587785f;
		SphereData[83].u = 0.600000f;
		SphereData[83].v = 0.300000f;

		SphereData[84].x =  0.384710f;
		SphereData[84].y =  -0.125000f;
		SphereData[84].z =  -0.293893f;
		SphereData[84].nx = -1.0f * 0.769421f;
		SphereData[84].ny = -1.0f * -0.250000f;
		SphereData[84].nz = -1.0f * -0.587785f;
		SphereData[84].u = 0.700000f;
		SphereData[84].v = 0.300000f;

		SphereData[85].x =  0.384710f;
		SphereData[85].y =  0.125000f;
		SphereData[85].z =  -0.293893f;
		SphereData[85].nx = -1.0f * 0.769421f;
		SphereData[85].ny = -1.0f * 0.250000f;
		SphereData[85].nz = -1.0f * -0.587785f;
		SphereData[85].u = 0.800000f;
		SphereData[85].v = 0.300000f;

		SphereData[86].x =  0.237764f;
		SphereData[86].y =  0.327254f;
		SphereData[86].z =  -0.293893f;
		SphereData[86].nx = -1.0f * 0.475528f;
		SphereData[86].ny = -1.0f * 0.654509f;
		SphereData[86].nz = -1.0f * -0.587785f;
		SphereData[86].u = 0.900000f;
		SphereData[86].v = 0.300000f;

		SphereData[87].x =  -0.000000f;
		SphereData[87].y =  0.404509f;
		SphereData[87].z =  -0.293893f;
		SphereData[87].nx = -1.0f * -0.000000f;
		SphereData[87].ny = -1.0f * 0.809017f;
		SphereData[87].nz = -1.0f * -0.587785f;
		SphereData[87].u = 1.000000f;
		SphereData[87].v = 0.300000f;

		SphereData[88].x =  -0.000000f;
		SphereData[88].y =  0.293893f;
		SphereData[88].z =  -0.404509f;
		SphereData[88].nx = -1.0f * -0.000000f;
		SphereData[88].ny = -1.0f * 0.587785f;
		SphereData[88].nz = -1.0f * -0.809017f;
		SphereData[88].u = 0.000000f;
		SphereData[88].v = 0.200000f;

		SphereData[89].x =  -0.172746f;
		SphereData[89].y =  0.237764f;
		SphereData[89].z =  -0.404509f;
		SphereData[89].nx = -1.0f * -0.345491f;
		SphereData[89].ny = -1.0f * 0.475528f;
		SphereData[89].nz = -1.0f * -0.809017f;
		SphereData[89].u = 0.100000f;
		SphereData[89].v = 0.200000f;

		SphereData[90].x =  -0.279508f;
		SphereData[90].y =  0.090818f;
		SphereData[90].z =  -0.404509f;
		SphereData[90].nx = -1.0f * -0.559017f;
		SphereData[90].ny = -1.0f * 0.181636f;
		SphereData[90].nz = -1.0f * -0.809017f;
		SphereData[90].u = 0.200000f;
		SphereData[90].v = 0.200000f;

		SphereData[91].x =  -0.279508f;
		SphereData[91].y =  -0.090818f;
		SphereData[91].z =  -0.404509f;
		SphereData[91].nx = -1.0f * -0.559017f;
		SphereData[91].ny = -1.0f * -0.181636f;
		SphereData[91].nz = -1.0f * -0.809017f;
		SphereData[91].u = 0.300000f;
		SphereData[91].v = 0.200000f;

		SphereData[92].x =  -0.172746f;
		SphereData[92].y =  -0.237764f;
		SphereData[92].z =  -0.404509f;
		SphereData[92].nx = -1.0f * -0.345491f;
		SphereData[92].ny = -1.0f * -0.475528f;
		SphereData[92].nz = -1.0f * -0.809017f;
		SphereData[92].u = 0.400000f;
		SphereData[92].v = 0.200000f;

		SphereData[93].x =  0.000000f;
		SphereData[93].y =  -0.293893f;
		SphereData[93].z =  -0.404509f;
		SphereData[93].nx = -1.0f * 0.000000f;
		SphereData[93].ny = -1.0f * -0.587785f;
		SphereData[93].nz = -1.0f * -0.809017f;
		SphereData[93].u = 0.500000f;
		SphereData[93].v = 0.200000f;

		SphereData[94].x =  0.172746f;
		SphereData[94].y =  -0.237764f;
		SphereData[94].z =  -0.404509f;
		SphereData[94].nx = -1.0f * 0.345492f;
		SphereData[94].ny = -1.0f * -0.475528f;
		SphereData[94].nz = -1.0f * -0.809017f;
		SphereData[94].u = 0.600000f;
		SphereData[94].v = 0.200000f;

		SphereData[95].x =  0.279508f;
		SphereData[95].y =  -0.090818f;
		SphereData[95].z =  -0.404509f;
		SphereData[95].nx = -1.0f * 0.559017f;
		SphereData[95].ny = -1.0f * -0.181636f;
		SphereData[95].nz = -1.0f * -0.809017f;
		SphereData[95].u = 0.700000f;
		SphereData[95].v = 0.200000f;

		SphereData[96].x =  0.279508f;
		SphereData[96].y =  0.090818f;
		SphereData[96].z =  -0.404509f;
		SphereData[96].nx = -1.0f * 0.559017f;
		SphereData[96].ny = -1.0f * 0.181636f;
		SphereData[96].nz = -1.0f * -0.809017f;
		SphereData[96].u = 0.800000f;
		SphereData[96].v = 0.200000f;

		SphereData[97].x =  0.172746f;
		SphereData[97].y =  0.237764f;
		SphereData[97].z =  -0.404509f;
		SphereData[97].nx = -1.0f * 0.345491f;
		SphereData[97].ny = -1.0f * 0.475528f;
		SphereData[97].nz = -1.0f * -0.809017f;
		SphereData[97].u = 0.900000f;
		SphereData[97].v = 0.200000f;

		SphereData[98].x =  -0.000000f;
		SphereData[98].y =  0.293893f;
		SphereData[98].z =  -0.404509f;
		SphereData[98].nx = -1.0f * -0.000000f;
		SphereData[98].ny = -1.0f * 0.587785f;
		SphereData[98].nz = -1.0f * -0.809017f;
		SphereData[98].u = 1.000000f;
		SphereData[98].v = 0.200000f;

		SphereData[99].x =  -0.000000f;
		SphereData[99].y =  0.154508f;
		SphereData[99].z =  -0.475528f;
		SphereData[99].nx = -1.0f * -0.000000f;
		SphereData[99].ny = -1.0f * 0.309017f;
		SphereData[99].nz = -1.0f * -0.951057f;
		SphereData[99].u = 0.000000f;
		SphereData[99].v = 0.100000f;

		SphereData[100].x =  -0.090818f;
		SphereData[100].y =  0.125000f;
		SphereData[100].z =  -0.475528f;
		SphereData[100].nx = -1.0f * -0.181636f;
		SphereData[100].ny = -1.0f * 0.250000f;
		SphereData[100].nz = -1.0f * -0.951057f;
		SphereData[100].u = 0.100000f;
		SphereData[100].v = 0.100000f;

		SphereData[101].x =  -0.146946f;
		SphereData[101].y =  0.047746f;
		SphereData[101].z =  -0.475528f;
		SphereData[101].nx = -1.0f * -0.293892f;
		SphereData[101].ny = -1.0f * 0.095491f;
		SphereData[101].nz = -1.0f * -0.951057f;
		SphereData[101].u = 0.200000f;
		SphereData[101].v = 0.100000f;

		SphereData[102].x =  -0.146946f;
		SphereData[102].y =  -0.047746f;
		SphereData[102].z =  -0.475528f;
		SphereData[102].nx = -1.0f * -0.293892f;
		SphereData[102].ny = -1.0f * -0.095491f;
		SphereData[102].nz = -1.0f * -0.951057f;
		SphereData[102].u = 0.300000f;
		SphereData[102].v = 0.100000f;

		SphereData[103].x =  -0.090818f;
		SphereData[103].y =  -0.125000f;
		SphereData[103].z =  -0.475528f;
		SphereData[103].nx = -1.0f * -0.181635f;
		SphereData[103].ny = -1.0f * -0.250000f;
		SphereData[103].nz = -1.0f * -0.951057f;
		SphereData[103].u = 0.400000f;
		SphereData[103].v = 0.100000f;

		SphereData[104].x =  0.000000f;
		SphereData[104].y =  -0.154508f;
		SphereData[104].z =  -0.475528f;
		SphereData[104].nx = -1.0f * 0.000000f;
		SphereData[104].ny = -1.0f * -0.309017f;
		SphereData[104].nz = -1.0f * -0.951057f;
		SphereData[104].u = 0.500000f;
		SphereData[104].v = 0.100000f;

		SphereData[105].x =  0.090818f;
		SphereData[105].y =  -0.125000f;
		SphereData[105].z =  -0.475528f;
		SphereData[105].nx = -1.0f * 0.181636f;
		SphereData[105].ny = -1.0f * -0.250000f;
		SphereData[105].nz = -1.0f * -0.951057f;
		SphereData[105].u = 0.600000f;
		SphereData[105].v = 0.100000f;

		SphereData[106].x =  0.146946f;
		SphereData[106].y =  -0.047746f;
		SphereData[106].z =  -0.475528f;
		SphereData[106].nx = -1.0f * 0.293892f;
		SphereData[106].ny = -1.0f * -0.095491f;
		SphereData[106].nz = -1.0f * -0.951057f;
		SphereData[106].u = 0.700000f;
		SphereData[106].v = 0.100000f;

		SphereData[107].x =  0.146946f;
		SphereData[107].y =  0.047746f;
		SphereData[107].z =  -0.475528f;
		SphereData[107].nx = -1.0f * 0.293892f;
		SphereData[107].ny = -1.0f * 0.095491f;
		SphereData[107].nz = -1.0f * -0.951057f;
		SphereData[107].u = 0.800000f;
		SphereData[107].v = 0.100000f;

		SphereData[108].x =  0.090818f;
		SphereData[108].y =  0.125000f;
		SphereData[108].z =  -0.475528f;
		SphereData[108].nx = -1.0f * 0.181635f;
		SphereData[108].ny = -1.0f * 0.250000f;
		SphereData[108].nz = -1.0f * -0.951057f;
		SphereData[108].u = 0.900000f;
		SphereData[108].v = 0.100000f;

		SphereData[109].x =  -0.000000f;
		SphereData[109].y =  0.154508f;
		SphereData[109].z =  -0.475528f;
		SphereData[109].nx = -1.0f * -0.000000f;
		SphereData[109].ny = -1.0f * 0.309017f;
		SphereData[109].nz = -1.0f * -0.951057f;
		SphereData[109].u = 1.000000f;
		SphereData[109].v = 0.100000f;

		SphereData[110].x =  0.000000f;
		SphereData[110].y =  -0.000000f;
		SphereData[110].z =  -0.500000f;
		SphereData[110].nx = -1.0f * 0.000000f;
		SphereData[110].ny = -1.0f * -0.000000f;
		SphereData[110].nz = -1.0f * -1.000000f;
		SphereData[110].u = 0.000000f;
		SphereData[110].v = -0.000000f;

		SphereData[111].x =  0.000000f;
		SphereData[111].y =  -0.000000f;
		SphereData[111].z =  -0.500000f;
		SphereData[111].nx = -1.0f * 0.000000f;
		SphereData[111].ny = -1.0f * -0.000000f;
		SphereData[111].nz = -1.0f * -1.000000f;
		SphereData[111].u = 0.100000f;
		SphereData[111].v = -0.000000f;

		SphereData[112].x =  0.000000f;
		SphereData[112].y =  -0.000000f;
		SphereData[112].z =  -0.500000f;
		SphereData[112].nx = -1.0f * 0.000000f;
		SphereData[112].ny = -1.0f * -0.000000f;
		SphereData[112].nz = -1.0f * -1.000000f;
		SphereData[112].u = 0.200000f;
		SphereData[112].v = -0.000000f;

		SphereData[113].x =  0.000000f;
		SphereData[113].y =  0.000000f;
		SphereData[113].z =  -0.500000f;
		SphereData[113].nx = -1.0f * 0.000000f;
		SphereData[113].ny = -1.0f * 0.000000f;
		SphereData[113].nz = -1.0f * -1.000000f;
		SphereData[113].u = 0.300000f;
		SphereData[113].v = -0.000000f;

		SphereData[114].x =  0.000000f;
		SphereData[114].y =  0.000000f;
		SphereData[114].z =  -0.500000f;
		SphereData[114].nx = -1.0f * 0.000000f;
		SphereData[114].ny = -1.0f * 0.000000f;
		SphereData[114].nz = -1.0f * -1.000000f;
		SphereData[114].u = 0.400000f;
		SphereData[114].v = -0.000000f;

		SphereData[115].x =  -0.000000f;
		SphereData[115].y =  0.000000f;
		SphereData[115].z =  -0.500000f;
		SphereData[115].nx = -1.0f * -0.000000f;
		SphereData[115].ny = -1.0f * 0.000000f;
		SphereData[115].nz = -1.0f * -1.000000f;
		SphereData[115].u = 0.500000f;
		SphereData[115].v = -0.000000f;

		SphereData[116].x =  -0.000000f;
		SphereData[116].y =  0.000000f;
		SphereData[116].z =  -0.500000f;
		SphereData[116].nx = -1.0f * -0.000000f;
		SphereData[116].ny = -1.0f * 0.000000f;
		SphereData[116].nz = -1.0f * -1.000000f;
		SphereData[116].u = 0.600000f;
		SphereData[116].v = -0.000000f;

		SphereData[117].x =  -0.000000f;
		SphereData[117].y =  0.000000f;
		SphereData[117].z =  -0.500000f;
		SphereData[117].nx = -1.0f * -0.000000f;
		SphereData[117].ny = -1.0f * 0.000000f;
		SphereData[117].nz = -1.0f * -1.000000f;
		SphereData[117].u = 0.700000f;
		SphereData[117].v = -0.000000f;

		SphereData[118].x =  -0.000000f;
		SphereData[118].y =  -0.000000f;
		SphereData[118].z =  -0.500000f;
		SphereData[118].nx = -1.0f * -0.000000f;
		SphereData[118].ny = -1.0f * -0.000000f;
		SphereData[118].nz = -1.0f * -1.000000f;
		SphereData[118].u = 0.800000f;
		SphereData[118].v = -0.000000f;

		SphereData[119].x =  -0.000000f;
		SphereData[119].y =  -0.000000f;
		SphereData[119].z =  -0.500000f;
		SphereData[119].nx = -1.0f * -0.000000f;
		SphereData[119].ny = -1.0f * -0.000000f;
		SphereData[119].nz = -1.0f * -1.000000f;
		SphereData[119].u = 0.900000f;
		SphereData[119].v = -0.000000f;

		SphereData[120].x =  0.000000f;
		SphereData[120].y =  -0.000000f;
		SphereData[120].z =  -0.500000f;
		SphereData[120].nx = -1.0f * 0.000000f;
		SphereData[120].ny = -1.0f * -0.000000f;
		SphereData[120].nz = -1.0f * -1.000000f;
		SphereData[120].u = 1.000000f;
		SphereData[120].v = -0.000000f;
	}

	SphereModel::SphereModel(const char* const pModelFileName)
		: PrimitiveModel(Name::SPHERE)
	{

		this->CreateSphere();

		assert(pModelFileName);
		this->privCreateVAO(pModelFileName);

	}

	SphereModel::~SphereModel()
	{
		// remove anything dynamic here
	}
}

// --- End of File ---
