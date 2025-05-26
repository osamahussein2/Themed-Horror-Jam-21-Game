#include "SpriteTexture.h"

SpriteTexture::SpriteTexture() : sprite(texture)
{
}

SpriteTexture::~SpriteTexture()
{
}

sf::Sprite SpriteTexture::InitializeSprite(const char* filePath_, sf::Vector2f initialSpritePos_)
{
	// Load a graphic into the texture
	texture.loadFromFile(filePath_);

	// Create a sprite from the texture
	sprite = sf::Sprite(texture);

	// Attach the texture to the sprite
	sprite.setTexture(texture);

	// Set the spriteBackground to cover the screen
	sprite.setPosition(initialSpritePos_);

	return sprite;
}

sf::Sprite SpriteTexture::LoadSprite()
{
	return sprite;
}
