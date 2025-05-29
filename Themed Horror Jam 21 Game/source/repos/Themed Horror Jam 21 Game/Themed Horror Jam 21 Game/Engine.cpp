#include "Engine.h"

Engine* Engine::engineInstance = nullptr;

Engine::Engine()
    : currentState(GameState::Menu)
    , player(nullptr)
    , initialText(nullptr)
    , currentDialogueIndex(0)
    , hideDialogue(false)
    , skippedTypewriting(false)
    , inputCooldown(INPUT_DELAY)
{
}

Engine::~Engine()
{
    if (player)
    {
        delete player;
        player = nullptr;
    }
    if (initialText)
    {
        delete initialText;
        initialText = nullptr;
    }

    // Only if the dialogue texts aren't empty, delete the dialogue texts and clear the vector
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
}

Engine* Engine::Instance()
{
    if (engineInstance == nullptr)
    {
        engineInstance = new Engine();
    }

    return engineInstance;
}


void Engine::RunEngine()
{
    // Get the screen resolution and create an SFML window
    resolution.x = VideoMode::getDesktopMode().size.x;
    resolution.y = VideoMode::getDesktopMode().size.y;

    // Create a video mode object
    videoMode = VideoMode(Vector2u(resolution.x, resolution.y));

    // Create and open a window for the game
    window = RenderWindow(videoMode, "Themed Horror Jam 21 Game", State::Fullscreen);

    // Initialize SFML View for both the main menu and player
    mainMenuView = View(sf::FloatRect(Vector2f(0, 0), Vector2f(resolution.x, resolution.y)));
    playerView = View(sf::FloatRect(Vector2f(0, 0), Vector2f(resolution.x, resolution.y)));

    // Initialize the menu
    gameMenu.Initialize(resolution);

    // Delta time for smooth movement
    Clock deltaClock;

    while (window.isOpen())
    {
        float deltaTime = deltaClock.restart().asSeconds();
        // do not chage
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // Handle different game states
        switch (currentState)
        {
        case GameState::Menu:
            UpdateMenu(deltaTime);
            RenderMenu();
            break;

        case GameState::Playing:
            UpdateGame(deltaTime);
            RenderGame();
            break;

        case GameState::Paused:
            UpdateMenu(deltaTime);
            RenderMenu();
            break;
        }

        // Display
        window.display();
    }
}

void Engine::InitializeGame()
{
    // Initialize or reset game variables
    if (inputCooldown != INPUT_DELAY) inputCooldown = INPUT_DELAY;
    if (currentDialogueIndex != 0) currentDialogueIndex = 0;
    if (hideDialogue != false) hideDialogue = false;
    if (typeTextTime != 0.0f) typeTextTime = 0.0f;
    if (skippedTypewriting != false) skippedTypewriting = false;

    // Create game objects only when starting the game
    if (!player)
    {
        player = new Player();
        player->Initialize("Art Assets/Player.png",
            Vector2f(resolution.x / 2.0f, resolution.y / 2.0f),
            Vector2f(2.0f, 2.0f));
    }

    if (!initialText)
    {
        initialText = new Game::Text();
        initialText->InitializeText("Fonts/Roboto-Regular.ttf", "Hello", 50.0f, true, false, sf::Color::White,
            sf::Vector2f(resolution.x / 2.0f, (resolution.y - resolution.y + 50.0f)));
    }

    // Initialize dialogue panel if isn't already
    if (!dialoguePanel)
    {
        dialoguePanel = new DialoguePanel();

        float outlineThickness = 4.0f;

        dialoguePanel->InitializeDialoguePanel(Vector2f(resolution.x / resolution.x * outlineThickness, 
            resolution.y - 300.0f), Vector2f(resolution.x - 10.0f, 295.0f), Color::Red, Color::Blue, outlineThickness);
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

    dialogueTexts[0]->InitializeText("Fonts/Roboto-Regular.ttf", "Welcome to game!", 50.0f, false, false,
        sf::Color::White, Vector2f(resolution.x / 2.0f, resolution.y / 2.0f));

    dialogueTexts[1]->InitializeText("Fonts/Roboto-Regular.ttf", "Play!", 50.0f, false, false,
        sf::Color::White, Vector2f(resolution.x / 2.0f, resolution.y / 2.0f));

    dialogueTexts[2]->InitializeText("Fonts/Roboto-Regular.ttf", "Go!", 50.0f, false, false,
        sf::Color::White, Vector2f(resolution.x / 2.0f, resolution.y / 2.0f));
}

void Engine::UpdateMenu(float deltaTime)
{
    Vector2i mousePixelPos = Mouse::getPosition(window);  // Get pixel coordinates
    Vector2f mousePos = window.mapPixelToCoords(mousePixelPos);  // Convert to world coordinat

    MenuAction action = gameMenu.Update(deltaTime, mousePos);

    switch (action)
    {
    case MenuAction::StartGame:
        InitializeGame();
        currentState = GameState::Playing;
        break;

    case MenuAction::ExitGame:
        window.close();
        break;

    case MenuAction::BackToMain:
        if (currentState == GameState::Paused)
        {
            currentState = GameState::Playing;
        }
        break;

    default:
        // No action needed
        break;
    }
}

void Engine::UpdateGame(float deltaTime)
{
    UpdateText(deltaTime, 0.2f);

    // Update input cooldown
    if (inputCooldown > 0.0f)
    {
        inputCooldown -= deltaTime;
    }

    // Check for escape key to pause/return to menu
    if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
    {
        currentState = GameState::Paused;
        gameMenu.ResetToMainMenu();
        return;
    }

    // Press enter key to skip dialogue
    if (Keyboard::isKeyPressed(Keyboard::Key::Enter) && inputCooldown <= 0.0f && currentDialogueIndex >= 0
        && !dialogueTexts.empty() && !skippedTypewriting)
    {
        skippedTypewriting = true;

        inputCooldown = INPUT_DELAY;
    }

    // Press enter key to go to next dialogue
    else if (Keyboard::isKeyPressed(Keyboard::Key::Enter) && inputCooldown <= 0.0f && currentDialogueIndex >= 0
        && !dialogueTexts.empty() && skippedTypewriting)
    {
        skippedTypewriting = false;

        if (typeTextTime != 0.0f) typeTextTime = 0.0f;
        currentDialogueIndex += 1;

        // Hide the dialogue for now once the end dialogue is reached
        if (currentDialogueIndex > dialogueTexts.size() - 1) 
        { 
            currentDialogueIndex = dialogueTexts.size() - 1; 
            hideDialogue = true;
        }

        inputCooldown = INPUT_DELAY;
    }

    // Move the dialogue panel more on the left and bottom sides of the screen than dialogue texts themselves
    if (dialoguePanel)
    {
        dialoguePanel->SetPosition(Vector2f(player->getPosition().x - 955.0f, player->getPosition().y + 235.0f));
    }

    // Move the dialogue texts on the left and bottom sides of the screen
    if (dialogueTexts[currentDialogueIndex] && !hideDialogue)
    {
        dialogueTexts[currentDialogueIndex]->SetTextPosition(Vector2f(player->getPosition().x - 900.0f,
            player->getPosition().y + 300.0f));
    }

    // Update game objects
    if (player)
    {
        playerView.setCenter(player->getPosition()); // Center the camera where the player position's is
        player->Update(deltaTime);
    }
}

void Engine::RenderMenu()
{
    gameMenu.Render(window);
    window.setView(mainMenuView); // Make sure the window is set to the main menu view
}

void Engine::RenderGame()
{
    // Clear window
    window.clear();

    // Draw game objects
    if (player)
    {
        player->Draw(window);
        window.setView(playerView); // Make sure the window is set to the player view in-game
    }
    
    // Only render the dialogue texts and panel when hide dialogue is false
    if (dialogueTexts[currentDialogueIndex] && !hideDialogue)
    {
        if (dialoguePanel)
        {
            dialoguePanel->DrawDialoguePanel(window);
        }

        window.draw(dialogueTexts[currentDialogueIndex]->LoadText());
    }

    // Otherwise, delete the dialogue texts and hide the dialogue panel
    else if (hideDialogue)
    {
        for (int i = 0; i < dialogueTexts.size(); i++)
        {
            delete dialogueTexts[i];
            dialogueTexts[i] = nullptr;
        }
    }
    
    // Uncomment to draw text
    // if (initialText)
    // {
    //     window.draw(initialText->LoadText());
    // }
}

void Engine::UpdateText(float deltaTime, float maxTimerThreshold)
{
    // Update the text when the dialogue isn't hidden yet
    if (!hideDialogue)
    {
        // First dialogue
        if (currentDialogueIndex == 0)
        {
            typeTextTime += deltaTime;

            if (!skippedTypewriting)
            {
                if (typeTextTime >= 0.0f && typeTextTime < maxTimerThreshold)
                {
                    if (dialogueTexts[0]->GetString() != "W")
                        dialogueTexts[0]->SetTypewriterString("W");
                }

                else if (typeTextTime >= maxTimerThreshold && typeTextTime < maxTimerThreshold * 2)
                {
                    if (dialogueTexts[0]->GetString() != "We")
                        dialogueTexts[0]->SetTypewriterString("We");
                }

                else if (typeTextTime >= maxTimerThreshold * 2 && typeTextTime < maxTimerThreshold * 3)
                {
                    if (dialogueTexts[0]->GetString() != "Wel")
                        dialogueTexts[0]->SetTypewriterString("Wel");
                }

                else if (typeTextTime >= maxTimerThreshold * 3 && typeTextTime < maxTimerThreshold * 4)
                {
                    if (dialogueTexts[0]->GetString() != "Welc")
                        dialogueTexts[0]->SetTypewriterString("Welc");
                }

                else if (typeTextTime >= maxTimerThreshold * 4 && typeTextTime < maxTimerThreshold * 5)
                {
                    if (dialogueTexts[0]->GetString() != "Welco")
                        dialogueTexts[0]->SetTypewriterString("Welco");
                }

                else if (typeTextTime >= maxTimerThreshold * 5 && typeTextTime < maxTimerThreshold * 6)
                {
                    if (dialogueTexts[0]->GetString() != "Welcom")
                        dialogueTexts[0]->SetTypewriterString("Welcom");
                }

                else if (typeTextTime >= maxTimerThreshold * 5 && typeTextTime < maxTimerThreshold * 6)
                {
                    if (dialogueTexts[0]->GetString() != "Welcome")
                        dialogueTexts[0]->SetTypewriterString("Welcome");
                }

                else if (typeTextTime >= maxTimerThreshold * 6 && typeTextTime < maxTimerThreshold * 7)
                {
                    if (dialogueTexts[0]->GetString() != "Welcome ")
                        dialogueTexts[0]->SetTypewriterString("Welcome ");
                }

                else if (typeTextTime >= maxTimerThreshold * 7 && typeTextTime < maxTimerThreshold * 8)
                {
                    if (dialogueTexts[0]->GetString() != "Welcome t")
                        dialogueTexts[0]->SetTypewriterString("Welcome t");
                }

                else if (typeTextTime >= maxTimerThreshold * 8 && typeTextTime < maxTimerThreshold * 9)
                {
                    if (dialogueTexts[0]->GetString() != "Welcome to")
                        dialogueTexts[0]->SetTypewriterString("Welcome to");
                }

                else if (typeTextTime >= maxTimerThreshold * 9 && typeTextTime < maxTimerThreshold * 10)
                {
                    if (dialogueTexts[0]->GetString() != "Welcome to ")
                        dialogueTexts[0]->SetTypewriterString("Welcome to ");
                }

                else if (typeTextTime >= maxTimerThreshold * 10 && typeTextTime < maxTimerThreshold * 11)
                {
                    if (dialogueTexts[0]->GetString() != "Welcome to g")
                        dialogueTexts[0]->SetTypewriterString("Welcome to g");
                }

                else if (typeTextTime >= maxTimerThreshold * 11 && typeTextTime < maxTimerThreshold * 12)
                {
                    if (dialogueTexts[0]->GetString() != "Welcome to ga")
                        dialogueTexts[0]->SetTypewriterString("Welcome to ga");
                }

                else if (typeTextTime >= maxTimerThreshold * 12 && typeTextTime < maxTimerThreshold * 13)
                {
                    if (dialogueTexts[0]->GetString() != "Welcome to gam")
                        dialogueTexts[0]->SetTypewriterString("Welcome to gam");
                }

                else if (typeTextTime >= maxTimerThreshold * 13 && typeTextTime < maxTimerThreshold * 14)
                {
                    if (dialogueTexts[0]->GetString() != "Welcome to game")
                        dialogueTexts[0]->SetTypewriterString("Welcome to game");
                }

                else if (typeTextTime >= maxTimerThreshold * 14 && typeTextTime < maxTimerThreshold * 15)
                {
                    if (dialogueTexts[0]->GetString() != "Welcome to game!")
                        dialogueTexts[0]->SetTypewriterString("Welcome to game!");

                    if (!skippedTypewriting) skippedTypewriting = true;
                }
            }

            else
            {
                if (dialogueTexts[0]->GetString() != "Welcome to game!")
                    dialogueTexts[0]->SetTypewriterString("Welcome to game!");
            }
        }

        // Second dialogue
        if (currentDialogueIndex == 1)
        {
            typeTextTime += deltaTime;

            if (!skippedTypewriting)
            {
                if (typeTextTime >= 0.0f && typeTextTime < maxTimerThreshold)
                {
                    if (dialogueTexts[1]->GetString() != "P")
                        dialogueTexts[1]->SetTypewriterString("P");
                }

                else if (typeTextTime >= maxTimerThreshold && typeTextTime < maxTimerThreshold * 2)
                {
                    if (dialogueTexts[1]->GetString() != "Pl")
                        dialogueTexts[1]->SetTypewriterString("Pl");
                }

                else if (typeTextTime >= maxTimerThreshold * 2 && typeTextTime < maxTimerThreshold * 3)
                {
                    if (dialogueTexts[1]->GetString() != "Pla")
                        dialogueTexts[1]->SetTypewriterString("Pla");
                }

                else if (typeTextTime >= maxTimerThreshold * 3 && typeTextTime < maxTimerThreshold * 4)
                {
                    if (dialogueTexts[1]->GetString() != "Play")
                        dialogueTexts[1]->SetTypewriterString("Play");
                }

                else if (typeTextTime >= maxTimerThreshold * 4 && typeTextTime < maxTimerThreshold * 5)
                {
                    if (dialogueTexts[1]->GetString() != "Play!")
                        dialogueTexts[1]->SetTypewriterString("Play!");

                    if (!skippedTypewriting) skippedTypewriting = true;
                }
            }

            else
            {
                if (dialogueTexts[1]->GetString() != "Play!")
                    dialogueTexts[1]->SetTypewriterString("Play!");
            }
        }

        // Third dialogue
        if (currentDialogueIndex == 2)
        {
            typeTextTime += deltaTime;

            if (!skippedTypewriting)
            {
                if (typeTextTime >= 0.0f && typeTextTime < maxTimerThreshold)
                {
                    if (dialogueTexts[2]->GetString() != "G")
                        dialogueTexts[2]->SetTypewriterString("G");
                }

                else if (typeTextTime >= maxTimerThreshold && typeTextTime < maxTimerThreshold * 2)
                {
                    if (dialogueTexts[2]->GetString() != "Go")
                        dialogueTexts[2]->SetTypewriterString("Go");
                }

                else if (typeTextTime >= maxTimerThreshold * 2 && typeTextTime < maxTimerThreshold * 3)
                {
                    if (dialogueTexts[2]->GetString() != "Go!")
                        dialogueTexts[2]->SetTypewriterString("Go!");

                    if (!skippedTypewriting) skippedTypewriting = true;
                }
            }

            else
            {
                if (dialogueTexts[2]->GetString() != "Go!")
                    dialogueTexts[2]->SetTypewriterString("Go!");
            }
        }
    }
}

void Engine::DeleteEngineInstance()
{
    delete engineInstance;
    engineInstance = nullptr;
}