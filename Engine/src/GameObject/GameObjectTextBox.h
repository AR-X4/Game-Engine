#ifndef GAMEOBJECT_TEXTBOX_H
#define GAMEOBJECT_TEXTBOX_H

#include "GameObject/GlyphSprite.h"
#include "Glyph/Glyph.h"
#include "2D_Primitives/Rect.h"

namespace Azul {

	class GameObjectTextBox : public GameObject
	{
	public:
		GameObjectTextBox() = delete;
		GameObjectTextBox(GraphicsObject* nullobject, Glyph::FontType _glyphName);
		GameObjectTextBox(const GameObjectTextBox&) = delete;
		GameObjectTextBox& operator=(GameObjectTextBox&) = delete;
		virtual ~GameObjectTextBox() override;


		virtual void Update(AnimTime currentTime) override;
		virtual void Draw() override;

		//update message
		void UpdateMessage(const char* message);


	private:
		const char* pMessage;
		GlyphSprite* pSprite;
		Rect pTextArea;
		Glyph::FontType glyphName;

	public:
		//float x;
		//float y;

	};





		//gameObjText owns 2D game object
			//gameobjText added to game obj man
				// nothing on update
				// draw updates and draws 2D game obj for every glyph in text box

		

}

#endif