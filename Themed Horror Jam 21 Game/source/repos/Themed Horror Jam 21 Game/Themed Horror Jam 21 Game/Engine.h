#ifndef ENGINE_H
#define ENGINE_H
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "Player.h"
#include "Text.h"

using namespace sf;

enum class GameState
{
	Menu,
	Playing,
	Paused
};

class Engine
{
public:
	~Engine();
	void RunEngine();
	void DeleteEngineInstance();
	static Engine* Instance();
	RenderWindow* GetWindow() { return &window; }
private:
	static Engine* engineInstance;
	// Create a video mode object
	VideoMode videoMode;
	Vector2f resolution;
	// Create and open a window for the game
	RenderWindow window;

	// Game state management
	GameState currentState;

	// Menu system
	Menu gameMenu;

	// Game objects
	Player* player;
	Game::Text* initialText;

	Engine();
	void InitializeGame();
	void UpdateMenu(float deltaTime);
	void UpdateGame(float deltaTime);
	void RenderMenu();
	void RenderGame();
};
#endif