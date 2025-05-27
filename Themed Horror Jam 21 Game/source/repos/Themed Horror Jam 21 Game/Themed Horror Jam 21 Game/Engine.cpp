#include "Engine.h"

Engine* Engine::engineInstance = nullptr;

Engine::Engine()
    : currentState(GameState::Menu)
    , player(nullptr)
    , initialText(nullptr)
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

    // Initialize the menu
    gameMenu.Initialize(resolution);

    // Delta time for smooth movement
    Clock deltaClock;

    while (window.isOpen())
    {
        float deltaTime = deltaClock.restart().asSeconds();
        //// Handle window close event
        // this is bugged I don't know why 
        //Event event;
        //while (window.pollEvent(event))
        //{
        //    if (event.type == Event::Closed)
        //    {
        //        window.close();
        //    }
        //}

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
    // Create game objects only when starting the game
    if (!player)
    {
        player = new Player();
        player->Initialize("Art Assets/RedImage.png",
            Vector2f(resolution.x / 2.0f, resolution.y / 2.0f),
            Vector2f(2.0f, 2.0f));
    }

    if (!initialText)
    {
        initialText = new Game::Text();
        initialText->InitializeText("Fonts/Roboto-Regular.ttf", "Hello", 50.0f, true, sf::Color::White,
            sf::Vector2f(resolution.x / 2.0f, (resolution.y - resolution.y + 50.0f)));
    }
}

void Engine::UpdateMenu(float deltaTime)
{
    MenuAction action = gameMenu.Update(deltaTime);
    
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
    // Check for escape key to pause/return to menu
    if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
    {
        currentState = GameState::Paused;
        gameMenu.ResetToMainMenu();
        return;
    }

    // Update game objects
    if (player)
    {
        player->Update(deltaTime);
    }
}

void Engine::RenderMenu()
{
    gameMenu.Render(window);
}

void Engine::RenderGame()
{
    // Clear window
    window.clear();

    // Draw game objects
    if (player)
    {
        player->Draw(window);
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