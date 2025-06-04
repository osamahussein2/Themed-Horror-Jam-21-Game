#ifndef SPRITE_H
#define SPRITE_H

#include <SFML/Graphics.hpp>
#include <map>
#include <iostream>

class SpriteTexture
{
public:
	SpriteTexture();
	~SpriteTexture();

	sf::Sprite& InitializeSprite(const char* filePath_, sf::Vector2f initialSpritePos_, sf::Vector2f initialScale_ = 
		sf::Vector2f(1, 1));

	sf::Sprite& LoadSprite();

	const char* GetTexture();
	void SetTexture(const char* filePath_);

	sf::Color GetColor();
	void SetColor(sf::Color newColor);

	sf::Vector2f GetScale();
	void SetScale(sf::Vector2f newScale);

private:
	sf::Sprite sprite;
	sf::Texture texture;

	const char* filePath;
};

#endif