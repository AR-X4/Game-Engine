#ifndef TEXTURE_H
#define TEXTURE_H

#include "sb7.h"
#include "Manager/DLink.h"

namespace Azul
{

	class Texture : public DLink
	{
	public:

		static const unsigned int ASSET_NAME_SIZE = 64;

		enum class Name
		{
			DEFAULT,
			STONES,
			RED_BRICK,
			DUCKWEED,
			ROCKS,
			SPACE_SHIP,
			ASTROBOY,
			WARBEAR,
			POLARBEAR,
			BUGGY,
			REVOLVER,
			SPACEINVADERS_2D,
			ALIENS_2D,
			CONSOLAS36_FONT,
			TEDDY,
			NOT_INITIALIZED
		};

		Texture();

		Texture(const Texture&) = delete;
		Texture& operator = (const Texture&) = delete;
		~Texture();

		void Dump();
		void Wash();

		void set(const char* const _assetName,
			Name _name,
			GLuint _TextureID,
			GLenum minFilter,
			GLenum magFilter,
			GLenum horizWrapMode,
			GLenum vertWrapMode,
			int _width,
			int _height);

		void SetName(Name _name);

	private:
		char assetName[ASSET_NAME_SIZE];

	public:
		Name name;
		GLuint textureID;
		GLenum minFilter;
		GLenum magFilter;
		GLenum horizWrapMode;
		GLenum vertWrapMode;
		int width;
		int height;
		
	};

}

#endif

// --- End of File --------------------------------------------------
