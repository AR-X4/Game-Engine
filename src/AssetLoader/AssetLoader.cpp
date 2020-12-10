#include "AssetLoader.h"
#include "File.h"
#include "PackageHeader.h"
#include "ChunkHeader.h"

#include "Models/ModelManager.h"
#include "Textures/TextureManager.h"

#include "Animation/SkeletonManager.h"
#include "Animation/ClipManager.h"


namespace Azul {




	Model* AssetLoader::LoadModel(const char* const pkgName)
	{
		//open pkg file in dir
		File::Handle pkgFile;
		File::Error status = File::Open(pkgFile, pkgName, File::Mode::READ);
		assert(status == File::Error::SUCCESS);

		//get file size
		unsigned int pkgSize = 0;
		status = File::Size(pkgFile, pkgSize);
		assert(status == File::Error::SUCCESS);

		//dump read into buffer
		unsigned char* pkgBuffer = new unsigned char[pkgSize];
		status = File::Read(pkgFile, pkgBuffer, pkgSize);
		assert(status == File::Error::SUCCESS);

		status = File::Close(pkgFile);
		assert(status == File::Error::SUCCESS);

		//get pgk header
		PackageHeader pkgHdr;
		memcpy_s(&pkgHdr, sizeof(PackageHeader), pkgBuffer, sizeof(PackageHeader));
		unsigned int NumChunks = pkgHdr.numChunks;

		//set ptr to top of pkg data
		unsigned char* pkgPtr = pkgBuffer;
		pkgPtr += sizeof(PackageHeader);


		//loop for num of chunks..? we know the ordering so loop unecessary?
		Model::Vert_xyzuvn* pVertBuffer = nullptr;
		Model::Tri_index* pTriBuffer  = nullptr;

		for (unsigned int i = 0; i < NumChunks; i++)
		{
			ChunkHeader* chunkPtr = (ChunkHeader*)pkgPtr;

			ChunkHeader tmpChunk;
			memcpy_s(&tmpChunk, sizeof(ChunkHeader), chunkPtr, sizeof(ChunkHeader));

			pkgPtr += sizeof(ChunkHeader);

			
			
			if (tmpChunk.type == ChunkType::VERTS_TYPE) 
			{

				//copy verts into buffer
				pVertBuffer = new Model::Vert_xyzuvn[pkgHdr.numVerts];
				memcpy_s(pVertBuffer, tmpChunk.chunkSize, pkgPtr, tmpChunk.chunkSize);
			}
			else if (tmpChunk.type == ChunkType::TRI_TYPE) 
			{

				//copy tris into buffer
				pTriBuffer = new Model::Tri_index[pkgHdr.numTris];
				memcpy_s(pTriBuffer, tmpChunk.chunkSize, pkgPtr, tmpChunk.chunkSize);

			}
			else if (tmpChunk.type == ChunkType::TEXTURE_TYPE){

				unsigned char* textureBuffer = new unsigned char[tmpChunk.chunkSize];
				memcpy_s(textureBuffer, tmpChunk.chunkSize, pkgPtr, tmpChunk.chunkSize);

				//hack
				if (!strcmp(tmpChunk.ChunkName, "space_frigate.tga")) {
					TextureManager::Add(tmpChunk.ChunkName, textureBuffer, TextureManager::Name::SPACE_SHIP);
				}
				else if (!strcmp(tmpChunk.ChunkName, "astroboy.tga")) {
					TextureManager::Add(tmpChunk.ChunkName, textureBuffer, TextureManager::Name::ASTROBOY);
				}
				else if (!strcmp(tmpChunk.ChunkName, "warbear.tga")) {
					TextureManager::Add(tmpChunk.ChunkName, textureBuffer, TextureManager::Name::WARBEAR);
				}
				else if (!strcmp(tmpChunk.ChunkName, "buggy_diff.tga")) {
					TextureManager::Add(tmpChunk.ChunkName, textureBuffer, TextureManager::Name::BUGGY);
				}
				else if (!strcmp(tmpChunk.ChunkName, "Pietta_txt_AL.tga")) {
					TextureManager::Add(tmpChunk.ChunkName, textureBuffer, TextureManager::Name::REVOLVER);
				}

				delete[] textureBuffer;

			}

			pkgPtr += tmpChunk.chunkSize;
		}

		delete[] pkgBuffer;


		//create new model w buffer inputs
		//add to model manager
		Model* loadedModel = new Model(pVertBuffer, pTriBuffer, pkgHdr.numVerts, pkgHdr.numTris);
		ModelManager::Add(loadedModel);

		
		return loadedModel;
	}
	void AssetLoader::LoadAnim(const char* const pkgName)
	{

		//open pkg file in dir
		File::Handle pkgFile;
		File::Error status = File::Open(pkgFile, pkgName, File::Mode::READ);
		assert(status == File::Error::SUCCESS);

		//get file size
		unsigned int pkgSize = 0;
		status = File::Size(pkgFile, pkgSize);
		assert(status == File::Error::SUCCESS);

		//dump read into buffer
		unsigned char* pkgBuffer = new unsigned char[pkgSize];
		status = File::Read(pkgFile, pkgBuffer, pkgSize);
		assert(status == File::Error::SUCCESS);

		status = File::Close(pkgFile);
		assert(status == File::Error::SUCCESS);

		//get pgk header
		PackageHeader pkgHdr;
		memcpy_s(&pkgHdr, sizeof(PackageHeader), pkgBuffer, sizeof(PackageHeader));
		unsigned int NumChunks = pkgHdr.numChunks;

		//set ptr to top of pkg data
		unsigned char* pkgPtr = pkgBuffer;
		pkgPtr += sizeof(PackageHeader);

		//declare buffers
		SkeletonHierarchy::BoneData* pBoneBuffer = nullptr;

		for (unsigned int i = 0; i < NumChunks; i++)
		{
			ChunkHeader* chunkPtr = (ChunkHeader*)pkgPtr;

			ChunkHeader tmpChunk;
			memcpy_s(&tmpChunk, sizeof(ChunkHeader), chunkPtr, sizeof(ChunkHeader));

			pkgPtr += sizeof(ChunkHeader);

			if (tmpChunk.type == ChunkType::ANIM_TYPE)
			{
				Bone* tmp = (Bone*)pkgPtr;
				unsigned int boneChunkSize = sizeof(Bone) * pkgHdr.numBones;

				//get head bucket
				Clip::FrameBucket* headBucket = new Clip::FrameBucket();
				headBucket->KeyTime = 0 * Time(Time::Duration::NTSC_30_FRAME);
				headBucket->poBone = new Bone[pkgHdr.numBones];
				memcpy_s(headBucket->poBone, boneChunkSize, tmp, boneChunkSize);
				tmp += pkgHdr.numBones;

				for (unsigned int j = 0; j < pkgHdr.numBones; j++)
				{
					headBucket->poBone[j].Q = Quat(Quat::Rot3::XYZ, headBucket->poBone[j].Q.qx(), headBucket->poBone[j].Q.qy(), headBucket->poBone[j].Q.qz());
				}

				Clip::FrameBucket* bucketTmp = headBucket;
				bucketTmp->prevBucket = nullptr;

				//get bucket for each frames
				for (int k = 1; k < 31; k++)
				{
					
					//Create frame bucket
					Clip::FrameBucket* bucket = new Clip::FrameBucket();
					bucket->poBone = new Bone[pkgHdr.numBones];
					memcpy_s(bucket->poBone, boneChunkSize, tmp, boneChunkSize);
					

					bucket->KeyTime = k * Time(Time::Duration::NTSC_30_FRAME);

					bucketTmp->nextBucket = bucket;
					bucket->prevBucket = bucketTmp;
					bucketTmp = bucket;



					//print
					for (unsigned int j = 0; j < pkgHdr.numBones; j++)
					{

						bucket->poBone[j].Q = Quat(Quat::Rot3::XYZ, bucket->poBone[j].Q.qx(), bucket->poBone[j].Q.qy(), bucket->poBone[j].Q.qz());


						/*Trace::out2("\nINDEX: %d\n\n", j);

						Trace::out2("Vect(%f, %f, %f)\nQuat(%f, %f, %f)\nVect(%f, %f, %f)\n\n",
							bucket->poBone[j].T.x(),  bucket->poBone[j].T.y(),  bucket->poBone[j].T.z(),
							bucket->poBone[j].Q.qx(), bucket->poBone[j].Q.qy(), bucket->poBone[j].Q.qz(),
							bucket->poBone[j].S.x(),  bucket->poBone[j].S.y(),  bucket->poBone[j].S.z());*/
					}

					
					tmp += pkgHdr.numBones;
				}

				//hack to get name
				Clip* pClip = nullptr;
				if (!strcmp(tmpChunk.ChunkName, "humanoid_run_ANIM"))
				{
					pClip = new Clip((int)pkgHdr.numBones, headBucket, Clip::Name::HUMANOID_RUN);
				}
				else if (!strcmp(tmpChunk.ChunkName, "punch_ANIM"))
				{
					pClip = new Clip((int)pkgHdr.numBones, headBucket, Clip::Name::PUNCH);
				}
				else if (!strcmp(tmpChunk.ChunkName, "teddy_ANIM"))
				{
					pClip = new Clip((int)pkgHdr.numBones, headBucket, Clip::Name::TEDDY);
				}
				else if (!strcmp(tmpChunk.ChunkName, "shotgun_ANIM"))
				{
					pClip = new Clip((int)pkgHdr.numBones, headBucket, Clip::Name::HUMANOID_SHOTGUN);
				}
				else {
					assert(false);
				}

				//add clip to clip manager
				ClipManager::Add(pClip);

			}
			else if (tmpChunk.type == ChunkType::SKEL_TYPE) 
			{
				//copy buffer into skel data
				pBoneBuffer = new SkeletonHierarchy::BoneData[pkgHdr.numBones];
				memcpy_s(pBoneBuffer, tmpChunk.chunkSize, pkgPtr, tmpChunk.chunkSize);

				//hack
				SkeletonHierarchy* skel = nullptr;

				if (!strcmp(tmpChunk.ChunkName, "humanoid_run_SKEL")) {
					skel = new SkeletonHierarchy(pBoneBuffer, (int)pkgHdr.numBones, SkeletonHierarchy::Name::HUMANOID_RUN);
				}
				else if (!strcmp(tmpChunk.ChunkName, "punch_SKEL")) {
					skel = new SkeletonHierarchy(pBoneBuffer, (int)pkgHdr.numBones, SkeletonHierarchy::Name::PUNCH);
				}
				else if (!strcmp(tmpChunk.ChunkName, "shotgun_SKEL")) {
					skel = new SkeletonHierarchy(pBoneBuffer, (int)pkgHdr.numBones, SkeletonHierarchy::Name::HUMANOID_SHOTGUN);
				}
				else if(!strcmp(tmpChunk.ChunkName, "teddy_SKEL"))
				{
					skel = new SkeletonHierarchy(pBoneBuffer, (int)pkgHdr.numBones, SkeletonHierarchy::Name::TEDDY);
				}

				SkeletonManager::Add(skel);

				//print test
				/*for (unsigned int j = 0; j < pkgHdr.numBones; j++)
				{
					Trace::out2("{ %s %d %d }\n", pBoneBuffer[j].name, pBoneBuffer[j].index, pBoneBuffer[j].parentIndex);
				}*/
			}

			pkgPtr += tmpChunk.chunkSize;
		}

		delete[] pkgBuffer;
		
	}
}
