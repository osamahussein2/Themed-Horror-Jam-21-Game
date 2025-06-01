#ifndef SURGERY_ROOM_H
#define SURGERY_ROOM_H

#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>
#include "SpriteTexture.h"

using namespace sf;
class SurgeryRoom
{
public:
	SurgeryRoom();
	~SurgeryRoom();
	// Initialize the surgery room with an image file and screen resolution
	bool Initialize(const char* Backgroundpath, Vector2u screenResolution,
		const char* BouttomUIPath, const char* TopUIPath, const char* LifePath, const char* DeathPath, const char* Timer);

	// Draw the surgery room to the render window
	void Draw(RenderWindow& window);

	// Draw only UI elements (without background)
	void DrawUI(RenderWindow& window);

	// Update surgery room position (useful for scrolling backgrounds)
	void SetPosition(Sprite  sprite, Vector2f position);
	void Move(Sprite  sprite, Vector2f offset);
	// Scale the surgery room
	void SetScale(Sprite  sprite, Vector2f scale);

	// Check if surgery room is loaded successfully
	bool IsLoaded() const { return isLoaded; }
	int TimerValue = 0;

	Sprite BouttomUISprite;
	Sprite TopUISprite;
	Sprite LifeSprite_0;
	Sprite LifeSprite_1;
	Sprite LifeSprite_2;
	Sprite DeathSprite;
	Sprite TimerSprite;
private:

	SpriteTexture BouttomUISpriteTexture;
	Texture BouttomUITexture;

	SpriteTexture TopUISpriteTexture;
	Texture TopUITexture;

	SpriteTexture LifeSpriteTexture;
	Texture LifeTexture;

	SpriteTexture backgroundSpriteTexture;
	Texture backgroundTexture;
	Sprite backgroundSprite;

	SpriteTexture DeathSpriteTexture;
	Texture DeathTexture;

	SpriteTexture TimerSpriteTexture;
	Texture TimerTexture;

	bool isLoaded;
	// Helper function to scale background to fit screen
	void ScaleToFitScreen(sf::Vector2u screenResolution);
};

#endif