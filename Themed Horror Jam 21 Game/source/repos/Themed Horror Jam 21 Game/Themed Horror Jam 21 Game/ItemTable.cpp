#include "ItemTable.h"

ItemTable::ItemTable() : ItemtableSprite(ItemtableTexture), GroundSprite(GroundTexture),
bookSprite(bookTexture), ChickenSprite(ChickenTexture), cobwebSprite(cobwebTexture),
Cramp_coineSprite(Cramp_coineTexture), CrucifixSprite(CrucifixTexture),
CupOfAntimonySprite(CupOfAntimonyTexture), MaggotsSprite(MaggotsTexture), OintmentSprite(OintmentTexture),
ringSprite(ringTexture), ScalpelpngSprite(ScalpelpngTexture), MortarPestleSprite(MortarPestleTexture),
haySprite(hayTexture), potSprite(potTexture)
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
	ItemtableSprite.setScale({ 1.8f,1.8f });

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
	bookSprite = sf::Sprite(bookTexture);
	bookSprite.setTexture(bookTexture);
	bookSprite.setPosition({ tablePosition.x+ 1000, tablePosition.y + 200 });
	bookSprite.setScale({ 0.8,0.8});
	bookSprite.setOrigin({ bookSprite.getGlobalBounds().size.x / 2, bookSprite.getGlobalBounds().size.y / 2 }); // Center the book sprite


	if (!ChickenTexture.loadFromFile(Chicken)) {
		std::cout << "Failed to load: " << book << std::endl;
	}
	ChickenTexture.loadFromFile(Chicken);
	ChickenSprite = sf::Sprite(ChickenTexture);
	ChickenSprite.setTexture(ChickenTexture);
	ChickenSprite.setPosition({ tablePosition.x + 1600,tablePosition.y + 300 });
	ChickenSprite.setScale({0.5,0.5});
	ChickenSprite.setOrigin({ ChickenSprite.getGlobalBounds().size.x / 2, ChickenSprite.getGlobalBounds().size.y / 2 }); // Center the book sprite

	if (!cobwebTexture.loadFromFile(cobweb)) {
		std::cout << "Failed to load: " << cobweb << std::endl;
	}
	// Continue this pattern for all items...
	cobwebTexture.loadFromFile(cobweb);
	cobwebSprite = sf::Sprite(cobwebTexture);
	cobwebSprite.setTexture(cobwebTexture);
	cobwebSprite.setPosition({ tablePosition.x+550 ,tablePosition.y +600 });
	cobwebSprite.setScale({ 0.99,0.99 });
	cobwebSprite.setOrigin({ cobwebSprite.getGlobalBounds().size.x / 2, cobwebSprite.getGlobalBounds().size.y / 2 }); // Center the book sprite

	if (!Cramp_coineTexture.loadFromFile(Cramp_coine)) {
		std::cout << "Failed to load: " << Cramp_coine << std::endl;
	}
	Cramp_coineTexture.loadFromFile(Cramp_coine);
	Cramp_coineSprite = sf::Sprite(Cramp_coineTexture);
	Cramp_coineSprite.setTexture(Cramp_coineTexture);
	Cramp_coineSprite.setPosition({ tablePosition.x + 1000, tablePosition.y+600 }); // Set position as needed
	Cramp_coineSprite.setOrigin({ Cramp_coineSprite.getGlobalBounds().size.x / 2, Cramp_coineSprite.getGlobalBounds().size.y / 2 }); // Center the book sprite


	if (!CrucifixTexture.loadFromFile(Crucifix)) {
		std::cout << "Failed to load: " << Crucifix << std::endl;
	}
	CrucifixTexture.loadFromFile(Crucifix);
	CrucifixSprite = sf::Sprite(CrucifixTexture);
	CrucifixSprite.setTexture(CrucifixTexture);
	CrucifixSprite.setPosition({ tablePosition.x +1000, tablePosition.y + 400 }); // Set position as needed
	CrucifixSprite.setOrigin({ CrucifixSprite.getGlobalBounds().size.x / 2, CrucifixSprite.getGlobalBounds().size.y / 2 }); // Center the book sprite


	if (!CupOfAntimonyTexture.loadFromFile(CupOfAntimony)) {
		std::cout << "Failed to load: " << CupOfAntimony << std::endl;
	}
	CupOfAntimonyTexture.loadFromFile(CupOfAntimony);
	CupOfAntimonySprite = sf::Sprite(CupOfAntimonyTexture);
	CupOfAntimonySprite.setTexture(CupOfAntimonyTexture);
	CupOfAntimonySprite.setPosition({ tablePosition.x + 750,tablePosition.y + 200 }); // Set position as needed`
	CupOfAntimonySprite.setScale({ 1.5,1.5 });
	CupOfAntimonySprite.setOrigin({ CupOfAntimonySprite.getGlobalBounds().size.x / 2, CupOfAntimonySprite.getGlobalBounds().size.y / 2 }); // Center the book sprite

	if (!MaggotsTexture.loadFromFile(Maggots)) {
		std::cout << "Failed to load: " << Maggots << std::endl;
	}
	MaggotsTexture.loadFromFile(Maggots);
	MaggotsSprite = sf::Sprite(MaggotsTexture);
	MaggotsSprite.setTexture(MaggotsTexture);
	MaggotsSprite.setPosition({ tablePosition.x +400 , tablePosition.y +150 }); // Set position as needed
	MaggotsSprite.setOrigin({ MaggotsSprite.getGlobalBounds().size.x / 2, MaggotsSprite.getGlobalBounds().size.y / 2 }); // Center the book sprite


	

	if (!OintmentTexture.loadFromFile(Ointment)) {
		std::cout << "Failed to load: " << Ointment << std::endl;
	}
	OintmentTexture.loadFromFile(Ointment);
	OintmentSprite = sf::Sprite(OintmentTexture);
	OintmentSprite.setTexture(OintmentTexture);
	OintmentSprite.setPosition({ tablePosition.x+400,tablePosition.y+300 }); // Set position as needed
	OintmentSprite.setOrigin({ OintmentSprite.getGlobalBounds().size.x / 2, OintmentSprite.getGlobalBounds().size.y / 2 }); // Center the book sprite


	if (!ringTexture.loadFromFile(ring)) {
		std::cout << "Failed to load: " << ring << std::endl;
	}
	ringTexture.loadFromFile(ring);
	ringSprite = sf::Sprite(ringTexture);
	ringSprite.setTexture(ringTexture);
	ringSprite.setPosition({ tablePosition.x+1300 , tablePosition.y+200 }); // Set position as needed
	ringSprite.scale({ 0.5f, 0.5f }); // Scale the ring sprite as needed
	ringSprite.setOrigin({ ringSprite.getGlobalBounds().size.x / 2, ringSprite.getGlobalBounds().size.y / 2 }); // Center the book sprite


	if (!ScalpelpngTexture.loadFromFile(Scalpelpng)) {
		std::cout << "Failed to load: " << Scalpelpng << std::endl;
	}
	ScalpelpngTexture.loadFromFile(Scalpelpng);
	ScalpelpngSprite = sf::Sprite(ScalpelpngTexture);
	ScalpelpngSprite.setTexture(ScalpelpngTexture);
	ScalpelpngSprite.setPosition({ tablePosition.x + 850 ,tablePosition.y + 600 }); // Set position as needed
	ScalpelpngSprite.setOrigin({ ScalpelpngSprite.getGlobalBounds().size.x / 2, ScalpelpngSprite.getGlobalBounds().size.y / 2 }); // Center the book sprite


	if (!MortarPestleTexture.loadFromFile(MortarPestle)) {
		std::cout << "Failed to load: " << MortarPestle << std::endl;
	}

	MortarPestleTexture.loadFromFile(MortarPestle);
	MortarPestleSprite = sf::Sprite(MortarPestleTexture);
	MortarPestleSprite.setTexture(MortarPestleTexture);
	MortarPestleSprite.setPosition({ tablePosition.x +550,tablePosition.y+200 }); // Set position as needed
	MortarPestleSprite.setOrigin({ MortarPestleSprite.getGlobalBounds().size.x / 2, MortarPestleSprite.getGlobalBounds().size.y / 2 }); // Center the book sprite


	hayTexture.loadFromFile(hay);
	haySprite = sf::Sprite(hayTexture);
	haySprite.setTexture(hayTexture);
	haySprite.setPosition({ tablePosition.x + 350, tablePosition.y +850 }); // Set position as needed
	haySprite.scale({1.5f, 1.5f }); // Scale the ring sprite as needed
	haySprite.setOrigin({ haySprite.getGlobalBounds().size.x / 2, haySprite.getGlobalBounds().size.y / 2 }); // Center the book sprite


	potTexture.loadFromFile(pot);
	potSprite = sf::Sprite(potTexture);
	potSprite.setTexture(potTexture);
	potSprite.setPosition({ tablePosition.x +1900 , tablePosition.y +600}); // Set position as needed
	potSprite.setOrigin({ potSprite.getGlobalBounds().size.x / 2, potSprite.getGlobalBounds().size.y / 2 }); // Center the book sprite

}


void ItemTable::Draw(RenderWindow& window)
{
    window.draw(GroundSprite);
	window.draw(haySprite);

	window.draw(potSprite);
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
	haySprite.move(offset);
	potSprite.move(offset);
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
	haySprite.setScale(scale);
	potSprite.setScale(scale);
}
