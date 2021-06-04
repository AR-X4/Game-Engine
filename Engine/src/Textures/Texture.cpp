

#include "Texture.h"


namespace Azul
{

	Texture::Texture()
		: name(Name::NOT_INITIALIZED),
		textureID(0),
		minFilter(GL_LINEAR),
		magFilter(GL_LINEAR),
		horizWrapMode(GL_CLAMP_TO_EDGE),
		vertWrapMode(GL_CLAMP_TO_EDGE),
		width(-1),
		height(-1)
	{
		memset(this->assetName, 0, Texture::ASSET_NAME_SIZE);

		
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
				Name::NOT_INITIALIZED,
				0, 
				GL_LINEAR, 
				GL_LINEAR, 
				GL_CLAMP_TO_EDGE,
				GL_CLAMP_TO_EDGE,
				-1,
				-1);
	}

	void Texture::set(const char* const _assetName,
		Name _name,
		GLuint _TextureID,
		GLenum _minFilter,
		GLenum _magFilter,
		GLenum _horizWrapMode,
		GLenum _vertWrapMode,
		int _width,
		int _height)
	{
		memset(this->assetName, 0x0, Texture::ASSET_NAME_SIZE);
		memcpy(this->assetName, _assetName, Texture::ASSET_NAME_SIZE - 1);
		this->name = _name;
		this->magFilter = _magFilter;
		this->minFilter = _minFilter;
		this->horizWrapMode = _horizWrapMode;
		this->vertWrapMode = _vertWrapMode;
		this->textureID = _TextureID;
		this->width = _width;
		this->height = _height;
	}

	void Texture::SetName(Texture::Name _name)
	{
		this->name = _name;
	}

}

// --- End of File ---
