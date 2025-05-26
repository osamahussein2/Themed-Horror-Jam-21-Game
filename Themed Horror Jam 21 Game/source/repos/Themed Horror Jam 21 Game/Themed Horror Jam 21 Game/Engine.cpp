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

	Sprite redSprite = red.LoadSprite();
	IntRect dir[4];

	for (int i = 0; i < 4; i++)
	{
		dir[i] = IntRect({ 32 * i,0 }, { 32,32 }); // Set the texture rectangle for each direction
	}

	redSprite.setTextureRect(dir[down]);
	redSprite.setOrigin({ 16, 16 }); // Set the origin to the center of the sprite
	redSprite.setPosition({ resolution.x / 2.0f, resolution.y / 2.0f }); // Center the sprite on the screen
	
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

		if (Keyboard::isKeyPressed(Keyboard::Key::Up)|| Keyboard::isKeyPressed(Keyboard::Key::W))
		{
			redSprite.move({ 0, -speed });
			redSprite.setTextureRect(dir[up]);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Key::Down) || Keyboard::isKeyPressed(Keyboard::Key::S))
		{
			redSprite.move({ 0,speed });
			redSprite.setTextureRect(dir[down]);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Key::Left) || Keyboard::isKeyPressed(Keyboard::Key::A))
		{
			redSprite.move({-speed, 0});
			redSprite.setTextureRect(dir[left]);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Key::Right)|| Keyboard::isKeyPressed(Keyboard::Key::D))
		{
			redSprite.move({ speed, 0 });
			redSprite.setTextureRect(dir[right]);
		}

		// Draw our game scene here (and load the sprite texture objects here as well)
		window.draw(redSprite);

		// Display
		window.display();
	}
}


void Engine::DeleteEngineInstance()
{
	delete engineInstance;
	engineInstance = nullptr;
}