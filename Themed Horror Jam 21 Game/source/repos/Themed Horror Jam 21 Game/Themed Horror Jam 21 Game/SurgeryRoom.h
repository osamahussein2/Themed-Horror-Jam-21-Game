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
    bool Initialize(const char* Backgroundpath, Vector2u screenResolution, const char* BouttomUIPath, 
        Vector2f bottomUIposition, Vector2f bottomUIscale, const char* TopUIPath, Vector2f topUIposition,
        Vector2f topUIscale, Vector2f lifeSprite0position, Vector2f lifeSprite1position, Vector2f lifeSprite2position,
        Vector2f lifeSpriteScale, Vector2f deathSprite0position, Vector2f deathSprite1position, 
        Vector2f deathSprite2position, Vector2f deathSpriteScale, Vector2f timerSpritePosition, Vector2f timerSpriteScale);

    // Draw the surgery room to the render window
    void Draw(RenderWindow& window, Sprite body);

    // Draw only UI elements (without background)
    void DrawUI(RenderWindow& window);

    // Update surgery room position (useful for scrolling backgrounds)
    void SetPosition(Sprite sprite, Vector2f position);
    void Move(Sprite sprite, Vector2f offset);

    // Scale the surgery room
    void SetScale(Sprite sprite, Vector2f scale);

    // Timer and animation functions
    void StartTimer(float duration = 60.0f); // Start timer with duration in seconds
    void UpdateTimer(float deltaTime); // Update timer (call this every frame)
    void StopTimer();
    bool IsTimerRunning() const { return timerRunning; }
    float GetTimeRemaining() const { return timeRemaining; }

    // Life management functions
    void LoseLife(int lifeIndex); // Change a specific life sprite to death sprite (0, 1, or 2)
    void RestoreLife(int lifeIndex); // Restore a life sprite
    void ResetAllLives(); // Reset all lives to alive state
    int GetLivesRemaining() const;

    // Check if surgery room is loaded successfully
    bool IsLoaded() const { return isLoaded; }

    void DrawBackground(RenderWindow& window);
    // Public members
    int TimerValue = 0;
    Sprite backgroundSprite;
    Sprite BouttomUISprite;
    SpriteTexture BouttomUISpriteTexture;
    Sprite TopUISprite;
    Sprite LifeSprite_0;
    Sprite LifeSprite_1;
    Sprite LifeSprite_2;
    Sprite DeathSprite_0;
    Sprite DeathSprite_1;
    Sprite DeathSprite_2;
    Sprite TimerSprite;

private:
    // Texture and sprite management
 
    Texture BouttomUITexture;

    SpriteTexture TopUISpriteTexture;
    Texture TopUITexture;

    SpriteTexture LifeSpriteTexture;
    Texture LifeTexture;

    SpriteTexture backgroundSpriteTexture;
    Texture backgroundTexture;


    SpriteTexture DeathSpriteTexture;
    Texture DeathTexture;

    // Multiple timer textures and sprite textures
    SpriteTexture TimerSpriteTexture_Start;
    SpriteTexture TimerSpriteTexture_Mid;
    SpriteTexture TimerSpriteTexture_Low;
    SpriteTexture TimerSpriteTexture_End;

    Texture TimerTexture_Start;
    Texture TimerTexture_Mid;
    Texture TimerTexture_Low;
    Texture TimerTexture_End;

    // Timer system
    bool timerRunning;
    float timeRemaining;
    float totalTime;
    Clock animationClock; // For animations

    // Life state tracking
    bool lifeStates[3]; // true = alive, false = dead

    bool isLoaded;

    // Helper functions
    void ScaleToFitScreen(sf::Vector2u screenResolution);
    void UpdateTimerSprite(); // Update timer visual based on remaining time

    // Asset paths for different timer states
    const char* LifePath = "Art Assets/SurgeryRoom/life.png";
    const char* DeathPath = "Art Assets/SurgeryRoom/death.png";
    const char* Timer_Start = "Art Assets/SurgeryRoom/Timer/Timer_start.png";
    const char* Timer_Mid = "Art Assets/SurgeryRoom/Timer/Timer_0.png";
    const char* Timer_End = "Art Assets/SurgeryRoom/Timer/Timer_End.png";
};

#endif