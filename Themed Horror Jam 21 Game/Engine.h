#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics.hpp>

using namespace sf;

class Engine
{
public:
	~Engine();

	void RunEngine();
	void DeleteEngineInstance();

	static Engine* Instance();

private:
	static Engine* engineInstance;

	// Create a video mode object
	VideoMode videoMode;

	Vector2f resolution;

	// Create and open a window for the game
	RenderWindow window;

	Engine();
};

#endif