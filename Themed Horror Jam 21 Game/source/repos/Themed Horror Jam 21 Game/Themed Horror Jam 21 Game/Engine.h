#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics.hpp>

using namespace sf;
enum  dir
{
	down, left, right,up
};
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
	dir direction[4];
	float speed = 200.0f;
	Engine();
};

#endif

