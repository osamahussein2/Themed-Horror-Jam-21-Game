#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

using namespace sf;

enum class ItemType {
    BOOK,
    CHICKEN,
    COBWEB,
    CRAMP_COINE,
    CRUCIFIX,
    CUP_OF_ANTIMONY,
    MAGGOTS,
    OINTMENT,
    RING,
    SCALPEL,
    MORTAR_PESTLE,
    HAY,
    POT,
    NONE
};

struct InventoryItem {
    ItemType type;
    std::string name;
    std::string texturePath;
    Texture texture;
    Sprite sprite;
    bool isActive;

    InventoryItem(ItemType t, const std::string& n, const std::string& path)
        : type(t), name(n), texturePath(path), isActive(true) ,sprite(texture){
        texture.loadFromFile(path);
        sprite.setTexture(texture);
    }
};

class Inventory {
public:
    Inventory();
    ~Inventory();

    // Add item to inventory (returns false if item already exists)
    bool AddItem(ItemType itemType, const std::string& name, const std::string& texturePath);

    // Remove item from inventory
    bool RemoveItem(ItemType itemType);

    // Check if item exists in inventory
    bool HasItem(ItemType itemType) const;

    // Get all items in inventory
    const std::vector<InventoryItem>& GetItems() const { return items; }

    // Use item (marks it as used but keeps it in inventory)
    bool UseItem(ItemType itemType);

    // Check if item is still usable
    bool IsItemUsable(ItemType itemType) const;

    // Initialize inventory display
    void Initialize(Vector2f position, Vector2f itemSize, int maxItemsPerRow = 5);

    // Draw inventory
    void Draw(RenderWindow& window);

    // Handle click on inventory
    ItemType HandleClick(Vector2f mousePos);

    // Update inventory positions
    void UpdatePositions();

    // Clear all items
    void Clear();

    // Get item count
    int GetItemCount() const { return items.size(); }

private:
    std::vector<InventoryItem> items;
    Vector2f basePosition;
    Vector2f itemSize;
    int maxItemsPerRow;
    float itemSpacing;

    // Helper function to find item by type
    std::vector<InventoryItem>::iterator FindItem(ItemType itemType);
    std::vector<InventoryItem>::const_iterator FindItem(ItemType itemType) const;
};

#endif
