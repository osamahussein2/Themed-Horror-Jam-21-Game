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

void ItemTable::Initialize(const char* filePath_, Vector2f position_, Vector2f scale_, Vector2f fitToResolution_,
	bool centerTexture_)
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
	ItemtableSprite.setScale({ 1.8f * fitToResolution_.x, 1.8f * fitToResolution_.y });

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

	bookSprite = sf::Sprite(bookTexture);
	bookSprite.setTexture(bookTexture);
	bookSprite.setPosition({ tablePosition.x + (1000.0f * fitToResolution_.x),
		tablePosition.y + (200.0f * fitToResolution_.y) });

	bookSprite.setScale({ 0.8f * fitToResolution_.x, 0.8f * fitToResolution_.y });
	bookSprite.setOrigin({ bookSprite.getGlobalBounds().size.x / 2, bookSprite.getGlobalBounds().size.y / 2 });

	if (!ChickenTexture.loadFromFile(Chicken)) {
		std::cout << "Failed to load: " << Chicken << std::endl;
	}
	ChickenSprite = sf::Sprite(ChickenTexture);
	ChickenSprite.setTexture(ChickenTexture);
	ChickenSprite.setPosition({ tablePosition.x + (1600.0f * fitToResolution_.x), 
		tablePosition.y + (300.0f * fitToResolution_.y) });

	ChickenSprite.setScale({ 0.5f * fitToResolution_.x, 0.5f * fitToResolution_.y });
	ChickenSprite.setOrigin({ ChickenSprite.getGlobalBounds().size.x / 2, ChickenSprite.getGlobalBounds().size.y / 2 });

	if (!cobwebTexture.loadFromFile(cobweb)) {
		std::cout << "Failed to load: " << cobweb << std::endl;
	}
	cobwebSprite = sf::Sprite(cobwebTexture);
	cobwebSprite.setTexture(cobwebTexture);
	cobwebSprite.setPosition({ tablePosition.x + (550.0f * fitToResolution_.x), 
		tablePosition.y + (600.0f * fitToResolution_.y) });

	cobwebSprite.setScale({ 0.99f * fitToResolution_.x, 0.99f * fitToResolution_.y });
	cobwebSprite.setOrigin({ cobwebSprite.getGlobalBounds().size.x / 2, cobwebSprite.getGlobalBounds().size.y / 2 });

	if (!Cramp_coineTexture.loadFromFile(Cramp_coine)) {
		std::cout << "Failed to load: " << Cramp_coine << std::endl;
	}
	Cramp_coineSprite = sf::Sprite(Cramp_coineTexture);
	Cramp_coineSprite.setTexture(Cramp_coineTexture);
	Cramp_coineSprite.setPosition({ tablePosition.x + (1000.0f * fitToResolution_.x), 
		tablePosition.y + (600.0f * fitToResolution_.y) });

	Cramp_coineSprite.setScale({ 1.0f * fitToResolution_.x, 1.0f * fitToResolution_.y });
	Cramp_coineSprite.setOrigin({ Cramp_coineSprite.getGlobalBounds().size.x / 2, Cramp_coineSprite.getGlobalBounds().size.y / 2 });

	if (!CrucifixTexture.loadFromFile(Crucifix)) {
		std::cout << "Failed to load: " << Crucifix << std::endl;
	}
	CrucifixSprite = sf::Sprite(CrucifixTexture);
	CrucifixSprite.setTexture(CrucifixTexture);
	CrucifixSprite.setPosition({ tablePosition.x + (1000.0f * fitToResolution_.x), 
		tablePosition.y + (400.0f * fitToResolution_.y) });

	CrucifixSprite.setScale({ 1.0f * fitToResolution_.x, 1.0f * fitToResolution_.y });
	CrucifixSprite.setOrigin({ CrucifixSprite.getGlobalBounds().size.x / 2, CrucifixSprite.getGlobalBounds().size.y / 2 });

	if (!CupOfAntimonyTexture.loadFromFile(CupOfAntimony)) {
		std::cout << "Failed to load: " << CupOfAntimony << std::endl;
	}
	CupOfAntimonySprite = sf::Sprite(CupOfAntimonyTexture);
	CupOfAntimonySprite.setTexture(CupOfAntimonyTexture);
	CupOfAntimonySprite.setPosition({ tablePosition.x + (750.0f * fitToResolution_.x), 
		tablePosition.y + (200.0f * fitToResolution_.y) });

	CupOfAntimonySprite.setScale({ 1.5f * fitToResolution_.x, 1.5f * fitToResolution_.y });
	CupOfAntimonySprite.setOrigin({ CupOfAntimonySprite.getGlobalBounds().size.x / 2, CupOfAntimonySprite.getGlobalBounds().size.y / 2 });

	if (!MaggotsTexture.loadFromFile(Maggots)) {
		std::cout << "Failed to load: " << Maggots << std::endl;
	}
	MaggotsSprite = sf::Sprite(MaggotsTexture);
	MaggotsSprite.setTexture(MaggotsTexture);
	MaggotsSprite.setPosition({ tablePosition.x + (400.0f * fitToResolution_.x), 
		tablePosition.y + (150.0f * fitToResolution_.y) });

	MaggotsSprite.setScale({ 1.0f * fitToResolution_.x, 1.0f * fitToResolution_.y });
	MaggotsSprite.setOrigin({ MaggotsSprite.getGlobalBounds().size.x / 2, MaggotsSprite.getGlobalBounds().size.y / 2 });

	if (!OintmentTexture.loadFromFile(Ointment)) {
		std::cout << "Failed to load: " << Ointment << std::endl;
	}
	OintmentSprite = sf::Sprite(OintmentTexture);
	OintmentSprite.setTexture(OintmentTexture);
	OintmentSprite.setPosition({ tablePosition.x + (400.0f * fitToResolution_.x), 
		tablePosition.y + (300.0f * fitToResolution_.y) });

	OintmentSprite.setScale({ 1.0f * fitToResolution_.x, 1.0f * fitToResolution_.y });
	OintmentSprite.setOrigin({ OintmentSprite.getGlobalBounds().size.x / 2, OintmentSprite.getGlobalBounds().size.y / 2 });

	if (!ringTexture.loadFromFile(ring)) {
		std::cout << "Failed to load: " << ring << std::endl;
	}
	ringSprite = sf::Sprite(ringTexture);
	ringSprite.setTexture(ringTexture);
	ringSprite.setPosition({ tablePosition.x + (1300.0f * fitToResolution_.x), 
		tablePosition.y + (200.0f * fitToResolution_.y) });

	ringSprite.scale({ 0.5f * fitToResolution_.x, 0.5f * fitToResolution_.y });
	ringSprite.setOrigin({ ringSprite.getGlobalBounds().size.x / 2, ringSprite.getGlobalBounds().size.y / 2 });

	if (!ScalpelpngTexture.loadFromFile(Scalpelpng)) {
		std::cout << "Failed to load: " << Scalpelpng << std::endl;
	}
	ScalpelpngSprite = sf::Sprite(ScalpelpngTexture);
	ScalpelpngSprite.setTexture(ScalpelpngTexture);
	ScalpelpngSprite.setPosition({ tablePosition.x + (850.0f * fitToResolution_.x), 
		tablePosition.y + (600.0f * fitToResolution_.y) });

	ScalpelpngSprite.setScale({ 1.0f * fitToResolution_.x, 1.0f * fitToResolution_.y });
	ScalpelpngSprite.setOrigin({ ScalpelpngSprite.getGlobalBounds().size.x / 2, ScalpelpngSprite.getGlobalBounds().size.y / 2 });

	if (!MortarPestleTexture.loadFromFile(MortarPestle)) {
		std::cout << "Failed to load: " << MortarPestle << std::endl;
	}
	MortarPestleSprite = sf::Sprite(MortarPestleTexture);
	MortarPestleSprite.setTexture(MortarPestleTexture);
	MortarPestleSprite.setPosition({ tablePosition.x + (550.0f * fitToResolution_.x), 
		tablePosition.y + (200.0f * fitToResolution_.y) });

	MortarPestleSprite.setScale({ 1.0f * fitToResolution_.x, 1.0f * fitToResolution_.y });
	MortarPestleSprite.setOrigin({ MortarPestleSprite.getGlobalBounds().size.x / 2, MortarPestleSprite.getGlobalBounds().size.y / 2 });

	if (!hayTexture.loadFromFile(hay)) {
		std::cout << "Failed to load: " << hay << std::endl;
	}
	haySprite = sf::Sprite(hayTexture);
	haySprite.setTexture(hayTexture);
	haySprite.setPosition({ tablePosition.x + (350.0f * fitToResolution_.x), 
		tablePosition.y + (850.0f * fitToResolution_.y) });

	haySprite.scale({ 1.5f * fitToResolution_.x, 1.5f * fitToResolution_.y });
	haySprite.setOrigin({ haySprite.getGlobalBounds().size.x / 2, haySprite.getGlobalBounds().size.y / 2 });

	if (!potTexture.loadFromFile(pot)) {
		std::cout << "Failed to load: " << pot << std::endl;
	}
	potSprite = sf::Sprite(potTexture);
	potSprite.setTexture(potTexture);
	potSprite.setPosition({ tablePosition.x + (1900.0f * fitToResolution_.x), 
		tablePosition.y + (600.0f * fitToResolution_.x) });

	potSprite.setScale({ 1.0f * fitToResolution_.x, 1.0f * fitToResolution_.y });
	potSprite.setOrigin({ potSprite.getGlobalBounds().size.x / 2, potSprite.getGlobalBounds().size.y / 2 });

	// Initialize the table items map after all sprites are set up
	InitializeTableItems();

	std::cout << "ItemTable initialized with " << tableItems.size() << " items" << std::endl;
}

void ItemTable::Draw(RenderWindow& window)
{
	window.draw(GroundSprite);
	window.draw(haySprite);
	window.draw(potSprite);
	window.draw(ItemtableSprite);

	// Draw all items using the tableItems map for consistency
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
	for (auto& pair : tableItems) {
		if (pair.second.sprite) {
			pair.second.sprite->move(offset);
		}
	}
}

void ItemTable::SetScale(Vector2f scale)
{
	ItemtableSprite.setScale(scale);
	GroundSprite.setScale(scale);
	// Scale all items
	for (auto& pair : tableItems) {
		if (pair.second.sprite) {
			pair.second.sprite->setScale(scale);
		}
	}
}

ItemType ItemTable::GetClickedItem(Vector2f mousePos) const
{
	for (const auto& pair : tableItems) {
		const TableItem& item = pair.second;
		if (!item.isCollected && item.sprite && item.sprite->getGlobalBounds().contains(mousePos)) {
			std::cout << "Clicked on table item: " << item.name << std::endl;
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
	std::cout << "All items reset to uncollected state" << std::endl;
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
	if (!tableItems[ItemType::BOOK].isCollected)
		tableItems[ItemType::BOOK] = TableItem(ItemType::BOOK, "Book", book, &bookSprite);

	if (!tableItems[ItemType::CHICKEN].isCollected)
		tableItems[ItemType::CHICKEN] = TableItem(ItemType::CHICKEN, "Chicken", Chicken, &ChickenSprite);

	if (!tableItems[ItemType::COBWEB].isCollected)
		tableItems[ItemType::COBWEB] = TableItem(ItemType::COBWEB, "Cobweb", cobweb, &cobwebSprite);

	if (!tableItems[ItemType::CRAMP_COINE].isCollected)
		tableItems[ItemType::CRAMP_COINE] = TableItem(ItemType::CRAMP_COINE, "Cramp Coine", Cramp_coine, &Cramp_coineSprite);

	if (!tableItems[ItemType::CRUCIFIX].isCollected)
		tableItems[ItemType::CRUCIFIX] = TableItem(ItemType::CRUCIFIX, "Crucifix", Crucifix, &CrucifixSprite);

	if (!tableItems[ItemType::CUP_OF_ANTIMONY].isCollected)
		tableItems[ItemType::CUP_OF_ANTIMONY] = TableItem(ItemType::CUP_OF_ANTIMONY, "Cup of Antimony", CupOfAntimony, &CupOfAntimonySprite);

	if (!tableItems[ItemType::MAGGOTS].isCollected)
		tableItems[ItemType::MAGGOTS] = TableItem(ItemType::MAGGOTS, "Maggots", Maggots, &MaggotsSprite);

	if (!tableItems[ItemType::OINTMENT].isCollected)
		tableItems[ItemType::OINTMENT] = TableItem(ItemType::OINTMENT, "Ointment", Ointment, &OintmentSprite);

	if (!tableItems[ItemType::RING].isCollected)
		tableItems[ItemType::RING] = TableItem(ItemType::RING, "Ring", ring, &ringSprite);

	if (!tableItems[ItemType::SCALPEL].isCollected)
		tableItems[ItemType::SCALPEL] = TableItem(ItemType::SCALPEL, "Scalpel", Scalpelpng, &ScalpelpngSprite);

	if (!tableItems[ItemType::MORTAR_PESTLE].isCollected)
		tableItems[ItemType::MORTAR_PESTLE] = TableItem(ItemType::MORTAR_PESTLE, "Mortar & Pestle", MortarPestle, &MortarPestleSprite);

	if (!tableItems[ItemType::HAY].isCollected)
		tableItems[ItemType::HAY] = TableItem(ItemType::HAY, "Hay", hay, &haySprite);

	if (!tableItems[ItemType::POT].isCollected)
		tableItems[ItemType::POT] = TableItem(ItemType::POT, "Pot", pot, &potSprite);

}