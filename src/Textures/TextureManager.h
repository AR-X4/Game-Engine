#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "sb7.h"
#include "Manager/Manager.h"


namespace Azul
{
	class Texture;


	class TextureManager: public Manager
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
			NOT_INITIALIZED
		};

	private:
		// methods
		
		TextureManager(int reserveNum = 3, int reserveGrow = 1);
		TextureManager() = delete;
		TextureManager(const TextureManager&) = delete;
		TextureManager& operator = (const TextureManager&) = delete;
		~TextureManager();

		// helper private methods
		void privLoadTexture(const char* const _assetName, GLuint*& textureID);
		void privLoadTexture(const char* const _assetName, unsigned char* textBuffer, GLuint*& textureID);
		bool privLoadTGATexture(const char* szFileName, GLint minFilter, GLint magFilter, GLint horizWrapMode, GLint vertWrapMode);
		bool privLoadTGATexture(const char* szFileName, unsigned char* textBuffer, GLint minFilter, GLint magFilter, GLint horizWrapMode, GLint vertWrapMode);
		
		GLbyte* gltReadTGABits(const char* szFileName, GLint* iWidth, GLint* iHeight, GLint* iComponents, GLenum* eFormat);
		GLbyte* gltReadTGABits(const char* szFileName, unsigned char* textBuffer, GLint* iWidth, GLint* iHeight, GLint* iComponents, GLenum* eFormat);

	public:

		static void Create(int reserveNum = 3, int reserveGrow = 1);
		static void Destroy();
		static void Add(const char* const _assetName, const TextureManager::Name _name);
		static void Add(const char* const _assetName, unsigned char* textBuffer, const TextureManager::Name _name);


		static void DeleteAllTextures();
		static Texture* Find(const TextureManager::Name _name);

	protected:
		virtual DLink* DerivedCreateNode() override;
		virtual bool DerivedCompare(DLink* pLinkA, DLink* pLinkB) override;
		virtual void DerivedWash(DLink* pLink) override;
		virtual void DerivedDumpNode(DLink* pLink) override;
		virtual void DerivedDestroyNode(DLink* pLink) override;

	private:

		//Texture* active;
		static TextureManager* GetInstance();

		//DATA
		static TextureManager* poInstance;
		Texture* poNodeCompare;
	};

}

#endif

// --- End of File --------------------------------------------------
