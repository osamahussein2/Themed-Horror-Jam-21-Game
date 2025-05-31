#ifndef ENGINE_H
#define ENGINE_H
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "Text.h"
#include "DialoguePanel.h"
#include "TypewriterEffect.h"
#include "RenderBackground.h"
#include "SceneManager.h"

using namespace sf;

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

	View GetMainMenuView();
	void SetMainMenuView(Vector2f position, Vector2f scale);

	// Scene management
	SceneManager* GetSceneManager() { return &sceneManager; }

private:
	static Engine* engineInstance;

	// Core systems
	SceneManager sceneManager;

	// Create a video mode object
	VideoMode videoMode;
	Vector2u resolution;
	// Create and open a window for the game
	RenderWindow window;

	// Create a an SFML View for main menu and for following player around in-game
	View mainMenuView, playerView;

	Engine();
	void InitializeScenes();
};
#endif