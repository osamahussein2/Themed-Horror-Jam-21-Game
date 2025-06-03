#include "ItemTable.h"

ItemTable::ItemTable(): isLoaded(false), backgroundSprite(backgroundTexture)
{
}
ItemTable::~ItemTable()
{
}

bool ItemTable::Initialize(const char* filePath, Vector2u screenResolution)
{
    try
    {
        // Use the SpriteTexture class to load the background
        backgroundSprite = backgroundSpriteTexture.InitializeSprite(filePath, sf::Vector2f(0.0f, 0.0f),
            sf::Vector2f(screenResolution.x / 1920.0f, screenResolution.y / 1080.0f));

        // Scale the background to fit the screen
        ScaleToFitScreen(screenResolution);

        isLoaded = true;
        return true;
    }
    catch (...)
    {
        isLoaded = false;
        return false;
    }
}

bool ItemTable::Initialize(const char* filePath, Vector2f position, Vector2f scale)
{
    try
    {
        // Use the SpriteTexture class to load the background
        backgroundSpriteTexture.InitializeSprite(filePath, position);

        // Set custom scale
        backgroundSprite.setScale(scale);

        isLoaded = true;
        return true;
    }
    catch (...)
    {
        isLoaded = false;
        return false;
    }
}

void ItemTable::Draw(sf::RenderWindow& window)
{
    if (isLoaded)
    {
        window.draw(backgroundSprite);
    }
}

void ItemTable::SetPosition(sf::Vector2f position)
{
    if (isLoaded)
    {
        backgroundSprite.setPosition(position);
    }
}

void ItemTable::Move(sf::Vector2f offset)
{
    if (isLoaded)
    {
        backgroundSprite.move(offset);
    }
}

void ItemTable::SetScale(sf::Vector2f scale)
{
    if (isLoaded)
    {
        backgroundSprite.setScale(scale);
    }
}

Sprite& ItemTable::GetSprite()
{
    return backgroundSprite;
}

const char* ItemTable::GetTexture()
{
    return backgroundSpriteTexture.GetTexture();
}

void ItemTable::SetTexture(const char* filePath_)
{
    backgroundSpriteTexture.SetTexture(filePath_);
}

void ItemTable::Unload()
{
    isLoaded = false;
}

void ItemTable::ScaleToFitScreen(sf::Vector2u screenResolution)
{
    if (isLoaded)
    {
        // Get the texture size
        sf::Vector2u textureSize = backgroundTexture.getSize();

        // Calculate scale factors
        float scaleX = static_cast<float>(screenResolution.x) / static_cast<float>(textureSize.x);
        float scaleY = static_cast<float>(screenResolution.y) / static_cast<float>(textureSize.y);

        // Use the larger scale factor to ensure the background covers the entire screen
        // This maintains aspect ratio but may crop parts of the image
        float scale = std::max(scaleX, scaleY);

        // Apply uniform scaling
        backgroundSprite.setScale({ scale, scale });

        // Center the background on screen
        sf::Vector2f scaledSize = sf::Vector2f(textureSize.x * scale, textureSize.y * scale);
        sf::Vector2f centerPosition = sf::Vector2f(
            (static_cast<float>(screenResolution.x) - scaledSize.x) / 2.0f,
            (static_cast<float>(screenResolution.y) - scaledSize.y) / 2.0f
        );

        backgroundSprite.setPosition(centerPosition);
    }
}

