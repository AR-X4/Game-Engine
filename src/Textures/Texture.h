#ifndef TEXTURE_H
#define TEXTURE_H

#include "TextureManager.h"


namespace Azul
{

	class Texture : public DLink
	{
	public:
		Texture();

		Texture(const Texture&) = delete;
		Texture& operator = (const Texture&) = delete;
		~Texture();

		void Dump();
		void Wash();

		void set(const char* const _assetName,
			TextureManager::Name _name,
			GLuint _TextureID,
			GLenum minFilter,
			GLenum magFilter,
			GLenum horizWrapMode,
			GLenum vertWrapMode);

		void SetName(TextureManager::Name _name);

	private:
		char assetName[TextureManager::ASSET_NAME_SIZE];

	public:
		TextureManager::Name name;
		GLuint textureID;
		GLenum minFilter;
		GLenum magFilter;
		GLenum horizWrapMode;
		GLenum vertWrapMode;

		
	};

}

#endif

// --- End of File --------------------------------------------------
