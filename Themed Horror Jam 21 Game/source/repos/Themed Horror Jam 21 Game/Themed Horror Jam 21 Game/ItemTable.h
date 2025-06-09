#ifndef ITEM_TABLE_H
#define ITEM_TABLE_H
#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>
#include "SpriteTexture.h"
#include "Bag.h" // Include the Bag header

using namespace sf;

struct TableItem {
	ItemType type;
	std::string name;
	std::string texturePath;
	Sprite* sprite;
	bool isCollected;

	TableItem() : sprite(nullptr), isCollected(false), type(ItemType::NONE) {}
	TableItem(ItemType t, const std::string& n, const std::string& path, Sprite* s)
		: type(t), name(n), texturePath(path), sprite(s), isCollected(false) {}
};

class ItemTable
{
public:
	ItemTable();
	~ItemTable();

	void Initialize(const char* filePath_, Vector2f position_, Vector2f scale_, bool centerTexture_ = false);

	// Draw the surgery room to the render window
	void Draw(RenderWindow& window);

	// Update operation scene position (useful for scrolling backgrounds)
	void SetPosition(Vector2f position);

	void Move(Vector2f offset);

	// Scale the operation scene
	void SetScale(Vector2f scale);

	// Click detection and item interaction
	ItemType GetClickedItem(Vector2f mousePos) const;
	bool CollectItem(ItemType itemType);
	void ResetCollectedItems(); // Reset all items to uncollected state

	// Get item information
	std::string GetItemTexturePath(ItemType itemType) const;
	std::string GetItemName(ItemType itemType) const;

private:
	void InitializeTableItems(); // Initialize the table items map

	Texture ItemtableTexture;
	Sprite ItemtableSprite;

	Texture GroundTexture;
	Sprite GroundSprite;

	const char* book = "Art Assets/SurgeryRoom/items_table/book.png";
	Texture bookTexture;
	Sprite bookSprite;

	const char* Chicken = "Art Assets/SurgeryRoom/items_table/Chicken.png";
	Texture ChickenTexture;
	Sprite ChickenSprite;

	const char* cobweb = "Art Assets/SurgeryRoom/items_table/cobweb.png";
	Texture cobwebTexture;
	Sprite cobwebSprite;

	const char* Cramp_coine = "Art Assets/SurgeryRoom/items_table/Cramp_coine.png";
	Texture Cramp_coineTexture;
	Sprite Cramp_coineSprite;

	const char* Crucifix = "Art Assets/SurgeryRoom/items_table/Crucifix.png";
	Texture CrucifixTexture;
	Sprite CrucifixSprite;

	const char* CupOfAntimony = "Art Assets/SurgeryRoom/items_table/Cup of Antimony.png";
	Texture CupOfAntimonyTexture;
	Sprite CupOfAntimonySprite;

	const char* Maggots = "Art Assets/SurgeryRoom/items_table/Maggots.png";
	Texture MaggotsTexture;
	Sprite MaggotsSprite;

	const char* Ointment = "Art Assets/SurgeryRoom/items_table/Ointment.png";
	Texture OintmentTexture;
	Sprite OintmentSprite;

	const char* ring = "Art Assets/SurgeryRoom/items_table/ring.png";
	Texture ringTexture;
	Sprite ringSprite;

	const char* Scalpelpng = "Art Assets/SurgeryRoom/items_table/Scalpelpng.png";
	Texture ScalpelpngTexture;
	Sprite ScalpelpngSprite;

	const char* MortarPestle = "Art Assets/SurgeryRoom/items_table/MortarPestle.png";
	Texture MortarPestleTexture;
	Sprite MortarPestleSprite;

	const char* hay = "Art Assets/SurgeryRoom/items_table/hay.png";
	Texture hayTexture;
	Sprite haySprite;

	const char* pot = "Art Assets/SurgeryRoom/items_table/pot.png";
	Texture potTexture;
	Sprite potSprite;

	// Map to store all table items for easy access
	std::map<ItemType, TableItem> tableItems;
};

#endif