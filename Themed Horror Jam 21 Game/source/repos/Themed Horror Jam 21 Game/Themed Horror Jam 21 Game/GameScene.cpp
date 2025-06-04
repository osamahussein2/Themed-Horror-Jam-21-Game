#include "GameScene.h"
#include "Engine.h"
#include "SceneManager.h"

float DIALOGUE_TEXT_CHARACTER_SIZE;

GameScene::GameScene()
    : currentDialogueIndex(0)
    , currentGameState(GameState::INITIALIZING)
    , skippedTypewriting(false)
    , inputCooldown(INPUT_DELAY)
    , dialoguePanel(nullptr)
    , dialogueSystemInitialized(false)
    , typeTextTime(0.0f)
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


        // Update based on current game state
        switch (currentGameState)
        {
            case GameState::DIALOGUE_ACTIVE:
            {
                // Update the typewriter effect and dialogue panel textures
                typewriterEffect.Update(deltaTime);
                UpdateDialoguePanelTexture();

                if (currentDialogueIndex >= 0 && currentDialogueIndex < static_cast<int>(dialogueTexts.size())
                    && dialogueTexts[currentDialogueIndex])
                {
                    std::string currentText = typewriterEffect.GetCurrentText();
                    dialogueTexts[currentDialogueIndex]->SetTypewriterString(sf::String(currentText));
                }

                // Handle input for dialogue
                if (Keyboard::isKeyPressed(Keyboard::Key::Enter) && inputCooldown <= 0.0f)
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
                        currentGameState = GameState::DIALOGUE_HIDDEN;
                        // Activate surgery room when dialogue ends
                        gameBackground.Unload();

                        if (!surgeryRoom.IsLoaded())
                        {
                            // Initialize surgery room with your asset paths
                            surgeryRoom.Initialize(
                                "Art Assets/SurgeryRoom/Background.png",
                                "Art Assets/SurgeryRoom/BottomUI.png",
                                "Art Assets/SurgeryRoom/TopUI.png",
                                resolution,
                                sf::Vector2f(resolution.x / 1920.0f, resolution.y / 1080.0f),//size
                                sf::Vector2f(0.0f, resolution.y / 1.35f), // bottom UI
                                sf::Vector2f(resolution.x / 4.0f, 0.0f), // top UI
                                sf::Vector2f(resolution.x / 7.0f, resolution.y / 1.15f), // life sprite 0 position
                                sf::Vector2f(resolution.x / 5.5f, resolution.y / 1.15f), // life sprite 1 position
                                sf::Vector2f(resolution.x / 4.5f, resolution.y / 1.15f), // life sprite 2 position
                                sf::Vector2f(resolution.x / 7.0f, resolution.y / 1.15f), // death sprite 0 position
                                sf::Vector2f(resolution.x / 5.5f, resolution.y / 1.15f), // death sprite 1 position
                                sf::Vector2f(resolution.x / 4.5f, resolution.y / 1.15f), // death sprite 2 position
                                sf::Vector2f(resolution.x / 25.0f, resolution.y / 1.235f), // timer sprite position
                                sf::Vector2f(resolution.x / 1.27f, resolution.y / 1.15f),// notes sprite position
                                sf::Vector2f(resolution.x / 1.43f, resolution.y / 1.15f), // bag sprite position
                                sf::Vector2f(resolution.x / 1.13f, resolution.y / 1.15f),// table UI sprite position
                                sf::Vector2f(resolution.x / 1.53f, resolution.y / 1.22f));// OperationTableSprite UI sprite position
                        }

                        person.InitializeSprite("Art Assets/SurgeryRoom/sickness/basebody.png", Vector2f(resolution.x / 2.238f,
                            resolution.y / 2.5f), sf::Vector2f(resolution.x / 1920.0f, resolution.y / 1080.0f));

                        currentGameState = GameState::SURGERY_ROOM_ACTIVE;
                    }

                    inputCooldown = INPUT_DELAY;
                }
                break;
            }

            case GameState::SURGERY_ROOM_ACTIVE:
            {
                // Update the timer
                surgeryRoom.UpdateTimer(deltaTime);

                Vector2i mousePixelPos = Mouse::getPosition(*Engine::Instance()->GetWindow());
                Vector2f mousePos = Engine::Instance()->GetWindow()->mapPixelToCoords(mousePixelPos);

                if (surgeryRoom.TopUISpriteTexture.LoadSprite().getGlobalBounds().contains(mousePos))
                {
                    if (surgeryRoom.TopUISpriteTexture.GetColor() != Color::Red)
                        surgeryRoom.TopUISpriteTexture.SetColor(Color::Red);

                    if (Mouse::isButtonPressed(Mouse::Button::Left))
                    {
                        // Change to ITEM_TABLE_ACTIVE instead of DIALOGUE_ACTIVE
                        currentGameState = GameState::ITEM_TABLE_ACTIVE;
                        itemTable.Initialize("Art Assets/SurgeryRoom/Items.png",
                            Vector2f(resolution.x / 2.8f, 0.0f),
                            Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)));
                    }
                }
                else
                {
                    // Reset color when not hovering
                    if (surgeryRoom.TopUISpriteTexture.GetColor() != Color::White)
                        surgeryRoom.TopUISpriteTexture.SetColor(Color::White);
                }

                // Make sure the mouse position is on the sprite to change its sprite color
                if (person.LoadSprite().getGlobalBounds().contains(mousePos))
                {
                    if (person.GetColor() != Color::Red) person.SetColor(Color::Red);

                    // Set up the operation scene after clicking the left mouse button
                    if (Mouse::isButtonPressed(Mouse::Button::Left))
                    {
                        currentGameState = GameState::OPERATION_ACTIVE;

                        operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                            Vector2f(resolution.x / 2.8f, 0.0f),
                            Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                            true);

                        // Start the timer when operation scene becomes active
                        surgeryRoom.StartTimer(57.0f); // Start with 60 seconds, adjust as needed
                    }
                }
                // Otherwise, reset the sprite's color back to white once the mouse is no longer hovering on the sprite
                else if (!person.LoadSprite().getGlobalBounds().contains(mousePos))
                {
                    if (person.GetColor() != Color::White) person.SetColor(Color::White);
                }
                break;
            }

            case GameState::OPERATION_ACTIVE:
            {
                // Operation scene specific updates can go here
                break;
            }

            case GameState::DIALOGUE_HIDDEN:
            {
                // Handle any logic for when dialogue is hidden but surgery room isn't active yet
                break;
            }

            case GameState::INITIALIZING:
            {
                // Handle initialization state if needed
                break;
            }

            // Add this new case after OPERATION_ACTIVE:
            case GameState::ITEM_TABLE_ACTIVE:
            {
                // Handle input for item table
                if (Keyboard::isKeyPressed(Keyboard::Key::Enter) && inputCooldown <= 0.0f)
                {
                    // Return to surgery room when Enter is pressed
                    currentGameState = GameState::SURGERY_ROOM_ACTIVE;
                    inputCooldown = INPUT_DELAY;
                }

                // Also allow clicking outside to close (optional)
                if (Mouse::isButtonPressed(Mouse::Button::Right) && inputCooldown <= 0.0f)
                {
                    currentGameState = GameState::SURGERY_ROOM_ACTIVE;
                    inputCooldown = INPUT_DELAY;
                }
                break;
            }
        }
       
}
// Update your Render method to include the new case:
void GameScene::Render(RenderWindow& window)
{
    window.clear();
    switch (currentGameState)
    {
        case GameState::DIALOGUE_ACTIVE:
        {
            // Draw normal game background
            gameBackground.Draw(window);

            if (dialogueTexts[currentDialogueIndex])
            {
                if (dialoguePanel)
                {
                    dialoguePanel->DrawDialoguePanel(window);
                }
                window.draw(dialogueTexts[currentDialogueIndex]->LoadText());
            }
            break;
        }

        case GameState::SURGERY_ROOM_ACTIVE:
        {
            // Draw surgery room when active
            surgeryRoom.Draw(window, person.LoadSprite());
            break;
        }

        case GameState::OPERATION_ACTIVE:
        {
            // Draw operation scene and overlay the UI from surgery room
            operationScene.Draw(window);
            surgeryRoom.DrawUI(window);  // Draw the UI elements on top of operation scene
            break;
        }

        // Add this new case:
        case GameState::ITEM_TABLE_ACTIVE:
        {
            // Draw surgery room as background
            // Draw surgery room as background
            surgeryRoom.Draw(window, person.LoadSprite());

            // Draw item table on top
            if (itemTable.IsLoaded())
            {
                itemTable.Draw(window);
            }
            break;
        }

        case GameState::DIALOGUE_HIDDEN:
        {
            // Clean up dialogue texts when hidden
            for (int i = 0; i < dialogueTexts.size(); i++)
            {
                delete dialogueTexts[i];
                dialogueTexts[i] = nullptr;
            }
            break;
        }

        case GameState::INITIALIZING:
        {
            // Handle initialization rendering if needed
            break;
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
    // Reset to initial state
    currentGameState = GameState::DIALOGUE_ACTIVE;

	// Clear previous dialogue texts
	for (int i = 0; i < dialogueTexts.size(); i++)
	{
		delete dialogueTexts[i];
		dialogueTexts[i] = nullptr;
	}
	dialogueTexts.clear();

    // Stop any running timer when reinitializing
    if (surgeryRoom.IsLoaded())
    {
        surgeryRoom.StopTimer();
    }

    if (inputCooldown != INPUT_DELAY) inputCooldown = INPUT_DELAY;
    if (currentDialogueIndex != 0) currentDialogueIndex = 0;
    if (typeTextTime != 0.0f) typeTextTime = 0.0f;
    if (skippedTypewriting != false) skippedTypewriting = false;

    if (!gameBackground.IsLoaded())
    {
        gameBackground.Initialize("Art Assets/Background.jpg", resolution);
    }

    if (!dialoguePanel)
    {
        dialoguePanel = new DialoguePanel();

        float panelWidth = 1820.0f * (resolution.x / 1920.0f);
        float panelHeight = 700.0f * (resolution.y / 1080.0f);

        float panelX = resolution.x / 35.0f;
        float panelY = resolution.y / 3.0f;

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

    float textPanelX = resolution.x / 25.0f; // Move text panels slightly to the right from the dialogue panel
    float textPanelY = resolution.y / 1.4f; // Move text panels slightly below from the dialogue panel

    DIALOGUE_TEXT_CHARACTER_SIZE = 50.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

    dialogueTexts[0]->InitializeText("Fonts/Roboto-Regular.ttf", DIALOGUE_TEXT_CHARACTER_SIZE, false, false,
        sf::Color::White, Vector2f(textPanelX, textPanelY));

    dialogueTexts[1]->InitializeText("Fonts/Roboto-Regular.ttf", DIALOGUE_TEXT_CHARACTER_SIZE, false, false,
        sf::Color::White, Vector2f(textPanelX, textPanelY));

    dialogueTexts[2]->InitializeText("Fonts/Roboto-Regular.ttf", DIALOGUE_TEXT_CHARACTER_SIZE, false, false,
        sf::Color::White, Vector2f(textPanelX, textPanelY));
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