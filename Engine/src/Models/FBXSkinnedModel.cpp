#include "FBXSkinnedModel.h"
namespace Azul
{

	/*int Hierarchy[]
	{
		0, 20, 20, 20,  20,  20,  20,  20,  20,
		0,  1, 20, 20,  20,  20,  20,  20,  20,
		0,  1,  2, 20,  20,  20,  20,  20,  20,
		0,  1,  2,  3,  20,  20,  20,  20,  20,
		0,  1,  2,  3,   4,  20,  20,  20,  20,
		0,  1,  2,  3,   4,   5,  20,  20,  20,
		0,  1,  2,  6,  20,  20,  20,  20,  20,
		0,  1,  2,  6,   7,  20,  20,  20,  20,
		0,  1,  2,  6,   7,   8,  20,  20,  20,
		0,  1,  2,  9,  20,  20,  20,  20,  20,
		0,  1,  2,  9,  10,  20,  20,  20,  20,
		0,  1,  2,  9,  10,  11,  20,  20,  20,
		0,  1,  2,  9,  10,  11,  12,  20,  20,
		0,  1,  2,  9,  10,  11,  12,  13,  20,
		0,  1,  2,  9,  10,  11,  12,  13,  14,
		0,  1,  2,  9,  10,  15,  20,  20,  20,
		0,  1,  2,  9,  10,  15,  16,  20,  20,
		0,  1,  2,  9,  10,  15,  16,  17,  20,
		0,  1,  2,  9,  10,  15,  16,  17,  18,
		0,  1,  2,  9,  10,  19,  20,  20,  20
	};*/

	FBXSkinnedModel::FBXSkinnedModel(const Vert_xyzuvn* vertBuffer,
									const Tri_index* triBuffer,
									const Bone_weight* weightBuffer,
									const Bone_index* BoneIndexBuffer,
									Matrix* InvPoseBuffer,
									unsigned int _numVerts,
									unsigned int _numTris,
									unsigned int _numBones,
									int* pHierarchyTab,
									unsigned int Depth,
									Name nameIn)

		:Model(_numVerts, _numTris, nameIn),
		vbo_boneweight(0),
		vbo_boneindex(0),
		NumBones(_numBones),
		poInversePose(InvPoseBuffer),
		poHierarchyTable(pHierarchyTab),
		HierarchyDepth(Depth)
		//poBone(nullptr)
	{

		//Matrix matIdentity(Matrix::Special::Identity);
		//this->poBone = new Matrix[this->NumBones]{ matIdentity };


		this->poInversePose_SBO = new SSBO();
		this->poHierarchy_SBO = new SSBO();

		this->privCreateVAO(vertBuffer, triBuffer, weightBuffer, BoneIndexBuffer);
	}

	FBXSkinnedModel::~FBXSkinnedModel()
	{
		//delete[] poBone;
		delete[] poInversePose;
		delete[] poHierarchyTable;

		delete this->poInversePose_SBO;
		delete this->poHierarchy_SBO;
	}

	void Azul::FBXSkinnedModel::privCreateVAO(const Vert_xyzuvn* vertBuffer, const Tri_index* triBuffer, const Bone_weight* weightBuffer, const Bone_index* BoneIndexBuffer)
	{
	
		// Set the SBO 
		this->poInversePose_SBO->Set(this->NumBones, sizeof(Matrix), poInversePose);
		this->poHierarchy_SBO->Set((this->NumBones * this->HierarchyDepth), sizeof(int), &poHierarchyTable[0]);

		// Create a VAO
		glGenVertexArrays(1, &this->vao);
		assert(this->vao != 0);

		glBindVertexArray(this->vao);  //<---- active

		// Create a VBO
		glGenBuffers(1, &this->vbo_verts);
		assert(this->vbo_verts != 0);

		glGenBuffers(1, &this->vbo_tris);
		assert(this->vbo_tris != 0);

		glGenBuffers(1, &this->vbo_boneweight);
		assert(this->vbo_boneweight != 0);

		glGenBuffers(1, &this->vbo_boneindex);
		assert(this->vbo_boneindex != 0);

		// Load VERTS: ---------------------------------------------------------

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_verts);

		// load the data to the GPU
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyzuvn) * this->numVerts), vertBuffer, GL_STATIC_DRAW);

		// Vertext data in location 0
		void* offsetVert = (void*)((unsigned int)&vertBuffer[0].x - (unsigned int)vertBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetVert);
		glEnableVertexAttribArray(0);

		// Texture data in location 1
		void* offsetTex = (void*)((unsigned int)&vertBuffer[0].u - (unsigned int)vertBuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetTex);
		glEnableVertexAttribArray(1);

		// Normal data in location 2
		void* offsetNorm = (void*)((unsigned int)&vertBuffer[0].nx - (unsigned int)vertBuffer);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetNorm);
		glEnableVertexAttribArray(2);

		delete[] vertBuffer;

		// Load BoneWeight: ---------------------------------------------------------

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_boneweight);

		// load the data to the GPU
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Bone_weight) * this->numVerts), weightBuffer, GL_STATIC_DRAW);

		// BoneWeight data in location 3
		offsetVert = (void*)((unsigned int)&weightBuffer[0].A - (unsigned int)weightBuffer);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Bone_weight), offsetVert);
		glEnableVertexAttribArray(3);

		delete[] weightBuffer;

		// Load BoneIndex: ---------------------------------------------------------

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_boneindex);

		// load the data to the GPU
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Bone_index) * this->numVerts), BoneIndexBuffer, GL_STATIC_DRAW);

		// BoneIndex data in location 4
		offsetVert = (void*)((unsigned int)&BoneIndexBuffer[0].iA - (unsigned int)BoneIndexBuffer);
		glVertexAttribIPointer(4, 4, GL_UNSIGNED_INT, sizeof(Bone_index), offsetVert);
		glEnableVertexAttribArray(4);

		delete[] BoneIndexBuffer;

		// Load TriList: ---------------------------------------------------------

		/* Bind our 2nd VBO as being the active buffer and storing index ) */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_tris);

		/* Copy the index data to our buffer */
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Tri_index) * this->numTris), triBuffer, GL_STATIC_DRAW);

		delete[] triBuffer;
	}
}