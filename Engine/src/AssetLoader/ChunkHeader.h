#ifndef CHUNK_HEADER_H
#define CHUNK_HEADER_H

enum class ChunkType : uint32_t 
{
	VERTS_TYPE = 0xA0000000u,
	TRI_TYPE,
	ANIM_TYPE,
	SKEL_TYPE,
	TEXTURE_TYPE,
	INV_POSE_TYPE,
	WEIGHTS_TYPE,
	INDEX_TYPE,
	HIERARCHY_TYPE,

	UNINITIALIZED
};

struct ChunkHeader
{
	//static const unsigned int CHUNK_NAME_SIZE = 64u;
	static const unsigned int CHUNK_NAME_SIZE = 20u;

	// data:
	char		   ChunkName[CHUNK_NAME_SIZE];
	ChunkType      type;
	unsigned int   chunkSize;
	
};

#endif