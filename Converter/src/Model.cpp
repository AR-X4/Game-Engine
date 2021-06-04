#include "Model.h"
#include "QuatApp.h"
#include "VectApp.h"


Model::Model(FbxMesh* pMeshIn, const std::string& NameIn, const std::string& VersionIn)
{
	
	assert(!NameIn.empty());
	assert(!VersionIn.empty());

	this->ModelName = NameIn;
	this->ModelVersion = VersionIn;

	//Process Data
	this->ProcessVerts(pMeshIn);
}

Model::~Model()
{

}

const std::string&  Model::GetName() const
{
	assert(!this->ModelName.empty());
	return this->ModelName;
}

const std::string&  Model::GetVersion() const
{
	assert(!this->ModelVersion.empty());
	return this->ModelVersion;
}

std::vector<Model::TRILIST> Model::GetTriList() const
{
	return this->VBO_TRILIST;
}

std::vector<Model::VBO_VERTS> Model::GetVertList() const
{
	return this->VBO_VERTS_LIST;
}

bool Model::Vect_sort(BoneWeight const& v1, BoneWeight const& v2)
{
	if (v1.fBoneWeight > v2.fBoneWeight)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Model::FindSkeleton(FbxNode* pNode, std::map<std::string, Bone>& BoneMap, std::vector< Model::Bone>& _BoneList, int& BoneIndex, unsigned int& LocalDepth, unsigned int& BoneDepth)
{
	if (pNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		std::string CurrBone = pNode->GetName();
		std::string ParentBone = pNode->GetParent()->GetName();

		//find parent
		auto it = BoneMap.find(ParentBone);

		int ParentBoneIndex = -1;
		if (it != BoneMap.end())
		{
			ParentBoneIndex = it->second.BoneIndex;
		}

		//print
		printf("   { %s, %d, %d }\n", CurrBone.c_str(), BoneIndex, ParentBoneIndex);

		//add to map
		BoneMap.emplace(CurrBone, Bone(CurrBone.c_str(), BoneIndex, ParentBoneIndex));
		_BoneList.push_back(Bone(CurrBone.c_str(), BoneIndex, ParentBoneIndex));
		BoneIndex++;

		LocalDepth += 1;
	}
	else {

		printf("\n Not bone\n");
	}

	for (int i = 0; i < pNode->GetChildCount(); i++)
	{
		FindSkeleton(pNode->GetChild(i), BoneMap, _BoneList, BoneIndex,  LocalDepth, BoneDepth);
	}
	
	if (pNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		if (LocalDepth > BoneDepth)
			BoneDepth = LocalDepth;
		LocalDepth -= 1;
	}
}

void Model::FindInverseBonePose(FbxGeometry* pGeometry, std::vector<Model::Bone>& BoneListIn, std::vector<InvBonePose>& InvPoseTableOut)
{
	// ----------- GET INVERSE POSE -----------


	int lSkinCount = pGeometry->GetDeformerCount(FbxDeformer::eSkin);

	for (unsigned int n = 0; n < BoneListIn.size(); n++)
	{
		InvBonePose pibe(n, BoneListIn[n].BoneName);
		InvPoseTableOut.push_back(pibe);
	}

	for (int skinIndex = 0; skinIndex < lSkinCount; skinIndex++)
	{
		FbxSkin* skinDeformer = static_cast<FbxSkin*>(pGeometry->GetDeformer(skinIndex, FbxDeformer::eSkin));
		const int clusterCount = skinDeformer->GetClusterCount();

		for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++)
		{
			FbxCluster* cluster = skinDeformer->GetCluster(clusterIndex);
			//printf("%s \n", cluster->GetLink()->GetName());
			// Get the bone Index
			int boneIndex = -1;
			for (unsigned int n = 0; n < BoneListIn.size(); n++)
			{
				if (strcmp(BoneListIn[n].BoneName.c_str(), cluster->GetLink()->GetName()) == 0)
				{
					boneIndex = n;
					break;
				}
			}
			assert(boneIndex != -1);

			// Populate invBindPose for each cluster (bone)
			FbxAMatrix linkMatrix;
			FbxAMatrix transformMatrix;
			FbxAMatrix invMatrix;
			cluster->GetTransformMatrix(transformMatrix);
			cluster->GetTransformLinkMatrix(linkMatrix);

			//printf("Keenan: invMatrix [%d] %s\n", boneIndex, cluster->GetLink()->GetName());
			invMatrix = linkMatrix.Inverse() * transformMatrix;


			InvPoseTableOut[boneIndex].Set(boneIndex,
										cluster->GetLink()->GetName(),
										invMatrix.GetRow(0),
										invMatrix.GetRow(1),
										invMatrix.GetRow(2),
										invMatrix.GetRow(3));
		}
	}

	// print inv post table
	printf("\n--- Inverse Pose Table ---\n\n");

	for (int i = 0; i < (int)InvPoseTableOut.size(); i++) 
	{
	
		printf("\t { %0ff, %0ff, %0ff, %0ff,   // [%d] \n",
			InvPoseTableOut[i].m.m0(),
			InvPoseTableOut[i].m.m1(),
			InvPoseTableOut[i].m.m2(),
			InvPoseTableOut[i].m.m3(),
			i
		);
		printf("\t   %0ff, %0ff, %0ff, %0ff,    \n",
			InvPoseTableOut[i].m.m4(),
			InvPoseTableOut[i].m.m5(),
			InvPoseTableOut[i].m.m6(),
			InvPoseTableOut[i].m.m7());

		printf("\t   %0ff, %0ff, %0ff, %0ff,    \n",
			InvPoseTableOut[i].m.m8(),
			InvPoseTableOut[i].m.m9(),
			InvPoseTableOut[i].m.m10(),
			InvPoseTableOut[i].m.m11());

		printf("\t   %0ff, %0ff, %0ff, %0ff },   \n",
			InvPoseTableOut[i].m.m12(),
			InvPoseTableOut[i].m.m13(),
			InvPoseTableOut[i].m.m14(),
			InvPoseTableOut[i].m.m15());
	}


	// -------- GET BONE WEIGHTS --------------

	//lLinkCount = pGeometry->GetLinkCount();
	for (int i = 0; i != lSkinCount; ++i)
	{
		int lClusterCount = ((FbxSkin*)pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetClusterCount();
		for (int j = 0; j != lClusterCount; ++j)
		{
			
			FbxCluster* lCluster = ((FbxSkin*)pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetCluster(j);

			int lIndexCount = lCluster->GetControlPointIndicesCount();
			int* lIndices = lCluster->GetControlPointIndices();
			double* lWeights = lCluster->GetControlPointWeights();

			// Find the index given string name
			
			int boneIndex = -1;
			for (unsigned int n = 0; n < BoneListIn.size(); n++)
			{
				if (strcmp(BoneListIn[n].BoneName.c_str(), lCluster->GetLink()->GetName()) == 0)
				{
					boneIndex = n;
					break;
				}
			}
			assert(boneIndex != -1);

			for (int k = 0; k < lIndexCount; k++)
			{
				// ----------------------------------------------------
				// 
				// ----------------------------------------------------
				VertWeight vw;
				vw.iVertIndex = lIndices[k];
				vw.fBoneWeight = (float)lWeights[k];

				if (boneIndex != -1)
				{
					BoneListIn[boneIndex].VertWeightList.push_back(vw);
				}
				else
				{
					assert(false);
				}


				
			}

		
		}
	}
}

static int counter = 0;// move this

void Model::GetAminBonePos(FbxNode* pNodeIn, FbxAnimLayer* pAnimLayerIn, FbxTime KeyTime, std::vector<BonePos>& AnimBones)
{
	if (pNodeIn->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		FbxAnimCurve* pAnimCurve = pNodeIn->LclTranslation.GetCurve(pAnimLayerIn, FBXSDK_CURVENODE_COMPONENT_X);

		FbxAMatrix matrix;
		FbxVector4 Trans;
		FbxVector4 RotXYZ;
		FbxVector4 Scale;

		if (!pAnimCurve)
		{
			matrix = pNodeIn->EvaluateLocalTransform();
		}
		else
		{
			matrix = pNodeIn->EvaluateLocalTransform(KeyTime);
		}

		Trans = matrix.GetT();
		RotXYZ = matrix.GetR();
		Scale = matrix.GetS();

		BonePos pos;
		pos.x = (float)Trans[0];
		pos.y = (float)Trans[1];
		pos.z = (float)Trans[2];

		pos.rx = (float)RotXYZ[0] * MATH_PI_180;
		pos.ry = (float)RotXYZ[1] * MATH_PI_180;
		pos.rz = (float)RotXYZ[2] * MATH_PI_180;

		pos.sx = (float)Scale[0];
		pos.sy = (float)Scale[1];
		pos.sz = (float)Scale[2];

		AnimBones.push_back(pos);

		//PRINT
		char	buff[256];
		FbxString lOutputString;

		lOutputString = "pTmp->poBone[";
		lOutputString += counter;
		lOutputString += "].T = Vect( ";
		sprintf(buff, "%ff", pos.x);
		lOutputString += buff;
		lOutputString += ", ";
		sprintf(buff, "%ff", pos.y);
		lOutputString += buff;
		lOutputString += ", ";
		sprintf(buff, "%ff", pos.z);
		lOutputString += buff;
		lOutputString += " );\n";
		printf(lOutputString);

		lOutputString = "pTmp->poBone[";
		lOutputString += counter;
		lOutputString += "].Q = Quat( Quat::Rot3::XYZ,";
		sprintf(buff, "%ff", pos.rx); //* MATH_PI_180);
		lOutputString += buff;
		lOutputString += ", ";
		sprintf(buff, "%ff", pos.ry); //* MATH_PI_180);
		lOutputString += buff;
		lOutputString += ", ";
		sprintf(buff, "%ff", pos.rz); //* MATH_PI_180);
		lOutputString += buff;
		lOutputString += " );\n";
		printf(lOutputString);

		lOutputString = "pTmp->poBone[";
		lOutputString += counter;
		lOutputString += "].S = Vect( ";
		sprintf(buff, "%ff", pos.sx);
		lOutputString += buff;
		lOutputString += ", ";
		sprintf(buff, "%ff", pos.sy);
		lOutputString += buff;
		lOutputString += ", ";
		sprintf(buff, "%ff", pos.sz);
		lOutputString += buff;
		lOutputString += " );\n\n";

		printf(lOutputString);

		counter++;
	}

	for (int i = 0; i < pNodeIn->GetChildCount(); i++)
	{
		//recursive get child.. break into two functions..
		GetAminBonePos(pNodeIn->GetChild(i), pAnimLayerIn, KeyTime, AnimBones);
	}
}

void Model::FindAnimations(FbxScene* pScene, std::vector<BonePos>& anims)
{
	//FbxAnimEvaluator* pAnimEvaluator = pScene->GetAnimationEvaluator();

	FbxAnimStack* pAnimStack = pScene->GetSrcObject<FbxAnimStack>(0);//change index? only gets first anim stack

	pScene->SetCurrentAnimationStack(pAnimStack);

	int numAnimLayers = pAnimStack->GetMemberCount<FbxAnimLayer>();

	FbxNode* pRoot = pScene->GetRootNode();

	//for num of anims?
	for (int i = 0; i < numAnimLayers; i++)
	{
		FbxAnimLayer* pAnimLayer = pAnimStack->GetMember<FbxAnimLayer>(i);

		FbxTime KeyTime;
		char    TimeString[256];

		//for each frame
		for (int j = 0; j < 31; j++)//30 hz
		{
			//Note this is hard coded... problem for non - 30Hz
			KeyTime.SetTime(0, 0, 0, j, 0, 0, FbxTime::eFrames30);

			//print
			FbxString lOutputString = "\n\n ------- Key Time: ";
			lOutputString += KeyTime.GetTimeString(TimeString, FbxUShort(256));
			lOutputString += "  ms: ";
			int msTime = (int)KeyTime.GetMilliSeconds();
			lOutputString += msTime;
			lOutputString += " ------------------ \n\n";
			printf(lOutputString);

			counter = 0;

			//---->recursively get pos/rot/scale of all bones in skel tree

			for (int k = 0; k < pRoot->GetChildCount(); k++)
			{
				GetAminBonePos(pRoot->GetChild(k), pAnimLayer, KeyTime, anims);
			}
		}
	}
}

void Model::CompressAnimations(std::vector<BonePos>& anims, int numBones)
{
	const float error = 0.1f;
	bool keepFrame = false;

	printf("\n\n\n\n\n------------------COMPRESSION------------------\n\n");

	// create new list and add first keyframe
	std::vector<BonePos> _anims;
	_anims.reserve(anims.size());
	for (int i = 0; i < numBones; i++) {
		_anims.push_back(anims.at(i));
	}

	const int numKeyFrames = anims.size() / numBones; // should be 30
	int anchorIndex = 0;
	//for each key frame
	for (int i = 1; i < numKeyFrames - 1; i++)//30 hz
	{
		printf("\n-------------KEYFRAME: %d----------------\n\n", i);

		// get hip bone pos of current keyframe
		BonePos hip = anims.at(i * numBones);
		Quat hipBone(hip.rx, hip.ry, hip.rz, hip.rw);

		
		Quat   Q_hip = Quat(hip.rx, hip.ry, hip.rz, hip.rw);

		//Matrix worldhip = S * Q * T;

		// get spine bone pos of current keyframe
		BonePos spine = anims.at((i * numBones) + 2);

		//T = Matrix(Matrix::Trans::XYZ, spine.x, spine.y, spine.z);
		//S = Matrix(Matrix::Scale::XYZ, spine.sx, spine.sy, spine.sz);
		Quat Q_spine = Quat(spine.rx, spine.ry, spine.rz, spine.rw);

		//Matrix worldSpine = S * Q * T;

		// get ref vector for current keyframe 
		Vect Anchor = Vect(Q_spine) - Vect(Q_hip);
		Anchor.norm();
		
		for (int j = 1; j < numBones; j++)
		{
			printf("BONE: %d ", j);

			// get n bone pos
			BonePos bone = anims.at((i * numBones) + j);

			//T = Matrix(Matrix::Trans::XYZ, bone.x, bone.y, bone.z);
			//S = Matrix(Matrix::Scale::XYZ, bone.sx, bone.sy, bone.sz);
			Quat Q_bone = Quat(bone.rx, bone.ry, bone.rz, bone.rw);

			//Matrix worldBone = S * Q * T;

			//printf("Pos: %f, %f, %f\n\n", nBone.x(), nBone.y(), nBone.z());

			// get vect to next bone
			Vect nVect = Vect(Q_bone) - Vect(Q_hip);
			nVect.norm();

			//get orig angle between anchor vect and nVect
			float origAngle = Anchor.getAngle(nVect);

			//get est bone pos by interpolating between bone of previous keyframe bone and next keyframe bone
			BonePos PrevBonePos = anims.at(((i - 1) * numBones) + j);
			BonePos NextBonePos = anims.at(((i + 1) * numBones) + j);
			

			//interpolate
			//Vect estT;
			//Vect estS;
			Quat estQ;

			//VectApp::Lerp(estT, Vect(PrevBonePos.x, PrevBonePos.y, PrevBonePos.z), Vect(NextBonePos.x, NextBonePos.y, NextBonePos.z), 0.5f);
			QuatApp::Slerp(estQ, Quat(PrevBonePos.rx, PrevBonePos.ry, PrevBonePos.rz, PrevBonePos.rw), Quat(NextBonePos.rx, NextBonePos.ry, NextBonePos.rz, NextBonePos.rw), 0.5f);
			//VectApp::Lerp(estS, Vect(PrevBonePos.sx, PrevBonePos.sy, PrevBonePos.sz), Vect(NextBonePos.sx, NextBonePos.sy, NextBonePos.sz), 0.5f);

			//T = Matrix(Matrix::Trans::XYZ, estT.x(), estT.y(), estT.z());
			//S = Matrix(Matrix::Scale::XYZ, estS.x(), estS.y(), estS.z());
			

			//Matrix worldEST = S * Q * T;

			//get est vect between est bone and hip bone

			Vect estBoneVect = Vect(estQ) - Vect(Q_hip);
			estBoneVect.norm();

			//get angle between est vector and anchor vect
			float estAngle = Anchor.getAngle(estBoneVect);

			// get diff of angles and compare to error
			if ((origAngle - estAngle) > error) {
				keepFrame = true;
				//_anims.push_back(anims.at((i * numBones) + j));
				//break;
			}
		}
		
		
		// if any angles for keyframe outside threshold, add to _anim list as keeper
		if (keepFrame == true) {
			for (int j = 0; j < numBones; j++) {
				_anims.push_back(anims.at(((i + 1) * numBones) + j));
			}
		}
		

	}

	// puah last keyframe
	for (int i = anims.size() - numBones; i < anims.size(); i++) {
		_anims.push_back(anims.at(i));
	}
	

	
	printf("\n\nUncompressed:	%d\n", anims.size());
	printf( "Compressed:	%d\n\n\n\n\n", _anims.size());

	// set new list to output
	anims = _anims;
}

void Model::ProcessBoneWeights(FbxMesh* pMesh, std::vector<Model::Bone>& BoneListIn, std::vector<FinalVertEntry>& FinalBoneWeightTableOut)
{

	int NumVerts = pMesh->GetControlPointsCount();

	std::vector< VertEntry > BoneWeightTable;
	BoneWeightTable.reserve(NumVerts);

	// fill table w empty entries

	for (auto j = 0; j < NumVerts; j++)
	{
		VertEntry v;
		BoneWeightTable.push_back(v);
	}

	// fill vert centric table

	for (unsigned int j = 0; j < BoneListIn.size(); j++)
	{
		//vBoneTable[j].PrintMe();

		for (unsigned int k = 0; k < BoneListIn[j].VertWeightList.size(); k++)
		{
			int boneIndex = j;
			int vertIndex = BoneListIn[j].VertWeightList[k].iVertIndex;
			float boneWeight = BoneListIn[j].VertWeightList[k].fBoneWeight;

			BoneWeight bw;

			bw.iBoneIndex = boneIndex;
			bw.fBoneWeight = boneWeight;

			BoneWeightTable[vertIndex].vBoneWeight.push_back(bw);

		}
	}

	// sort by weight

	for (auto j = 0; j < NumVerts; j++)
	{
		//printf("vert:%d\n", j);
		std::sort(BoneWeightTable[j].vBoneWeight.begin(), BoneWeightTable[j].vBoneWeight.end(), Model::Vect_sort);
	}

	// fill final table sorted by weight

	for (auto j = 0; j < NumVerts; j++)
	{
		BoneWeight bw;
		bw.iBoneIndex = 0;
		bw.fBoneWeight = 0.0f;

		// Add more... in case there is only one entry
		BoneWeightTable[j].vBoneWeight.push_back(bw);
		BoneWeightTable[j].vBoneWeight.push_back(bw);
		BoneWeightTable[j].vBoneWeight.push_back(bw);
		BoneWeightTable[j].vBoneWeight.push_back(bw);

		// Load it
		FinalVertEntry fbw;
		fbw.A.iBoneIndex = BoneWeightTable[j].vBoneWeight[0].iBoneIndex;
		fbw.A.fBoneWeight = BoneWeightTable[j].vBoneWeight[0].fBoneWeight;

		fbw.B.iBoneIndex = BoneWeightTable[j].vBoneWeight[1].iBoneIndex;
		fbw.B.fBoneWeight = BoneWeightTable[j].vBoneWeight[1].fBoneWeight;

		fbw.C.iBoneIndex = BoneWeightTable[j].vBoneWeight[2].iBoneIndex;
		fbw.C.fBoneWeight = BoneWeightTable[j].vBoneWeight[2].fBoneWeight;

		fbw.D.iBoneIndex = BoneWeightTable[j].vBoneWeight[3].iBoneIndex;
		fbw.D.fBoneWeight = BoneWeightTable[j].vBoneWeight[3].fBoneWeight;

		// Normalize it
		float sum = fbw.A.fBoneWeight + fbw.B.fBoneWeight + fbw.C.fBoneWeight + fbw.D.fBoneWeight;

		fbw.A.fBoneWeight /= sum;
		fbw.B.fBoneWeight /= sum;
		fbw.C.fBoneWeight /= sum;
		fbw.D.fBoneWeight /= sum;

		// push back
		FinalBoneWeightTableOut.push_back(fbw);
	}

	// print weights

	//printf("//VBO Bone_Weight\n");
	//printf("\n");
	//printf("Bone_weight BoneWeight[] =\n");
	//printf("{ \n");
	//for (int i = 0; i < NumVerts; i++)
	//{
	//	FinalVertEntry vTmp = VBO_vert[i];
	//	int index = vTmp.index;

	//	printf("\t {%0ff, %0ff, %0ff, %0ff}, // [%d] index:%d \n",
	//		FinalBoneWeightTableOut[index].A.fBoneWeight,
	//		FinalBoneWeightTableOut[index].B.fBoneWeight,
	//		FinalBoneWeightTableOut[index].C.fBoneWeight,
	//		FinalBoneWeightTableOut[index].D.fBoneWeight,
	//		i,
	//		index);
	//}
	//printf("}; \n");


	// print bone indicies

	

}

void Model::ProcessRiggedVerts(FbxMesh* pMeshIn, std::vector<VBO_VERTS>& VERTS_LIST_OUT, std::vector<TRILIST>& TRI_LIST_OUT)
{

	assert(pMeshIn != nullptr);

	std::vector<FBX_VERTS> FBX_VERTS_LIST;
	std::vector<VBO_VERTS> FBX_FAT_VERTS;
	std::vector<TRILIST> FBX_TRILIST;

	int PolygonCount = pMeshIn->GetPolygonCount();
	int ControlPointsCount = pMeshIn->GetControlPointsCount();

	FBX_VERTS_LIST.reserve(ControlPointsCount);
	FBX_FAT_VERTS.reserve(PolygonCount * 3);
	FBX_TRILIST.reserve(PolygonCount);

	VERTS_LIST_OUT.reserve(PolygonCount * 3);
	TRI_LIST_OUT.reserve(PolygonCount);

	//-------------------------
	//------Get FBX VERTS------
	//-------------------------

	FBX_VERTS vTmp;
	FbxVector4* pControlPoints = pMeshIn->GetControlPoints();
	bool FoundNormals = false;

	for (int i = 0; i < ControlPointsCount; i++)
	{
		printf(" %d: ----------\n", i);
		printf("       [%d] v: %f %f %f\n", i, pControlPoints[i][0],
			pControlPoints[i][1],
			pControlPoints[i][2]);

		vTmp.x = (float)pControlPoints[i][0];
		vTmp.y = (float)pControlPoints[i][1];
		vTmp.z = (float)pControlPoints[i][2];

		for (int j = 0; j < pMeshIn->GetElementNormalCount(); j++)
		{
			FbxGeometryElementNormal* pNormal = pMeshIn->GetElementNormal(j);
			if (pNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{

				printf("       [%d] n: %f %f %f\n", i, pNormal->GetDirectArray().GetAt(i)[0],
					pNormal->GetDirectArray().GetAt(i)[1],
					pNormal->GetDirectArray().GetAt(i)[2]);


				vTmp.nx = (float)(pNormal->GetDirectArray().GetAt(i)[0]);
				vTmp.ny = (float)(pNormal->GetDirectArray().GetAt(i)[1]);
				vTmp.nz = (float)(pNormal->GetDirectArray().GetAt(i)[2]);

				FoundNormals = true;

				printf("       --> FBX_V[i]: v:%f %f %f  n: %f %f %f\n", vTmp.x, vTmp.y, vTmp.z, vTmp.nx, vTmp.ny, vTmp.nz);
			}
			//else if(pNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			//{
			//	switch (pNormal->GetReferenceMode()) 
			//	{
			//	case FbxGeometryElement::eDirect:
			//		//Display3DVector(header, leNormal->GetDirectArray().GetAt(vertexId));

			//		printf("        index[%d] nxnynz: %f %f %f \n", j,
			//			pNormal->GetDirectArray().GetAt(i)[0],
			//			pNormal->GetDirectArray().GetAt(i)[1],
			//			pNormal->GetDirectArray().GetAt(i)[2]);

			//		vTmp.nx = (float)pNormal->GetDirectArray().GetAt(i)[0];
			//		vTmp.ny = (float)pNormal->GetDirectArray().GetAt(i)[1];
			//		vTmp.nz = (float)pNormal->GetDirectArray().GetAt(i)[2];

			//		break;
			//	case FbxGeometryElement::eIndexToDirect:
			//	{
			//		int id = pNormal->GetIndexArray().GetAt(i);
			//		//Display3DVector(header, leNormal->GetDirectArray().GetAt(id));

			//		printf("        index[%d] nxnynz: %f %f %f \n", j,
			//			pNormal->GetDirectArray().GetAt(id)[0],
			//			pNormal->GetDirectArray().GetAt(id)[1],
			//			pNormal->GetDirectArray().GetAt(id)[2]);

			//		vTmp.nx = (float)pNormal->GetDirectArray().GetAt(id)[0];
			//		vTmp.ny = (float)pNormal->GetDirectArray().GetAt(id)[1];
			//		vTmp.nz = (float)pNormal->GetDirectArray().GetAt(id)[2];
			//	}
			//	break;
			//	/*default:
			//		break;*/
			//	}
			//}
			else
			{
				//assert(false);
			}

		}

		FBX_VERTS_LIST.push_back(vTmp);
	}



	printf("\n\n\n FBX_VERTEX\n\n");
	for (int k = 0; k < (int)FBX_VERTS_LIST.size(); k++)
	{
		vTmp = FBX_VERTS_LIST[k];
		printf("FBX_V[%d]: v:%f %f %f  n: %f %f %f\n", k, vTmp.x, vTmp.y, vTmp.z, vTmp.nx, vTmp.ny, vTmp.nz);
	}

	//-------------------------------
	//------Create FAT FBX LIST------
	//-------------------------------


	printf("\n");
	printf("k1:  Num Polys: %d \n", PolygonCount);

	VBO_VERTS vTmp2;
	TRILIST	tmpTrilist;

	int trilist_count = 0;
	//int vertexId = 0;

	for (int i = 0; i < PolygonCount; i++)
	{
		printf("poly[%d]: --------------- \n", i);


		int PolygonSize = pMeshIn->GetPolygonSize(i);
		assert(PolygonSize == 3);

		for (int j = 0; j < PolygonSize; j++)
		{

			//GET XYZ AND NORMAL
			int ControlPointIndex = pMeshIn->GetPolygonVertex(i, j);

			printf("        index[%d] v: %f %f %f\n", j, pControlPoints[ControlPointIndex][0],
				pControlPoints[ControlPointIndex][1],
				pControlPoints[ControlPointIndex][2]);

			vTmp2.index = ControlPointIndex;
			vTmp2.x = FBX_VERTS_LIST[ControlPointIndex].x;
			vTmp2.y = FBX_VERTS_LIST[ControlPointIndex].y;
			vTmp2.z = FBX_VERTS_LIST[ControlPointIndex].z;


			if (FoundNormals) 
			{
				vTmp2.nx = FBX_VERTS_LIST[ControlPointIndex].nx;
				vTmp2.ny = FBX_VERTS_LIST[ControlPointIndex].ny;
				vTmp2.nz = FBX_VERTS_LIST[ControlPointIndex].nz;
			}

			if (j == 0)
			{
				tmpTrilist.v0 = trilist_count++;
			}
			else if (j == 1)
			{
				tmpTrilist.v1 = trilist_count++;
			}
			else if (j == 2)
			{
				tmpTrilist.v2 = trilist_count++;
			}
			else
			{
				assert(0 == 1);
			}

			//GET UV
			for (int l = 0; l < pMeshIn->GetElementUVCount(); ++l)
			{
				FbxGeometryElementUV* pUV = pMeshIn->GetElementUV(l);

				if (pUV->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {

					int lTextureUVIndex = pMeshIn->GetTextureUVIndex(i, j);

					if (pUV->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
						printf("        index[%d] uv: %f %f \n", j, pUV->GetDirectArray().GetAt(lTextureUVIndex)[0],
							pUV->GetDirectArray().GetAt(lTextureUVIndex)[1]);
						// IMPORTANT
						vTmp2.s = (float)pUV->GetDirectArray().GetAt(lTextureUVIndex)[0];
						vTmp2.t = (float)pUV->GetDirectArray().GetAt(lTextureUVIndex)[1];

						//printf("FBX_fat: v: %f %f %f s: %f %f n:%f %f %f \n", vTmp2.x, vTmp2.y, vTmp2.z, vTmp2.s, vTmp2.t, vTmp2.nx, vTmp2.ny, vTmp2.nz);

					}
				}
			}

			if (!FoundNormals) 
			{
				for (int k = 0; k < pMeshIn->GetElementNormalCount(); k++)
				{
					FbxGeometryElementNormal* pNormal = pMeshIn->GetElementNormal(k);
					if (pNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
					{
						switch (pNormal->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							//Display3DVector(header, leNormal->GetDirectArray().GetAt(vertexId));

							printf("        index[%d] nxnynz: %f %f %f \n", j,
								pNormal->GetDirectArray().GetAt(j)[0],
								pNormal->GetDirectArray().GetAt(j)[1],
								pNormal->GetDirectArray().GetAt(j)[2]);

							vTmp2.nx = (float)pNormal->GetDirectArray().GetAt(j)[0];
							vTmp2.ny = (float)pNormal->GetDirectArray().GetAt(j)[1];
							vTmp2.nz = (float)pNormal->GetDirectArray().GetAt(j)[2];

							break;
						case FbxGeometryElement::eIndexToDirect:

							int id = pNormal->GetIndexArray().GetAt(j);
							//Display3DVector(header, leNormal->GetDirectArray().GetAt(id));

							printf("        index[%d] nxnynz: %f %f %f \n", j,
								pNormal->GetDirectArray().GetAt(id)[0],
								pNormal->GetDirectArray().GetAt(id)[1],
								pNormal->GetDirectArray().GetAt(id)[2]);

							vTmp2.nx = (float)pNormal->GetDirectArray().GetAt(id)[0];
							vTmp2.ny = (float)pNormal->GetDirectArray().GetAt(id)[1];
							vTmp2.nz = (float)pNormal->GetDirectArray().GetAt(id)[2];

							break;

						}
					}
				}
			}

			//vertexId++;
			FBX_FAT_VERTS.push_back(vTmp2);

		} // for polygonSize

		FBX_TRILIST.push_back(tmpTrilist);
		printf("FBX_tri: %d %d %d \n", tmpTrilist.v0, tmpTrilist.v1, tmpTrilist.v2);

	} // for polygonCount


	//FAT PRINT
	printf("\n\n\nFBX_VERT_FAT\n\n");
	for (int k = 0; k < (int)FBX_FAT_VERTS.size(); k++)
	{
		vTmp2 = FBX_FAT_VERTS[k];
		printf("FBX_fat[%d]: v: %f %f %f s: %f %f n:%f %f %f \n", k, vTmp2.x, vTmp2.y, vTmp2.z, vTmp2.s, vTmp2.t, vTmp2.nx, vTmp2.ny, vTmp2.nz);
	}

	//TRILIST PRINT
	printf("\n\n\nFBX_TRILIST\n\n");
	for (int s = 0; s < (int)FBX_TRILIST.size(); s++)
	{
		tmpTrilist = FBX_TRILIST[s];
		printf("FBX_tri[%d]: %d %d %d \n", s, tmpTrilist.v0, tmpTrilist.v1, tmpTrilist.v2);
	}



	//-----------------------------------------------------
	//-------------------SORT FAT VERTS--------------------
	//-----------------------------------------------------

	std::vector<VBO_VERTS> FBX_SORTED_FAT_VERTS(FBX_FAT_VERTS);
	std::sort(FBX_SORTED_FAT_VERTS.begin(), FBX_SORTED_FAT_VERTS.end());

	// Print sorted Verts
	printf("\n\nFBX SORT\n\n");
	for (int i = 0; i < (int)FBX_SORTED_FAT_VERTS.size(); i++)
	{
		vTmp2 = FBX_SORTED_FAT_VERTS[i];
		printf("FBX_sort[%d]: v: %f %f %f s: %f %f n:%f %f %f \n",
			i, vTmp2.x, vTmp2.y, vTmp2.z, vTmp2.s, vTmp2.t, vTmp2.nx, vTmp2.ny, vTmp2.nz);
	}
	printf("\n\n");


	//Copy FAT VERTS to FINAL VBO VERTS
	VBO_VERTS currVert = FBX_SORTED_FAT_VERTS[0];
	VERTS_LIST_OUT.push_back(currVert);

	for (int i = 0; i < (int)FBX_SORTED_FAT_VERTS.size(); i++)
	{
		vTmp2 = FBX_SORTED_FAT_VERTS[i];

		//check for duplicate
		// if curr != next, then next is good so stort it into redux buffer
		if (!(vTmp2 == currVert))
		{
			currVert = vTmp2;
			VERTS_LIST_OUT.push_back(currVert);
		}
	}

	// print final VBO vert buffer
	for (int i = 0; i < (int)VERTS_LIST_OUT.size(); i++)
	{
		vTmp2 = VERTS_LIST_OUT[i];
		printf("VBO_vert[%d]: v: %f %f %f s: %f %f n:%f %f %f \n",
			i, vTmp2.x, vTmp2.y, vTmp2.z, vTmp2.s, vTmp2.t, vTmp2.nx, vTmp2.ny, vTmp2.nz);

	}
	printf("\n\n");
	assert(65536 > (int)VERTS_LIST_OUT.size());

	//------------------------------------------------
	//------------------Update Trilist----------------
	//------------------------------------------------

	std::vector< VBO_VERTS >::iterator it;

	int new_v0;
	int new_v1;
	int new_v2;
	TRILIST tmpVBOTri;

	for (int i = 0; i < (int)FBX_TRILIST.size(); i++)
	{
		TRILIST old_Tri = FBX_TRILIST[i];
		VBO_VERTS vert;

		// -- V0 ---------
		// find the old V0 in the fat list unsorted, get the vert
		// goto the new VBO_vert, find the vert get the index

		vert = FBX_FAT_VERTS[old_Tri.v0];

		it = std::find(VERTS_LIST_OUT.begin(), VERTS_LIST_OUT.end(), vert);

		new_v0 = std::distance(VERTS_LIST_OUT.begin(), it);

		// -- V1 ---------

		vert = FBX_FAT_VERTS[old_Tri.v1];

		it = std::find(VERTS_LIST_OUT.begin(), VERTS_LIST_OUT.end(), vert);

		new_v1 = std::distance(VERTS_LIST_OUT.begin(), it);

		// -- V2 ---------

		vert = FBX_FAT_VERTS[old_Tri.v2];

		it = std::find(VERTS_LIST_OUT.begin(), VERTS_LIST_OUT.end(), vert);

		new_v2 = std::distance(VERTS_LIST_OUT.begin(), it);

		// create the VBO_tri

		tmpVBOTri.v0 = (unsigned short)new_v0;
		tmpVBOTri.v1 = (unsigned short)new_v1;
		tmpVBOTri.v2 = (unsigned short)new_v2;

		TRI_LIST_OUT.push_back(tmpVBOTri);
	}

	// print VBO trilist 
	printf("\n\n");
	for (int j = 0; j < (int)TRI_LIST_OUT.size(); j++)
	{
		tmpVBOTri = TRI_LIST_OUT[j];
		printf("VBO_tri[%d]: %d %d %d\n", j, tmpVBOTri.v0, tmpVBOTri.v1, tmpVBOTri.v2);
	}

	printf("\n\n");


}

void Model::ProcessVerts(FbxMesh* pMeshIn)
{
	assert(pMeshIn != nullptr);

	std::vector<FBX_VERTS> FBX_VERTS_LIST;
	std::vector<VBO_VERTS> FBX_FAT_VERTS; 
	std::vector<TRILIST> FBX_TRILIST;

	int PolygonCount = pMeshIn->GetPolygonCount();
	int ControlPointsCount = pMeshIn->GetControlPointsCount();

	FBX_VERTS_LIST.reserve(ControlPointsCount);
	FBX_FAT_VERTS.reserve(PolygonCount * 3);
	FBX_TRILIST.reserve(PolygonCount);

	this->VBO_VERTS_LIST.reserve(PolygonCount * 3);
	this->VBO_TRILIST.reserve(PolygonCount);

	//-------------------------
	//------Get FBX VERTS------
	//-------------------------

	FBX_VERTS vTmp;
	FbxVector4* pControlPoints = pMeshIn->GetControlPoints();
	bool FoundNormals = false;

	for (int i = 0; i < ControlPointsCount; i++)
	{
		printf(" %d: ----------\n", i);
		printf("       [%d] v: %f %f %f\n", i, pControlPoints[i][0],
											   pControlPoints[i][1],
											   pControlPoints[i][2]);
		
		vTmp.x = (float)pControlPoints[i][0];
		vTmp.y = (float)pControlPoints[i][1];
		vTmp.z = (float)pControlPoints[i][2];

		for (int j = 0; j < pMeshIn->GetElementNormalCount(); j++)
		{
			FbxGeometryElementNormal* pNormal = pMeshIn->GetElementNormal(j);
			if (pNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{

				printf("       [%d] n: %f %f %f\n", i, pNormal->GetDirectArray().GetAt(i)[0],
													   pNormal->GetDirectArray().GetAt(i)[1],
													   pNormal->GetDirectArray().GetAt(i)[2]);

			
				vTmp.nx = (float)(pNormal->GetDirectArray().GetAt(i)[0]);
				vTmp.ny = (float)(pNormal->GetDirectArray().GetAt(i)[1]);
				vTmp.nz = (float)(pNormal->GetDirectArray().GetAt(i)[2]);

				FoundNormals = true;

				printf("       --> FBX_V[i]: v:%f %f %f  n: %f %f %f\n", vTmp.x, vTmp.y, vTmp.z, vTmp.nx, vTmp.ny, vTmp.nz);
			}
			
			else 
			{
				// problem? ed has hack here..
			}
			
		}

		FBX_VERTS_LIST.push_back(vTmp);
	}

	
	
	printf("\n\n\n FBX_VERTEX\n\n");
	for (int k = 0; k < (int)FBX_VERTS_LIST.size(); k++)
	{
		vTmp = FBX_VERTS_LIST[k];
		printf("FBX_V[%d]: v:%f %f %f  n: %f %f %f\n", k, vTmp.x, vTmp.y, vTmp.z, vTmp.nx, vTmp.ny, vTmp.nz);
	}

	//-------------------------------
	//------Create FAT FBX LIST------
	//-------------------------------


	printf("\n");
	printf("k1:  Num Polys: %d \n", PolygonCount);

	VBO_VERTS vTmp2;
	TRILIST	tmpTrilist;

	int trilist_count = 0;
	int vertexId = 0;

	for (int i = 0; i < PolygonCount; i++)
	{
		printf("poly[%d]: --------------- \n", i);


		int PolygonSize = pMeshIn->GetPolygonSize(i);
		assert(PolygonSize == 3);

		for (int j = 0; j < PolygonSize; j++)
		{

			//GET XYZ AND NORMAL
			int ControlPointIndex = pMeshIn->GetPolygonVertex(i, j);
			
			printf("        index[%d] v: %f %f %f\n", j, pControlPoints[ControlPointIndex][0],
														 pControlPoints[ControlPointIndex][1],
														 pControlPoints[ControlPointIndex][2]);
			
			vTmp2.x = FBX_VERTS_LIST[ControlPointIndex].x;
			vTmp2.y = FBX_VERTS_LIST[ControlPointIndex].y;
			vTmp2.z = FBX_VERTS_LIST[ControlPointIndex].z;
				
			vTmp2.nx = FBX_VERTS_LIST[ControlPointIndex].nx;
			vTmp2.ny = FBX_VERTS_LIST[ControlPointIndex].ny;
			vTmp2.nz = FBX_VERTS_LIST[ControlPointIndex].nz;

			if (j == 0)
			{
				tmpTrilist.v0 = trilist_count++;
			}
			else if (j == 1)
			{
				tmpTrilist.v1 = trilist_count++;
			}
			else if (j == 2)
			{
				tmpTrilist.v2 = trilist_count++;
			}
			else
			{
				assert(0 == 1);
			}

			//GET UV
			for (int l = 0; l < pMeshIn->GetElementUVCount(); ++l)
			{
				FbxGeometryElementUV* pUV = pMeshIn->GetElementUV(l);
				
				if (pUV->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
				
					int lTextureUVIndex = pMeshIn->GetTextureUVIndex(i, j);

					if (pUV->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
						printf("        index[%d] uv: %f %f \n", j, pUV->GetDirectArray().GetAt(lTextureUVIndex)[0],
							pUV->GetDirectArray().GetAt(lTextureUVIndex)[1]);
						// IMPORTANT
						vTmp2.s = (float)pUV->GetDirectArray().GetAt(lTextureUVIndex)[0];
						vTmp2.t = (float)pUV->GetDirectArray().GetAt(lTextureUVIndex)[1];

						printf("FBX_fat: v: %f %f %f s: %f %f n:%f %f %f \n", vTmp2.x, vTmp2.y, vTmp2.z, vTmp2.s, vTmp2.t, vTmp2.nx, vTmp2.ny, vTmp2.nz);

					}				
				}
			}
			if (!FoundNormals)
			{
				for (int k = 0; k < pMeshIn->GetElementNormalCount(); k++)
				{
					FbxGeometryElementNormal* pNormal = pMeshIn->GetElementNormal(k);
					if (pNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
					{
						switch (pNormal->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							//Display3DVector(header, leNormal->GetDirectArray().GetAt(vertexId));

							printf("        index[%d] nxnynz: %f %f %f \n", j,
								pNormal->GetDirectArray().GetAt(j)[0],
								pNormal->GetDirectArray().GetAt(j)[1],
								pNormal->GetDirectArray().GetAt(j)[2]);

							vTmp2.nx = (float)pNormal->GetDirectArray().GetAt(j)[0];
							vTmp2.ny = (float)pNormal->GetDirectArray().GetAt(j)[1];
							vTmp2.nz = (float)pNormal->GetDirectArray().GetAt(j)[2];

							break;
						case FbxGeometryElement::eIndexToDirect:

							int id = pNormal->GetIndexArray().GetAt(j);
							//Display3DVector(header, leNormal->GetDirectArray().GetAt(id));

							printf("        index[%d] nxnynz: %f %f %f \n", j,
								pNormal->GetDirectArray().GetAt(id)[0],
								pNormal->GetDirectArray().GetAt(id)[1],
								pNormal->GetDirectArray().GetAt(id)[2]);

							vTmp2.nx = (float)pNormal->GetDirectArray().GetAt(id)[0];
							vTmp2.ny = (float)pNormal->GetDirectArray().GetAt(id)[1];
							vTmp2.nz = (float)pNormal->GetDirectArray().GetAt(id)[2];

							break;

						}
					}
				}
			}

			vertexId++;
			FBX_FAT_VERTS.push_back(vTmp2);

		} // for polygonSize

		FBX_TRILIST.push_back(tmpTrilist);
		printf("FBX_tri: %d %d %d \n", tmpTrilist.v0, tmpTrilist.v1, tmpTrilist.v2);

	} // for polygonCount


	//FAT PRINT
	printf("\n\n\nFBX_VERT_FAT\n\n");
	for (int k = 0; k < (int)FBX_FAT_VERTS.size(); k++)
	{
		vTmp2 = FBX_FAT_VERTS[k];
		printf("FBX_fat[%d]: v: %f %f %f s: %f %f n:%f %f %f \n", k, vTmp2.x, vTmp2.y, vTmp2.z, vTmp2.s, vTmp2.t, vTmp2.nx, vTmp2.ny, vTmp2.nz);
	}

	//TRILIST PRINT
	printf("\n\n\nFBX_TRILIST\n\n");
	for (int s = 0; s < (int)FBX_TRILIST.size(); s++)
	{
		tmpTrilist = FBX_TRILIST[s];
		printf("FBX_tri[%d]: %d %d %d \n", s, tmpTrilist.v0, tmpTrilist.v1, tmpTrilist.v2);
	}



	//-----------------------------------------------------
	//-------------------SORT FAT VERTS--------------------
	//-----------------------------------------------------

	std::vector<VBO_VERTS> FBX_SORTED_FAT_VERTS(FBX_FAT_VERTS);
	std::sort(FBX_SORTED_FAT_VERTS.begin(), FBX_SORTED_FAT_VERTS.end());

	// Print sorted Verts
	printf("\n\nFBX SORT\n\n");
	for (int i = 0; i < (int)FBX_SORTED_FAT_VERTS.size(); i++)
	{
		vTmp2 = FBX_SORTED_FAT_VERTS[i];
		printf("FBX_sort[%d]: v: %f %f %f s: %f %f n:%f %f %f \n",
			i, vTmp2.x, vTmp2.y, vTmp2.z, vTmp2.s, vTmp2.t, vTmp2.nx, vTmp2.ny, vTmp2.nz);
	}
	printf("\n\n");


	//Copy FAT VERTS to FINAL VBO VERTS
	VBO_VERTS currVert = FBX_SORTED_FAT_VERTS[0];
	this->VBO_VERTS_LIST.push_back(currVert);

	for (int i = 0; i < (int)FBX_SORTED_FAT_VERTS.size(); i++)
	{
		vTmp2 = FBX_SORTED_FAT_VERTS[i];

		//check for duplicate
		// if curr != next, then next is good so stort it into redux buffer
		if (!(vTmp2  == currVert))
		{
			currVert = vTmp2;
			this->VBO_VERTS_LIST.push_back(currVert);
		}
	}

	// print final VBO vert buffer
	for (int i = 0; i < (int)this->VBO_VERTS_LIST.size(); i++)
	{
		vTmp2 = this->VBO_VERTS_LIST[i];
		printf("VBO_vert[%d]: v: %f %f %f s: %f %f n:%f %f %f \n",
			i, vTmp2.x, vTmp2.y, vTmp2.z, vTmp2.s, vTmp2.t, vTmp2.nx, vTmp2.ny, vTmp2.nz);

	}
	printf("\n\n");
	assert(65536 > (int)this->VBO_VERTS_LIST.size());

	//------------------------------------------------
	//------------------Update Trilist----------------
	//------------------------------------------------

	std::vector< VBO_VERTS >::iterator it;

	int new_v0;
	int new_v1;
	int new_v2;
	TRILIST tmpVBOTri;

	for (int i = 0; i < (int)FBX_TRILIST.size(); i++)
	{
		TRILIST old_Tri = FBX_TRILIST[i];
		VBO_VERTS vert;

		// -- V0 ---------
		// find the old V0 in the fat list unsorted, get the vert
		// goto the new VBO_vert, find the vert get the index

		vert = FBX_FAT_VERTS[old_Tri.v0];

		it = std::find(this->VBO_VERTS_LIST.begin(), this->VBO_VERTS_LIST.end(), vert);

		new_v0 = std::distance(this->VBO_VERTS_LIST.begin(), it);

		// -- V1 ---------

		vert = FBX_FAT_VERTS[old_Tri.v1];

		it = std::find(this->VBO_VERTS_LIST.begin(), this->VBO_VERTS_LIST.end(), vert);

		new_v1 = std::distance(VBO_VERTS_LIST.begin(), it);

		// -- V2 ---------

		vert = FBX_FAT_VERTS[old_Tri.v2];

		it = std::find(this->VBO_VERTS_LIST.begin(), this->VBO_VERTS_LIST.end(), vert);

		new_v2 = std::distance(this->VBO_VERTS_LIST.begin(), it);

		// create the VBO_tri

		tmpVBOTri.v0 = (unsigned short)new_v0;
		tmpVBOTri.v1 = (unsigned short)new_v1;
		tmpVBOTri.v2 = (unsigned short)new_v2;

		this->VBO_TRILIST.push_back(tmpVBOTri);
	}

	// print VBO trilist 
	printf("\n\n");
	for (int j = 0; j < (int)this->VBO_TRILIST.size(); j++)
	{
		tmpVBOTri = this->VBO_TRILIST[j];
		printf("VBO_tri[%d]: %d %d %d\n", j, tmpVBOTri.v0, tmpVBOTri.v1, tmpVBOTri.v2);
	}

	printf("\n\n");

}




