#include "Player.h"
#include <SFML/Window/Keyboard.hpp>

Player::Player() : currentDirection(down), animationFrame(0), animationTime(0.1f), speed(200.0f), playerSprite(nullptr)
{
}

Player::~Player()
{
    if (playerSprite != nullptr)
    {
        delete playerSprite;
        playerSprite = nullptr;
    }
}

void Player::Initialize(const char* spritePath, Vector2f position, Vector2f scale)
{
    // Initialize the sprite texture
    playerTexture.InitializeSprite(spritePath, Vector2f(0.0f, 0.0f));

    // Create the sprite and assign the texture
    if (playerSprite != nullptr)
    {
        delete playerSprite;
    }
    playerSprite = new Sprite(playerTexture.LoadSprite());

    // Setup base rectangles for each direction (idle frames)
    baseDir[down] = IntRect({ 0, Pixelhight * 0 }, { PixelWidth, Pixelhight });  // Row 0  
    baseDir[left] = IntRect({ 0, Pixelhight * 1 }, { PixelWidth, Pixelhight });  // Row 1
    baseDir[right] = IntRect({ 0, Pixelhight * 2 }, { PixelWidth, Pixelhight }); // Row 2
    baseDir[up] = IntRect({ 0, Pixelhight * 3 }, { PixelWidth, Pixelhight });    // Row 3

    // Set initial properties
    playerSprite->setTextureRect(baseDir[down]);
    playerSprite->setOrigin({ 9.5f, 19.0f }); // Center origin based on 19x38 size
    playerSprite->setPosition(position);
    playerSprite->setScale(scale);
}

void Player::Update(float deltaTime)
{
    // Movement and animation
    bool isMoving = false;
    Vector2f movement(0, 0);

    // Handle input and movement
    if (Keyboard::isKeyPressed(Keyboard::Key::Up) || Keyboard::isKeyPressed(Keyboard::Key::W))
    {
        movement.y = -speed * deltaTime;
        currentDirection = up;
        isMoving = true;
    }
    else if (Keyboard::isKeyPressed(Keyboard::Key::Down) || Keyboard::isKeyPressed(Keyboard::Key::S))
    {
        movement.y = speed * deltaTime;
        currentDirection = down;
        isMoving = true;
    }
    else if (Keyboard::isKeyPressed(Keyboard::Key::Left) || Keyboard::isKeyPressed(Keyboard::Key::A))
    {
        movement.x = -speed * deltaTime;
        currentDirection = left;
        isMoving = true;
    }
    else if (Keyboard::isKeyPressed(Keyboard::Key::Right) || Keyboard::isKeyPressed(Keyboard::Key::D))
    {
        movement.x = speed * deltaTime;
        currentDirection = right;
        isMoving = true;
    }

    // Apply movement
    if (isMoving)
    {
        playerSprite->move(movement);
        updateAnimation(deltaTime);
    }
    else
    {
        setIdleFrame();
    }
}

void Player::Draw(RenderWindow& window)
{
    if (playerSprite != nullptr)
    {
        window.draw(*playerSprite);
    }
}

void Player::updateAnimation(float deltaTime)
{
    // Update animation frame
    if (animationClock.getElapsedTime().asSeconds() >= animationTime)
    {
        animationFrame++;
        if (animationFrame >= maxFrames)
        {
            animationFrame = 0;
        }
        animationClock.restart();
    }

    setAnimationFrame();
}

void Player::setIdleFrame()
{
    if (playerSprite != nullptr)
    {
        playerSprite->setTextureRect(baseDir[currentDirection]);
        animationFrame = 0; // Reset animation when not moving
    }
}

void Player::setAnimationFrame()
{
    if (playerSprite == nullptr) return;

    IntRect currentFrame;

    if (currentDirection == up)
    {
        currentFrame = IntRect({ PixelWidth * animationFrame, Pixelhight * 3 }, { PixelWidth, Pixelhight }); // Row 3 for up walk
    }
    else if (currentDirection == down)
    {
        currentFrame = IntRect({ PixelWidth * animationFrame, Pixelhight * 0 }, { PixelWidth, Pixelhight }); // Row 0 for down walk
    }
    else if (currentDirection == left)
    {
        currentFrame = IntRect({ PixelWidth * animationFrame, Pixelhight * 1 }, { PixelWidth, Pixelhight }); // Row 1 for left walk
    }
    else if (currentDirection == right)
    {
        currentFrame = IntRect({ PixelWidth * animationFrame, Pixelhight * 2 }, { PixelWidth, Pixelhight }); // Row 2 for right walk
    }

    playerSprite->setTextureRect(currentFrame);
}

// Getters
Vector2f Player::getPosition() const
{
    if (playerSprite != nullptr)
    {
        return playerSprite->getPosition();
    }
    return Vector2f(0.0f, 0.0f);
}

Sprite& Player::getSprite()
{
    return *playerSprite;
}

// Setters
void Player::setPosition(Vector2f position)
{
    if (playerSprite != nullptr)
    {
        playerSprite->setPosition(position);
    }
}

void Player::setSpeed(float newSpeed)
{
    speed = newSpeed;
}