#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"
#include "Text.h"
#include "DialoguePanel.h"
#include "TypewriterEffect.h"
#include "RenderBackground.h"
#include "SurgeryRoom.h"
#include "OperationScene.h"

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
    RenderBackground gameBackground;
    SurgeryRoom surgeryRoom;
    OperationScene operationScene;
    TypewriterEffect typewriterEffect;
    bool dialogueSystemInitialized;
    bool surgeryRoomActive;
    Vector2u resolution;

    // Game objects
    std::vector<Game::Text*> dialogueTexts;
    float typeTextTime;
    DialoguePanel* dialoguePanel;

    // Input handling
    float inputCooldown;
    const float INPUT_DELAY = 0.15f;

    bool hideDialogue;
    bool skippedTypewriting;
    int currentDialogueIndex;
    int maxDialogueTexts = 3;

    std::vector<std::string> dialoguePanelTextures;

    SpriteTexture person;

    void InitializeGame();
    void InitializeDialogueSystem();

    void UpdateDialoguePanelTexture();

    bool operationSceneActive;
};

#endif