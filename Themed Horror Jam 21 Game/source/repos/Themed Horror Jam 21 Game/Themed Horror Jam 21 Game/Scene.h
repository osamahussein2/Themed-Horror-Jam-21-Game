#ifndef SCENE_H
#define SCENE_H

#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;

class SceneManager; // Forward declaration

class Scene
{
public:
    Scene() = default;
    virtual ~Scene() = default;

    // Pure virtual functions that must be implemented by derived scenes
    virtual void Initialize() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render(RenderWindow& window) = 0;
    virtual void Cleanup() = 0;

    // Optional virtual functions
    virtual void OnEnter() {} // Called when scene becomes active
    virtual void OnExit() {}  // Called when scene becomes inactive

    // Scene identification
    virtual std::string GetSceneName() const = 0;

protected:
    // Reference to scene manager for scene transitions
    SceneManager* sceneManager = nullptr;

public:
    void SetSceneManager(SceneManager* manager) { sceneManager = manager; }
};

#endif
