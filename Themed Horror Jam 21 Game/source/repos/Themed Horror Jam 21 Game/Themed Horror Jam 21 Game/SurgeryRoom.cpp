#include "SurgeryRoom.h"
SurgeryRoom::SurgeryRoom() : isLoaded(false), TimerValue(0),
backgroundSprite(backgroundTexture),
BouttomUISprite(BouttomUITexture),
TopUISprite(TopUITexture),
DeathSprite(DeathTexture),
TimerSprite(TimerTexture),
LifeSprite_0( LifeTexture ),
LifeSprite_1(LifeTexture),
LifeSprite_2(LifeTexture)
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

       
        LifeSprite_0 = LifeSpriteTexture.InitializeSprite(LifePath, sf::Vector2f(0, 0));
        LifeSprite_1 = LifeSpriteTexture.InitializeSprite(LifePath, sf::Vector2f(0, 0));
        LifeSprite_2 = LifeSpriteTexture.InitializeSprite(LifePath, sf::Vector2f(0, 0));

        // Load the death sprite
       // DeathSprite = DeathSpriteTexture.InitializeSprite(DeathPath, sf::Vector2f(0, 0));

        // load the timer sprite
        TimerSprite = TimerSpriteTexture.InitializeSprite(Timer, sf::Vector2f(0, 0));

        // Scale the background to fit the screen
        ScaleToFitScreen(screenResolution);

        // Position the bottom UI sprite at the bottom center of the screen
        sf::FloatRect bottomUIBounds = BouttomUISprite.getLocalBounds();
        float bottomUIX = (static_cast<float>(screenResolution.x) - bottomUIBounds.size.x) / 2.0f;
        float bottomUIY = static_cast<float>(screenResolution.y) - bottomUIBounds.size.y;
        BouttomUISprite.setPosition({ bottomUIX, bottomUIY });

        // Position top UI at top center
        sf::FloatRect topUIBounds = TopUISprite.getLocalBounds();
        float topUIX = (static_cast<float>(screenResolution.x) - topUIBounds.size.x) / 2.0f;
        float topUIY = 0.0f;
        TopUISprite.setPosition({ topUIX, topUIY });

        // Position timer sprite at the bottom left corner
        sf::FloatRect timeBounds = TimerSprite.getLocalBounds();
        float TimerX = 30.0f; // Adjust as needed
        float TimerY = static_cast<float>(screenResolution.y) - timeBounds.size.y - 10.0f; // 10 pixels from bottom
        TimerSprite.setPosition({ TimerX, TimerY });

      
        sf::FloatRect LifeBounds = LifeSprite_0.getLocalBounds();
        float LifeX = TimerX + 130.0f; // Adjust as needed
        float LifeY = static_cast<float>(screenResolution.y) - LifeBounds.size.y - 155.0f ; // Spacing between life sprites
        LifeSprite_0.setPosition({ LifeX, LifeY });
		LifeSprite_1.setPosition({ LifeX + 80.0f, LifeY });
        LifeSprite_2.setPosition({ LifeX + (80.0f *2), LifeY });


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

      
        window.draw(LifeSprite_0);
        window.draw(LifeSprite_1);
        window.draw(LifeSprite_2);

        //window.draw(DeathSprite);
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