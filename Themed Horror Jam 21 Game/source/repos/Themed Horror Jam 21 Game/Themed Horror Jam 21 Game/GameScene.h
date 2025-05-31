#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"
#include "Text.h"
#include "DialoguePanel.h"
#include "TypewriterEffect.h"
#include "RenderBackground.h"
#include "SurgeryRoom.h"

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

private:
    RenderBackground gameBackground;
    SurgeryRoom surgeryRoom;
    TypewriterEffect typewriterEffect;
    bool dialogueSystemInitialized;
    bool surgeryRoomActive;
    Vector2u resolution;

    // Game objects
    Game::Text* initialText;
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

    void InitializeGame();
    void InitializeDialogueSystem();
    void UpdateDialoguePanelTexture();
};

#endif