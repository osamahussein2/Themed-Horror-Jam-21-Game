
#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"
#include "Text.h"
#include "DialoguePanel.h"
#include "TypewriterEffect.h"
#include "RenderBackground.h"
#include "SurgeryRoom.h"
#include "OperationScene.h"
#include "ItemTable.h"
#include "Bag.h" // Include the Bag header


enum class SceneType {
    MENU_SCENE,
    DIALOGUE_SCENE,
    SURGERY_ROOM,
    OPERATION_SCENE,
    TABLE_SCENE,
    INVENTORY_SCENE,
    GAME_SCENE
};

enum class GameState {
    DIALOGUE_ACTIVE,
    DIALOGUE_HIDDEN,
    SURGERY_ROOM_ACTIVE,
    OPERATION_ACTIVE,
    ITEM_TABLE_ACTIVE,
    INITIALIZING,
    FAILURE_ACTIVE,
    SUCCESSFUL_DAY_ACTIVE
};

class GameScene : public Scene
{
public:
    GameScene();
    ~GameScene() override;

    void Initialize() override;
    void Update(float deltaTime) override;
    void Render(RenderWindow& window) override;
    void Cleanup() override;
    void OnEnter() override;
    void OnExit() override;
    std::string GetSceneName() const override;

    static int currentDay;

    Vector2u GetResolution() { return resolution; }
    void SetResolution(Vector2u newResolution) { resolution = newResolution; }
private:
    SceneType currentSceneType;
    GameState currentGameState;
    GameState previousGameState; // To remember what state we were in before opening inventory

    RenderBackground gameBackground;
    SurgeryRoom surgeryRoom;
    OperationScene operationScene;
    ItemTable itemTable;
    Bag bag; // Add the bag instance
    TypewriterEffect typewriterEffect;


    Vector2u resolution;

    // Game objects
    std::vector<Game::Text*> dialogueTexts;
    float typeTextTime;
    DialoguePanel* dialoguePanel;

    // Code for different days in game
    void InitializeDay1();
    void UpdateDay1(float deltaTime);
    void UpdateDay1Patients();
    void UpdateDay1OperationScene(float deltaTime);
    void RenderDay1(RenderWindow& window);

    void InitializeDay2();
    void UpdateDay2(float deltaTime);
    void RenderDay2(RenderWindow& window);

    void InitializeDay3();
    void UpdateDay3(float deltaTime);
    void RenderDay3(RenderWindow& window);

    void InitializeDay4();
    void UpdateDay4(float deltaTime);
    void RenderDay4(RenderWindow& window);

    void InitializeDay5();
    void UpdateDay5(float deltaTime);
    void RenderDay5(RenderWindow& window);


    // Input handling
    float inputCooldown;
    const float INPUT_DELAY = 0.15f;

    bool skippedTypewriting;
    bool dialogueSystemInitialized = false;
    int currentDialogueIndex;
    int maxDialogueTexts = 3;

    std::vector<std::string> dialoguePanelTextures;
    std::vector<SpriteTexture> person;

    void InitializeGame();
    void InitializeDialogueSystem();
    void UpdateDialoguePanelTexture();

    // Bag-related methods
    void HandleItemTableClicks(Vector2f mousePos);
    void HandleBagClicks(Vector2f mousePos);
    void InitializeBag();

    float alpha = 255.0f;
    bool alphaIncrease = false;

    float successfulOperationTime{};

    bool operationSceneChanged{};

    // Operation successful text
    Game::Text successfulText{};

    // Failure state
    Game::Text dayFailedText{};
    float failedTextAlpha{};
    float failedTimer{};

    // Success state
    Game::Text daySuccessfulText{};
    float daySuccessfulTextAlpha{};
    float daySuccessfulTimer{};

    // Patient index and max size
    int maxPatients{};
    int currentPatientIndex{};

    // Disable/enable input
    bool isInputEnabled{true};
};

#endif