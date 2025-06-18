#ifndef OPERATION_SCENE_H
#define OPERATION_SCENE_H

#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>
#include "SpriteTexture.h"
#include <vector>

using namespace sf;

class OperationScene
{
public:
	OperationScene();
	~OperationScene();

	// Initialize the surgery room with an image file and screen resolution
	void Initialize(const char* filePath_, Vector2f position_, Vector2f scale_, bool centerTexture_ = false);

	void InitializeDot(Vector2f position_, float radius_, sf::Color fillColor_, sf::Color outlineColor_,
		float outlineThickness_, float spacingX_, float spacingY_);

	void InitializeDot(std::vector<Vector2f> position_, float radius_, sf::Color fillColor_, sf::Color outlineColor_,
		float outlineThickness_);

	void InitializeSuccessPanel(Vector2f position_, Vector2f scale_, sf::Color fillColor_, bool centerOrigin_ = false);

	// Draw the surgery room to the render window
	void Draw(RenderWindow& window);

	// Update operation scene position (useful for scrolling backgrounds)
	void SetPosition(Vector2f position);

	void Move(Vector2f offset);

	// Scale the operation scene
	void SetScale(Vector2f scale);

	std::vector<CircleShape> dotCircleShape;
	RectangleShape successPanel;

	unsigned int maxDots;

private:
	SpriteTexture operationSpriteTexture;
	Texture operationTexture;
	Sprite operationSprite;

	SpriteTexture GroundSpriteTexture;
	Texture GroundTexture;
	Sprite GroundSprite;
};

#endif