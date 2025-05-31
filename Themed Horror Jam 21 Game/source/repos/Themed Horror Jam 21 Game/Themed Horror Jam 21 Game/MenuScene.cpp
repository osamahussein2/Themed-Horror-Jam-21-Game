#include "MenuScene.h"
#include "Engine.h"
#include "SceneManager.h"

MenuScene::MenuScene()
{
}

MenuScene::~MenuScene()
{
}

void MenuScene::Initialize()
{
    resolution = Engine::Instance()->GetResolution();
    mainMenuView = View(sf::FloatRect(Vector2f(0, 0), Vector2f(resolution.x, resolution.y)));
    gameMenu.Initialize(resolution);
}

void MenuScene::Update(float deltaTime)
{
    Vector2i mousePixelPos = Mouse::getPosition(*Engine::Instance()->GetWindow());
    Vector2f mousePos = Engine::Instance()->GetWindow()->mapPixelToCoords(mousePixelPos);

    MenuAction action = gameMenu.Update(deltaTime, mousePos);

    switch (action)
    {
    case MenuAction::StartGame:
        sceneManager->ChangeScene("Game");
        break;

    case MenuAction::ExitGame:
        Engine::Instance()->GetWindow()->close();
        break;

    case MenuAction::BackToMain:
        gameMenu.ResetToMainMenu();
        break;

    default:
        break;
    }
}

void MenuScene::Render(RenderWindow& window)
{
    window.setView(mainMenuView);
    gameMenu.Render(window);
}

void MenuScene::Cleanup()
{
}

void MenuScene::OnEnter()
{
    gameMenu.ResetToMainMenu();
}

void MenuScene::OnExit()
{
}

std::string MenuScene::GetSceneName() const
{
    return "Menu";
}