#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <memory>
#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>
#include "Scene.h"

using namespace sf;

class SceneManager
{
public:
    SceneManager();
    ~SceneManager();

    // Scene management
    void AddScene(const std::string& sceneName, std::unique_ptr<Scene> scene);
    void ChangeScene(const std::string& sceneName);
    void RemoveScene(const std::string& sceneName);

    // Update and render current scene
    void Update(float deltaTime);
    void Render(RenderWindow& window);

    // Get current scene info
    std::string GetCurrentSceneName() const;
    Scene* GetCurrentScene() const;

    // Check if scene exists
    bool HasScene(const std::string& sceneName) const;

private:
    std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;
    Scene* currentScene;
    std::string currentSceneName;
    bool sceneChanged;
    std::string nextSceneName;

    void ProcessSceneChange();
};

#endif