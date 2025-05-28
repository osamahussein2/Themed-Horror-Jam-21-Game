#pragma once
#include <SFML/Graphics.hpp>
#include "SpriteTexture.h"

using namespace sf;

enum Direction
{
    down = 0,
    left = 1,
    right = 2,
    up = 3
};

class Player
{
private:
    SpriteTexture playerTexture;
    Sprite* playerSprite;
    int  PixelWidth = 32;
    int  Pixelhight = 50;
    // Animation variables
    int currentDirection;
    int animationFrame;
    static const int maxFrames = 4;
    IntRect baseDir[4]; // Base rectangles for idle frames

    // Timing
    Clock animationClock;
    float animationTime;

    // Movement
    float speed;

    // Helper methods
    void updateAnimation(float deltaTime);
    void setIdleFrame();
    void setAnimationFrame();

public:
    Player();
    ~Player();

    void Initialize(const char* spritePath, Vector2f position, Vector2f scale = Vector2f(2.0f, 2.0f));
    void Update(float deltaTime);
    void Draw(RenderWindow& window);

    // Getters
    Vector2f getPosition() const;
    Sprite& getSprite();

    // Setters
    void setPosition(Vector2f position);
    void setSpeed(float newSpeed);
};