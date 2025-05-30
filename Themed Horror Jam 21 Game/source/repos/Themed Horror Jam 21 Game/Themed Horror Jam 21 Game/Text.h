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

	private:
		sf::Text text;
		sf::Font font;

		sf::Vector2f textSize;
	};
}

#endif