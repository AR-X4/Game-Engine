#ifndef FBX_SKINNED_MODEL_H
#define FBX_SKINNED_MODEL_H

#include "Model.h"
#include "Animation/Bone.h"
#include "Shaders/SSBO.h"

namespace Azul
{
	class FBXSkinnedModel : public Model
	{
    public:

		struct Bone_weight
		{
			float A;
			float B;
			float C;
			float D;
		};

		struct Bone_index
		{
			int iA;
			int iB;
			int iC;
			int iD;
		};


        FBXSkinnedModel(const Vert_xyzuvn* vertBuffer, 
						const Tri_index* triBuffer, 
						const Bone_weight* weightBuffer, 
						const Bone_index* BoneIndexBuffer,
						Matrix* InvPoseBuffer, 
						unsigned int _numVerts, 
						unsigned int _numTris, 
						unsigned int _numBones,
						int* pHierarchyTab,
						unsigned int Depth,
						Name nameIn = Name::UNINITIALIZED);

        FBXSkinnedModel(const FBXSkinnedModel&) = delete;
        FBXSkinnedModel& operator = (FBXSkinnedModel&) = delete;
        virtual ~FBXSkinnedModel();


    private:
        // load VAO
        void privCreateVAO(const Vert_xyzuvn* vertBuffer, const Tri_index* triBuffer, const Bone_weight* weightBuffer, const Bone_index* BoneIndexBuffer);

    public:
        // data
        GLuint vbo_boneweight;   // Bone_weight
        GLuint vbo_boneindex;   // Bone_index

        unsigned int NumBones;
        Matrix* poInversePose;
        //Matrix* poBone;
		int* poHierarchyTable;

		unsigned int HierarchyDepth;
		SSBO* poInversePose_SBO;
		SSBO* poHierarchy_SBO;
	};
}
#endif