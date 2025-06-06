#ifndef ITEM_TABLE_H
#define ITEM_TABLE_H
#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>
#include "SpriteTexture.h"

using namespace sf;
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

private:
    SpriteTexture ItemtableSpriteTexture;
    Texture ItemtableTexture;
    Sprite ItemtableSprite;

	SpriteTexture GroundSpriteTexture;
	Texture GroundTexture;
	Sprite GroundSprite;

	const char* book = "Art Assets/SurgeryRoom/items_table/book.png";

	SpriteTexture bookSpriteTexture;
	Texture bookTexture;
	Sprite bookSprite;

	const char* Chicken = "Art Assets/SurgeryRoom/items_table/Chicken.png";
	SpriteTexture ChickenSpriteTexture;
	Texture ChickenTexture;
	Sprite ChickenSprite;


	const char* cobweb = "Art Assets/SurgeryRoom/items_table/cobweb.png";
	SpriteTexture cobwebSpriteTexture;
	Texture cobwebTexture;
	Sprite cobwebSprite;


	const char* Cramp_coine = "Art Assets/SurgeryRoom/items_table/Cramp_coine.png";
	SpriteTexture Cramp_coineSpriteTexture;
	Texture Cramp_coineTexture;
	Sprite Cramp_coineSprite;

	const char* Crucifix = "Art Assets/SurgeryRoom/items_table/Crucifix.png";
	SpriteTexture CrucifixSpriteTexture;
	Texture CrucifixTexture;
	Sprite CrucifixSprite;

	const char* CupOfAntimony = "Art Assets/SurgeryRoom/items_table/Cup of Antimony.png";
	SpriteTexture CupOfAntimonySpriteTexture;
	Texture CupOfAntimonyTexture;
	Sprite CupOfAntimonySprite;

	const char* Maggots = "Art Assets/SurgeryRoom/items_table/Maggots.png";
	SpriteTexture MaggotsSpriteTexture;
	Texture MaggotsTexture;
	Sprite MaggotsSprite;


	const char* Ointment = "Art Assets/SurgeryRoom/items_table/Ointment.png";
	SpriteTexture OintmentSpriteTexture;
	Texture OintmentTexture;
	Sprite OintmentSprite;

	const char* ring = "Art Assets/SurgeryRoom/items_table/ring.png";
	SpriteTexture ringSpriteTexture;
	Texture ringTexture;
	Sprite ringSprite;

	const char* Scalpelpng = "Art Assets/SurgeryRoom/items_table/Scalpelpng.png";
	SpriteTexture ScalpelpngSpriteTexture;
	Texture ScalpelpngTexture;
	Sprite ScalpelpngSprite;

	const char* MortarPestle = "Art Assets/SurgeryRoom/items_table/MortarPestle.png";
	SpriteTexture MortarPestleSpriteTexture;
	Texture MortarPestleTexture;
	Sprite MortarPestleSprite;

};

#endif

