#ifndef ITEM_TABLE_H
#define ITEM_TABLE_H
#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>
#include "SpriteTexture.h"

using namespace sf;
class ItemTable
{
public:
	ItemTable();
	~ItemTable();

	void Initialize(const char* filePath_, Vector2f position_, Vector2f scale_, bool centerTexture_ = false);

	// Draw the surgery room to the render window
	void Draw(RenderWindow& window);

	// Update operation scene position (useful for scrolling backgrounds)
	void SetPosition(Vector2f position);

	void Move(Vector2f offset);

	// Scale the operation scene
	void SetScale(Vector2f scale);

private:
    SpriteTexture ItemtableSpriteTexture;
    Texture ItemtableTexture;
    Sprite ItemtableSprite;

	SpriteTexture GroundSpriteTexture;
	Texture GroundTexture;
	Sprite GroundSprite;
};

#endif

