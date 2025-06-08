#include "Inventory.h"
#include <algorithm>
#include <iostream>

Inventory::Inventory() : itemSpacing(10.0f), maxItemsPerRow(5) {
}

Inventory::~Inventory() {
}

bool Inventory::AddItem(ItemType itemType, const std::string& name, const std::string& texturePath) {
    // Check if item already exists
    if (HasItem(itemType)) {
        std::cout << "Item " << name << " already exists in inventory!" << std::endl;
        return false;
    }

    // Add new item
    items.emplace_back(itemType, name, texturePath);
    std::cout << "Added " << name << " to inventory!" << std::endl;

    // Update positions after adding
    UpdatePositions();
    return true;
}

bool Inventory::RemoveItem(ItemType itemType) {
    auto it = FindItem(itemType);
    if (it != items.end()) {
        std::cout << "Removed " << it->name << " from inventory!" << std::endl;
        items.erase(it);
        UpdatePositions();
        return true;
    }
    return false;
}

bool Inventory::HasItem(ItemType itemType) const {
    return FindItem(itemType) != items.end();
}

bool Inventory::UseItem(ItemType itemType) {
    auto it = FindItem(itemType);
    if (it != items.end() && it->isActive) {
        it->isActive = false;
        std::cout << "Used " << it->name << "!" << std::endl;
        return true;
    }
    return false;
}

bool Inventory::IsItemUsable(ItemType itemType) const {
    auto it = FindItem(itemType);
    return it != items.end() && it->isActive;
}

void Inventory::Initialize(Vector2f position, Vector2f itemSize, int maxItemsPerRow) {
    this->basePosition = position;
    this->itemSize = itemSize;
    this->maxItemsPerRow = maxItemsPerRow;
    UpdatePositions();
}

void Inventory::Draw(RenderWindow& window) {
    for (const auto& item : items) {
        // Draw item with transparency if used
        Sprite tempSprite = item.sprite;
        if (!item.isActive) {
            tempSprite.setColor(Color(255, 255, 255, 100)); // Semi-transparent
        }
        window.draw(tempSprite);
    }
}

ItemType Inventory::HandleClick(Vector2f mousePos) {
    for (auto& item : items) {
        if (item.sprite.getGlobalBounds().contains(mousePos) && item.isActive) {
            return item.type;
        }
    }
    return ItemType::NONE;
}

void Inventory::UpdatePositions() {
    for (size_t i = 0; i < items.size(); ++i) {
        int row = i / maxItemsPerRow;
        int col = i % maxItemsPerRow;

        Vector2f position(
            basePosition.x + col * (itemSize.x + itemSpacing),
            basePosition.y + row * (itemSize.y + itemSpacing)
        );

        items[i].sprite.setPosition(position);
        items[i].sprite.setScale(
            { itemSize.x / items[i].texture.getSize().x,
            itemSize.y / items[i].texture.getSize().y
            });
    }
}

void Inventory::Clear() {
    items.clear();
}

std::vector<InventoryItem>::iterator Inventory::FindItem(ItemType itemType) {
    return std::find_if(items.begin(), items.end(),
        [itemType](const InventoryItem& item) {
            return item.type == itemType;
        });
}

std::vector<InventoryItem>::const_iterator Inventory::FindItem(ItemType itemType) const {
    return std::find_if(items.begin(), items.end(),
        [itemType](const InventoryItem& item) {
            return item.type == itemType;
        });
}