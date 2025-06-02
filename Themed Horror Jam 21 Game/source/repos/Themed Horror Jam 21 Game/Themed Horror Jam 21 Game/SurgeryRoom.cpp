#include "SurgeryRoom.h"
#include <algorithm>

SurgeryRoom::SurgeryRoom() : isLoaded(false), TimerValue(0),
backgroundSprite(backgroundTexture),
BouttomUISprite(BouttomUITexture),
TopUISprite(TopUITexture),
TimerSprite(TimerTexture),
LifeSprite_0(LifeTexture),
LifeSprite_1(LifeTexture),
LifeSprite_2(LifeTexture),
DeathSprite_0(DeathTexture),
DeathSprite_1(DeathTexture),
DeathSprite_2(DeathTexture),
timerRunning(false),
timeRemaining(0.0f),
totalTime(0.0f)
{
    // Initialize all lives as alive
    for (int i = 0; i < 3; i++) {
        lifeStates[i] = true;
    }
}

SurgeryRoom::~SurgeryRoom()
{
}

bool SurgeryRoom::Initialize(const char* Backgroundpath, Vector2u screenResolution,
    const char* BouttomUIPath, const char* TopUIPath)
{
    try
    {
        // Use the SpriteTexture class to load the background
        backgroundSprite = backgroundSpriteTexture.InitializeSprite(Backgroundpath, sf::Vector2f(0, 0),
            sf::Vector2f(screenResolution.x / 1920.0f, screenResolution.y / 1080.0f));

        // Load the bottom UI sprite
        BouttomUISprite = BouttomUISpriteTexture.InitializeSprite(BouttomUIPath, sf::Vector2f(0, 0));

        // Load the top UI sprite
        TopUISprite = TopUISpriteTexture.InitializeSprite(TopUIPath, sf::Vector2f(0, 0));

        // Load life sprites
        LifeSprite_0 = LifeSpriteTexture.InitializeSprite(LifePath, sf::Vector2f(0, 0));
        LifeSprite_1 = LifeSpriteTexture.InitializeSprite(LifePath, sf::Vector2f(0, 0));
        LifeSprite_2 = LifeSpriteTexture.InitializeSprite(LifePath, sf::Vector2f(0, 0));

        // Load death sprites
        DeathSprite_0 = DeathSpriteTexture.InitializeSprite(DeathPath, sf::Vector2f(0, 0));
        DeathSprite_1 = DeathSpriteTexture.InitializeSprite(DeathPath, sf::Vector2f(0, 0));
        DeathSprite_2 = DeathSpriteTexture.InitializeSprite(DeathPath, sf::Vector2f(0, 0));

        // Load the timer sprite
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
        float TimerX = 30.0f;
        float TimerY = static_cast<float>(screenResolution.y) - timeBounds.size.y - 10.0f;
        TimerSprite.setPosition({ TimerX, TimerY });

        // Position life and death sprites
        sf::FloatRect LifeBounds = LifeSprite_0.getLocalBounds();
        float LifeX = TimerX + 130.0f;
        float LifeY = static_cast<float>(screenResolution.y) - LifeBounds.size.y - 155.0f;

        // Set positions for life sprites
        LifeSprite_0.setPosition({ LifeX, LifeY });
        LifeSprite_1.setPosition({ LifeX + 80.0f, LifeY });
        LifeSprite_2.setPosition({ LifeX + (80.0f * 2), LifeY });

        // Set same positions for death sprites
        DeathSprite_0.setPosition({ LifeX, LifeY });
        DeathSprite_1.setPosition({ LifeX + 80.0f, LifeY });
        DeathSprite_2.setPosition({ LifeX + (80.0f * 2), LifeY });

        isLoaded = true;
        return true;
    }
    catch (...)
    {
        isLoaded = false;
        return false;
    }
}

void SurgeryRoom::StartTimer(float duration)
{
    if (isLoaded) {
        totalTime = duration;
        timeRemaining = duration;
        timerRunning = true;
        animationClock.restart();

        // You can add timer start animation here
        std::cout << "Timer started with " << duration << " seconds!" << std::endl;
    }
}

void SurgeryRoom::UpdateTimer(float deltaTime)
{
    if (isLoaded && timerRunning) {
        timeRemaining -= deltaTime;

        if (timeRemaining <= 0.0f) {
            timeRemaining = 0.0f;
            timerRunning = false;
            std::cout << "Timer finished!" << std::endl;
            // You can trigger end-of-timer events here
        }

        // Update timer visual representation
        UpdateTimerSprite();

        // Timer value for display (rounded)
        TimerValue = static_cast<int>(timeRemaining);
    }
}

void SurgeryRoom::StopTimer()
{
    timerRunning = false;
    timeRemaining = 0.0f;
}

void SurgeryRoom::UpdateTimerSprite()
{
    // You can implement timer animation here
    // For example, change timer sprite based on remaining time percentage
    float timePercentage = timeRemaining / totalTime;

    // Example: Make timer blink when time is low
    if (timePercentage < 0.2f) {
        // Get elapsed time for blinking effect
        float elapsed = animationClock.getElapsedTime().asSeconds();

        // Blink every 0.5 seconds
        if (static_cast<int>(elapsed * 2) % 2 == 0) {
            // Timer visible
            TimerSprite.setColor(sf::Color::Red);
        }
        else {
            // Timer semi-transparent for blink effect
            TimerSprite.setColor(sf::Color(255, 100, 100, 128));
        }
    }
    else {
        // Normal timer color
        TimerSprite.setColor(sf::Color::White);
    }
}

void SurgeryRoom::LoseLife(int lifeIndex)
{
    if (isLoaded && lifeIndex >= 0 && lifeIndex < 3) {
        lifeStates[lifeIndex] = false;
        std::cout << "Life " << lifeIndex << " lost!" << std::endl;

        // You can add death animation here
        // For example, fade out the life sprite and fade in the death sprite
    }
}

void SurgeryRoom::RestoreLife(int lifeIndex)
{
    if (isLoaded && lifeIndex >= 0 && lifeIndex < 3) {
        lifeStates[lifeIndex] = true;
        std::cout << "Life " << lifeIndex << " restored!" << std::endl;
    }
}

void SurgeryRoom::ResetAllLives()
{
    for (int i = 0; i < 3; i++) {
        lifeStates[i] = true;
    }
    std::cout << "All lives restored!" << std::endl;
}

int SurgeryRoom::GetLivesRemaining() const
{
    int count = 0;
    for (int i = 0; i < 3; i++) {
        if (lifeStates[i]) count++;
    }
    return count;
}

void SurgeryRoom::Draw(RenderWindow& window)
{
    if (isLoaded)
    {
        window.draw(backgroundSprite);
        window.draw(TopUISprite);
        DrawUI(window);
    }
}

void SurgeryRoom::DrawUI(RenderWindow& window)
{
    if (isLoaded)
    {
        window.draw(BouttomUISprite);

        // Draw life or death sprites based on current state
        if (lifeStates[0]) {
            window.draw(LifeSprite_0);
        }
        else {
            window.draw(DeathSprite_0);
        }

        if (lifeStates[1]) {
            window.draw(LifeSprite_1);
        }
        else {
            window.draw(DeathSprite_1);
        }

        if (lifeStates[2]) {
            window.draw(LifeSprite_2);
        }
        else {
            window.draw(DeathSprite_2);
        }

        window.draw(TimerSprite);
    }
}

void SurgeryRoom::SetPosition(Sprite sprite, Vector2f position)
{
    if (isLoaded)
    {
        sprite.setPosition(position);
    }
}

void SurgeryRoom::Move(Sprite sprite, Vector2f offset)
{
    if (isLoaded)
    {
        backgroundSprite.move(offset);
    }
}

void SurgeryRoom::SetScale(Sprite sprite, Vector2f scale)
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