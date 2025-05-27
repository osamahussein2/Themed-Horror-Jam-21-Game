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

		sf::Text InitializeText(const char* filePath_, sf::String string_, float characterSize_, bool centerText_, 
			sf::Color textColor_, sf::Vector2f initialSpritePos_);

		sf::Text LoadText();

	private:
		sf::Text text;
		sf::Font font;
	};
}

#endif