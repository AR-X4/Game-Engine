#ifndef CHUNK_HEADER_H
#define CHUNK_HEADER_H

enum class ChunkType : uint32_t 
{
	VERTS_TYPE = 0xA0000000u,
	TRI_TYPE,
	ANIM_TYPE,
	SKEL_TYPE,
	TEXTURE_TYPE,

	UNINITIALIZED
};

struct ChunkHeader
{
	static const unsigned int CHUNK_NAME_SIZE = 20;

	// data:
	char		   ChunkName[CHUNK_NAME_SIZE];
	ChunkType      type;
	unsigned int   chunkSize;
	
};

#endif