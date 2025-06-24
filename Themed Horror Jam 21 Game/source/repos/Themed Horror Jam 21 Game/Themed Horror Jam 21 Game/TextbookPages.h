#ifndef TEXTBOOKPAGES_H
#define TEXTBOOKPAGES_H

#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>
#include "SpriteTexture.h"
#include "Text.h"
#include "Audio.h"

using namespace sf;

class TextbookPages
{
public:
    TextbookPages();
    ~TextbookPages();

    // Initialize with custom position and scale
    void Initialize(const char* filePath_, Vector2f position_, Vector2f scale_, bool centerTexture_,
        const char* fontPath_, Vector2f leftTextPosition_, Vector2f rightTextPosition_, float characterSize_,
        Vector2f buttonSize_, Vector2f leftButtonPosition_, Vector2f rightButtonPosition_);

    void UpdateTextbookPages(Vector2f mousePos);
    void ClearTextVector();

    // Draw the textbook pages to the render window
    void Draw(RenderWindow& window);

    // Set the textbook pages position or move it just in case
    void SetPosition(Vector2f position);
    void Move(Vector2f offset);

    // Scale the background
    void SetScale(Vector2f scale);

    // Get the background sprite for additional manipulation if needed
    Sprite& GetSprite();

    const char* GetTexture();

private:
    SpriteTexture textbookSpriteTexture;
    Texture textbookTexture;
    Sprite textbookSprite;

    SpriteTexture GroundSpriteTexture;
    Texture GroundTexture;
    Sprite GroundSprite;

    Audio pageFlipSound;

    Game::Text leftPageText;
    Game::Text rightPageText;

    std::vector<std::string> leftPageTexts;
    std::vector<std::string> rightPageTexts;

    int pageNumber;

    RectangleShape turnPageLeftButton, turnPageRightButton;
    Texture leftButtonTexture, rightButtonTexture;

    bool mouseClicked;
};

#endif