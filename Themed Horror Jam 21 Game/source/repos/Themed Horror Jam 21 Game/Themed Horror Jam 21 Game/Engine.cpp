#include "Engine.h"

Engine* Engine::engineInstance = nullptr;

Engine::Engine()
    : currentState(GameState::Menu)
    , player(nullptr)
    , initialText(nullptr)
    , currentDialogueIndex(0)
    , hideDialogue(false)
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
        && !dialogueTexts.empty())
    {
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

void Engine::DeleteEngineInstance()
{
    delete engineInstance;
    engineInstance = nullptr;
}