#include "Engine.h"
#include "SpriteTexture.h"

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

	// Setup sprite texture objects here
	SpriteTexture red;

	// Initialize the sprite texture objects here
	red.InitializeSprite("Art Assets/RedImage.png", sf::Vector2f(0.0f, 0.0f));

	while (window.isOpen())
	{
		if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
		{
			window.close();
		}

		// Update the scene here

		// Draw the scene here

		// Clear everything from the last frame
		window.clear();

		// Draw our game scene here (and load the sprite texture objects here as well)
		window.draw(red.LoadSprite());

		// Display
		window.display();
	}
}

void Engine::DeleteEngineInstance()
{
	delete engineInstance;
	engineInstance = nullptr;
}