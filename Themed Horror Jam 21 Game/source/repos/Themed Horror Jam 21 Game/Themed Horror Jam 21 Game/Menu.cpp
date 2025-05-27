#include "Menu.h"

Menu::Menu()
    : currentState(MenuState::MainMenu)
    , selectedMainOption(0)
    , maxMainOptions(4)
    , inputCooldown(0.0f)
    , selectedColor(Color::Yellow)
    , normalColor(Color::White)
    , titleColor(Color::Red)
    , wasUpPressed(false)
    , wasDownPressed(false)
    , wasEnterPressed(false)
    , wasEscapePressed(false)
{
}

Menu::~Menu()
{
}

void Menu::Initialize(Vector2f screenResolution)
{
    resolution = screenResolution;

    CreateMainMenuTexts();
    CreateAboutText();
    CreateSettingsText();
}

void Menu::CreateMainMenuTexts()
{
    // Create title
    titleText.InitializeText("Fonts/Roboto-Regular.ttf", "HORROR GAME", 80.0f, true,
        titleColor, Vector2f(resolution.x / 2.0f, resolution.y / 5.0f));

    // Create main menu options
    mainMenuTexts.clear();
    mainMenuTexts.resize(maxMainOptions);

    std::vector<std::string> menuOptions = { "Start Game", "Settings", "About", "Exit" };

    float startY = resolution.y / 2.0f;
    float spacing = 80.0f;

    for (int i = 0; i < maxMainOptions; i++)
    {
        Color textColor = (i == selectedMainOption) ? selectedColor : normalColor;
        mainMenuTexts[i].InitializeText("Fonts/Roboto-Regular.ttf", menuOptions[i], 50.0f, true,
            textColor, Vector2f(resolution.x / 2.0f, startY + (i * spacing)));
    }

    // Instructions
    instructionsText.InitializeText("Fonts/Roboto-Regular.ttf",
        "Use UP/DOWN arrows to navigate • ENTER to select • ESC to exit",
        25.0f, true, Color({128, 128,128}),
        Vector2f(resolution.x / 2.0f, resolution.y - 80.0f));
}

void Menu::CreateAboutText()
{
    std::string aboutContent = "HORROR GAME\n\n"
        "Created for Themed Horror Jam 21\n\n"
        "Controls:\n"
        "• WASD or Arrow Keys to move\n"
        "• ESC to pause/return to menu\n\n"
        "Programmer by Osama Hussein\n"
        "Programmer/Graphics by ScriptFox\n"
        "Game Design/Writing by fauxdude97\n";

    aboutContentText.InitializeText("Fonts/Roboto-Regular.ttf", aboutContent, 35.0f, true,
        normalColor, Vector2f(resolution.x / 2.0f, resolution.y / 2.0f));

    backInstructionText.InitializeText("Fonts/Roboto-Regular.ttf",
        "Press ESC to return to main menu",
        30.0f, true, Color({ 128, 128,128 }),
        Vector2f(resolution.x / 2.0f, resolution.y - 100.0f));
}

void Menu::CreateSettingsText()
{
    std::string settingsContent = "SETTINGS\n\n"
        "Volume: 100%\n"
        "Difficulty: Normal\n"
        "Fullscreen: On\n"
        "Resolution: " + std::to_string((int)resolution.x) + "x" + std::to_string((int)resolution.y) + "\n\n"
        "Note: Settings are currently read-only";

    settingsContentText.InitializeText("Fonts/Roboto-Regular.ttf", settingsContent, 35.0f, true,
        normalColor, Vector2f(resolution.x / 2.0f, resolution.y / 2.0f));
}

void Menu::UpdateMainMenuColors()
{
    std::vector<std::string> menuOptions = { "Start Game", "Settings", "About", "Exit" };

    float startY = resolution.y / 2.0f;
    float spacing = 80.0f;

    for (int i = 0; i < maxMainOptions; i++)
    {
        Color textColor = (i == selectedMainOption) ? selectedColor : normalColor;
        mainMenuTexts[i].InitializeText("Fonts/Roboto-Regular.ttf", menuOptions[i], 50.0f, true,
            textColor, Vector2f(resolution.x / 2.0f, startY + (i * spacing)));
    }
}

MenuAction Menu::Update(float deltaTime)
{
    // Update input cooldown
    if (inputCooldown > 0.0f)
    {
        inputCooldown -= deltaTime;
    }

    MenuAction action = MenuAction::None;

    switch (currentState)
    {
    case MenuState::MainMenu:
        action = (inputCooldown <= 0.0f) ? HandleMainMenuInput() : MenuAction::None;
        break;

    case MenuState::Settings:
    case MenuState::About:
        action = (inputCooldown <= 0.0f) ? HandleSubMenuInput() : MenuAction::None;
        break;
    }

    return action;
}

MenuAction Menu::HandleMainMenuInput()
{
    MenuAction action = MenuAction::None;

    // Track current input states
    bool isUpPressed = Keyboard::isKeyPressed(Keyboard::Key::Up);
    bool isDownPressed = Keyboard::isKeyPressed(Keyboard::Key::Down);
    bool isEnterPressed = Keyboard::isKeyPressed(Keyboard::Key::Enter);
    bool isEscapePressed = Keyboard::isKeyPressed(Keyboard::Key::Escape);

    // Check for key press events (not held)
    if (isUpPressed && !wasUpPressed && inputCooldown <= 0.0f)
    {
        selectedMainOption--;
        if (selectedMainOption < 0) selectedMainOption = maxMainOptions - 1;
        UpdateMainMenuColors();
        inputCooldown = INPUT_DELAY;
    }
    else if (isDownPressed && !wasDownPressed && inputCooldown <= 0.0f)
    {
        selectedMainOption++;
        if (selectedMainOption >= maxMainOptions) selectedMainOption = 0;
        UpdateMainMenuColors();
        inputCooldown = INPUT_DELAY;
    }
    else if (isEnterPressed && !wasEnterPressed && inputCooldown <= 0.0f)
    {
        switch (selectedMainOption)
        {
        case 0: // Start Game
            action = MenuAction::StartGame;
            break;
        case 1: // Settings
            currentState = MenuState::Settings;
            action = MenuAction::GoToSettings;
            break;
        case 2: // About
            currentState = MenuState::About;
            action = MenuAction::GoToAbout;
            break;
        case 3: // Exit
            action = MenuAction::ExitGame;
            break;
        }
        inputCooldown = INPUT_DELAY;
    }
    else if (isEscapePressed && !wasEscapePressed && inputCooldown <= 0.0f)
    {
        action = MenuAction::ExitGame;
        inputCooldown = INPUT_DELAY;
    }

    // Update previous input states
    wasUpPressed = isUpPressed;
    wasDownPressed = isDownPressed;
    wasEnterPressed = isEnterPressed;
    wasEscapePressed = isEscapePressed;

    return action;
}

MenuAction Menu::HandleSubMenuInput()
{
    MenuAction action = MenuAction::None;

    bool isEscapePressed = Keyboard::isKeyPressed(Keyboard::Key::Escape);

    if (isEscapePressed && !wasEscapePressed && inputCooldown <= 0.0f)
    {
        currentState = MenuState::MainMenu;
        action = MenuAction::BackToMain;
        inputCooldown = INPUT_DELAY;
    }

    wasEscapePressed = isEscapePressed;

    return action;
}

void Menu::Render(RenderWindow& window)
{
    window.clear(Color::Black);

    switch (currentState)
    {
    case MenuState::MainMenu:
        window.draw(titleText.LoadText());
        for (auto& menuText : mainMenuTexts)
        {
            window.draw(menuText.LoadText());
        }
        window.draw(instructionsText.LoadText());
        break;

    case MenuState::About:
        window.draw(aboutContentText.LoadText());
        window.draw(backInstructionText.LoadText());
        break;

    case MenuState::Settings:
        window.draw(settingsContentText.LoadText());
        window.draw(backInstructionText.LoadText());
        break;
    }
}

void Menu::ResetToMainMenu()
{
    currentState = MenuState::MainMenu;
    selectedMainOption = 0;
    UpdateMainMenuColors();
}