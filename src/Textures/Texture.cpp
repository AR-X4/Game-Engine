

#include "Texture.h"


namespace Azul
{

	Texture::Texture()
		: name(TextureManager::Name::NOT_INITIALIZED),
		textureID(0),
		minFilter(GL_LINEAR),
		magFilter(GL_LINEAR),
		horizWrapMode(GL_CLAMP_TO_EDGE),
		vertWrapMode(GL_CLAMP_TO_EDGE)
	{
		memset(this->assetName, 0, TextureManager::ASSET_NAME_SIZE);

		
	}

	Texture::~Texture()
	{
	}

	void Texture::Dump()
	{
	}

	void Texture::Wash()
	{
		this->set("Uninitialized",
				TextureManager::Name::NOT_INITIALIZED,
				0, 
				GL_LINEAR, 
				GL_LINEAR, 
				GL_CLAMP_TO_EDGE,
				GL_CLAMP_TO_EDGE);
	}

	void Texture::set(const char* const _assetName,
		TextureManager::Name _name,
		GLuint _TextureID,
		GLenum _minFilter,
		GLenum _magFilter,
		GLenum _horizWrapMode,
		GLenum _vertWrapMode)
	{
		memset(this->assetName, 0x0, TextureManager::ASSET_NAME_SIZE);
		memcpy(this->assetName, _assetName, TextureManager::ASSET_NAME_SIZE - 1);
		this->name = _name;
		this->magFilter = _magFilter;
		this->minFilter = _minFilter;
		this->horizWrapMode = _horizWrapMode;
		this->vertWrapMode = _vertWrapMode;
		this->textureID = _TextureID;
	}

	void Texture::SetName(TextureManager::Name _name)
	{
		this->name = _name;
	}

}

// --- End of File ---
