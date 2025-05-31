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
	// Update surgery room position (useful for scrolling backgrounds)
	void SetPosition(Sprite  sprite, Vector2f position);
	void Move(Sprite  sprite, Vector2f offset);
	// Scale the surgery room
	void SetScale(Sprite  sprite, Vector2f scale);

	// Check if surgery room is loaded successfully
	bool IsLoaded() const { return isLoaded; }
	int TimerValue = 0;
private:

	SpriteTexture backgroundSpriteTexture;
	Texture backgroundTexture;
	Sprite backgroundSprite;

	SpriteTexture BouttomUISpriteTexture;
	Texture BouttomUITexture;
	Sprite BouttomUISprite;


	SpriteTexture TopUISpriteTexture;
	Texture TopUITexture;
	Sprite TopUISprite;


	SpriteTexture LifeSpriteTexture;
	Texture LifeTexture;
	Sprite LifeSprite;



	SpriteTexture DeathSpriteTexture;
	Texture DeathTexture;
	Sprite DeathSprite;


	SpriteTexture TimerSpriteTexture;
	Texture TimerTexture;
	Sprite TimerSprite;

	bool isLoaded;
	// Helper function to scale background to fit screen
	void ScaleToFitScreen(sf::Vector2u screenResolution);
};

#endif