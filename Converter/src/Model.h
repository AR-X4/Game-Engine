#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <algorithm>
#include <fbxsdk.h>
#include <map>
#include "MathEngine.h"

using namespace Azul;

//add vert colors?
#define MATH_PI_180  0.0174532925f

class Model
{

public:

	

	struct BonePos
	{
		//trans
		float x;
		float y;
		float z;
		float w = 0.0f;

		//rot
		float rx;
		float ry;
		float rz;
		float rw = 0.0f;

		//scale
		float sx;
		float sy;
		float sz;
		float sw = 0.0f;
	};

	struct FBX_VERTS
	{
		float x;
		float y;
		float z;
		float nx;
		float ny;
		float nz;
	};

	struct FINAL_VBO_VERTS 
	{
		float x;
		float y;
		float z;
		float s;
		float t;
		float nx;
		float ny;
		float nz;
	};

	struct VBO_VERTS
	{
		int index;
		float x;
		float y;
		float z;
		float s;
		float t;
		float nx;
		float ny;
		float nz;

		bool operator == (const VBO_VERTS& b)
		{
			return (
				(this->x == b.x)   &&
				(this->y == b.y)   &&
				(this->z == b.z)   &&
				(this->s == b.s)   &&
				(this->t == b.t)   &&
				(this->nx == b.nx) &&
				(this->ny == b.ny) &&
				(this->nz == b.nz)
				);
		}

		bool operator < (const VBO_VERTS& b)
		{
			if (this->x < b.x)
			{
				return true;
			}
			else if (this->x > b.x)
			{
				return false;
			}

			if (this->y < b.y)
			{
				return true;
			}
			else if (this->y > b.y)
			{
				return false;
			}

			if (this->z < b.z)
			{
				return true;
			}
			else if (this->z > b.z)
			{
				return false;
			}

			if (this->s < b.s)
			{
				return true;
			}
			else if (this->s > b.s)
			{
				return false;
			}

			if (this->t < b.t)
			{
				return true;
			}
			else if (this->t > b.t)
			{
				return false;
			}

			if (this->nx < b.nx)
			{
				return true;
			}
			else if (this->nx > b.nx)
			{
				return false;
			}

			if (this->ny < b.ny)
			{
				return true;
			}
			else if (this->ny > b.ny)
			{
				return false;
			}

			if (this->nz < b.nz)
			{
				return true;
			}
			else if (this->nz > b.nz)
			{
				return false;
			}
			else
			{
				return false;
			}
		}
	};

	struct TRILIST
	{
		int v0;
		int v1;
		int v2;
	};

	// ---Weights---
	struct VertWeight
	{
		VertWeight()
		{
			iVertIndex = 0;
			fBoneWeight = 0.0f;
		}
		int   iVertIndex;
		float fBoneWeight;
	};

	struct BoneWeight
	{
		BoneWeight()
		{
			iBoneIndex = 0;
			fBoneWeight = 0.0f;
		}
		int   iBoneIndex;
		float fBoneWeight;
	};

	struct BoneIndex 
	{
		int A;
		int B;
		int C;
		int D;
	};

	struct FinalVertEntry
	{
		BoneWeight A;
		BoneWeight B;
		BoneWeight C;
		BoneWeight D;
	};

	struct VertEntry
	{
	public:
		std::vector< BoneWeight >  vBoneWeight;
	};

	struct Bone
	{
		Bone() = default;

		Bone(const std::string& pNameIn, int bIndex, int pIndex)
		{
			//memset(name, 0, 64);
			//memcpy(name, pBuff, strlen(pBuff));
			this->BoneName = pNameIn;

			BoneIndex = bIndex;
			ParentBoneIndex = pIndex;
		}

		std::string BoneName;
		int BoneIndex;
		int ParentBoneIndex;
		std::vector< VertWeight >  VertWeightList;
	};

	struct FinalBone
	{

		FinalBone() = default;

		FinalBone(const std::string& pNameIn, int bIndex, int pIndex)
		{
			memset(BoneName, 0, 64);
			memcpy(BoneName, pNameIn.c_str(), strlen(pNameIn.c_str()));
			//this->BoneName = pNameIn;

			BoneIndex = bIndex;
			ParentBoneIndex = pIndex;
		}

		char BoneName[64];
		int BoneIndex;
		int ParentBoneIndex;
	};

	// --- Pose ---

	struct InvBonePose
	{
		
	public:
		InvBonePose(int Index, const std::string& pNameIn)
		{
			assert(Index >= 0);
			this->iBoneIndex = Index;

			this->poBoneName = pNameIn;

			m.set(Matrix::Special::Identity);
		}

		void Set(int Index,
				const std::string& pNameIn,
				FbxVector4 r0,
				FbxVector4 r1,
				FbxVector4 r2,
				FbxVector4 r3)
		{
			assert(Index >= 0);
			this->iBoneIndex = Index;

			this->poBoneName = pNameIn;

			m.m0() = (float)r0[0];
			m.m1() = (float)r0[1];
			m.m2() = (float)r0[2];
			m.m3() = (float)r0[3];

			m.m4() = (float)r1[0];
			m.m5() = (float)r1[1];
			m.m6() = (float)r1[2];
			m.m7() = (float)r1[3];

			m.m8() = (float)r2[0];
			m.m9() = (float)r2[1];
			m.m10() = (float)r2[2];
			m.m11() = (float)r2[3];

			m.m12() = (float)r3[0];
			m.m13() = (float)r3[1];
			m.m14() = (float)r3[2];
			m.m15() = (float)r3[3];
		}

		//void PrintMe();

	public:
		int iBoneIndex;
		std::string poBoneName;
		Matrix m;
	};

	Model(FbxMesh* pMeshIn, const std::string& NameIn, const std::string& VersionIn);
	Model() = delete;
	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;
	~Model();


	//Getters
	const std::string& GetName() const;
	const std::string& GetVersion( )const;
	std::vector<TRILIST> GetTriList() const;
	std::vector<VBO_VERTS> GetVertList() const;

	static bool Vect_sort(BoneWeight const& v1, BoneWeight const& v2);
	


	
	static void FindSkeleton(FbxNode* pNode, std::map<std::string, Bone>& BoneMap, std::vector< Model::Bone>& _BoneList, int& BoneIndex, unsigned int& pLocalDepth, unsigned int& pBoneDepth);
	//static void FindMesh();
	static void FindInverseBonePose(FbxGeometry* pGeometry, std::vector< Model::Bone>& BoneListIn, std::vector< InvBonePose >& InvPoseTableOut);
	static void GetAminBonePos(FbxNode* pNodeIn, FbxAnimLayer* pAnimLayerIn, FbxTime KeyTime, std::vector<BonePos>& AnimBones);
	static void FindAnimations(FbxScene* pScene, std::vector<BonePos>& anims);
	static void CompressAnimations(std::vector<BonePos>& anims, int numBones);

	static void ProcessBoneWeights(FbxMesh* pMesh, std::vector< Model::Bone>& BoneListIn, std::vector< FinalVertEntry >& FinalBoneWeightTableOut);
	static void ProcessRiggedVerts(FbxMesh* pMeshIn, std::vector<VBO_VERTS>& VERTS_LIST_OUT, std::vector<TRILIST>& TRI_LIST_OUT);
	//print()

private:
	void ProcessVerts(FbxMesh* pMeshIn);

	//data
	std::string ModelName;
	std::string ModelVersion;
	std::vector<TRILIST> VBO_TRILIST;
	std::vector<VBO_VERTS> VBO_VERTS_LIST;
};

#endif