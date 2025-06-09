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
	bookSprite.setPosition({ tablePosition.x + 1000, tablePosition.y + 200 });
	bookSprite.setScale({ 0.8,0.8 });
	bookSprite.setOrigin({ bookSprite.getGlobalBounds().size.x / 2, bookSprite.getGlobalBounds().size.y / 2 });

	if (!ChickenTexture.loadFromFile(Chicken)) {
		std::cout << "Failed to load: " << Chicken << std::endl;
	}
	ChickenTexture.loadFromFile(Chicken);
	ChickenSprite = sf::Sprite(ChickenTexture);
	ChickenSprite.setTexture(ChickenTexture);
	ChickenSprite.setPosition({ tablePosition.x + 1600,tablePosition.y + 300 });
	ChickenSprite.setScale({ 0.5,0.5 });
	ChickenSprite.setOrigin({ ChickenSprite.getGlobalBounds().size.x / 2, ChickenSprite.getGlobalBounds().size.y / 2 });

	if (!cobwebTexture.loadFromFile(cobweb)) {
		std::cout << "Failed to load: " << cobweb << std::endl;
	}
	cobwebTexture.loadFromFile(cobweb);
	cobwebSprite = sf::Sprite(cobwebTexture);
	cobwebSprite.setTexture(cobwebTexture);
	cobwebSprite.setPosition({ tablePosition.x + 550 ,tablePosition.y + 600 });
	cobwebSprite.setScale({ 0.99,0.99 });
	cobwebSprite.setOrigin({ cobwebSprite.getGlobalBounds().size.x / 2, cobwebSprite.getGlobalBounds().size.y / 2 });

	if (!Cramp_coineTexture.loadFromFile(Cramp_coine)) {
		std::cout << "Failed to load: " << Cramp_coine << std::endl;
	}
	Cramp_coineTexture.loadFromFile(Cramp_coine);
	Cramp_coineSprite = sf::Sprite(Cramp_coineTexture);
	Cramp_coineSprite.setTexture(Cramp_coineTexture);
	Cramp_coineSprite.setPosition({ tablePosition.x + 1000, tablePosition.y + 600 });
	Cramp_coineSprite.setOrigin({ Cramp_coineSprite.getGlobalBounds().size.x / 2, Cramp_coineSprite.getGlobalBounds().size.y / 2 });

	if (!CrucifixTexture.loadFromFile(Crucifix)) {
		std::cout << "Failed to load: " << Crucifix << std::endl;
	}
	CrucifixTexture.loadFromFile(Crucifix);
	CrucifixSprite = sf::Sprite(CrucifixTexture);
	CrucifixSprite.setTexture(CrucifixTexture);
	CrucifixSprite.setPosition({ tablePosition.x + 1000, tablePosition.y + 400 });
	CrucifixSprite.setOrigin({ CrucifixSprite.getGlobalBounds().size.x / 2, CrucifixSprite.getGlobalBounds().size.y / 2 });

	if (!CupOfAntimonyTexture.loadFromFile(CupOfAntimony)) {
		std::cout << "Failed to load: " << CupOfAntimony << std::endl;
	}
	CupOfAntimonyTexture.loadFromFile(CupOfAntimony);
	CupOfAntimonySprite = sf::Sprite(CupOfAntimonyTexture);
	CupOfAntimonySprite.setTexture(CupOfAntimonyTexture);
	CupOfAntimonySprite.setPosition({ tablePosition.x + 750,tablePosition.y + 200 });
	CupOfAntimonySprite.setScale({ 1.5,1.5 });
	CupOfAntimonySprite.setOrigin({ CupOfAntimonySprite.getGlobalBounds().size.x / 2, CupOfAntimonySprite.getGlobalBounds().size.y / 2 });

	if (!MaggotsTexture.loadFromFile(Maggots)) {
		std::cout << "Failed to load: " << Maggots << std::endl;
	}
	MaggotsTexture.loadFromFile(Maggots);
	MaggotsSprite = sf::Sprite(MaggotsTexture);
	MaggotsSprite.setTexture(MaggotsTexture);
	MaggotsSprite.setPosition({ tablePosition.x + 400 , tablePosition.y + 150 });
	MaggotsSprite.setOrigin({ MaggotsSprite.getGlobalBounds().size.x / 2, MaggotsSprite.getGlobalBounds().size.y / 2 });

	if (!OintmentTexture.loadFromFile(Ointment)) {
		std::cout << "Failed to load: " << Ointment << std::endl;
	}
	OintmentTexture.loadFromFile(Ointment);
	OintmentSprite = sf::Sprite(OintmentTexture);
	OintmentSprite.setTexture(OintmentTexture);
	OintmentSprite.setPosition({ tablePosition.x + 400,tablePosition.y + 300 });
	OintmentSprite.setOrigin({ OintmentSprite.getGlobalBounds().size.x / 2, OintmentSprite.getGlobalBounds().size.y / 2 });

	if (!ringTexture.loadFromFile(ring)) {
		std::cout << "Failed to load: " << ring << std::endl;
	}
	ringTexture.loadFromFile(ring);
	ringSprite = sf::Sprite(ringTexture);
	ringSprite.setTexture(ringTexture);
	ringSprite.setPosition({ tablePosition.x + 1300 , tablePosition.y + 200 });
	ringSprite.scale({ 0.5f, 0.5f });
	ringSprite.setOrigin({ ringSprite.getGlobalBounds().size.x / 2, ringSprite.getGlobalBounds().size.y / 2 });

	if (!ScalpelpngTexture.loadFromFile(Scalpelpng)) {
		std::cout << "Failed to load: " << Scalpelpng << std::endl;
	}
	ScalpelpngTexture.loadFromFile(Scalpelpng);
	ScalpelpngSprite = sf::Sprite(ScalpelpngTexture);
	ScalpelpngSprite.setTexture(ScalpelpngTexture);
	ScalpelpngSprite.setPosition({ tablePosition.x + 850 ,tablePosition.y + 600 });
	ScalpelpngSprite.setOrigin({ ScalpelpngSprite.getGlobalBounds().size.x / 2, ScalpelpngSprite.getGlobalBounds().size.y / 2 });

	if (!MortarPestleTexture.loadFromFile(MortarPestle)) {
		std::cout << "Failed to load: " << MortarPestle << std::endl;
	}
	MortarPestleTexture.loadFromFile(MortarPestle);
	MortarPestleSprite = sf::Sprite(MortarPestleTexture);
	MortarPestleSprite.setTexture(MortarPestleTexture);
	MortarPestleSprite.setPosition({ tablePosition.x + 550,tablePosition.y + 200 });
	MortarPestleSprite.setOrigin({ MortarPestleSprite.getGlobalBounds().size.x / 2, MortarPestleSprite.getGlobalBounds().size.y / 2 });

	hayTexture.loadFromFile(hay);
	haySprite = sf::Sprite(hayTexture);
	haySprite.setTexture(hayTexture);
	haySprite.setPosition({ tablePosition.x + 350, tablePosition.y + 850 });
	haySprite.scale({ 1.5f, 1.5f });
	haySprite.setOrigin({ haySprite.getGlobalBounds().size.x / 2, haySprite.getGlobalBounds().size.y / 2 });

	potTexture.loadFromFile(pot);
	potSprite = sf::Sprite(potTexture);
	potSprite.setTexture(potTexture);
	potSprite.setPosition({ tablePosition.x + 1900 , tablePosition.y + 600 });
	potSprite.setOrigin({ potSprite.getGlobalBounds().size.x / 2, potSprite.getGlobalBounds().size.y / 2 });

	// Initialize the table items map after all sprites are set up
	InitializeTableItems();
}

void ItemTable::Draw(RenderWindow& window)
{
	window.draw(GroundSprite);
	window.draw(haySprite);
	window.draw(potSprite);
	window.draw(ItemtableSprite);
	window.draw(bookSprite);
	// Only draw items that haven't been collected
	for (const auto& pair : tableItems) {
		const TableItem& item = pair.second;
		if (!item.isCollected && item.sprite) {
			window.draw(*item.sprite);
		}
	}
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

ItemType ItemTable::GetClickedItem(Vector2f mousePos) const
{
	for (const auto& pair : tableItems) {
		const TableItem& item = pair.second;
		if (!item.isCollected && item.sprite && item.sprite->getGlobalBounds().contains(mousePos)) {
			return item.type;
		}
	}
	return ItemType::NONE;
}

bool ItemTable::CollectItem(ItemType itemType)
{
	auto it = tableItems.find(itemType);
	if (it != tableItems.end() && !it->second.isCollected) {
		it->second.isCollected = true;
		std::cout << "Collected item: " << it->second.name << std::endl;
		return true;
	}
	return false;
}

void ItemTable::ResetCollectedItems()
{
	for (auto& pair : tableItems) {
		pair.second.isCollected = false;
	}
}

std::string ItemTable::GetItemTexturePath(ItemType itemType) const
{
	auto it = tableItems.find(itemType);
	if (it != tableItems.end()) {
		return it->second.texturePath;
	}
	return "";
}

std::string ItemTable::GetItemName(ItemType itemType) const
{
	auto it = tableItems.find(itemType);
	if (it != tableItems.end()) {
		return it->second.name;
	}
	return "";
}

void ItemTable::InitializeTableItems()
{
	// Initialize the map with all table items
	tableItems[ItemType::CHICKEN] = TableItem(ItemType::CHICKEN, "Chicken", Chicken, &ChickenSprite);
	tableItems[ItemType::COBWEB] = TableItem(ItemType::COBWEB, "Cobweb", cobweb, &cobwebSprite);
	tableItems[ItemType::CRAMP_COINE] = TableItem(ItemType::CRAMP_COINE, "Cramp Coine", Cramp_coine, &Cramp_coineSprite);
	tableItems[ItemType::CRUCIFIX] = TableItem(ItemType::CRUCIFIX, "Crucifix", Crucifix, &CrucifixSprite);
	tableItems[ItemType::CUP_OF_ANTIMONY] = TableItem(ItemType::CUP_OF_ANTIMONY, "Cup of Antimony", CupOfAntimony, &CupOfAntimonySprite);
	tableItems[ItemType::MAGGOTS] = TableItem(ItemType::MAGGOTS, "Maggots", Maggots, &MaggotsSprite);
	tableItems[ItemType::OINTMENT] = TableItem(ItemType::OINTMENT, "Ointment", Ointment, &OintmentSprite);
	tableItems[ItemType::RING] = TableItem(ItemType::RING, "Ring", ring, &ringSprite);
	tableItems[ItemType::SCALPEL] = TableItem(ItemType::SCALPEL, "Scalpel", Scalpelpng, &ScalpelpngSprite);
	tableItems[ItemType::MORTAR_PESTLE] = TableItem(ItemType::MORTAR_PESTLE, "Mortar & Pestle", MortarPestle, &MortarPestleSprite);
}