#include "GameScene.h"
#include "Engine.h"
#include "SceneManager.h"

float DIALOGUE_TEXT_CHARACTER_SIZE;

GameScene::GameScene()
    : currentDialogueIndex(0)
    , hideDialogue(false)
    , skippedTypewriting(false)
    , inputCooldown(INPUT_DELAY)
    , dialoguePanel(nullptr)
    , dialogueSystemInitialized(false)
    , surgeryRoomActive(false)
    , typeTextTime(0.0f)
    , operationSceneActive(false)
{
}

GameScene::~GameScene()
{
    if (!dialogueTexts.empty())
    {
        for (int i = 0; i < dialogueTexts.size(); i++)
        {
            delete dialogueTexts[i];
            dialogueTexts[i] = nullptr;
        }
        dialogueTexts.clear();
    }

    if (dialoguePanel)
    {
        delete dialoguePanel;
    }

    if (!dialoguePanelTextures.empty()) dialoguePanelTextures.clear();
}

void GameScene::Initialize()
{
    resolution = Engine::Instance()->GetResolution();
}

void GameScene::Update(float deltaTime)
{
    // Update input cooldown
    if (inputCooldown > 0.0f)
    {
        inputCooldown -= deltaTime;
    }

    // Check for escape key to return to menu
    if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
    {
        typewriterEffect.Reset();
        sceneManager->ChangeScene("Menu");
        return;
    }

    // Move the dialogue texts on the left and bottom sides of the screen
    if (currentDialogueIndex < dialogueTexts.size() && dialogueTexts[currentDialogueIndex] && !hideDialogue)
    {
        float panelWidth = resolution.x - 100.0f;
        float panelHeight = 700.0f;

        float panelX = ((resolution.x - panelWidth) / 2.0f) * (resolution.x / 1920.0f);
        float panelY = (resolution.y - panelHeight - 10.0f);

        if (dialoguePanel->GetPosition() != Vector2f(panelX, panelY))
            dialoguePanel->SetPosition(Vector2f(panelX, panelY));

        if (dialoguePanel->GetSize() != Vector2f(panelWidth, panelHeight))
            dialoguePanel->SetSize(Vector2f(panelWidth, panelHeight));

        float textX = panelX + 25;
        float textY = panelY + 400.0f;

        if (dialogueTexts[currentDialogueIndex]->GetTextPosition() != Vector2f(textX, textY))
            dialogueTexts[currentDialogueIndex]->SetTextPosition(Vector2f(textX, textY));
    }

    if (!hideDialogue)
    {
        typewriterEffect.Update(deltaTime);
        UpdateDialoguePanelTexture();

        if (currentDialogueIndex >= 0 && currentDialogueIndex < static_cast<int>(dialogueTexts.size())
            && dialogueTexts[currentDialogueIndex])
        {
            std::string currentText = typewriterEffect.GetCurrentText();
            dialogueTexts[currentDialogueIndex]->SetTypewriterString(sf::String(currentText));
        }
    }

    // Handle input for dialogue
    if (Keyboard::isKeyPressed(Keyboard::Key::Enter) && inputCooldown <= 0.0f && !hideDialogue)
    {
        if (!typewriterEffect.IsCurrentDialogueComplete())
        {
            typewriterEffect.Skip();
        }
        else if (typewriterEffect.HasNextDialogue())
        {
            typewriterEffect.NextDialogue();
            currentDialogueIndex = typewriterEffect.GetCurrentDialogueIndex();
        }
        else
        {
            hideDialogue = true;
            // Activate surgery room when dialogue ends
            if (!surgeryRoomActive)
            {
                gameBackground.Unload();

                surgeryRoomActive = true;

                if (!surgeryRoom.IsLoaded())
                {
                    // Initialize surgery room with your asset paths
                    surgeryRoom.Initialize(
                        "Art Assets/SurgeryRoom/Background.png",
                        resolution,
                        "Art Assets/SurgeryRoom/BottomUI.png",
                        "Art Assets/SurgeryRoom/TopUI.png");
                }

                person.InitializeSprite("Art Assets/SurgeryRoom/sickness/basebody.png", Vector2f(resolution.x / 2.238f,
                    resolution.y / 2.5f));
            }
        }

        inputCooldown = INPUT_DELAY;
    }

    // If surgery room is active
    if (surgeryRoomActive)
    {
        Vector2i mousePixelPos = Mouse::getPosition(*Engine::Instance()->GetWindow());
        Vector2f mousePos = Engine::Instance()->GetWindow()->mapPixelToCoords(mousePixelPos);

        // Make the sure the mouse position is on the sprite to change its sprite color
        if (person.LoadSprite().getGlobalBounds().contains(mousePos))
        {
            if (person.GetColor() != Color::Red) person.SetColor(Color::Red);

            // Set up the operation scene after clicking the left mouse button
            if (Mouse::isButtonPressed(Mouse::Button::Left))
            {
                if (!operationSceneActive)
                {
                    operationSceneActive = true;

                    operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png", 
                        Vector2f(resolution.x / 2.8f, 0.0f), 
                        Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)), 
                        true);
                }
            }
        }

        // Otherwise, reset the sprite's color back to white once the mouse is longer hovering on the sprite
        else if (!person.LoadSprite().getGlobalBounds().contains(mousePos))
        {
            if (person.GetColor() != Color::White) person.SetColor(Color::White);
        }
    }

    // If operation scene is active
    if (operationSceneActive)
    {

    }
}

void GameScene::Render(RenderWindow& window)
{
    window.clear();

    if (surgeryRoomActive)
    {
        if (!operationSceneActive)
        {
            // Draw surgery room when active
            surgeryRoom.Draw(window);
            window.draw(person.LoadSprite());
        }

        else if (operationSceneActive)
        {
            // Draw operation scene and overlay the UI from surgery room
            operationScene.Draw(window);
            surgeryRoom.DrawUI(window);  // Draw the UI elements on top of operation scene
        }
    }
    else
    {
        // Draw normal game background
        gameBackground.Draw(window);

        if (dialogueTexts[currentDialogueIndex] && !hideDialogue)
        {
            if (dialoguePanel)
            {
                dialoguePanel->DrawDialoguePanel(window);
            }

            window.draw(dialogueTexts[currentDialogueIndex]->LoadText());
        }
        else if (hideDialogue)
        {
            for (int i = 0; i < dialogueTexts.size(); i++)
            {
                delete dialogueTexts[i];
                dialogueTexts[i] = nullptr;
            }
        }
    }
}

void GameScene::Cleanup()
{
}

void GameScene::OnEnter()
{
    InitializeGame();
}

void GameScene::OnExit()
{
}

std::string GameScene::GetSceneName() const
{
    return "Game";
}

void GameScene::InitializeGame()
{
    if (inputCooldown != INPUT_DELAY) inputCooldown = INPUT_DELAY;
    if (currentDialogueIndex != 0) currentDialogueIndex = 0;
    if (hideDialogue != false) hideDialogue = false;
    if (typeTextTime != 0.0f) typeTextTime = 0.0f;
    if (skippedTypewriting != false) skippedTypewriting = false;
    if (operationSceneActive != false) operationSceneActive = false;

    // Reset surgery room state
    surgeryRoomActive = false;

    if (!gameBackground.IsLoaded())
    {
        gameBackground.Initialize("Art Assets/Background.jpg", resolution);
    }

    if (!dialoguePanel)
    {
        dialoguePanel = new DialoguePanel();

        float panelWidth = resolution.x - 100.0f;
        float panelHeight = 700.0f;

        float panelX = ((resolution.x - panelWidth) / 2.0f) * (resolution.x / 1920.0f);
        float panelY = (resolution.y - panelHeight - 10.0f) * (resolution.y / 1080.0f);

        dialoguePanelTextures = { "Art Assets/Ui/chat_box_0.png", "Art Assets/Ui/chat_box_1.png",
            "Art Assets/Ui/chat_box_2.png", "Art Assets/Ui/chat_box_3.png", "Art Assets/Ui/chat_box_4.png" };

        dialoguePanel->InitializeDialoguePanel("Art Assets/Ui/chat_box_3.png", Vector2f(panelX, panelY),
            Vector2f(panelWidth, panelHeight));
    }

    dialogueTexts.clear();
    dialogueTexts.resize(maxDialogueTexts);

    for (int i = 0; i < dialogueTexts.size(); i++)
    {
        if (!dialogueTexts[i])
        {
            dialogueTexts[i] = new Game::Text();
        }
    }

    if (!dialogueSystemInitialized)
    {
        InitializeDialogueSystem();
        dialogueSystemInitialized = true;
    }

    DIALOGUE_TEXT_CHARACTER_SIZE = 50.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

    dialogueTexts[0]->InitializeText("Fonts/Roboto-Regular.ttf", DIALOGUE_TEXT_CHARACTER_SIZE, false, false, 
        sf::Color::White, Vector2f(resolution.x / 2.0f, resolution.y / 2.0f));

    dialogueTexts[1]->InitializeText("Fonts/Roboto-Regular.ttf", DIALOGUE_TEXT_CHARACTER_SIZE, false, false, 
        sf::Color::White, Vector2f(resolution.x / 2.0f, resolution.y / 2.0f));

    dialogueTexts[2]->InitializeText("Fonts/Roboto-Regular.ttf", DIALOGUE_TEXT_CHARACTER_SIZE, false, false, 
        sf::Color::White, Vector2f(resolution.x / 2.0f, resolution.y / 2.0f));
}

void GameScene::InitializeDialogueSystem()
{
    std::vector<TypewriterEffect::DialogueData> allDialogueData = {
         TypewriterEffect::DialogueData("Welcome to game!", 8.0f),
         TypewriterEffect::DialogueData("Play!", 12.0f),
         TypewriterEffect::DialogueData("Go!", 10.0f)
    };

    typewriterEffect.Initialize(allDialogueData);
}

void GameScene::UpdateDialoguePanelTexture()
{
    switch (currentDialogueIndex)
    {
    case 0:
        if (dialoguePanel->GetDialoguePanelTexture() != dialoguePanelTextures[3].c_str())
        {
            dialoguePanel->SetDialoguePanelTexture(dialoguePanelTextures[3].c_str());
        }

        break;

    case 1:
        if (dialoguePanel->GetDialoguePanelTexture() != dialoguePanelTextures[2].c_str())
        {
            dialoguePanel->SetDialoguePanelTexture(dialoguePanelTextures[2].c_str());
        }

        break;

    case 2:
        if (dialoguePanel->GetDialoguePanelTexture() != dialoguePanelTextures[1].c_str())
        {
            dialoguePanel->SetDialoguePanelTexture(dialoguePanelTextures[1].c_str());
        }

        break;

    default:
        break;
    }
}