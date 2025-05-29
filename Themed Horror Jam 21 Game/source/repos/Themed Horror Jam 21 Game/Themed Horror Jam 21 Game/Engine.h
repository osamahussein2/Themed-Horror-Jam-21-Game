#ifndef ENGINE_H
#define ENGINE_H
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "Player.h"
#include "Text.h"
#include "DialoguePanel.h"
#include "TypewriterEffect.h"
#include "RenderBackground.h"

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

	Vector2u GetResolution() { return resolution; }
	void SetResolution(Vector2u newResolution) { resolution = newResolution; }

private:
	RenderBackground gameBackground;
	TypewriterEffect typewriterEffect;
	bool dialogueSystemInitialized;
	static Engine* engineInstance;
	// Create a video mode object
	VideoMode videoMode;
	Vector2u resolution;
	// Create and open a window for the game
	RenderWindow window;

	// Game state management
	GameState currentState;

	// Create a an SFML View for main menu and for following player around in-game
	View mainMenuView, playerView;

	// Menu system
	Menu gameMenu;

	// Game objects
	Player* player;
	Game::Text* initialText;

	std::vector<Game::Text*> dialogueTexts;

	float typeTextTime;

	DialoguePanel* dialoguePanel;

	// Input handling
	float inputCooldown;
	const float INPUT_DELAY = 0.15f;

	Engine();
	void InitializeGame();
	void UpdateMenu(float deltaTime);
	void UpdateGame(float deltaTime);
	void RenderMenu();
	void RenderGame();
	void InitializeDialogueSystem();
	int maxDialogueTexts = 3;

	bool hideDialogue;
	bool skippedTypewriting;

	int currentDialogueIndex;
};
#endif