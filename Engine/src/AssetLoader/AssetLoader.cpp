#include "AssetLoader.h"
#include "File.h"
#include "PackageHeader.h"
#include "ChunkHeader.h"

#include "Models/ModelManager.h"
#include "Models/FBXModel.h"
#include "Textures/TextureManager.h"
#include "Animation/ClipManager.h"


namespace Azul {

	FBXModel* AssetLoader::LoadModel(const char* const pkgName)
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
					TextureManager::Add(tmpChunk.ChunkName, textureBuffer, Texture::Name::SPACE_SHIP);
				}
				else if (!strcmp(tmpChunk.ChunkName, "astroboy.tga")) {
					TextureManager::Add(tmpChunk.ChunkName, textureBuffer, Texture::Name::ASTROBOY);
				}
				else if (!strcmp(tmpChunk.ChunkName, "warbear.tga")) {
					TextureManager::Add(tmpChunk.ChunkName, textureBuffer, Texture::Name::WARBEAR);
				}
				else if (!strcmp(tmpChunk.ChunkName, "buggy_diff.tga")) {
					TextureManager::Add(tmpChunk.ChunkName, textureBuffer, Texture::Name::BUGGY);
				}
				else if (!strcmp(tmpChunk.ChunkName, "Pietta_txt_AL.tga")) {
					TextureManager::Add(tmpChunk.ChunkName, textureBuffer, Texture::Name::REVOLVER);
				}

				delete[] textureBuffer;

			}

			pkgPtr += tmpChunk.chunkSize;
		}

		delete[] pkgBuffer;


		//create new model w buffer inputs
		//add to model manager
		FBXModel* loadedModel = new FBXModel(pVertBuffer, pTriBuffer, pkgHdr.numVerts, pkgHdr.numTris);
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
		//SkeletonHierarchy::BoneData* pBoneBuffer = nullptr;

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
				headBucket->KeyTime = 0 * AnimTime(AnimTime::Duration::NTSC_30_FRAME);
				//headBucket->poBone = new Bone[pkgHdr.numBones];
				Bone* pBone = new Bone[pkgHdr.numBones];
				memcpy_s(pBone, boneChunkSize, tmp, boneChunkSize);
				tmp += pkgHdr.numBones;

				for (unsigned int j = 0; j < pkgHdr.numBones; j++)
				{
					pBone[j].Q = Quat(Quat::Rot3::XYZ, pBone[j].Q.qx(), pBone[j].Q.qy(), pBone[j].Q.qz());
				}

				// set keyframe SSBO
				headBucket->poKeyframe = new SSBO();
				headBucket->poKeyframe->Set(pkgHdr.numBones, sizeof(Bone), &pBone[0]);
				delete[] pBone;

				Clip::FrameBucket* bucketTmp = headBucket;
				bucketTmp->prevBucket = nullptr;

				//get bucket for each frames.. hack

				//const int numKeyFrames = anims.size() / pkgHdr.numBones;
				for (int k = 1; k < 31; k++)
				{

					//Create frame bucket
					Clip::FrameBucket* bucket = new Clip::FrameBucket();
					//bucket->poBone = new Bone[pkgHdr.numBones];
					pBone = new Bone[pkgHdr.numBones];
					memcpy_s(pBone, boneChunkSize, tmp, boneChunkSize);

					for (unsigned int j = 0; j < pkgHdr.numBones; j++)
					{
						pBone[j].Q = Quat(Quat::Rot3::XYZ, pBone[j].Q.qx(), pBone[j].Q.qy(), pBone[j].Q.qz());

						/*Trace::out2("\nINDEX: %d\n\n", j);

						Trace::out2("Vect(%f, %f, %f)\nQuat(%f, %f, %f)\nVect(%f, %f, %f)\n\n",
							bucket->poBone[j].T.x(),  bucket->poBone[j].T.y(),  bucket->poBone[j].T.z(),
							bucket->poBone[j].Q.qx(), bucket->poBone[j].Q.qy(), bucket->poBone[j].Q.qz(),
							bucket->poBone[j].S.x(),  bucket->poBone[j].S.y(),  bucket->poBone[j].S.z());*/
					}

					// set keyframe SSBO
					bucket->poKeyframe = new SSBO();
					bucket->poKeyframe->Set(pkgHdr.numBones, sizeof(Bone), &pBone[0]);
					delete[] pBone;

					bucket->KeyTime = k * AnimTime(AnimTime::Duration::NTSC_30_FRAME);

					bucketTmp->nextBucket = bucket;
					bucket->prevBucket = bucketTmp;
					bucketTmp = bucket;

					tmp += pkgHdr.numBones;
				}

				//hack to get name
				Clip* pClip = nullptr;
				if (!strcmp(tmpChunk.ChunkName, "walk.flcn_ANIM"))
				{
					pClip = new Clip((int)pkgHdr.numBones, headBucket, Clip::Name::WALK);
				}
				else if (!strcmp(tmpChunk.ChunkName, "humanCOMP.flcn_ANIM"))
				{
					pClip = new Clip((int)pkgHdr.numBones, headBucket, Clip::Name::COMPRESSED_RUN);
				}
				else {
					assert(false);
				}

				//add clip to clip manager
				ClipManager::Add(pClip);

			}
			
			else
			{
				assert(false);// something is wrong
			}

			pkgPtr += tmpChunk.chunkSize;
		}

		delete[] pkgBuffer;
	}
	FBXSkinnedModel* AssetLoader::LoadAnimModel(const char* const pkgName)
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
		//SkeletonHierarchy::BoneData* pBoneBuffer = nullptr;
		Model::Vert_xyzuvn* pVertBuffer = nullptr;
		Model::Tri_index* pTriBuffer = nullptr;
		Matrix* pInvPoseBuffer = nullptr;
		FBXSkinnedModel::Bone_weight* pWeightsBuffer = nullptr;
		FBXSkinnedModel::Bone_index* pBoneIndexBuffer = nullptr;
		int* pHierarchyTable = nullptr;


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
			else if (tmpChunk.type == ChunkType::ANIM_TYPE)
			{
				Bone* tmp = (Bone*)pkgPtr;
				unsigned int boneChunkSize = sizeof(Bone) * pkgHdr.numBones;

				//get head bucket
				Clip::FrameBucket* headBucket = new Clip::FrameBucket();
				headBucket->KeyTime = 0 * AnimTime(AnimTime::Duration::NTSC_30_FRAME);
				//headBucket->poBone = new Bone[pkgHdr.numBones];
				Bone* pBone = new Bone[pkgHdr.numBones];
				memcpy_s(pBone, boneChunkSize, tmp, boneChunkSize);
				tmp += pkgHdr.numBones;

				for (unsigned int j = 0; j < pkgHdr.numBones; j++)
				{
					pBone[j].Q = Quat(Quat::Rot3::XYZ, pBone[j].Q.qx(), pBone[j].Q.qy(), pBone[j].Q.qz());
				}

				// set keyframe SSBO
				headBucket->poKeyframe = new SSBO();
				headBucket->poKeyframe->Set(pkgHdr.numBones, sizeof(Bone), &pBone[0]);
				delete[] pBone;

				Clip::FrameBucket* bucketTmp = headBucket;
				bucketTmp->prevBucket = nullptr;

				//get bucket for each frames.. hack
				for (int k = 1; k < 31; k++)
				{

					//Create frame bucket
					Clip::FrameBucket* bucket = new Clip::FrameBucket();
					//bucket->poBone = new Bone[pkgHdr.numBones];
					pBone = new Bone[pkgHdr.numBones];
					memcpy_s(pBone, boneChunkSize, tmp, boneChunkSize);

					for (unsigned int j = 0; j < pkgHdr.numBones; j++)
					{
						pBone[j].Q = Quat(Quat::Rot3::XYZ, pBone[j].Q.qx(), pBone[j].Q.qy(), pBone[j].Q.qz());

						/*Trace::out2("\nINDEX: %d\n\n", j);

						Trace::out2("Vect(%f, %f, %f)\nQuat(%f, %f, %f)\nVect(%f, %f, %f)\n\n",
							bucket->poBone[j].T.x(),  bucket->poBone[j].T.y(),  bucket->poBone[j].T.z(),
							bucket->poBone[j].Q.qx(), bucket->poBone[j].Q.qy(), bucket->poBone[j].Q.qz(),
							bucket->poBone[j].S.x(),  bucket->poBone[j].S.y(),  bucket->poBone[j].S.z());*/
					}

					// set keyframe SSBO
					bucket->poKeyframe = new SSBO();
					bucket->poKeyframe->Set(pkgHdr.numBones, sizeof(Bone), &pBone[0]);
					delete[] pBone;

					bucket->KeyTime = k * AnimTime(AnimTime::Duration::NTSC_30_FRAME);

					bucketTmp->nextBucket = bucket;
					bucket->prevBucket = bucketTmp;
					bucketTmp = bucket;

					tmp += pkgHdr.numBones;
				}

				//hack to get name
				Clip* pClip = nullptr;
				if (!strcmp(tmpChunk.ChunkName, "humanoid2_ANIM"))
				{
					pClip = new Clip((int)pkgHdr.numBones, headBucket, Clip::Name::HUMANOID);
				}
				/*else if (!strcmp(tmpChunk.ChunkName, "punch_ANIM"))
				{
					pClip = new Clip((int)pkgHdr.numBones, headBucket, Clip::Name::PUNCH);
				}*/
				else if (!strcmp(tmpChunk.ChunkName, "teddySKIN2_ANIM"))
				{
					pClip = new Clip((int)pkgHdr.numBones, headBucket, Clip::Name::TEDDY);
				}
				/*else if (!strcmp(tmpChunk.ChunkName, "shotgun_ANIM"))
				{
					pClip = new Clip((int)pkgHdr.numBones, headBucket, Clip::Name::HUMANOID_SHOTGUN);
				}*/
				else {
					assert(false);
				}

				//add clip to clip manager
				ClipManager::Add(pClip);

			}
			else if (tmpChunk.type == ChunkType::SKEL_TYPE) 
			{
				
			}
			else if (tmpChunk.type == ChunkType::INV_POSE_TYPE)
			{
				pInvPoseBuffer = new Matrix[pkgHdr.numBones];
				memcpy_s(pInvPoseBuffer, tmpChunk.chunkSize, pkgPtr, tmpChunk.chunkSize);
			}
			else if (tmpChunk.type == ChunkType::WEIGHTS_TYPE)
			{
				pWeightsBuffer = new FBXSkinnedModel::Bone_weight[pkgHdr.numVerts];
				memcpy_s(pWeightsBuffer, tmpChunk.chunkSize, pkgPtr, tmpChunk.chunkSize);
			}
			else if (tmpChunk.type == ChunkType::INDEX_TYPE)
			{
				pBoneIndexBuffer = new FBXSkinnedModel::Bone_index[pkgHdr.numVerts];
				memcpy_s(pBoneIndexBuffer, tmpChunk.chunkSize, pkgPtr, tmpChunk.chunkSize);
			}
			else if (tmpChunk.type == ChunkType::TEXTURE_TYPE)
			{
				// TODO
			}
			else if (tmpChunk.type == ChunkType::HIERARCHY_TYPE)
			{
				pHierarchyTable = new int[pkgHdr.BoneDepth * pkgHdr.numBones];
				memcpy_s(pHierarchyTable, tmpChunk.chunkSize, pkgPtr, tmpChunk.chunkSize);
			}
			else 
			{
				assert(false);// something is wrong
			}

			pkgPtr += tmpChunk.chunkSize;
		}

		delete[] pkgBuffer;

		assert(pVertBuffer);
		assert(pTriBuffer);
		assert(pInvPoseBuffer);
		assert(pWeightsBuffer);
		assert(pBoneIndexBuffer);
		assert(pHierarchyTable);

		//create new model w buffer inputs
		//add to model manager
		FBXSkinnedModel* loadedModel = new FBXSkinnedModel(pVertBuffer, 
														pTriBuffer, 
														pWeightsBuffer, 
														pBoneIndexBuffer, 
														pInvPoseBuffer, 
														pkgHdr.numVerts, 
														pkgHdr.numTris, 
														pkgHdr.numBones, 
														pHierarchyTable, 
														pkgHdr.BoneDepth);
		ModelManager::Add(loadedModel);

		return loadedModel;
	}
}

