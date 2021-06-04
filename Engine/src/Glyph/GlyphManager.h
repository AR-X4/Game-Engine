#ifndef GLYPH_MANAGER_H
#define GLYPH_MANAGER_H

#include "Manager/Manager.h"
#include "Glyph.h"

namespace Azul {

	class GlyphManager : public Manager
	{
	private:
		// methods

		GlyphManager(int reserveNum = 3, int reserveGrow = 1);
		GlyphManager() = delete;
		GlyphManager(const GlyphManager&) = delete;
		GlyphManager& operator = (const GlyphManager&) = delete;
		~GlyphManager();

	public:

		static void Create(int reserveNum = 3, int reserveGrow = 1);
		static void Destroy();
		
		static Glyph* Find(const Glyph::FontType _type, int key);
		static void ReadXML(Glyph::FontType FontName, const char* pFilename, Texture::Name textName);

	private:
		static void Add(Glyph::FontType FontName, Texture::Name textName, Rect rect, int _key);

	protected:
		virtual DLink* DerivedCreateNode() override;
		virtual bool DerivedCompare(DLink* pLinkA, DLink* pLinkB) override;
		virtual void DerivedWash(DLink* pLink) override;
		virtual void DerivedDumpNode(DLink* pLink) override;
		virtual void DerivedDestroyNode(DLink* pLink) override;

	private:

		//Texture* active;
		static GlyphManager* GetInstance();

		//DATA
		static GlyphManager* poInstance;
		Glyph* poNodeCompare;
	};

}

#endif
