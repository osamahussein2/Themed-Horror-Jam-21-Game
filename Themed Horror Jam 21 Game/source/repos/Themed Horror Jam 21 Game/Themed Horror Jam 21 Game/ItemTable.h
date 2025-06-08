#ifndef ITEM_TABLE_H
#define ITEM_TABLE_H
#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>
#include "SpriteTexture.h"
#include "Inventory.h"

using namespace sf;

struct TableItem {
	Sprite* sprite;
	ItemType itemType;
	std::string name;
	std::string texturePath;
	bool isAvailable;

	TableItem(Sprite* s, ItemType type, const std::string& n, const std::string& path)
		: sprite(s), itemType(type), name(n), texturePath(path), isAvailable(true) {
	}
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

	// Handle click on items - returns the item type that was clicked
	ItemType HandleItemClick(Vector2f mousePos);

	// Remove item from table (make it unavailable)
	void RemoveItem(ItemType itemType);

	// Check if item is available on table
	bool IsItemAvailable(ItemType itemType) const;

	// Get item info for inventory
	std::string GetItemName(ItemType itemType) const;
	std::string GetItemTexturePath(ItemType itemType) const;

private:

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

	// Vector to store all table items for easy management
	std::vector<TableItem> tableItems;

	// Initialize the table items vector
	void InitializeTableItems();
};

#endif