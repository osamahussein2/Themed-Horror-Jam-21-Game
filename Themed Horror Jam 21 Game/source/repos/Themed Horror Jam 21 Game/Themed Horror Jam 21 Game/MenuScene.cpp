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

    mainMenuAudio.InitializeAudio("Audio/Music/videogame2_horror_5_theme principal.mp3", true);

    // Make sure the main menu audio is equal to the volume variable set in the options menu
    if (mainMenuAudio.GetVolume() != Menu::GetVolume())
    {
        mainMenuAudio.SetVolume(Menu::GetVolume());
    }
}

void MenuScene::Update(float deltaTime)
{
    Vector2i mousePixelPos = Mouse::getPosition(*Engine::Instance()->GetWindow());
    Vector2f mousePos = Engine::Instance()->GetWindow()->mapPixelToCoords(mousePixelPos);

    MenuAction action = gameMenu.Update(deltaTime, mousePos);

    // Make sure the main menu audio is equal to the volume variable set in the options menu
    if (mainMenuAudio.GetVolume() != Menu::GetVolume())
    {
        mainMenuAudio.SetVolume(Menu::GetVolume());
    }

    // If the main menu music isn't playing, play it and set the bool to true
    if (!musicPlaying)
    {
        mainMenuAudio.PlayAudio();
        musicPlaying = true;
    }

    switch (action)
    {
    case MenuAction::StartGame:

        // If the main menu music is playing, stop the music and set the bool to false
        if (musicPlaying)
        {
            mainMenuAudio.StopAudio();
            musicPlaying = false;
        }

        if (Menu::nextDayUnlocked != false) Menu::nextDayUnlocked = false;
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