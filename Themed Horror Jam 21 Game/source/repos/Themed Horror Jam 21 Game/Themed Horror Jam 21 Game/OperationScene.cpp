#include "OperationScene.h"

OperationScene::OperationScene() : operationSprite(operationTexture), GroundSprite(GroundTexture)
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

	GroundTexture.loadFromFile("Art Assets/SurgeryRoom/ground.png");
	GroundSprite = sf::Sprite(GroundTexture);
	GroundSprite.setTexture(GroundTexture);
	GroundSprite.setScale(scale_);
	float GroundX = 0;
	float GroundY = 0;
    GroundSprite.setPosition({ GroundX,GroundY });
}

void OperationScene::InitializeDot(Vector2f position_, float radius_, sf::Color fillColor_, sf::Color outlineColor_, 
    float outlineThickness_, float spacingX_, float spacingY_)
{
    if (!dotCircleShape.empty()) dotCircleShape.clear();
    dotCircleShape.resize(maxDots);

    for (int i = 0; i < maxDots; i++)
    {
        dotCircleShape[i].setRadius(radius_);
        dotCircleShape[i].setFillColor(fillColor_);
        dotCircleShape[i].setOutlineColor(outlineColor_);
        dotCircleShape[i].setOutlineThickness(outlineThickness_);
        dotCircleShape[i].setPosition(Vector2f(position_.x + (i * spacingX_), 
            position_.y + (i * spacingY_)));
    }
}

void OperationScene::InitializeSuccessPanel(Vector2f position_, Vector2f scale_, sf::Color fillColor_, bool centerOrigin_)
{
    successPanel.setSize(scale_);
    successPanel.setFillColor(fillColor_);

    if (centerOrigin_)
    {
        successPanel.setOrigin(sf::Vector2f(successPanel.getLocalBounds().position.x + successPanel.getLocalBounds().size.x / 2.0f,
            successPanel.getLocalBounds().position.y + successPanel.getLocalBounds().size.y / 2.0f));
    }

    successPanel.setPosition(position_);
}

void OperationScene::Draw(RenderWindow& window)
{
    window.draw(GroundSprite);
    window.draw(operationSprite);

    for (CircleShape dot : dotCircleShape)
    {
        window.draw(dot);
    }
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