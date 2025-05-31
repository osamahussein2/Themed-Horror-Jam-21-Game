#include "SceneManager.h"
#include <iostream>

SceneManager::SceneManager()
    : currentScene(nullptr)
    , currentSceneName("")
    , sceneChanged(false)
    , nextSceneName("")
{
}

SceneManager::~SceneManager()
{
    // Cleanup current scene
    if (currentScene)
    {
        currentScene->OnExit();
        currentScene->Cleanup();
    }

    // Clear all scenes
    scenes.clear();
}

void SceneManager::AddScene(const std::string& sceneName, std::unique_ptr<Scene> scene)
{
    if (scene)
    {
        scene->SetSceneManager(this);
        scene->Initialize();
        scenes[sceneName] = std::move(scene);

        std::cout << "Added scene: " << sceneName << std::endl;
    }
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
    if (HasScene(sceneName))
    {
        nextSceneName = sceneName;
        sceneChanged = true;
    }
    else
    {
        std::cout << "Warning: Scene '" << sceneName << "' does not exist!" << std::endl;
    }
}

void SceneManager::RemoveScene(const std::string& sceneName)
{
    auto it = scenes.find(sceneName);
    if (it != scenes.end())
    {
        // If we're removing the current scene, clear current scene pointer
        if (currentScene == it->second.get())
        {
            currentScene->OnExit();
            currentScene->Cleanup();
            currentScene = nullptr;
            currentSceneName = "";
        }

        scenes.erase(it);
        std::cout << "Removed scene: " << sceneName << std::endl;
    }
}

void SceneManager::Update(float deltaTime)
{
    // Process scene change if needed
    if (sceneChanged)
    {
        ProcessSceneChange();
    }

    // Update current scene
    if (currentScene)
    {
        currentScene->Update(deltaTime);
    }
}

void SceneManager::Render(RenderWindow& window)
{
    if (currentScene)
    {
        currentScene->Render(window);
    }
}

std::string SceneManager::GetCurrentSceneName() const
{
    return currentSceneName;
}

Scene* SceneManager::GetCurrentScene() const
{
    return currentScene;
}

bool SceneManager::HasScene(const std::string& sceneName) const
{
    return scenes.find(sceneName) != scenes.end();
}

void SceneManager::ProcessSceneChange()
{
    if (!nextSceneName.empty() && HasScene(nextSceneName))
    {
        // Exit current scene
        if (currentScene)
        {
            currentScene->OnExit();
        }

        // Change to new scene
        currentScene = scenes[nextSceneName].get();
        currentSceneName = nextSceneName;

        // Enter new scene
        if (currentScene)
        {
            currentScene->OnEnter();
        }

        std::cout << "Changed to scene: " << currentSceneName << std::endl;
    }

    sceneChanged = false;
    nextSceneName = "";
}