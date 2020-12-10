

#include "TGA.h"
#include "TextureManager.h"
#include "Texture.h"

namespace Azul
{

	TextureManager* TextureManager::poInstance = nullptr;


	TextureManager::TextureManager(int reserveNum, int reserveGrow)
	{
		this->BaseInitialize(reserveNum, reserveGrow);

		this->poNodeCompare = new Texture();
		assert(this->poNodeCompare != nullptr);

	}

	TextureManager::~TextureManager()
	{
		delete this->poNodeCompare;
	}

	void TextureManager::Create(int reserveNum, int reserveGrow)
	{
		assert(reserveNum > 0);
		assert(reserveGrow > 0);

		// initialize the singleton here
		assert(poInstance == nullptr);

		// Do the initialization
		if (poInstance == nullptr)
		{
			poInstance = new TextureManager(reserveNum, reserveGrow);
		}
	}


	void TextureManager::Destroy()
	{
		TextureManager* pMan = TextureManager::GetInstance();
		assert(pMan != nullptr);

		pMan->BaseDestroy();

	
		delete pMan->poInstance;
	}

	TextureManager* TextureManager::GetInstance()
	{
		assert(poInstance != nullptr);
		return poInstance;
	}

	void TextureManager::Add(const char* const _assetName, const TextureManager::Name _name)
	{
		GLuint               textureID;
		GLuint* pTextureID = &textureID;

		// Get the instance to the manager
		TextureManager* pTextMan = TextureManager::GetInstance();

		// Load the texture and get the textureID
		pTextMan->privLoadTexture(_assetName, pTextureID);

		// Create a TextureNode
		Texture* pNode = (Texture*)pTextMan->BaseAdd();

		// initialize it
		pNode->set(_assetName, _name, textureID, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

		
	}

	void TextureManager::Add(const char* const _assetName, unsigned char* textBuffer, const TextureManager::Name _name)
	{
		GLuint               textureID;
		GLuint* pTextureID = &textureID;

		// Get the instance to the manager
		TextureManager* pTextMan = TextureManager::GetInstance();

		// Load the texture and get the textureID
		pTextMan->privLoadTexture(_assetName, textBuffer, pTextureID);

		// Create a TextureNode
		Texture* pNode = (Texture*)pTextMan->BaseAdd();

		// initialize it
		pNode->set(_assetName, _name, textureID, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);


	}

	Texture* TextureManager::Find(const TextureManager::Name _name)
	{
		// Get the instance to the manager
		TextureManager* pTextMan = TextureManager::GetInstance();
		assert(pTextMan != nullptr);

		pTextMan->poNodeCompare->SetName(_name);

		Texture* pNode = (Texture*)pTextMan->BaseFind(pTextMan->poNodeCompare);
		assert(pNode != nullptr);
		//Texture* pNode = (Texture*)pTextMan->active;
		//while (pNode != 0)
		//{
		//	if (pNode->name == _name)
		//	{
		//		// found it
		//		break;
		//	}

		//	pNode = (Texture*)pNode->next;
		//}

		return pNode;
	}


	void TextureManager::DeleteAllTextures()
	{
		// Get the instance to the manager
		TextureManager* pTextMan = TextureManager::GetInstance();

		Texture* pNode = (Texture*)pTextMan->BaseGetActiveHead();
		while (pNode != 0)
		{
			// bye bye
			glDeleteTextures(1, &pNode->textureID);

			// next
			pNode = (Texture*)pNode->pNext;
		}
	}

	void TextureManager::privLoadTexture(const char* const _assetName, GLuint*& textureID)
	{
		// Get an ID, for textures (OpenGL poor man's handle)
		glGenTextures(1, textureID);

		// Bind it.
		glBindTexture(GL_TEXTURE_2D, *textureID);

		// Loat the texture
		this->privLoadTGATexture(_assetName, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	}

	void TextureManager::privLoadTexture(const char* const _assetName, unsigned char* textBuffer, GLuint*& textureID)
	{
		// Get an ID, for textures (OpenGL poor man's handle)
		glGenTextures(1, textureID);

		// Bind it.
		glBindTexture(GL_TEXTURE_2D, *textureID);

		// Loat the texture
		this->privLoadTGATexture(_assetName, textBuffer, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

	}

	// Load a TGA as a 2D Texture. Completely initialize the state
	bool TextureManager::privLoadTGATexture(const char* szFileName, GLint minFilter, GLint magFilter, GLint horizWrapMode, GLint vertWrapMode)
	{
		GLbyte* pBits;
		int nWidth, nHeight, nComponents;
		GLenum eFormat;

		// Read the texture bits
		pBits = gltReadTGABits(szFileName, &nWidth, &nHeight, &nComponents, &eFormat);
		if (pBits == nullptr)
		{
			return false;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, horizWrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vertWrapMode);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBits);

		free(pBits);

		if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
			minFilter == GL_LINEAR_MIPMAP_NEAREST ||
			minFilter == GL_NEAREST_MIPMAP_LINEAR ||
			minFilter == GL_NEAREST_MIPMAP_NEAREST)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		return true;
	}

	bool TextureManager::privLoadTGATexture(const char* szFileName, unsigned char* textBuffer, GLint minFilter, GLint magFilter, GLint horizWrapMode, GLint vertWrapMode)
	{
		GLbyte* pBits;
		int nWidth, nHeight, nComponents;
		GLenum eFormat;

		// Read the texture bits
		pBits = gltReadTGABits(szFileName, textBuffer, &nWidth, &nHeight, &nComponents, &eFormat);
		if (pBits == nullptr)
		{
			return false;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, horizWrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vertWrapMode);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBits);

		free(pBits);

		if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
			minFilter == GL_LINEAR_MIPMAP_NEAREST ||
			minFilter == GL_NEAREST_MIPMAP_LINEAR ||
			minFilter == GL_NEAREST_MIPMAP_NEAREST)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		return true;
	}

	////////////////////////////////////////////////////////////////////
	// Allocate memory and load targa bits. Returns pointer to new buffer,
	// height, and width of texture, and the OpenGL format of data.
	// Call free() on buffer when finished!
	// This only works on pretty vanilla targas... 8, 24, or 32 bit color
	// only, no palettes, no RLE encoding.
	GLbyte* TextureManager::gltReadTGABits(const char* szFileName, GLint* iWidth, GLint* iHeight, GLint* iComponents, GLenum* eFormat)
	{
		FILE* pFile;			// File pointer
		TGAHEADER tgaHeader;		// TGA file header
		unsigned long lImageSize;		// Size in bytes of image
		short sDepth;			// Pixel depth;
		GLbyte* pBits = NULL;          // Pointer to bits

		// Default/Failed values
		*iWidth = 0;
		*iHeight = 0;
		*eFormat = GL_RGB;
		*iComponents = GL_RGB;

		// Attempt to open the file
		fopen_s(&pFile, szFileName, "rb");

		if (pFile == nullptr)
		{
			assert(false);
			return nullptr;
		}

		// Read in header (binary)
		fread(&tgaHeader, 18/* sizeof(TGAHEADER)*/, 1, pFile);


		// Get width, height, and depth of texture
		*iWidth = tgaHeader.width;
		*iHeight = tgaHeader.height;
		sDepth = tgaHeader.bits / 8;

		// Put some validity checks here. Very simply, I only understand
		// or care about 8, 24, or 32 bit targa's.
		if (tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
			return nullptr;

		// Calculate size of image buffer
		lImageSize = (unsigned int)tgaHeader.width * (unsigned int)tgaHeader.height * (unsigned int)sDepth;

		// Allocate memory and check for success
		pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
		if (pBits == nullptr)
			return nullptr;

		// Read in the bits
		// Check for read error. This should catch RLE or other 
		// weird formats that I don't want to recognize
		if (fread(pBits, lImageSize, 1, pFile) != 1)
		{
			free(pBits);
			return nullptr;
		}

		// Set OpenGL format expected
		switch (sDepth)
		{

		case 3:     // Most likely case
			*eFormat = GL_BGR;
			*iComponents = GL_RGB;
			break;

		case 4:
			*eFormat = GL_BGRA;
			*iComponents = GL_RGBA;
			break;
		case 1:
			assert(false);
			// bad case - keenan
			// i commented out
		  //  *eFormat = GL_LUMINANCE;
		  //  *iComponents = GL_LUMINANCE;
			break;
		default:        // RGB
			// If on the iPhone, TGA's are BGR, and the iPhone does not 
			// support BGR without alpha, but it does support RGB,
			// so a simple swizzle of the red and blue bytes will suffice.
			// For faster iPhone loads however, save your TGA's with an Alpha!

			break;
		}

		// Done with File
		fclose(pFile);

		// Return pointer to image data
		return pBits;
	}

	GLbyte* TextureManager::gltReadTGABits(const char* szFileName, unsigned char* textBuffer, GLint* iWidth, GLint* iHeight, GLint* iComponents, GLenum* eFormat)
	{
		AZUL_UNUSED_VAR(szFileName);

		//FILE* pFile;			// File pointer
		TGAHEADER tgaHeader;		// TGA file header
		unsigned long lImageSize;		// Size in bytes of image
		short sDepth;			// Pixel depth;
		GLbyte* pBits = NULL;          // Pointer to bits

		// Default/Failed values
		*iWidth = 0;
		*iHeight = 0;
		*eFormat = GL_RGB;
		*iComponents = GL_RGB;

		// Attempt to open the file
		/*fopen_s(&pFile, szFileName, "rb");

		if (pFile == nullptr)
		{
			assert(false);
			return nullptr;
		}*/

		// Read in header (binary)
		//fread(&tgaHeader, 18/* sizeof(TGAHEADER)*/, 1, pFile);

		unsigned char* tgaPtr = textBuffer;

		memcpy_s(&tgaHeader, sizeof(tgaHeader), tgaPtr, sizeof(tgaHeader));

		tgaPtr += sizeof(tgaHeader);

		// Get width, height, and depth of texture
		*iWidth = tgaHeader.width;
		*iHeight = tgaHeader.height;
		sDepth = tgaHeader.bits / 8;

		// Put some validity checks here. Very simply, I only understand
		// or care about 8, 24, or 32 bit targa's.
		if (tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
			return nullptr;

		// Calculate size of image buffer
		lImageSize = (unsigned int)tgaHeader.width * (unsigned int)tgaHeader.height * (unsigned int)sDepth;

		// Allocate memory and check for success
		pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
		if (pBits == nullptr)
			return nullptr;

		// Read in the bits
		// Check for read error. This should catch RLE or other 
		// weird formats that I don't want to recognize
		/*if (fread(pBits, lImageSize, 1, pFile) != 1)
		{
			free(pBits);
			return nullptr;
		}*/
		
		memcpy_s(pBits, lImageSize, tgaPtr, lImageSize);
		//delete[] tgaPtr;

		// Set OpenGL format expected
		switch (sDepth)
		{

		case 3:     // Most likely case
			*eFormat = GL_BGR;
			*iComponents = GL_RGB;
			break;

		case 4:
			*eFormat = GL_BGRA;
			*iComponents = GL_RGBA;
			break;
		case 1:
			assert(false);
			// bad case - keenan
			// i commented out
		  //  *eFormat = GL_LUMINANCE;
		  //  *iComponents = GL_LUMINANCE;
			break;
		default:        // RGB
			// If on the iPhone, TGA's are BGR, and the iPhone does not 
			// support BGR without alpha, but it does support RGB,
			// so a simple swizzle of the red and blue bytes will suffice.
			// For faster iPhone loads however, save your TGA's with an Alpha!

			break;
		}

		// Done with File
		//fclose(pFile);

		// Return pointer to image data
		return pBits;
	}


	//-------------------------------------------------------
	//-------------------------------------------------------
	//-------------------------------------------------------

	DLink* TextureManager::DerivedCreateNode()
	{
		DLink* pNode = new Texture();
		assert(pNode != nullptr);

		return pNode;
	}

	bool TextureManager::DerivedCompare(DLink* pLinkA, DLink* pLinkB)
	{
		// This is used in baseFind() 
		assert(pLinkA != nullptr);
		assert(pLinkB != nullptr);

		Texture* pDataA = (Texture*)pLinkA;
		Texture* pDataB = (Texture*)pLinkB;

		bool status = false;
		

		if (pDataA->name == pDataB->name)
		{
			status = true;
		}

		return status;
	}

	void TextureManager::DerivedWash(DLink* pLink)
	{
		assert(pLink != nullptr);
		Texture* pNode = (Texture*)pLink;
		pNode->Wash();
	}

	void TextureManager::DerivedDumpNode(DLink* pLink)
	{
		assert(pLink != nullptr);
		Texture* pData = (Texture*)pLink;
		pData->Dump();
	}

	void TextureManager::DerivedDestroyNode(DLink* pLink)
	{
		assert(pLink);
		Texture* pData = (Texture*)pLink;
		delete pData;
	}
}

// --- End of File -----

