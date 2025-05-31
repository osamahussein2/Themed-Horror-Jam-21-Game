#ifndef RENDERBACKGROUND_H
#define RENDERBACKGROUND_H
#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>
#include "SpriteTexture.h"

using namespace sf;
class RenderBackground
{
public:
    RenderBackground();
    ~RenderBackground();

    // Initialize the background with an image file and screen resolution
    bool Initialize(const char* filePath,Vector2u screenResolution);

    // Initialize with custom position and scale
    bool Initialize(const char* filePath, Vector2f position, Vector2f scale);

    // Draw the background to the render window
    void Draw(RenderWindow& window);

    // Update background position (useful for scrolling backgrounds)
    void SetPosition(Vector2f position);
    void Move(Vector2f offset);

    // Scale the background
    void SetScale(Vector2f scale);

    // Get the background sprite for additional manipulation if needed
    Sprite& GetSprite();

    // Check if background is loaded successfully
    bool IsLoaded() const { return isLoaded; }

    const char* GetTexture();
    void SetTexture(const char* filePath_);

    void Unload();

private:
    SpriteTexture backgroundSpriteTexture;
    const char* filePath;
    Texture backgroundTexture;
    Sprite backgroundSprite;
    bool isLoaded;

    // Helper function to scale background to fit screen
    void ScaleToFitScreen(sf::Vector2u screenResolution);
};

#endif