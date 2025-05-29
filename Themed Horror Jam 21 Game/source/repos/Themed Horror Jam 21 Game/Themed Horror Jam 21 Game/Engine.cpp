#include "Engine.h"

Engine* Engine::engineInstance = nullptr;

Engine::Engine()
    : currentState(GameState::Menu)

    , initialText(nullptr)
    , currentDialogueIndex(0)
    , hideDialogue(false)
    , skippedTypewriting(false)
    , inputCooldown(INPUT_DELAY)
{
}

Engine::~Engine()
{

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

    // Initialize the background image
    if (!gameBackground.IsLoaded())
    {
        // Replace "Art Assets/Background.png" with your actual background image path
        gameBackground.Initialize("Art Assets/Background.jpg", resolution);
    }

    // Create game objects only when starting the game
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

        float panelWidth = resolution.x - 100.0f; // Leave some margin on sides
        float panelHeight = 700.0f; // Reasonable height for dialogue

        // Center the panel horizontally and position at bottom
        float panelX = (resolution.x - panelWidth) / 2.0f;
        float panelY = resolution.y - panelHeight - 50.0f; // 50px from bottom

        dialoguePanel->InitializeDialoguePanel("Art Assets/chat_box_0.png", Vector2f(panelX, panelY),
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
        typewriterEffect.Reset(); // For now, reset dialogue state when going back to the main menu
        gameMenu.ResetToMainMenu();
        return;
    }

    // Move the dialogue texts on the left and bottom sides of the screen
    if (currentDialogueIndex < dialogueTexts.size() && dialogueTexts[currentDialogueIndex] && !hideDialogue)
    {
        // Position text in the center of the dialogue panel
        float textX = (resolution.x / 2.0f) - 890; // Center horizontally
        float textY = resolution.y - 335.0f; // Position within the dialogue panel area

        dialogueTexts[currentDialogueIndex]->SetTextPosition(Vector2f(textX, textY));
    }

    if (!hideDialogue)
    {
        typewriterEffect.Update(deltaTime);

        // Update the current dialogue text
        if (currentDialogueIndex >= 0 && currentDialogueIndex < static_cast<int>(dialogueTexts.size())
            && dialogueTexts[currentDialogueIndex])
        {
            std::string currentText = typewriterEffect.GetCurrentText();
            dialogueTexts[currentDialogueIndex]->SetTypewriterString(sf::String(currentText));
        }
    }

    // SIMPLIFIED: Handle input for skipping/advancing dialogue (REMOVE the old system)
    if (Keyboard::isKeyPressed(Keyboard::Key::Enter) && inputCooldown <= 0.0f && !hideDialogue)
    {
        if (!typewriterEffect.IsCurrentDialogueComplete())
        {
            // Skip current dialogue typing
            typewriterEffect.Skip();
        }
        else if (typewriterEffect.HasNextDialogue())
        {
            // Move to next dialogue
            typewriterEffect.NextDialogue();
            currentDialogueIndex = typewriterEffect.GetCurrentDialogueIndex(); // Sync indices
        }
        else
        {
            // End dialogue system
            hideDialogue = true;
        }

        inputCooldown = INPUT_DELAY;
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

    // Draw the background FIRST (so it appears behind everything else)
    gameBackground.Draw(window);
    
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
    
 
}
void Engine::InitializeDialogueSystem()
{
    std::vector<TypewriterEffect::DialogueData> allDialogueData = {
         TypewriterEffect::DialogueData("Welcome to game!", 8.0f),  // Fast typing
         TypewriterEffect::DialogueData("Play!", 12.0f),           // Very fast
         TypewriterEffect::DialogueData("Go!", 10.0f)              // Fast
    };

    // Initialize with ALL dialogues at once
    typewriterEffect.Initialize(allDialogueData);
}

void Engine::DeleteEngineInstance()
{
    delete engineInstance;
    engineInstance = nullptr;
}