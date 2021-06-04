#include "Exporter.h"
#include "ChunkHeader.h"
#include "PackageHeader.h"
#include "File.h"



bool Exporter::ExportStaticModelPackage(Model& pModelIn, const std::string& pFilename, std::vector<std::string>& TextureNames)
{
	int TotalPackageSize = 0;
	int TotalNumChunks = 0;

	//---Get str name and version---
	const std::string strName = pModelIn.GetName();
	const std::string strVersion = pModelIn.GetVersion();
	
	//----------PUT DATA INTO BUFFERS-------------

	std::vector<Model::TRILIST> pTRILIST = pModelIn.GetTriList();
	std::vector<Model::VBO_VERTS> pVERTSLIST = pModelIn.GetVertList();

	//Create Vert Buffer
	unsigned int VertSize = pVERTSLIST.size();
	Model::FINAL_VBO_VERTS* VERT_BUFFER = new Model::FINAL_VBO_VERTS[VertSize];

	// change this, overload copy assignment operator
	for (int i = 0; i < (int)VertSize; i++)
	{
		VERT_BUFFER[i].x = pVERTSLIST.at(i).x;
		VERT_BUFFER[i].y = pVERTSLIST.at(i).y;
		VERT_BUFFER[i].z = pVERTSLIST.at(i).z;

		VERT_BUFFER[i].t = pVERTSLIST.at(i).t;
		VERT_BUFFER[i].s = pVERTSLIST.at(i).s;

		VERT_BUFFER[i].nx = pVERTSLIST.at(i).nx;
		VERT_BUFFER[i].ny = pVERTSLIST.at(i).ny;
		VERT_BUFFER[i].nz = pVERTSLIST.at(i).nz;
	}

	std::string vertChunkName(strName + "_VERTS");

	ChunkHeader vertHdr;
	strncpy_s(vertHdr.ChunkName, vertHdr.CHUNK_NAME_SIZE, vertChunkName.c_str(), vertChunkName.length());
	vertHdr.type = ChunkType::VERTS_TYPE;
	vertHdr.chunkSize = sizeof(Model::FINAL_VBO_VERTS) * VertSize;

	//update total size and num chunks
	TotalPackageSize += sizeof(vertHdr);
	TotalPackageSize += vertHdr.chunkSize;
	TotalNumChunks++;

	//Create Tri Buffer
	unsigned int TriSize = pTRILIST.size();
	Model::TRILIST* TRI_BUFFER = new Model::TRILIST[TriSize];

	for (int i = 0; i < (int)TriSize; i++) {

		TRI_BUFFER[i] = pTRILIST.at(i);
	}

	std::string triChunkName(strName + "_TRIS");

	ChunkHeader triHdr;
	strncpy_s(triHdr.ChunkName, triHdr.CHUNK_NAME_SIZE, triChunkName.c_str(), triChunkName.length());
	triHdr.type = ChunkType::TRI_TYPE;
	triHdr.chunkSize = sizeof(Model::TRILIST) * TriSize;

	// update total size and num chunks
	TotalPackageSize += sizeof(triHdr);
	TotalPackageSize += triHdr.chunkSize;
	TotalNumChunks++;

	//Create Texture buffers 
	const int numTextures = TextureNames.size();
	std::vector<unsigned char*> TextureBuffers;
	TextureBuffers.reserve(numTextures);

	unsigned int TextFileSize = 0;

	for (int i = 0; i < numTextures; i++) {
	
		TextFileSize = 0;
		
		File::Handle textureFile;
		File::Error status = File::Open(textureFile, TextureNames.at(i).c_str(), File::Mode::READ);
		assert(status == File::Error::SUCCESS);

		status = File::Size(textureFile, TextFileSize);
		assert(status == File::Error::SUCCESS);

		unsigned char* texturebuff = new unsigned char[TextFileSize];

		status = File::Read(textureFile, texturebuff, TextFileSize);
		assert(status == File::Error::SUCCESS);

		status = File::Close(textureFile);
		assert(status == File::Error::SUCCESS);

		TextureBuffers.push_back(texturebuff);

		TotalPackageSize += sizeof(ChunkHeader);
		TotalPackageSize += TextFileSize;
		TotalNumChunks++;
	}

	//-------Create Package Hdr-------
	PackageHeader PkgHdr;

	strncpy_s(PkgHdr.packageName, PkgHdr.PACKAGE_NAME_SIZE, strName.c_str(), strName.length());
	strncpy_s(PkgHdr.versionString, PkgHdr.PACKAGE_VERSION_SIZE, strVersion.c_str(), strVersion.length());
	PkgHdr.numChunks = TotalNumChunks;
	PkgHdr.totalSize = TotalPackageSize;
	PkgHdr.numVerts = VertSize;
	PkgHdr.numTris = TriSize;


	//-----Create Package File--------
	File::Handle pkgFile;
	File::Error status = File::Open(pkgFile, pFilename.c_str(), File::Mode::WRITE);
	assert(status == File::Error::SUCCESS);

	status = File::Write(pkgFile, &PkgHdr, sizeof(PkgHdr));
	assert(status == File::Error::SUCCESS);


	//---Write in Verts
	status = File::Write(pkgFile, &vertHdr, sizeof(vertHdr));
	assert(status == File::Error::SUCCESS);

	status = File::Write(pkgFile, VERT_BUFFER, vertHdr.chunkSize);
	assert(status == File::Error::SUCCESS);

	//---Write in Tris
	status = File::Write(pkgFile, &triHdr, sizeof(triHdr));
	assert(status == File::Error::SUCCESS);

	status = File::Write(pkgFile, TRI_BUFFER, triHdr.chunkSize);
	assert(status == File::Error::SUCCESS);

	//Write in Texture data
	for (int i = 0; i < numTextures; i++) 
	{

		unsigned char* textBuffer = TextureBuffers.at(i);
		std::string textName = TextureNames.at(i);

		ChunkHeader textureHdr;
		strncpy_s(textureHdr.ChunkName, textureHdr.CHUNK_NAME_SIZE, textName.c_str(), textName.length());
		textureHdr.chunkSize = TextFileSize;
		textureHdr.type = ChunkType::TEXTURE_TYPE;
		
		status = File::Write(pkgFile, &textureHdr, sizeof(textureHdr));
		assert(status == File::Error::SUCCESS);

		status = File::Write(pkgFile, textBuffer, textureHdr.chunkSize);
		assert(status == File::Error::SUCCESS);

		delete[] textBuffer;
	}

	//Write in Amin data


	//---Clean Up---

	status = File::Close(pkgFile);
	assert(status == File::Error::SUCCESS);

	delete[] VERT_BUFFER;
	delete[] TRI_BUFFER;

	return true;
}

bool Exporter::ExportAnimModelPackage(std::vector<Model::VBO_VERTS>& VERTS_LIST, 
									std::vector<Model::TRILIST>& TRI_LIST, 
									std::vector<Model::Bone>& skel, 
									std::vector<Model::BonePos>& BoneAnims, 
									std::vector<Model::InvBonePose>& InvPoseTable, 
									std::vector<Model::FinalVertEntry>& FinalBoneWeightTable, 
									std::vector< int >& HierarchyTable,
									unsigned int BoneDepthIn,
									const std::string& pFilenameOut, 
									const std::string& modelName, 
									const std::string& versionName)
{
	

	int TotalPackageSize = 0;
	int TotalNumChunks = 0;

	//----------CREATE CHUNK HEADERS-------------

	// --- Create Vert chunk header --- 

	unsigned int VertSize = VERTS_LIST.size();
	Model::FINAL_VBO_VERTS* VERT_BUFFER = new Model::FINAL_VBO_VERTS[VertSize];

	// change this, overload copy assignment operator
	for (int i = 0; i < (int)VertSize; i++) 
	{
		VERT_BUFFER[i].x = VERTS_LIST.at(i).x;
		VERT_BUFFER[i].y = VERTS_LIST.at(i).y;
		VERT_BUFFER[i].z = VERTS_LIST.at(i).z;

		VERT_BUFFER[i].t = VERTS_LIST.at(i).t;
		VERT_BUFFER[i].s = VERTS_LIST.at(i).s;

		VERT_BUFFER[i].nx = VERTS_LIST.at(i).nx;
		VERT_BUFFER[i].ny = VERTS_LIST.at(i).ny;
		VERT_BUFFER[i].nz = VERTS_LIST.at(i).nz;
	}

	std::string vertChunkName(modelName + "_VERTS");

	ChunkHeader vertHdr;
	strncpy_s(vertHdr.ChunkName, vertHdr.CHUNK_NAME_SIZE, vertChunkName.c_str(), vertChunkName.length());
	vertHdr.type = ChunkType::VERTS_TYPE;
	vertHdr.chunkSize = sizeof(Model::FINAL_VBO_VERTS) * VertSize;

	//update total size and num chunks
	TotalPackageSize += sizeof(vertHdr);
	TotalPackageSize += vertHdr.chunkSize;
	TotalNumChunks++;


	// --- Create Tri chunk header ---

	unsigned int TriSize = TRI_LIST.size();
	std::string triChunkName(modelName + "_TRIS");

	ChunkHeader triHdr;
	strncpy_s(triHdr.ChunkName, triHdr.CHUNK_NAME_SIZE, triChunkName.c_str(), triChunkName.length());
	triHdr.type = ChunkType::TRI_TYPE;
	triHdr.chunkSize = sizeof(Model::TRILIST) * TriSize;

	// update total size and num chunks
	TotalPackageSize += sizeof(triHdr);
	TotalPackageSize += triHdr.chunkSize;
	TotalNumChunks++;

	// --- Create skel chunk header ---
	unsigned int SkelSize = skel.size();

	Model::FinalBone* skelBuffer = new Model::FinalBone[SkelSize];
	for (int i = 0; i < SkelSize; i++) 
	{
		skelBuffer[i] = Model::FinalBone(skel.at(i).BoneName, skel.at(i).BoneIndex, skel.at(i).ParentBoneIndex);
	}

	std::string skelChunkName(modelName + "_SKEL");

	ChunkHeader skelHdr;
	strncpy_s(skelHdr.ChunkName, skelHdr.CHUNK_NAME_SIZE, skelChunkName.c_str(), skelChunkName.length());
	skelHdr.type = ChunkType::SKEL_TYPE;
	skelHdr.chunkSize = sizeof(Model::FinalBone) * SkelSize;

	//update total size and num chunks
	TotalPackageSize += sizeof(skelHdr);
	TotalPackageSize += skelHdr.chunkSize;
	TotalNumChunks++;

	// --- Create anim chunk header ---
	unsigned int AnimSize = BoneAnims.size();
	std::string animChunkName(modelName + "_ANIM");

	ChunkHeader animHdr;
	strncpy_s(animHdr.ChunkName, animHdr.CHUNK_NAME_SIZE, animChunkName.c_str(), animChunkName.length());
	animHdr.type = ChunkType::ANIM_TYPE;
	animHdr.chunkSize = sizeof(Model::BonePos) * AnimSize;

	//update total size and num chunks
	TotalPackageSize += sizeof(animHdr);
	TotalPackageSize += animHdr.chunkSize;
	TotalNumChunks++;


	// --- Create Inverse Bone Pose header ---

	unsigned int PoseSize = InvPoseTable.size();
	Matrix* InvPoseBuffer = new Matrix[PoseSize];

	for (int i = 0; i < PoseSize; i++) 
	{
		InvPoseBuffer[i] = InvPoseTable[i].m;
	}

	std::string invChunkName(modelName + "_POSE");

	ChunkHeader InvPoseHdr;
	strncpy_s(InvPoseHdr.ChunkName, InvPoseHdr.CHUNK_NAME_SIZE, invChunkName.c_str(), invChunkName.length());
	InvPoseHdr.type = ChunkType::INV_POSE_TYPE;
	InvPoseHdr.chunkSize = sizeof(Matrix) * PoseSize;

	//update total size and num chunks
	TotalPackageSize += sizeof(InvPoseHdr);
	TotalPackageSize += InvPoseHdr.chunkSize;
	TotalNumChunks++;


	// --- Get bone weights and bone index ---

	Vect* weight_buff = new Vect[VertSize];
	Model::BoneIndex* index_buff = new Model::BoneIndex[VertSize];

	for (int i = 0; i < VertSize; i++)
	{
		int index = VERTS_LIST[i].index;

		weight_buff[i].x() = FinalBoneWeightTable[index].A.fBoneWeight;
		weight_buff[i].y() = FinalBoneWeightTable[index].B.fBoneWeight;
		weight_buff[i].z() = FinalBoneWeightTable[index].C.fBoneWeight;
		weight_buff[i].w() = FinalBoneWeightTable[index].D.fBoneWeight;

		index_buff[i].A = FinalBoneWeightTable[index].A.iBoneIndex;
		index_buff[i].B = FinalBoneWeightTable[index].B.iBoneIndex;
		index_buff[i].C = FinalBoneWeightTable[index].C.iBoneIndex;
		index_buff[i].D = FinalBoneWeightTable[index].D.iBoneIndex;

		printf("Index: %d  iA: %d, iB: %d, iC: %d, iD: %d\n", i, 
			index_buff[i].A,
			index_buff[i].B,
			index_buff[i].C,
			index_buff[i].D);
	}


	// --- Create bone weight header ---
	std::string weightChunkName(modelName + "_W");

	ChunkHeader WeightsHdr;
	strncpy_s(WeightsHdr.ChunkName, WeightsHdr.CHUNK_NAME_SIZE, weightChunkName.c_str(), weightChunkName.length());
	WeightsHdr.type = ChunkType::WEIGHTS_TYPE;
	WeightsHdr.chunkSize = sizeof(Vect) * VertSize;

	//update total size and num chunks
	TotalPackageSize += sizeof(WeightsHdr);
	TotalPackageSize += WeightsHdr.chunkSize;
	TotalNumChunks++;

	// --- Create bone index header ---
	std::string indexChunkName(modelName + "_IND");

	ChunkHeader indexHdr;
	strncpy_s(indexHdr.ChunkName, indexHdr.CHUNK_NAME_SIZE, indexChunkName.c_str(), indexChunkName.length());
	indexHdr.type = ChunkType::INDEX_TYPE;
	indexHdr.chunkSize = sizeof(Model::BoneIndex) * VertSize;

	//update total size and num chunks
	TotalPackageSize += sizeof(indexHdr);
	TotalPackageSize += indexHdr.chunkSize;
	TotalNumChunks++;

	// --- Create Hierarchy Table Header ---
	std::string hierChunkName(modelName + "_HIER");

	ChunkHeader hierHdr;
	strncpy_s(hierHdr.ChunkName, hierHdr.CHUNK_NAME_SIZE, hierChunkName.c_str(), hierChunkName.length());
	hierHdr.type = ChunkType::HIERARCHY_TYPE;
	hierHdr.chunkSize = sizeof(int) * HierarchyTable.size();

	//update total size and num chunks
	TotalPackageSize += sizeof(hierHdr);
	TotalPackageSize += hierHdr.chunkSize;
	TotalNumChunks++;

	//-------Create Package Hdr-------
	PackageHeader PkgHdr;

	strncpy_s(PkgHdr.packageName, PkgHdr.PACKAGE_NAME_SIZE, modelName.c_str(), modelName.length());
	strncpy_s(PkgHdr.versionString, PkgHdr.PACKAGE_VERSION_SIZE, versionName.c_str(), versionName.length());
	PkgHdr.numChunks = TotalNumChunks;
	PkgHdr.totalSize = TotalPackageSize;
	PkgHdr.numVerts = VertSize;
	PkgHdr.numTris = TriSize;
	PkgHdr.numBones = SkelSize;
	PkgHdr.BoneDepth = BoneDepthIn;

	//-----Create Package File--------
	File::Handle pkgFile;
	File::Error status = File::Open(pkgFile, pFilenameOut.c_str(), File::Mode::WRITE);
	assert(status == File::Error::SUCCESS);

	status = File::Write(pkgFile, &PkgHdr, sizeof(PkgHdr));
	assert(status == File::Error::SUCCESS);


	//---Write in verts---
	status = File::Write(pkgFile, &vertHdr, sizeof(vertHdr));
	assert(status == File::Error::SUCCESS);

	status = File::Write(pkgFile, VERT_BUFFER, vertHdr.chunkSize);
	assert(status == File::Error::SUCCESS);

	//---Write in Tris---
	status = File::Write(pkgFile, &triHdr, sizeof(triHdr));
	assert(status == File::Error::SUCCESS);

	status = File::Write(pkgFile, TRI_LIST.data(), triHdr.chunkSize);
	assert(status == File::Error::SUCCESS);

	//---Write in skel---
	status = File::Write(pkgFile, &skelHdr, sizeof(skelHdr));
	assert(status == File::Error::SUCCESS);

	status = File::Write(pkgFile, skelBuffer, skelHdr.chunkSize);
	assert(status == File::Error::SUCCESS);

	//---Write in Anims---
	status = File::Write(pkgFile, &animHdr, sizeof(animHdr));
	assert(status == File::Error::SUCCESS);

	status = File::Write(pkgFile, BoneAnims.data(), animHdr.chunkSize);
	assert(status == File::Error::SUCCESS);

	//---Write in inv pose---
	status = File::Write(pkgFile, &InvPoseHdr, sizeof(InvPoseHdr));
	assert(status == File::Error::SUCCESS);

	status = File::Write(pkgFile, InvPoseBuffer, InvPoseHdr.chunkSize);
	assert(status == File::Error::SUCCESS);

	//---Write in weights---
	status = File::Write(pkgFile, &WeightsHdr, sizeof(WeightsHdr));
	assert(status == File::Error::SUCCESS);

	status = File::Write(pkgFile, weight_buff, WeightsHdr.chunkSize);
	assert(status == File::Error::SUCCESS);

	//---Write in indicies---
	status = File::Write(pkgFile, &indexHdr, sizeof(indexHdr));
	assert(status == File::Error::SUCCESS);

	status = File::Write(pkgFile, index_buff, indexHdr.chunkSize);
	assert(status == File::Error::SUCCESS);

	//---Write in hierarchy table---
	status = File::Write(pkgFile, &hierHdr, sizeof(hierHdr));
	assert(status == File::Error::SUCCESS);

	status = File::Write(pkgFile, HierarchyTable.data(), hierHdr.chunkSize);
	assert(status == File::Error::SUCCESS);


	//---Clean Up---
	status = File::Close(pkgFile);
	assert(status == File::Error::SUCCESS);

	delete[] VERT_BUFFER;
	delete[] index_buff;
	delete[] weight_buff;
	delete[] InvPoseBuffer;
	delete[] skelBuffer;

	return true;
}

bool Exporter::ExportAnimPackage(std::vector<Model::BonePos>& BoneAnims, const std::string& pFilenameOut, const std::string& pkgName, const std::string& versionName)
{

	int TotalPackageSize = 0;
	int TotalNumChunks = 0;

	// --- Create anim chunk header ---
	unsigned int AnimSize = BoneAnims.size();
	std::string animChunkName(pkgName + "_ANIM");

	ChunkHeader animHdr;
	strncpy_s(animHdr.ChunkName, animHdr.CHUNK_NAME_SIZE, animChunkName.c_str(), animChunkName.length());
	animHdr.type = ChunkType::ANIM_TYPE;
	animHdr.chunkSize = sizeof(Model::BonePos) * AnimSize;

	//update total size and num chunks
	TotalPackageSize += sizeof(animHdr);
	TotalPackageSize += animHdr.chunkSize;
	TotalNumChunks++;

	//-------Create Package Hdr-------
	PackageHeader PkgHdr;

	strncpy_s(PkgHdr.packageName, PkgHdr.PACKAGE_NAME_SIZE, pkgName.c_str(), pkgName.length());
	strncpy_s(PkgHdr.versionString, PkgHdr.PACKAGE_VERSION_SIZE, versionName.c_str(), versionName.length());
	PkgHdr.numChunks = TotalNumChunks;
	PkgHdr.totalSize = TotalPackageSize;
	//PkgHdr.numVerts = VertSize;
	//PkgHdr.numTris = TriSize;
	PkgHdr.numBones = 80;
	//PkgHdr.BoneDepth = BoneDepthIn;

	//-----Create Package File--------
	File::Handle pkgFile;
	File::Error status = File::Open(pkgFile, pFilenameOut.c_str(), File::Mode::WRITE);
	assert(status == File::Error::SUCCESS);

	status = File::Write(pkgFile, &PkgHdr, sizeof(PkgHdr));
	assert(status == File::Error::SUCCESS);

	//---Write in Anims---
	status = File::Write(pkgFile, &animHdr, sizeof(animHdr));
	assert(status == File::Error::SUCCESS);

	status = File::Write(pkgFile, BoneAnims.data(), animHdr.chunkSize);
	assert(status == File::Error::SUCCESS);

	//---Clean Up---
	status = File::Close(pkgFile);
	assert(status == File::Error::SUCCESS);

	return true;
}


