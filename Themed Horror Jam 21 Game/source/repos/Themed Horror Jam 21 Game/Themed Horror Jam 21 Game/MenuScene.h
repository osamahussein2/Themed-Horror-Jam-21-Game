#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "Scene.h"
#include "Menu.h"

class MenuScene : public Scene
{
public:
    MenuScene();
    ~MenuScene() override;

    void Initialize() override;
    void Update(float deltaTime) override;
    void Render(RenderWindow& window) override;
    void Cleanup() override;
    void OnEnter() override;
    void OnExit() override;
    std::string GetSceneName() const override;

private:
    Menu gameMenu;
    Vector2u resolution;
    View mainMenuView;
};

#endif