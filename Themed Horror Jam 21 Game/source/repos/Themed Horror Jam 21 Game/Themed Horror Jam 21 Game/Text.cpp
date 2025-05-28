#include "Text.h"

Game::Text::Text() : text(font)
{
}

Game::Text::~Text()
{
}

sf::Text Game::Text::InitializeText(const char* filePath_, sf::String string_, float characterSize_, bool centerText_,
	bool alignTextRight_, sf::Color textColor_, sf::Vector2f initialTextPos_)
{
	textPosition = initialTextPos_; // Return the text position being passed in to parameter

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

	textSize = sf::Vector2f(text.getGlobalBounds().size.x / 2.0f, text.getGlobalBounds().size.y / 2.0f);

	sf::FloatRect textRect = text.getLocalBounds();

	// If the center is true, then center the text
	if (centerText_ == true)
	{
		text.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f,
			textRect.position.y + textRect.size.y / 2.0f));
	}

	// If center text is false, then align the text left or right depending on the align text boolean
	else if (centerText_ == false)
	{
		if (alignTextRight_ == false) text.setOrigin(sf::Vector2f(textRect.position.x, textRect.position.y));

		else if (alignTextRight_ == true)
			text.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x, textRect.position.y + textRect.size.y));
	}

	text.setPosition(initialTextPos_);

	return text;
}

sf::Text Game::Text::LoadText()
{
	return text;
}
