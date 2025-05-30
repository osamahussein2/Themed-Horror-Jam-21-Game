#ifndef TEXT_H
#define TEXT_H

#include <SFML/Graphics.hpp>

namespace Game
{
	class Text
	{
	public:
		Text();
		~Text();

		sf::Text InitializeText(const char* filePath_, float characterSize_, bool centerText_, bool alignTextRight_, 
			sf::Color textColor_, sf::Vector2f initialTextPos_);

		sf::Text InitializeText(const char* filePath_, sf::String string_, float characterSize_, bool centerText_,
			bool alignTextRight_, sf::Color textColor_, sf::Vector2f initialTextPos_);

		sf::Text LoadText();

		size_t GetStringSize() { return text.getString().getSize(); }
		sf::String GetString() { return text.getString(); }

		void SetTypewriterString(sf::String string_)  { text.setString(string_); }

		sf::Vector2f GetTextPosition() { return text.getPosition(); }
		void SetTextPosition(sf::Vector2f newPosition_) { text.setPosition(newPosition_); }

		sf::Color GetTextColor() { return text.getFillColor(); }
		void SetTextColor(sf::Color newColor_) { text.setFillColor(newColor_); }

		sf::String GetText() { return text.getString(); }
		void SetText(sf::String string_) { text.setString(string_); }

		float GetCharaterSize() { return text.getCharacterSize(); }
		void SetCharacterSize(float newCharacterSize) { text.setCharacterSize(newCharacterSize); }

	private:
		sf::Text text;
		sf::Font font;

		sf::Vector2f textSize;
	};
}

#endif