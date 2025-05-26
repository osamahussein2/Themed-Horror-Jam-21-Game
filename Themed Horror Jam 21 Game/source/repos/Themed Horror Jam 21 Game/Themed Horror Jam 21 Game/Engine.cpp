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

	// Base rectangles for each direction (idle frames)
	IntRect baseDir[4];

	baseDir[down] = IntRect({ 0, 38 * 0 }, { 19, 38 });  // Row 0  
	baseDir[left] = IntRect({ 0, 38 * 1 }, { 19, 38 });  // Row 1
	baseDir[right] = IntRect({ 0, 38 * 2 }, { 19, 38 }); // Row 2
    baseDir[up] = IntRect({ 0, 38 * 3 }, { 19, 38 });    // Row 3

	redSprite.setTextureRect(baseDir[down]);
	redSprite.setOrigin({ 9.5f, 19.0f }); // Center origin based on 19x38 size
	redSprite.setPosition({ resolution.x / 2.0f, resolution.y / 2.0f });
	redSprite.setScale({ 2.0f, 2.0f });
	

	// Animation variables
	int currentDirection = down;
	int animationFrame = 0;
	const int maxFrames = 4; // Assuming 4 animation frames per direction

	// Timing for animation
	Clock animationClock;
	float animationTime = 0.1f; // Time between frames (in seconds)

	// Delta time for smooth movement
	Clock deltaClock;


	while (window.isOpen())
	{
		float deltaTime = deltaClock.restart().asSeconds();

		if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
		{
			window.close();
		}

        // Movement and animation
        bool isMoving = false;
        Vector2f movement(0, 0);

        if (Keyboard::isKeyPressed(Keyboard::Key::Up) || Keyboard::isKeyPressed(Keyboard::Key::W))
        {
            movement.y = -speed * deltaTime;
            currentDirection = up;
            isMoving = true;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Key::Down) || Keyboard::isKeyPressed(Keyboard::Key::S))
        {
            movement.y = speed * deltaTime;
            currentDirection = down;
            isMoving = true;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Key::Left) || Keyboard::isKeyPressed(Keyboard::Key::A))
        {
            movement.x = -speed * deltaTime;
            currentDirection = left;
            isMoving = true;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Key::Right) || Keyboard::isKeyPressed(Keyboard::Key::D))
        {
            movement.x = speed * deltaTime;
            currentDirection = right;
            isMoving = true;
        }

        // Apply movement
        if (isMoving)
        {
            redSprite.move(movement);

            // Update animation frame
            if (animationClock.getElapsedTime().asSeconds() >= animationTime)
            {
                animationFrame++;
                if (animationFrame >= maxFrames)
                {
                    animationFrame = 0;
                }
                animationClock.restart();
            }

            // Set texture rectangle for current animation frame
            IntRect currentFrame;
            if (currentDirection == up)
            {
                currentFrame = IntRect({ 19 * animationFrame, 38 * 3}, { 19, 38 }); // Row 4 for up walk
            }
            else if (currentDirection == down)
            {
                currentFrame = IntRect({ 19 * animationFrame, 38 * 0 }, { 19, 38 }); // Row 5 for down walk
            }
            else if (currentDirection == left)
            {
                currentFrame = IntRect({ 19 * animationFrame, 38 *1 }, { 19, 38 }); // Row 6 for left walk
            }
            else if (currentDirection == right)
            {
                currentFrame = IntRect({ 19 * animationFrame, 38 * 2 }, { 19, 38 }); // Row 7 for right walk
            }

            redSprite.setTextureRect(currentFrame);
        }
        else
        {
            // Not moving - show idle frame
            redSprite.setTextureRect(baseDir[currentDirection]);
            animationFrame = 0; // Reset animation when not moving
        }

        window.clear();
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