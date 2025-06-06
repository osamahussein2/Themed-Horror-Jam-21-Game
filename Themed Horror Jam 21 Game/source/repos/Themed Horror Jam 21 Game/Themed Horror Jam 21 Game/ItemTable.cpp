#include "ItemTable.h"

ItemTable::ItemTable() : ItemtableSprite(ItemtableTexture), GroundSprite(GroundTexture),
bookSprite(bookTexture), ChickenSprite(ChickenTexture), cobwebSprite(cobwebTexture),
Cramp_coineSprite(Cramp_coineTexture), CrucifixSprite(CrucifixTexture),
CupOfAntimonySprite(CupOfAntimonyTexture), MaggotsSprite(MaggotsTexture), OintmentSprite(OintmentTexture),
ringSprite(ringTexture), ScalpelpngSprite(ScalpelpngTexture), MortarPestleSprite(MortarPestleTexture)
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
    ItemtableSprite.setPosition(position_);

    GroundTexture.loadFromFile("Art Assets/SurgeryRoom/ground.png");
    GroundSprite = sf::Sprite(GroundTexture);
    GroundSprite.setTexture(GroundTexture);
    GroundSprite.setScale(scale_);
    float GroundX = 0;
    float GroundY = 0;
    GroundSprite.setPosition({ GroundX,GroundY });

	// Store the base position and scale for relative positioning
	Vector2f tablePosition = position_;
	Vector2f itemScale = scale_;

	// Set up items relative to table position
	if (!bookTexture.loadFromFile(book)) {
		std::cout << "Failed to load: " << book << std::endl;
	}
	bookTexture.loadFromFile(book);
	bookSprite.setTexture(bookTexture);
	bookSprite.setPosition({ tablePosition.x + 100, tablePosition.y + 100 });
	bookSprite.setScale(itemScale);


	if (!bookTexture.loadFromFile(Chicken)) {
		std::cout << "Failed to load: " << book << std::endl;
	}
	ChickenTexture.loadFromFile(Chicken);
	ChickenSprite.setTexture(ChickenTexture);
	ChickenSprite.setPosition({ tablePosition.x + 200, tablePosition.y + 100 });
	ChickenSprite.setScale(itemScale);

	if (!cobwebTexture.loadFromFile(cobweb)) {
		std::cout << "Failed to load: " << cobweb << std::endl;
	}
	// Continue this pattern for all items...
	cobwebTexture.loadFromFile(cobweb);
	cobwebSprite.setTexture(cobwebTexture);
	cobwebSprite.setPosition({ tablePosition.x + 300, tablePosition.y + 100 });
	cobwebSprite.setScale(itemScale);

	if (!Cramp_coineTexture.loadFromFile(Cramp_coine)) {
		std::cout << "Failed to load: " << Cramp_coine << std::endl;
	}
	Cramp_coineTexture.loadFromFile(Cramp_coine);
	Cramp_coineSprite.setTexture(Cramp_coineTexture);
	Cramp_coineSprite.setPosition({ 400, 100 }); // Set position as needed


	if (!CrucifixTexture.loadFromFile(Crucifix)) {
		std::cout << "Failed to load: " << Crucifix << std::endl;
	}
	CrucifixTexture.loadFromFile(Crucifix);
	CrucifixSprite.setTexture(CrucifixTexture);
	CrucifixSprite.setPosition({ 500, 100 }); // Set position as needed


	if (!CupOfAntimonyTexture.loadFromFile(CupOfAntimony)) {
		std::cout << "Failed to load: " << CupOfAntimony << std::endl;
	}
	CupOfAntimonyTexture.loadFromFile(CupOfAntimony);
	CupOfAntimonySprite.setTexture(CupOfAntimonyTexture);
	CupOfAntimonySprite.setPosition({ 600, 100 }); // Set position as needed`


	if (!MaggotsTexture.loadFromFile(Maggots)) {
		std::cout << "Failed to load: " << Maggots << std::endl;
	}
	MaggotsTexture.loadFromFile(Maggots);
	MaggotsSprite.setTexture(MaggotsTexture);
	MaggotsSprite.setPosition({ 700, 100 }); // Set position as needed
	

	if (!OintmentTexture.loadFromFile(Ointment)) {
		std::cout << "Failed to load: " << Ointment << std::endl;
	}
	OintmentTexture.loadFromFile(Ointment);
	OintmentSprite.setTexture(OintmentTexture);
	OintmentSprite.setPosition({ 800, 100 }); // Set position as needed

	if (!ringTexture.loadFromFile(ring)) {
		std::cout << "Failed to load: " << ring << std::endl;
	}
	ringTexture.loadFromFile(ring);
	ringSprite.setTexture(ringTexture);
	ringSprite.setPosition({ 900, 100 }); // Set position as needed


	if (!ScalpelpngTexture.loadFromFile(Scalpelpng)) {
		std::cout << "Failed to load: " << Scalpelpng << std::endl;
	}
	ScalpelpngTexture.loadFromFile(Scalpelpng);
	ScalpelpngSprite.setTexture(ScalpelpngTexture);
	ScalpelpngSprite.setPosition({ 1000, 100 }); // Set position as needed


	if (!MortarPestleTexture.loadFromFile(MortarPestle)) {
		std::cout << "Failed to load: " << MortarPestle << std::endl;
	}

	MortarPestleTexture.loadFromFile(MortarPestle);
	MortarPestleSprite.setTexture(MortarPestleTexture);
	MortarPestleSprite.setPosition({ 1100, 100 }); // Set position as needed

}


void ItemTable::Draw(RenderWindow& window)
{
    window.draw(GroundSprite);
	window.draw(ItemtableSprite);

	window.draw(bookSprite);
	window.draw(ChickenSprite);
	window.draw(cobwebSprite);
	window.draw(Cramp_coineSprite);
	window.draw(CrucifixSprite);
	window.draw(CupOfAntimonySprite);
	window.draw(MaggotsSprite);
	window.draw(OintmentSprite);
	window.draw(ringSprite);
	window.draw(ScalpelpngSprite);
	window.draw(MortarPestleSprite);

}

void ItemTable::SetPosition(Vector2f position)
{
    ItemtableSprite.setPosition(position);
}

void ItemTable::Move(Vector2f offset)
{
	ItemtableSprite.move(offset);
	GroundSprite.move(offset);
	// Move all items with the table
	bookSprite.move(offset);
	ChickenSprite.move(offset);
	cobwebSprite.move(offset);
	Cramp_coineSprite.move(offset);
	CrucifixSprite.move(offset);
	CupOfAntimonySprite.move(offset);
	MaggotsSprite.move(offset);
	OintmentSprite.move(offset);
	ringSprite.move(offset);
	ScalpelpngSprite.move(offset);
	MortarPestleSprite.move(offset);
}

void ItemTable::SetScale(Vector2f scale)
{
	ItemtableSprite.setScale(scale);
	GroundSprite.setScale(scale);
	// Scale all items
	bookSprite.setScale(scale);
	ChickenSprite.setScale(scale);
	cobwebSprite.setScale(scale);
	Cramp_coineSprite.setScale(scale);
	CrucifixSprite.setScale(scale);
	CupOfAntimonySprite.setScale(scale);
	MaggotsSprite.setScale(scale);
	OintmentSprite.setScale(scale);
	ringSprite.setScale(scale);
	ScalpelpngSprite.setScale(scale);
	MortarPestleSprite.setScale(scale);
}
