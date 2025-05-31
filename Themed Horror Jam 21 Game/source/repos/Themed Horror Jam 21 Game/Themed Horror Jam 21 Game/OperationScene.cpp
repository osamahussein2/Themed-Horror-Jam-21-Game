#include "OperationScene.h"

OperationScene::OperationScene() : operationSprite(operationTexture)
{
}

OperationScene::~OperationScene()
{
}

void OperationScene::Initialize(const char* filePath_, Vector2f position_, Vector2f scale_, bool centerTexture_)
{
    operationTexture.loadFromFile(filePath_);

    // Create a sprite from the texture
    operationSprite = sf::Sprite(operationTexture);

    if (centerTexture_)
    {
        operationSprite.setOrigin(Vector2f(operationSprite.getPosition().x + operationSprite.getScale().x / 2.0f,
            operationSprite.getPosition().y + operationSprite.getScale().y / 2.0f));
    }

    else
    {
        operationSprite.setOrigin(Vector2f(0.0f, 0.0f));
    }

    operationSprite.setTexture(operationTexture);
    operationSprite.setScale(scale_);
    operationSprite.setPosition(position_);
}

void OperationScene::Draw(RenderWindow& window)
{
    window.draw(operationSprite);
}

void OperationScene::SetPosition(Vector2f position)
{
    operationSprite.setPosition(position);
}

void OperationScene::Move(Vector2f offset)
{
    operationSprite.move(offset);
}

void OperationScene::SetScale(Vector2f scale)
{
    operationSprite.setScale(scale);
}