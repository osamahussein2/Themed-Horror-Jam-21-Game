#include "Text.h"

Game::Text::Text() : text(font)
{
}

Game::Text::~Text()
{
}

sf::Text Game::Text::InitializeText(const char* filePath_, sf::String string_, float characterSize_, bool centerText_,
	sf::Color textColor_, sf::Vector2f initialTextPos_)
{
	// Open the font from the file
	font.openFromFile(filePath_);

	// Set the font to the text object
	text.setFont(font);

	// Assign a string
	text.setString(string_);

	// Set the character size
	text.setCharacterSize(characterSize_);

	// Choose a color
	text.setFillColor(textColor_);

	sf::FloatRect textRect = text.getLocalBounds();

	if (centerText_ == true)
	{
		text.setOrigin(sf::Vector2f(textRect.position.x +
			textRect.size.x / 2.0f,
			textRect.position.y +
			textRect.size.y / 2.0f));
	}

	else if (centerText_ == false)
	{
		text.setOrigin(sf::Vector2f(textRect.position.x, textRect.position.y));
	}

	text.setPosition(initialTextPos_);

	return text;
}

sf::Text Game::Text::LoadText()
{
	return text;
}
