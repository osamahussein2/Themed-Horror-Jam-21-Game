#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include "Text.h"
#include "Button.h"  // Add this include
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

enum class SettingsMenuAction
{
    ModifyNone,
    ModifyVolume,
    ModifyDifficulty,
    ModifyFullscreen,
    ModifyResolution
};

class Menu
{
public:
    Menu();
    ~Menu();

    // Initialize the menu with screen resolution
    void Initialize(Vector2u screenResolution);

    // Update menu logic
    MenuAction Update(float deltaTime, Vector2f mousePos);  // Add mousePos parameter

    // Render the menu
    void Render(RenderWindow& window);

    // Get current menu state
    MenuState GetCurrentState() const { return currentState; }

    // Reset to main menu
    void ResetToMainMenu();

    static int GetVolume() { return volume; }
    static std::string GetDifficulty() { return difficulty[currentDifficulty]; }
    static std::string GetFullscreenStatus() { return fullscreenStatus[currentFullscreenStatus]; }
    static Vector2u GetResolutionSize() { return resolutionSize[currentResolutionSize]; }

private:
    // Menu state management
    MenuState currentState;

    // Main menu variables
    int selectedMainOption;
    int maxMainOptions;

    // Settings menu variables
    int selectedSettingsOption;
    int maxSettingsOptions;

    // Input handling
    float inputCooldown;
    const float INPUT_DELAY = 0.15f;

    // Screen resolution
    Vector2u resolution;

    // Text objects for main menu
    Game::Text titleText;
    std::vector<Game::Text> mainMenuTexts;
    std::vector<Game::Text> settingMenuTexts;
    std::vector<Game::Text> settingMenuOptionsTexts;
    Game::Text instructionsText;
    Game::Text settingsInstructionsText;

    // Button objects for main menu
    std::vector<Button> mainMenuButtons;  // Add this
    std::vector<Button> settingMenuButtons;

    // Text objects for sub-menus
    Game::Text aboutContentText;
    Game::Text settingsContentText;
    Game::Text backInstructionText;

 

    // Colors
    Color selectedColor;
    Color modifySettingColor;
    Color normalColor;
    Color titleColor;

    // Button colors
    Color buttonIdleColor;
    Color buttonHoverColor;
    Color buttonActiveColor;

    // Static variables
    static int volume;
    static std::vector<std::string> difficulty;
    static std::vector<std::string> fullscreenStatus;
    static std::vector<Vector2u> resolutionSize;

    static int currentDifficulty;
    static int currentFullscreenStatus;
    static int currentResolutionSize;

    // Private methods
    void CreateMainMenuTexts();
    void CreateMainMenuButtons();  
    void CreateAboutText();
    void CreateSettingsText();
    void CreateSettingsButtons();

    void UpdateMainMenuColors();
    void UpdateSettingsMenuColors();

    void ToggleFullscreen();

    MenuAction HandleMainMenuInput(Vector2f mousePos);  // Add mousePos parameter
    MenuAction HandleSettingsInput(Vector2f mousePos);
    MenuAction HandleSubMenuInput();   // Add mousePos parameter

    // Declared in Resolution.cpp to prevent Menu.cpp from having too much code
    void InitializeScreenResolution();
    void ChangeResolution();

    // Input state tracking
    bool wasUpPressed;
    bool wasDownPressed;
    bool wasLeftPressed;
    bool wasRightPressed;
    bool wasEnterPressed;
    bool wasEscapePressed;

    SettingsMenuAction settingsAction = SettingsMenuAction::ModifyNone;

    Button* settingsBackButton;
    Game::Text settingsBackText;

    const Vector2u NINETEEN_TWENTY_BY_TEN_EIGHTY = Vector2u(1920, 1080); //check
    const Vector2u SIXTEEN_EIGHTY_BY_TEN_FIFTY = Vector2u(1680, 1050); //check
    const Vector2u SIXTEEN_HUNDRED_BY_NINE_HUNDRED = Vector2u(1600, 900); //check
    const Vector2u FOURTEEN_FORTY_BY_NINE_HUNDRED = Vector2u(1440, 900); //check
    const Vector2u FOURTEEN_HUNDRED_BY_TEN_FIFTY = Vector2u(1400, 1050); //check
    const Vector2u THIRTEEN_HUNDRED_SIXTY_SIX_BY_SEVEN_SIXTY_EIGHT = Vector2u(1366, 768); //check
    const Vector2u THIRTEEN_HUNDRED_SIXTY_BY_SEVEN_SIXTY_EIGHT = Vector2u(1360, 768); //check
    const Vector2u TWELEVE_HUNDRED_EIGHTY_BY_TEN_HUNDRED_TWENTY_FOUR = Vector2u(1280, 1024); //check
    const Vector2u TWELEVE_HUNDRED_EIGHTY_BY_NINE_SIXTY = Vector2u(1280, 960); //check
    const Vector2u TWELEVE_HUNDRED_EIGHTY_BY_EIGHT_HUNDRED = Vector2u(1280, 800); //check
    const Vector2u TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_SIXTY_EIGHT = Vector2u(1280, 768); //check
    const Vector2u TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_TWENTY = Vector2u(1280, 720); //check
    const Vector2u TWELEVE_HUNDRED_EIGHTY_BY_SIX_HUNDRED = Vector2u(1280, 600); //check
    const Vector2u ELEVEN_HUNDRED_FIFTY_TWO_BY_EIGHT_SIXTY_FOUR = Vector2u(1152, 864); //check
    const Vector2u TEN_HUNDRED_TWENTY_FOUR_BY_SEVEN_SIXTY_EIGHT = Vector2u(1024, 768); //check
    const Vector2u EIGHT_HUNDRED_BY_SIX_HUNDRED = Vector2u(800, 600); //check
};

#endif