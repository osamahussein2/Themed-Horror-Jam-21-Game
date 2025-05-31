#include "Engine.h"
#include "MenuScene.h"
#include "GameScene.h"
#include <memory>

Engine* Engine::engineInstance = nullptr;

Engine::Engine()
{
}

Engine::~Engine()
{
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

    // Initialize scenes
    InitializeScenes();

    // Delta time for smooth movement
    Clock deltaClock;

    while (window.isOpen())
    {
        float deltaTime = deltaClock.restart().asSeconds();

        // Handle window events
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // Update and render using SceneManager
        sceneManager.Update(deltaTime);
        sceneManager.Render(window);

        // Display
        window.display();
    }
}

void Engine::InitializeScenes()
{
    // Create and add scenes
    auto menuScene = std::make_unique<MenuScene>();
    auto gameScene = std::make_unique<GameScene>();

    sceneManager.AddScene("Menu", std::move(menuScene));
    sceneManager.AddScene("Game", std::move(gameScene));

    // Start with menu scene
    sceneManager.ChangeScene("Menu");
}

View Engine::GetMainMenuView()
{
    return mainMenuView;
}

void Engine::SetMainMenuView(Vector2f position, Vector2f scale)
{
    mainMenuView = View(sf::FloatRect(Vector2f(position), Vector2f(scale)));
}

void Engine::DeleteEngineInstance()
{
    delete engineInstance;
    engineInstance = nullptr;
}