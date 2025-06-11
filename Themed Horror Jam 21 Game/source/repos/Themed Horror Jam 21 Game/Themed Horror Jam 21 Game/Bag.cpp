#include "Bag.h"
#include <iostream>

Bag::Bag() : isVisible(false), isInitialized(false) , bagSprite(bagTexture){
    inventory.reserve(maxItems);
}

Bag::~Bag() {
    ClearBag();
}

void Bag::Initialize(const std::string& bagTexturePath, Vector2f position, Vector2f scale) {
    if (!bagTexture.loadFromFile(bagTexturePath)) {
        std::cout << "Failed to load bag texture: " << bagTexturePath << std::endl;
        return;
    }
    bagTexture.loadFromFile(bagTexturePath);
    bagSprite=Sprite(bagTexture);
    bagSprite.setTexture(bagTexture);
    bagPosition = position;
    bagScale = scale;
    bagSprite.setPosition(bagPosition);
    bagSprite.setScale(bagScale);

    // Calculate item grid positions within the bag
    FloatRect bagBounds = bagSprite.getGlobalBounds();
    float LeftBagBounds = (bagBounds.getCenter().x - bagBounds.size.x / 2);
    float TopBagBounds = (bagBounds.getCenter().y - bagBounds.size.y / 2);
    itemStartPosition = Vector2f({ LeftBagBounds + 50, TopBagBounds + 100 });
    itemSpacing = Vector2f(120, 120);

    isInitialized = true;
    isVisible = false;
    std::cout << "Bag initialized successfully!" << std::endl;
}

bool Bag::AddItem(ItemType itemType, const std::string& itemName, const std::string& texturePath) {
    if (!isInitialized) {
        std::cout << "Bag not initialized!" << std::endl;
        return false;
    }

    // Check if item already exists
    if (HasItem(itemType)) {
        std::cout << "Item already in bag: " << itemName << std::endl;
        return false;
    }

    // Check if bag is full
    if (inventory.size() >= maxItems) {
        std::cout << "Bag is full!" << std::endl;
        return false;
    }

    // Create new inventory item
    InventoryItem newItem(itemType, itemName, texturePath);

    // Load the texture and set up the sprite
    LoadItemTexture(newItem);

    if (newItem.isLoaded) {
        inventory.push_back(newItem);
        ArrangeItems();
        std::cout << "Added " << itemName << " to bag (texture loaded successfully)" << std::endl;
        return true;
    }
    else {
        std::cout << "Failed to load texture for " << itemName << " at path: " << texturePath << std::endl;
        return false;
    }
}

bool Bag::RemoveItem(ItemType itemType) {
    std::cout << "removing item from bag" << std::endl;
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if (it->type == itemType) {
            inventory.erase(it);
            ArrangeItems();
            return true;
        }
    }
    return false;
}

bool Bag::HasItem(ItemType itemType) const {
    for (const auto& item : inventory) {
        if (item.type == itemType) {
            return true;
        }
    }
    return false;
}

void Bag::ClearBag() {
    inventory.clear();
}

void Bag::Draw(RenderWindow& window) {
    if (!isVisible || !isInitialized) return;

    // Draw bag background
    window.draw(bagSprite);

    // Draw all items in the bag
    std::cout << "Drawing bag with " << inventory.size() << " items" << std::endl;
    for (const auto& item : inventory) {
        if (item.isLoaded) {
            window.draw(item.sprite);
            std::cout << "Drawing item: " << item.name << " at position ("
                << item.sprite.getPosition().x << ", " << item.sprite.getPosition().y << ")" << std::endl;
        }
        else {
            std::cout << "Skipping unloaded item: " << item.name << std::endl;
        }
    }
}

void Bag::SetPosition(Vector2f position) {
    bagPosition = position;
    bagSprite.setPosition(bagPosition);

    // Recalculate item positions
    FloatRect bagBounds = bagSprite.getGlobalBounds();
    float LeftBagBounds = (bagBounds.getCenter().x - bagBounds.size.x / 2);
    float TopBagBounds = (bagBounds.getCenter().y - bagBounds.size.y / 2);
    itemStartPosition = Vector2f(LeftBagBounds + 50, TopBagBounds + 100);
    ArrangeItems();
}

void Bag::SetScale(Vector2f scale) {
    bagScale = scale;
    bagSprite.setScale(bagScale);

    // Recalculate item positions
    FloatRect bagBounds = bagSprite.getGlobalBounds();
    float LeftBagBounds = (bagBounds.getCenter().x - bagBounds.size.x / 2);
    float TopBagBounds = (bagBounds.getCenter().y - bagBounds.size.y / 2);
    itemStartPosition = Vector2f(LeftBagBounds + 50, TopBagBounds + 100);
    itemSpacing = Vector2f(120 * scale.x, 120 * scale.y);
    ArrangeItems();
}

void Bag::Show() {
    isVisible = true;
}

void Bag::Hide() {
    isVisible = false;
}

ItemType Bag::GetClickedItem(Vector2f mousePos) const {
    if (!isVisible) return ItemType::NONE;

    for (const auto& item : inventory) {
        if (item.isLoaded && item.sprite.getGlobalBounds().contains(mousePos)) {
            return item.type;
        }
    }
    return ItemType::NONE;
}

void Bag::ArrangeItems() {
    const int itemsPerRow = 4;

    std::cout << "Arranging " << inventory.size() << " items in bag" << std::endl;
    std::cout << "Item start position: (" << itemStartPosition.x << ", " << itemStartPosition.y << ")" << std::endl;
    std::cout << "Item spacing: (" << itemSpacing.x << ", " << itemSpacing.y << ")" << std::endl;

    for (size_t i = 0; i < inventory.size(); ++i) {
        int row = i / itemsPerRow;
        int col = i % itemsPerRow;

        Vector2f itemPos = Vector2f(
            itemStartPosition.x + col * itemSpacing.x,
            itemStartPosition.y + row * itemSpacing.y
        );

        inventory[i].position = itemPos;
        if (inventory[i].isLoaded) {
            inventory[i].sprite.setPosition(itemPos);
            std::cout << "Item " << inventory[i].name << " positioned at ("
                << itemPos.x << ", " << itemPos.y << ")" << std::endl;
        }
        else {
            std::cout << "Warning: Item " << inventory[i].name << " not loaded, cannot position sprite" << std::endl;
        }
    }
}

void Bag::LoadItemTexture(InventoryItem& item) {
    std::cout << "starting to load item texture "<< std::endl;
    if (item.texture.loadFromFile(item.texturePath)) {

        item.sprite = Sprite(item.texture);
        item.sprite.setTexture(item.texture);

        // Scale item to fit in bag slot
        Vector2f itemScale(0.8f, 0.8f);
        item.sprite.setScale(itemScale);

        // Center the sprite
        FloatRect bounds = item.sprite.getLocalBounds();
        item.sprite.setOrigin({ bounds.size.x / 2, bounds.size.y / 2 });

        item.isLoaded = true;
        std::cout << "found to load item texture: " << item.texturePath << std::endl;
    }
    else {
        std::cout << "Failed to load item texture: " << item.texturePath << std::endl;
        item.isLoaded = false;
    }
}