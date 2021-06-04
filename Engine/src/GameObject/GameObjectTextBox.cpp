#include "GameObjectTextbox.h"
#include "Glyph/GlyphManager.h"
#include "GraphicsObject/GraphicsObject_Glyph.h"
#include "Models/Sprite.h"
#include "Models/ModelManager.h"
#include "Shaders/ShaderManager.h"


namespace Azul {
	GameObjectTextBox::GameObjectTextBox(GraphicsObject* nullobject, Glyph::FontType _glyphName)
		:GameObject(nullobject), glyphName(_glyphName)
	{

		// create 2D game object sprite
		Sprite* pTextBoxModel = new Sprite(Model::Name::TEXTBOX);
		assert(pTextBoxModel);
		ModelManager::Add(pTextBoxModel);
		
		GraphicsObject_Glyph* graphicsObj = new GraphicsObject_Glyph(pTextBoxModel, ShaderManager::Find(ShaderObject::Name::SPRITE));
		this->pSprite = new GlyphSprite(graphicsObj);

	}

	GameObjectTextBox::~GameObjectTextBox()
	{
		delete pSprite;
	}

	void GameObjectTextBox::Update(AnimTime currentTime)
	{
		// do nothing
		assert(pSprite);
	}

	void GameObjectTextBox::Draw()
	{
		float xTmp = pPos->x();
		float yTmp = pPos->y();

		float xEnd = pPos->x();

		for (int i = 0; i < (int)strlen(pMessage); i++)
		{
			int key = (uint8_t)pMessage[i];// correct?

			Glyph* pGlyph = GlyphManager::Find(this->glyphName, key);
			assert(pGlyph);

			xTmp = xEnd + pGlyph->glyphRect.width / 2;
			/*this->pTextArea.x = xTmp;
			this->pTextArea.y = yTmp;
			this->pTextArea.width = pGlyph->glyphRect.width;
			this->pTextArea.height = pGlyph->glyphRect.height;*/
			
			//pAzulSprite.Swap(pGlyph.GetAzulTexture(), pGlyph.GetAzulSubRect(), this.pScreenRect, this.pColor);
			pSprite->posX = xTmp;
			pSprite->posY = yTmp;
			GraphicsObject_Glyph* graphicsObj = (GraphicsObject_Glyph*)pSprite->GetGraphicsObject();
			graphicsObj->Swap(pGlyph, pGlyph->glyphRect);

			pSprite->Update(AnimTime());
			pSprite->Draw();

			// move the starting to the next character
			xEnd = pGlyph->glyphRect.width / 2 + xTmp;
		}
	}
	void GameObjectTextBox::UpdateMessage(const char* message)
	{
		assert(message);
		this->pMessage = message;
	}
}