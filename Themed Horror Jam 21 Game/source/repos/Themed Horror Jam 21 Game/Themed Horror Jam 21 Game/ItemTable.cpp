#include "ItemTable.h"

ItemTable::ItemTable() : ItemtableSprite(ItemtableTexture), GroundSprite(GroundTexture)
{
}

ItemTable::~ItemTable()
{
}

void ItemTable::Initialize(const char* filePath_, Vector2f position_, Vector2f scale_, bool centerTexture_)
{
    ItemtableTexture.loadFromFile(filePath_);

    // Create a sprite from the texture
    ItemtableSprite = sf::Sprite(ItemtableTexture);

    if (centerTexture_)
    {
        ItemtableSprite.setOrigin(Vector2f(ItemtableSprite.getPosition().x + ItemtableSprite.getScale().x / 2.0f,
            ItemtableSprite.getPosition().y + ItemtableSprite.getScale().y / 2.0f));
    }

    else
    {
        ItemtableSprite.setOrigin(Vector2f(0.0f, 0.0f));
    }

    ItemtableSprite.setTexture(ItemtableTexture);
    ItemtableSprite.setScale(scale_);
    ItemtableSprite.setPosition(position_);

    GroundTexture.loadFromFile("Art Assets/SurgeryRoom/ground.png");
    GroundSprite = sf::Sprite(GroundTexture);
    GroundSprite.setTexture(GroundTexture);
    GroundSprite.setScale(scale_);
    float GroundX = 0;
    float GroundY = 0;
    GroundSprite.setPosition({ GroundX,GroundY });
}


void ItemTable::Draw(RenderWindow& window)
{
    window.draw(GroundSprite);
	window.draw(ItemtableSprite);


}

void ItemTable::SetPosition(Vector2f position)
{
    ItemtableSprite.setPosition(position);
}

void ItemTable::Move(Vector2f offset)
{
	ItemtableSprite.move(offset);
	GroundSprite.move(offset);
}

void ItemTable::SetScale(Vector2f scale)
{
	ItemtableSprite.setScale(scale);
	GroundSprite.setScale(scale);
}
