#include "Engine.h"
#include "Player.h"
#include "Text.h"

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

    // Create player object
    Player player;

    // Setup text here
    Game::Text initialText;

    // Initialize the player
    player.Initialize("Art Assets/RedImage.png",
        Vector2f(resolution.x / 2.0f, resolution.y / 2.0f),
        Vector2f(2.0f, 2.0f));

    // Initialize the text objects from the Game's namespace here (to prevent conflict with SFML's text object)
    initialText.InitializeText("Fonts/Roboto-Regular.ttf", "Hello", 50.0f, true, sf::Color::White,
        sf::Vector2f(resolution.x / 2.0f, (resolution.y - resolution.y + 50.0f)));

    // Delta time for smooth movement
    Clock deltaClock;

    while (window.isOpen())
    {
        float deltaTime = deltaClock.restart().asSeconds();

        if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
        {
            window.close();
        }

        // Update player
        player.Update(deltaTime);

        // Clear window
        window.clear();

        // Draw our game scene here
        player.Draw(window);
        window.draw(initialText.LoadText()); // Draw text to the window

        // Display
        window.display();
    }
}

void Engine::DeleteEngineInstance()
{
    delete engineInstance;
    engineInstance = nullptr;
}