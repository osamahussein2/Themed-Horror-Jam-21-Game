#include "SurgeryRoom.h"

SurgeryRoom::SurgeryRoom() : isLoaded(false), TimerValue(0),
backgroundSprite(backgroundTexture),
BouttomUISprite(BouttomUITexture),
TopUISprite(TopUITexture),
LifeSprite(LifeTexture),
DeathSprite(DeathTexture),
TimerSprite(TimerTexture)
{
}

SurgeryRoom::~SurgeryRoom()
{
}

bool SurgeryRoom::Initialize(const char* Backgroundpath, Vector2u screenResolution, const char* BouttomUIPath, const char* TopUIPath, const char* LifePath, const char* DeathPath, const char* Timer)
{
    try
    {
        // Use the SpriteTexture class to load the background
        backgroundSprite = backgroundSpriteTexture.InitializeSprite(Backgroundpath, sf::Vector2f(0, 0));


        // Load the bottom UI sprite;
        BouttomUISprite = BouttomUISpriteTexture.InitializeSprite(BouttomUIPath, sf::Vector2f(0, 0));
        // Load the top UI sprite
        TopUISprite = TopUISpriteTexture.InitializeSprite(TopUIPath, sf::Vector2f(0, 0));
		// Load the death sprite
		DeathSprite = DeathSpriteTexture.InitializeSprite(DeathPath, sf::Vector2f(0, 0));
        // Load the life sprite
        LifeSprite = LifeSpriteTexture.InitializeSprite(LifePath, sf::Vector2f(0, 0));
        // load the timer sprite
        TimerSprite = TimerSpriteTexture.InitializeSprite(Timer, sf::Vector2f(0, 0));

        // Scale the background to fit the screen
        ScaleToFitScreen(screenResolution);

        // Position the bottom UI sprite at the bottom center of the screen
        sf::FloatRect bottomUIBounds = BouttomUISprite.getLocalBounds();
        float bottomUIX = (static_cast<float>(screenResolution.x) - bottomUIBounds.size.x) / 2.0f;
        float bottomUIY = static_cast<float>(screenResolution.y) - bottomUIBounds.size.y;
        BouttomUISprite.setPosition({ bottomUIX, bottomUIY });

        // Optionally position other UI elements as well
        // Position top UI at top center
        sf::FloatRect topUIBounds = TopUISprite.getLocalBounds();
        float topUIX = (static_cast<float>(screenResolution.x) - topUIBounds.size.x) / 2.0f;
        float topUIY = 0.0f;
        TopUISprite.setPosition({ topUIX, topUIY });

        isLoaded = true;
        return true;
    }
    catch (...)
    {
        isLoaded = false;
        return false;
    }
}

void SurgeryRoom::Draw(RenderWindow& window)
{
    if (isLoaded)
    {
        window.draw(backgroundSprite);
        window.draw(BouttomUISprite);
        window.draw(TopUISprite);
        window.draw(LifeSprite);
        window.draw(DeathSprite);
		window.draw(TimerSprite);
    }
}

void SurgeryRoom::SetPosition(Sprite sprit, Vector2f position)
{
    if (isLoaded)
    {
        sprit.setPosition(position);
    }
}

void SurgeryRoom::Move(Sprite  sprite, Vector2f offset)
{
    if (isLoaded)
    {
        backgroundSprite.move(offset);
    }
}

void SurgeryRoom::SetScale(Sprite  sprite, Vector2f scale)
{
    if (isLoaded)
    {
        backgroundSprite.setScale(scale);
    }
}
void SurgeryRoom::ScaleToFitScreen(sf::Vector2u screenResolution)
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
