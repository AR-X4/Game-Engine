#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "sb7.h"
#include "Manager/Manager.h"
#include "Texture.h"

namespace Azul
{
	//class Texture;


	class TextureManager: public Manager
	{
	public:
		

		

	private:
		// methods
		
		TextureManager(int reserveNum = 3, int reserveGrow = 1);
		TextureManager() = delete;
		TextureManager(const TextureManager&) = delete;
		TextureManager& operator = (const TextureManager&) = delete;
		~TextureManager();

		// helper private methods
		void privLoadTexture(const char* const _assetName, GLuint*& textureID, int& width, int& height);
		void privLoadTexture(const char* const _assetName, unsigned char* textBuffer, GLuint*& textureID, int& width, int& height);
		bool privLoadTGATexture(const char* szFileName, GLint minFilter, GLint magFilter, GLint horizWrapMode, GLint vertWrapMode, int& width, int& height);
		bool privLoadTGATexture(const char* szFileName, unsigned char* textBuffer, GLint minFilter, GLint magFilter, GLint horizWrapMode, GLint vertWrapMode, int& width, int& height);
		
		GLbyte* gltReadTGABits(const char* szFileName, GLint* iWidth, GLint* iHeight, GLint* iComponents, GLenum* eFormat);
		GLbyte* gltReadTGABits(const char* szFileName, unsigned char* textBuffer, GLint* iWidth, GLint* iHeight, GLint* iComponents, GLenum* eFormat);

	public:

		static void Create(int reserveNum = 3, int reserveGrow = 1);
		static void Destroy();
		static void Add(const char* const _assetName, const Texture::Name _name);
		static void Add(const char* const _assetName, unsigned char* textBuffer, const Texture::Name _name);


		static void DeleteAllTextures();
		static Texture* Find(const Texture::Name _name);

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
