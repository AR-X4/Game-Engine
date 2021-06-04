#include "GlyphManager.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"


using namespace rapidxml;

namespace Azul 
{

	GlyphManager* GlyphManager::poInstance = nullptr;

	GlyphManager::GlyphManager(int reserveNum, int reserveGrow)
		:poNodeCompare(new Glyph())
	{
		this->BaseInitialize(reserveNum, reserveGrow);

		//this->poNodeCompare = new Glyph();
		assert(this->poNodeCompare != nullptr);
	}

	GlyphManager::~GlyphManager()
	{
		delete this->poNodeCompare;
	}

	void GlyphManager::Create(int reserveNum, int reserveGrow)
	{
		assert(reserveNum > 0);
		assert(reserveGrow > 0);

		// initialize the singleton here
		assert(poInstance == nullptr);

		// Do the initialization
		if (poInstance == nullptr)
		{
			poInstance = new GlyphManager(reserveNum, reserveGrow);
		}
	}

	void GlyphManager::Destroy()
	{
		GlyphManager* pMan = GlyphManager::GetInstance();
		assert(pMan != nullptr);

		pMan->BaseDestroy();

		delete pMan->poInstance;
	}

	void GlyphManager::Add(Glyph::FontType FontName, Texture::Name textName, Rect rect, int _key)
	{
		GlyphManager* pMan = GlyphManager::GetInstance();
		assert(pMan != nullptr);

		Glyph* pNode = (Glyph*)pMan->BaseAdd();

		pNode->Set(FontName, textName, rect, _key);
	}

	Glyph* GlyphManager::Find(const Glyph::FontType _type, int key)
	{
		GlyphManager* pMan = GlyphManager::GetInstance();
		assert(pMan != nullptr);

		pMan->poNodeCompare->Set(_type, key);

		Glyph* pNode = (Glyph*)pMan->BaseFind(pMan->poNodeCompare);
		assert(pNode != nullptr);

		return pNode;
	}

	void GlyphManager::ReadXML(Glyph::FontType FontName, const char* pFilename, Texture::Name textName)
	{



		// TODO: DIRTY, make safer

		file<> xmlFile(pFilename);
		xml_document<> doc;
		doc.parse<0>(xmlFile.data());
		
		xml_node<>* node = doc.first_node();
		node = node->first_node();
		while(node != 0)
		{
			if (node->type() == node_type::node_element)
			{
				Rect rect;
				int key;

				//Trace::out2("\nELEMENT: %s\n%s\n", node->name(), node->value());
				
				// key
				xml_attribute<>* attribute = node->first_attribute();
				key = std::stoi(attribute->value());
				//Trace::out2("%s: %d\n", attribute->name(), key);

				// x
				xml_node<>* child = node->first_node();
				rect.x = (float)std::stoi(child->value());
				//Trace::out2("%s: %s\n", child->name(), child->value());

				// y
				child = child->next_sibling();
				rect.y = (float)std::stoi(child->value());
				//Trace::out2("%s: %s\n", child->name(), child->value());

				// width
				child = child->next_sibling();
				rect.width = (float)std::stoi(child->value());
				//Trace::out2("%s: %s\n", child->name(), child->value());

				// height
				child = child->next_sibling();
				rect.height = (float)std::stoi(child->value());
				//Trace::out2("%s: %s\n", child->name(), child->value());

				// add glyph
				GlyphManager::Add(FontName, textName, rect, key);
			}

			node = node->next_sibling();
		}
	}

	DLink* GlyphManager::DerivedCreateNode()
	{
		DLink* pNode = new Glyph();
		assert(pNode != nullptr);

		return pNode;
	}

	bool GlyphManager::DerivedCompare(DLink* pLinkA, DLink* pLinkB)
	{
		assert(pLinkA != nullptr);
		assert(pLinkB != nullptr);

		Glyph* pDataA = (Glyph*)pLinkA;
		Glyph* pDataB = (Glyph*)pLinkB;

		bool status = false;


		if (pDataA->FontName == pDataB->FontName &&
			pDataA->key == pDataB->key)
		{
			status = true;
		}

		return status;
	}

	void GlyphManager::DerivedWash(DLink* pLink)
	{
		assert(pLink != nullptr);
		Glyph* pNode = (Glyph*)pLink;
		pNode->Wash();
	}

	void GlyphManager::DerivedDumpNode(DLink* pLink)
	{
		assert(pLink != nullptr);
		Glyph* pData = (Glyph*)pLink;
		pData->Dump();
	}

	void GlyphManager::DerivedDestroyNode(DLink* pLink)
	{
		assert(pLink);
		Glyph* pData = (Glyph*)pLink;
		delete pData;
	}

	GlyphManager* GlyphManager::GetInstance()
	{
		assert(poInstance != nullptr);
		return poInstance;
	}

}