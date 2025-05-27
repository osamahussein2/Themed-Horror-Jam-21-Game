#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include "Text.h"
#include <vector>

using namespace sf;

enum class MenuState
{
    MainMenu,
    Settings,
    About
};

enum class MenuAction
{
    None,
    StartGame,
    ExitGame,
    GoToSettings,
    GoToAbout,
    BackToMain
};

class Menu
{
public:
    Menu();
    ~Menu();

    // Initialize the menu with screen resolution
    void Initialize(Vector2f screenResolution);

    // Update menu logic
    MenuAction Update(float deltaTime);

    // Render the menu
    void Render(RenderWindow& window);

    // Get current menu state
    MenuState GetCurrentState() const { return currentState; }

    // Reset to main menu
    void ResetToMainMenu();

private:
    // Menu state management
    MenuState currentState;

    // Main menu variables
    int selectedMainOption;
    int maxMainOptions;

    // Input handling
    float inputCooldown;
    const float INPUT_DELAY = 0.15f;

    // Screen resolution
    Vector2f resolution;

    // Text objects for main menu
    Game::Text titleText;
    std::vector<Game::Text> mainMenuTexts;
    Game::Text instructionsText;

    // Text objects for sub-menus
    Game::Text aboutContentText;
    Game::Text settingsContentText;
    Game::Text backInstructionText;

    // Colors
    Color selectedColor;
    Color normalColor;
    Color titleColor;

    // Private methods
    void CreateMainMenuTexts();
    void CreateAboutText();
    void CreateSettingsText();
    void UpdateMainMenuColors();
    MenuAction HandleMainMenuInput();
    MenuAction HandleSubMenuInput();

    // Input state tracking
    bool wasUpPressed;
    bool wasDownPressed;
    bool wasEnterPressed;
    bool wasEscapePressed;
};

#endif