#include "SpriteTexture.h"

SpriteTexture::SpriteTexture() : sprite(texture)
{
}

SpriteTexture::~SpriteTexture()
{
}

sf::Sprite SpriteTexture::InitializeSprite(const char* filePath_, sf::Vector2f initialSpritePos_, sf::Vector2f initialScale_)
{
	filePath = filePath_;

	// Load a graphic into the texture
	texture.loadFromFile(filePath_);

	// Create a sprite from the texture
	sprite = sf::Sprite(texture);

	// Attach the texture to the sprite
	sprite.setTexture(texture);

	sprite.setPosition(initialSpritePos_);
	sprite.setScale(initialScale_);

	return sprite;
}

sf::Sprite SpriteTexture::LoadSprite()
{
	return sprite;
}

const char* SpriteTexture::GetTexture()
{
	return filePath;
}

void SpriteTexture::SetTexture(const char* filePath_)
{
	texture.loadFromFile(filePath_);

	sprite.setTexture(texture);
}

sf::Color SpriteTexture::GetColor()
{
	return sprite.getColor();
}

void SpriteTexture::SetColor(sf::Color newColor)
{
	sprite.setColor(newColor);
}

sf::Vector2f SpriteTexture::GetScale()
{
	return sprite.getScale();
}

void SpriteTexture::SetScale(sf::Vector2f newScale)
{
	sprite.setScale(sprite.getScale());
}
