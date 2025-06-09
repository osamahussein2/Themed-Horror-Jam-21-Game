#ifndef BAG_H
#define BAG_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <map>

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
    Vector2f position;
    Sprite sprite;
    Texture texture;
    bool isLoaded;

    InventoryItem() : type(ItemType::NONE), isLoaded(false), sprite(texture) {}
    InventoryItem(ItemType t, const std::string& n, const std::string& path)
        : type(t), name(n), texturePath(path), isLoaded(false), sprite(texture) {}
};

class Bag {
public:
    Bag();
    ~Bag();

    void Initialize(const std::string& bagTexturePath, Vector2f position, Vector2f scale);

    // Inventory management
    bool AddItem(ItemType itemType, const std::string& itemName, const std::string& texturePath);
    bool RemoveItem(ItemType itemType);
    bool HasItem(ItemType itemType) const;
    void ClearBag();

    // Rendering
    void Draw(RenderWindow& window);
    void SetPosition(Vector2f position);
    void SetScale(Vector2f scale);
    void Show();
    void Hide();
    bool IsVisible() const { return isVisible; }

    // Interaction
    ItemType GetClickedItem(Vector2f mousePos) const;
    int GetItemCount() const { return inventory.size(); }

    // Getters
    const std::vector<InventoryItem>& GetInventory() const { return inventory; }
    FloatRect GetBagBounds() const { return bagSprite.getGlobalBounds(); }

private:
    void ArrangeItems();
    void LoadItemTexture(InventoryItem& item);

    Texture bagTexture;
    Sprite bagSprite;

    std::vector<InventoryItem> inventory;
    const int maxItems = 12; // Maximum items that can fit in the bag

    Vector2f bagPosition;
    Vector2f bagScale;
    Vector2f itemStartPosition;
    Vector2f itemSpacing;

    bool isVisible;
    bool isInitialized;
};

#endif