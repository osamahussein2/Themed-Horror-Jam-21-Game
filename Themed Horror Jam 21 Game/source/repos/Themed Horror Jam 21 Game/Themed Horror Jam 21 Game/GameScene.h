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
    INITIALIZING
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

    Vector2u GetResolution() { return resolution; }
    void SetResolution(Vector2u newResolution) { resolution = newResolution; }

private:
    SceneType currentSceneType;
    GameState currentGameState;

    RenderBackground gameBackground;
    SurgeryRoom surgeryRoom;
    OperationScene operationScene;
    ItemTable itemTable;
    TypewriterEffect typewriterEffect;

    Vector2u resolution;

    // Game objects
    std::vector<Game::Text*> dialogueTexts;
    float typeTextTime;
    DialoguePanel* dialoguePanel;

    // Input handling
    float inputCooldown;
    const float INPUT_DELAY = 0.15f;

    bool skippedTypewriting;
    bool dialogueSystemInitialized = false;
    int currentDialogueIndex;
    int maxDialogueTexts = 3;

    std::vector<std::string> dialoguePanelTextures;
    SpriteTexture person;

    void InitializeGame();
    void InitializeDialogueSystem();
    void UpdateDialoguePanelTexture();
};

#endif