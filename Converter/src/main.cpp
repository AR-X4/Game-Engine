#include "Exporter.h"
#include "Common/Common.h"
#include "Examples/DisplayHierarchy.h"
#include <map>


//Args should be "ModelConverter -anim -i inputModel.fbx -o outputFile.flcn -n ModelName -v 1.0.9  -t textureFile.tga"

// TODO improve arg processing
bool ProcessArgs(int argc, 
                 char** argv, 
                 FbxString& InputFileNameIn, 
                 std::string& OutputfileIn, 
                 std::string& ModelNameIn,
                 std::string& VersionNameIn,
                 std::vector<std::string>& TextureNames,
                 bool& _bAnim,
                 bool& _bSkinAnim)
{            
    

    if (argc < 9) {
        printf("\nERROR: TOO FEW COMMAND ARGS\n");
        return false;
    }

    if (!strcmp(argv[1], "-anim"))
    {
        _bAnim = true;
    }
    else if (!strcmp(argv[1], "-skin")) 
    {
        _bSkinAnim = true;
    }
    else 
    {
        //printf("\nERROR: SPECIFY EXPORT TYPE\n");
        //return false;
    }



    //---Check Input File Arg---
    if (!strcmp(argv[2], "-i"))
    {
        const std::string ext = ".fbx";
        const std::string arg = argv[3];
        const int arglen = arg.length();
        const int extlen = ext.length();

        if (arglen > extlen) {
            if (!arg.compare(arglen - extlen, extlen, ext))
            {
                InputFileNameIn = argv[3];
            }
        }
    }

    //---Check Output File Arg---
    if (!strcmp(argv[4], "-o"))
    {
        const std::string ext = ".flcn";
        const std::string arg = argv[5];
        const int arglen = arg.length();
        const int extlen = ext.length();

        if (arglen > extlen) {
            if (!arg.compare(arglen - extlen, extlen, ext))
            {
                OutputfileIn = arg;
            }
        }
    }

    //---Check Model Name Arg---
    if (!strcmp(argv[6], "-n"))
    {
        ModelNameIn = argv[7];
    }

    //---Check Version Name Arg---
    if (!strcmp(argv[8], "-v"))
    {
        VersionNameIn = argv[9];
    }
   

    //---Check Texture Files Arg--- optional ... test this
    if (argc >= 10) {

        const std::string ext = ".tga";
        const int extlen = ext.length();
        
        for (int i = 10; i < argc; i += 2) {
        
            if (!strcmp(argv[i], "-t"))
            {
                const std::string arg = argv[i+1];
                const int arglen = arg.length();

                if (arglen > extlen) {
                    if (!arg.compare(arglen - extlen, extlen, ext))
                    {
                        TextureNames.push_back(arg);
                    }
                    else {
                        printf("\nERROR: INVAILD TEXTURE FILE ARG\n");
                        return false;
                    }
                }
                else {
                    printf("\nERROR: INVAILD TEXTURE FILE ARG\n");
                    return false;
                }
            }
            else {
                printf("\nERROR: INVAILD TEXTURE FILE ARG\n");
                return false;
            }
        }
    }

    if (InputFileNameIn.IsEmpty() ||
        OutputfileIn.empty()      ||
        ModelNameIn.empty()       ||
        VersionNameIn.empty()) 
    {
        printf("\nERROR: INVAILD COMMAND ARGS\n");
        return false;
    }


    return true;
}



//TODO combine find mesh and find skel ?
//move this to model class ?
FbxMesh* FindMesh(FbxNode* pNodeIn)
{

    if (pNodeIn->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh) {
        return (FbxMesh*)pNodeIn->GetNodeAttribute();
    }

    FbxMesh* pMesh = nullptr;
    for (int i = 0; i < pNodeIn->GetChildCount(); i++)
    {
        pMesh = FindMesh(pNodeIn->GetChild(i));
        if (pMesh != nullptr) {
            return pMesh;
        }
    }

    return pMesh;
}

int main(int argc, char* argv[])
{
    
    //------Process args------
    FbxString InputFileName("");
    std::string OutputFileName("");
    std::string ModelName("");
    std::string VersionName("");
    bool bAnim = false;
    bool bSkinAnim = false;

    std::vector<std::string> TextureNames;
    TextureNames.reserve(2);

    if (!ProcessArgs(argc, argv, InputFileName, OutputFileName, ModelName, VersionName, TextureNames, bAnim, bSkinAnim)) {
    
        printf("ARGS SHOULD BE: '-i inputModel.fbx -o outputFile.flcn -n ModelName -v 1.0.9 -t textureFile.tga'\n\n");
        return -1;
    }


    // ------Init the SDK------
    FbxManager* pSdkManager = NULL;
    FbxScene* pScene = NULL;
    InitializeSdkObjects(pSdkManager, pScene);

    if (LoadScene(pSdkManager, pScene, InputFileName.Buffer())) {
    
        // ---Triangulate Data---
        FbxGeometryConverter lGeomConverter(pSdkManager);
        lGeomConverter.Triangulate(pScene, true);// Replace = true...?

        // --- Get FBX Scene Root ---
        FbxNode* pNode = pScene->GetRootNode();
        assert(pNode != nullptr);

        //---Get FBX Mesh---
        FbxMesh* pMesh = nullptr;
        for (int i = 0; i < pNode->GetChildCount(); i++)
        {
            pMesh = FindMesh(pNode->GetChild(i));
            if (pMesh != nullptr) {
                break;
            }
        }
       // assert(pMesh);
        
        if (bSkinAnim == true) // rigged model
        {

            //---Find bone hierarchy---
            std::map<std::string, Model::Bone> BoneMap;
            std::vector< Model::Bone> BoneList;
            int BoneIndex = 0;

            std::vector<int> BoneHierarchy;
            unsigned int BoneDepth = 0;
            

            for (int i = 0; i < pNode->GetChildCount(); i++)
            {
                unsigned int localDepth = 0;
                Model::FindSkeleton(pNode->GetChild(i), BoneMap, BoneList, BoneIndex, localDepth, BoneDepth);
            }
            assert(!BoneMap.empty());
            assert(!BoneList.empty());

            // TODO move to function
            //---Calc Bone Hierarchy int array for compute shader---
            printf("\nHierarchy Table\n");
            printf("Depth: %d\n", BoneDepth);

            for (int i = 0; i < (int)BoneList.size(); i++) 
            {
                // get path from curr bone to root
                std::vector<int> pathToRoot;
                int currIndex = i;

                while (currIndex != -1) 
                {
                    pathToRoot.push_back(currIndex);
                    currIndex = BoneList.at(currIndex).ParentBoneIndex;
                }

                std::reverse(pathToRoot.begin(), pathToRoot.end());

                for (int j = 0; j < (int)BoneDepth; j++) 
                {
                    if (j < (int)pathToRoot.size())
                    {
                        BoneHierarchy.push_back(pathToRoot.at(j));
                        printf("%d, ", pathToRoot.at(j));
                    }
                    else 
                    {
                        BoneHierarchy.push_back(BoneList.size());
                        printf("%d, ", BoneList.size());
                    }
                }
                printf("\n");
            }


            //---Find Inverse Pose and Weights---
            // we can find final inverse bone table now that we have bone list
            std::vector< Model::InvBonePose > InvPoseTable;

            Model::FindInverseBonePose(pMesh, BoneList, InvPoseTable);
            assert(!InvPoseTable.empty());

            // ---Process Bone Weights---
            std::vector< Model::FinalVertEntry > FinalBoneWeightTable;
            Model::ProcessBoneWeights(pMesh, BoneList, FinalBoneWeightTable);

            //---Get Animation data---
            std::vector<Model::BonePos> boneAnims;
            boneAnims.reserve(BoneList.size() * 30);// fix?
            Model::FindAnimations(pScene, boneAnims);


           // process verts - add index and bone weights to each vert using index
            std::vector<Model::VBO_VERTS> VERTS_LIST;
            std::vector<Model::TRILIST> TRI_LIST;

            Model::ProcessRiggedVerts(pMesh, VERTS_LIST, TRI_LIST);
            assert(!VERTS_LIST.empty());
            assert(!TRI_LIST.empty());


            // --- EXPORT ---
            Exporter::ExportAnimModelPackage(VERTS_LIST,
                                            TRI_LIST,
                                            BoneList,
                                            boneAnims,
                                            InvPoseTable,
                                            FinalBoneWeightTable,
                                            BoneHierarchy,
                                            BoneDepth,
                                            OutputFileName,
                                            ModelName,
                                            VersionName);
        }
        else if (bAnim == true) 
        {
            //---Find bone hierarchy---
            std::map<std::string, Model::Bone> BoneMap;
            std::vector< Model::Bone> BoneList;
            int BoneIndex = 0;
            unsigned int BoneDepth = 0;

            for (int i = 0; i < pNode->GetChildCount(); i++)
            {
                unsigned int localDepth = 0;
                Model::FindSkeleton(pNode->GetChild(i), BoneMap, BoneList, BoneIndex, localDepth, BoneDepth);
            }
            assert(!BoneMap.empty());
            assert(!BoneList.empty());


            //---Get Animation data---
            std::vector<Model::BonePos> boneAnims;
            boneAnims.reserve(81 * 30);
            Model::FindAnimations(pScene, boneAnims);

            // Compress Anims
            Model::CompressAnimations(boneAnims, BoneList.size());

            // --- EXPORT ---
            Exporter::ExportAnimPackage(boneAnims, OutputFileName, OutputFileName, VersionName);

        }
        else // static model, no skel / anims
        {
            
            //---Process Data---
            Model pModel(pMesh, ModelName, VersionName);// CHANGE THIS to be static function like ProcessRiggedVerts

            //-----------------------
            //------Export Data------
            //-----------------------
            if (!Exporter::ExportStaticModelPackage(pModel, OutputFileName, TextureNames))
            {
                printf("/nFAILED TO EXPORT.\n\n");
            }
           
        }
    }
    else {
        printf("\nFAILED TO LOAD FBX FILE.\n\n");
    }

    //Clean Up
    DestroySdkObjects(pSdkManager, true);//replace true?
    return 0;
}






