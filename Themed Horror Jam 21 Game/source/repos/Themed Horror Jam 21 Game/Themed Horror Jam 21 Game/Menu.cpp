#include "Menu.h"
#include "Engine.h"

int Menu::volume = 100;
std::vector<std::string> Menu::difficulty;
std::vector<std::string> Menu::fullscreenStatus;
std::vector<Vector2u> Menu::resolutionSize;

int Menu::currentDifficulty = 1;
int Menu::currentFullscreenStatus = 1;
int Menu::currentResolutionSize = 0;
Menu::Menu()
    : currentState(MenuState::MainMenu)
    , selectedMainOption(0)
    , maxMainOptions(4)
    , selectedSettingsOption(0)
    , maxSettingsOptions(4)
    , inputCooldown(0.0f)
    , selectedColor(Color::Yellow)
    , normalColor(Color::White)
    , titleColor(Color::Red)
    , modifySettingColor(Color(255, 165, 0, 255))
    , buttonIdleColor(Color(50, 50, 50))      // Dark gray
    , buttonHoverColor(Color(100, 100, 100))  // Medium gray  
    , buttonActiveColor(Color(150, 150, 150)) // Light gray
    , wasUpPressed(false)
    , wasDownPressed(false)
    , wasLeftPressed(false)
    , wasRightPressed(false)
    , wasEnterPressed(false)
    , wasEscapePressed(false)
{
}

Menu::~Menu()
{
}

void Menu::Initialize(Vector2u screenResolution)
{
    resolution = screenResolution;

    CreateMainMenuTexts();
    CreateMainMenuButtons();
    CreateAboutText();
    CreateSettingsText();
}

void Menu::CreateMainMenuTexts()
{
    // Create title
    titleText.InitializeText("Fonts/Roboto-Regular.ttf", "HORROR GAME", 80.0f, true,
        false,titleColor, Vector2f(resolution.x / 2.0f, resolution.y / 5.0f));

    // Create main menu options
    mainMenuTexts.clear();
    mainMenuTexts.resize(maxMainOptions);

    std::vector<std::string> menuOptions = { "Start Game", "Settings", "About", "Exit" };

    float startY = resolution.y / 2.0f;
    float spacing = 80.0f;

    for (int i = 0; i < maxMainOptions; i++)
    {
        Color textColor = (i == selectedMainOption) ? selectedColor : normalColor;
        mainMenuTexts[i].InitializeText("Fonts/Roboto-Regular.ttf", menuOptions[i], 50.0f, true, false,
            textColor, Vector2f(resolution.x / 2.0f, startY + (i * spacing)));
    }

    // Instructions
    instructionsText.InitializeText("Fonts/Roboto-Regular.ttf",
        "Use UP/DOWN arrows to navigate • ENTER to select • ESC to exit",
        25.0f, true, false, Color({ 128, 128,128 }),
        Vector2f(resolution.x / 2.0f, resolution.y - 80.0f));
}
void Menu::CreateMainMenuButtons()
{
    // Clear existing buttons
    mainMenuButtons.clear();
    mainMenuButtons.reserve(maxMainOptions);

    float buttonWidth = 300.0f;
    float buttonHeight = 60.0f;
    float startY = resolution.y / 2.0f - 20.0f;  // Slightly offset to align with text
    float spacing = 80.0f;

    for (int i = 0; i < maxMainOptions; i++)
    {
        float buttonX = (resolution.x - buttonWidth) / 2.0f;
        float buttonY = startY + (i * spacing);

        mainMenuButtons.emplace_back(buttonX, buttonY, buttonWidth, buttonHeight,
            buttonIdleColor, buttonHoverColor, buttonActiveColor);
    }

    // Initialize back button (positioned at bottom of screen)
    float backButtonX = (resolution.x - 200.0f) / 2.0f;
    float backButtonY = resolution.y - 150.0f;
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

    aboutContentText.InitializeText("Fonts/Roboto-Regular.ttf", aboutContent, 35.0f, true, false,
        normalColor, Vector2f(resolution.x / 2.0f, resolution.y / 2.0f));

    backInstructionText.InitializeText("Fonts/Roboto-Regular.ttf",
        "Press ESC to return to main menu",
        30.0f, true, false, Color({ 128, 128,128 }),
        Vector2f(resolution.x / 2.0f, resolution.y - 100.0f));
}

void Menu::CreateSettingsText()
{
    settingMenuTexts.clear();
    settingMenuTexts.resize(maxSettingsOptions);

    settingMenuOptionsTexts.clear();
    settingMenuOptionsTexts.resize(maxSettingsOptions);

    difficulty.clear();
    difficulty.resize(3);

    fullscreenStatus.clear();
    fullscreenStatus.resize(2);

    resolutionSize.clear();
    resolutionSize.resize(1);

    difficulty = { "Easy", "Normal", "Hard" };
    fullscreenStatus = { "Off", "On" };
    resolutionSize = { Vector2u(resolution.x, resolution.y) };

    settingsContentText.InitializeText("Fonts/Roboto-Regular.ttf", "SETTINGS\n\n", 35.0f, true, false,
        normalColor, Vector2f(resolution.x / 2.0f, resolution.y / 5.0f));

    std::vector<std::string> settingTexts = { "Volume", "Difficulty", "Fullscreen", "Resolution" };

    std::vector<std::string> settingOptions = { std::to_string(volume) + "%", difficulty[currentDifficulty],
        fullscreenStatus[currentFullscreenStatus], std::to_string(resolutionSize[currentResolutionSize].x) + "x" +
        std::to_string(resolutionSize[currentResolutionSize].y) };

    float startY = resolution.y / 3.0f;
    float startY2 = resolution.y / 3.35f;

    float spacing = 100.0f;

    for (int i = 0; i < maxSettingsOptions; i++)
    {
        Color textColor = (i == selectedSettingsOption) ? selectedColor : normalColor;
        settingMenuTexts[i].InitializeText("Fonts/Roboto-Regular.ttf", settingTexts[i], 50.0f, false, true,
            textColor, Vector2f(resolution.x / 2.0f, startY + (i * spacing)));

        settingMenuOptionsTexts[i].InitializeText("Fonts/Roboto-Regular.ttf", settingOptions[i], 50.0f, false, false,
            textColor, Vector2f(resolution.x / 1.75f, startY2 + (i * spacing)));
    }

    // Instructions for settings menu
    settingsInstructionsText.InitializeText("Fonts/Roboto-Regular.ttf",
        "Use UP/DOWN arrows to navigate • Use LEFT/RIGHT arrows to modify setting • ENTER to select • ESC to go back to main menu",
        25.0f, true, false, Color({ 128, 128,128 }),
        Vector2f(resolution.x / 2.0f, resolution.y - 80.0f));
}

void Menu::UpdateMainMenuColors()
{
    std::vector<std::string> menuOptions = { "Start Game", "Settings", "About", "Exit" };

    float startY = resolution.y / 2.0f;
    float spacing = 80.0f;

    for (int i = 0; i < maxMainOptions; i++)
    {
        Color textColor = (i == selectedMainOption) ? selectedColor : normalColor;
        mainMenuTexts[i].InitializeText("Fonts/Roboto-Regular.ttf", menuOptions[i], 50.0f, true,false,
            textColor, Vector2f(resolution.x / 2.0f, startY + (i * spacing)));
    }
}
void Menu::UpdateSettingsMenuColors()
{
    std::vector<std::string> settingTexts = { "Volume", "Difficulty", "Fullscreen", "Resolution" };

    std::vector<std::string> settingOptions = { std::to_string(volume) + "%", difficulty[currentDifficulty],
        fullscreenStatus[currentFullscreenStatus], std::to_string(resolutionSize[currentResolutionSize].x) + "x" +
        std::to_string(resolutionSize[currentResolutionSize].y) };

    if (settingsAction == SettingsMenuAction::ModifyNone && selectedColor != Color::Yellow) 
        selectedColor = Color::Yellow;

    else if (settingsAction != SettingsMenuAction::ModifyNone && selectedColor == Color::Yellow) 
        selectedColor = modifySettingColor;

    float startY = resolution.y / 3.0f;
    float startY2 = resolution.y / 3.35f;

    float spacing = 100.0f;

    for (int i = 0; i < maxSettingsOptions; i++)
    {
        Color textColor = (i == selectedSettingsOption) ? selectedColor : normalColor;
        settingMenuTexts[i].InitializeText("Fonts/Roboto-Regular.ttf", settingTexts[i], 50.0f, false, true,
            textColor, Vector2f(resolution.x / 2.0f, startY + (i * spacing)));

        settingMenuOptionsTexts[i].InitializeText("Fonts/Roboto-Regular.ttf", settingOptions[i], 50.0f, false, false,
            textColor, Vector2f(resolution.x / 1.75f, startY2 + (i * spacing)));
    }
}
MenuAction Menu::Update(float deltaTime, Vector2f mousePos)
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
        // Update main menu buttons
        for (auto& button : mainMenuButtons)
        {
            button.update(mousePos);
        }
        action = (inputCooldown <= 0.0f) ? HandleMainMenuInput(mousePos) : MenuAction::None;
        break;

    case MenuState::Settings:
        action = (inputCooldown <= 0.0f) ? HandleSettingsInput() : MenuAction::None;
        break;
    case MenuState::About:
        action = (inputCooldown <= 0.0f) ? HandleSubMenuInput() : MenuAction::None;
        break;
    }

    return action;
}
MenuAction Menu::HandleMainMenuInput(Vector2f mousePos)
{
    MenuAction action = MenuAction::None;

    // Track current input states
    bool isUpPressed = Keyboard::isKeyPressed(Keyboard::Key::Up);
    bool isDownPressed = Keyboard::isKeyPressed(Keyboard::Key::Down);
    bool isEnterPressed = Keyboard::isKeyPressed(Keyboard::Key::Enter);
    bool isEscapePressed = Keyboard::isKeyPressed(Keyboard::Key::Escape);

    // Check for button clicks first
    for (int i = 0; i < maxMainOptions; i++)
    {
        if (mainMenuButtons[i].isPressed())
        {
            switch (i)
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
            return action;
        }
    }

    // Handle keyboard input (existing functionality)
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
            settingsAction = SettingsMenuAction::ModifyNone;
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
MenuAction Menu::HandleSettingsInput()
{
    MenuAction action = MenuAction::None;

    // Track current input states
    bool isUpPressed = Keyboard::isKeyPressed(Keyboard::Key::Up);
    bool isDownPressed = Keyboard::isKeyPressed(Keyboard::Key::Down);
    bool isLeftPressed = Keyboard::isKeyPressed(Keyboard::Key::Left);
    bool isRightPressed = Keyboard::isKeyPressed(Keyboard::Key::Right);
    bool isEnterPressed = Keyboard::isKeyPressed(Keyboard::Key::Enter);
    bool isEscapePressed = Keyboard::isKeyPressed(Keyboard::Key::Escape);

    // Check for key press events (not held)
    if (isUpPressed && !wasUpPressed && inputCooldown <= 0.0f && settingsAction == SettingsMenuAction::ModifyNone)
    {
        selectedSettingsOption--;
        if (selectedSettingsOption < 0) selectedSettingsOption = maxSettingsOptions - 1;
        UpdateSettingsMenuColors();
        inputCooldown = INPUT_DELAY;
    }
    else if (isDownPressed && !wasDownPressed && inputCooldown <= 0.0f && settingsAction == SettingsMenuAction::ModifyNone)
    {
        selectedSettingsOption++;
        if (selectedSettingsOption >= maxSettingsOptions) selectedSettingsOption = 0;
        UpdateSettingsMenuColors();
        inputCooldown = INPUT_DELAY;
    }

    // Modify the volume controls
    else if (isLeftPressed && !wasLeftPressed && inputCooldown <= 0.0f && settingsAction == SettingsMenuAction::ModifyVolume)
    {
        volume -= 1;
        if (volume <= 0) volume = 0;
        UpdateSettingsMenuColors();
        inputCooldown = INPUT_DELAY;
    }
    else if (isRightPressed && !wasRightPressed && inputCooldown <= 0.0f && settingsAction == SettingsMenuAction::ModifyVolume)
    {
        volume += 1;
        if (volume >= 100) volume = 100;
        UpdateSettingsMenuColors();
        inputCooldown = INPUT_DELAY;
    }

    // Modify the difficulty
    else if (isLeftPressed && !wasLeftPressed && inputCooldown <= 0.0f && settingsAction == SettingsMenuAction::ModifyDifficulty)
    {
        currentDifficulty--;
        if (currentDifficulty <= 0) currentDifficulty = 0;
        UpdateSettingsMenuColors();
        inputCooldown = INPUT_DELAY;
    }
    else if (isRightPressed && !wasRightPressed && inputCooldown <= 0.0f && settingsAction == SettingsMenuAction::ModifyDifficulty)
    {
        currentDifficulty++;
        if (currentDifficulty >= difficulty.size() - 1) currentDifficulty = difficulty.size() - 1;
        UpdateSettingsMenuColors();
        inputCooldown = INPUT_DELAY;
    }

    // Modify fullscreen
    else if (isLeftPressed && !wasLeftPressed && inputCooldown <= 0.0f && 
        settingsAction == SettingsMenuAction::ModifyFullscreen)
    {
        currentFullscreenStatus--;
        if (currentFullscreenStatus <= 0) currentFullscreenStatus = 0;
        ToggleFullscreen();
        UpdateSettingsMenuColors();
        inputCooldown = INPUT_DELAY;
    }
    else if (isRightPressed && !wasRightPressed && inputCooldown <= 0.0f && 
        settingsAction == SettingsMenuAction::ModifyFullscreen)
    {
        currentFullscreenStatus++;
        if (currentFullscreenStatus >= fullscreenStatus.size() - 1) currentFullscreenStatus = fullscreenStatus.size() - 1;
        ToggleFullscreen();
        UpdateSettingsMenuColors();
        inputCooldown = INPUT_DELAY;
    }

    // Modify resolution
    else if (isLeftPressed && !wasLeftPressed && inputCooldown <= 0.0f && 
        settingsAction == SettingsMenuAction::ModifyResolution)
    {
        currentResolutionSize--;
        if (currentResolutionSize <= 0) currentResolutionSize = 0;
        UpdateSettingsMenuColors();
        inputCooldown = INPUT_DELAY;
    }
    else if (isRightPressed && !wasRightPressed && inputCooldown <= 0.0f && 
        settingsAction == SettingsMenuAction::ModifyResolution)
    {
        currentResolutionSize++;
        if (currentResolutionSize >= resolutionSize.size() - 1) currentResolutionSize = resolutionSize.size() - 1;
        UpdateSettingsMenuColors();
        inputCooldown = INPUT_DELAY;
    }

    // Check if enter was pressed and the action is equal to none
    else if (isEnterPressed && !wasEnterPressed && inputCooldown <= 0.0f && settingsAction == SettingsMenuAction::ModifyNone)
    {
        switch (selectedSettingsOption)
        {
        case 0: // Volume
            settingsAction = SettingsMenuAction::ModifyVolume;
            UpdateSettingsMenuColors();
            break;
        case 1: // Difficulty
            settingsAction = SettingsMenuAction::ModifyDifficulty;
            UpdateSettingsMenuColors();
            break;
        case 2: // Fullscreen
            settingsAction = SettingsMenuAction::ModifyFullscreen;
            UpdateSettingsMenuColors();
            break;
        case 3: // Resolution
            settingsAction = SettingsMenuAction::ModifyResolution;
            UpdateSettingsMenuColors();
            break;
        }
        inputCooldown = INPUT_DELAY;
    }

    // If enter key is pressed and the action isn't equal to none, set it back to none again
    else if (isEnterPressed && !wasEnterPressed && inputCooldown <= 0.0f && settingsAction != SettingsMenuAction::ModifyNone)
    {
        settingsAction = SettingsMenuAction::ModifyNone;
        UpdateSettingsMenuColors();
        inputCooldown = INPUT_DELAY;
    }

    // If escape key is pressed and the action is equal to none, go back to the main menu
    else if (isEscapePressed && !wasEscapePressed && inputCooldown <= 0.0f && settingsAction == SettingsMenuAction::ModifyNone)
    {
        currentState = MenuState::MainMenu;
        action = MenuAction::BackToMain;
        inputCooldown = INPUT_DELAY;
    }

    // If escape key is pressed and the action isn't equal to none, set it to none again
    else if (isEscapePressed && !wasEscapePressed && inputCooldown <= 0.0f && settingsAction != SettingsMenuAction::ModifyNone)
    {
        settingsAction = SettingsMenuAction::ModifyNone;
        UpdateSettingsMenuColors();
        inputCooldown = INPUT_DELAY;
    }

    // Update previous input states
    wasUpPressed = isUpPressed;
    wasDownPressed = isDownPressed;
    wasLeftPressed = isLeftPressed;
    wasRightPressed = isRightPressed;
    wasEnterPressed = isEnterPressed;
    wasEscapePressed = isEscapePressed;

    return action;
}
MenuAction Menu::HandleSubMenuInput()
{
    MenuAction action = MenuAction::None;

    bool isEscapePressed = Keyboard::isKeyPressed(Keyboard::Key::Escape);

   

    // Handle keyboard input
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
        // Draw title and instructions
        window.draw(titleText.LoadText());
        window.draw(instructionsText.LoadText());

        // Draw buttons first (background)
        for (auto& button : mainMenuButtons)
        {
            button.draw(&window);
        }

        // Draw text on top of buttons
        for (auto& menuText : mainMenuTexts)
        {
            window.draw(menuText.LoadText());
        }
        break;

    case MenuState::About:
        window.draw(aboutContentText.LoadText());
        break;

    case MenuState::Settings:
        window.draw(settingsContentText.LoadText());
        for (auto& settingText : settingMenuTexts)
        {
            window.draw(settingText.LoadText());
        }
        for (auto& settingOptionsText : settingMenuOptionsTexts)
        {
            window.draw(settingOptionsText.LoadText());
        }
        window.draw(settingsInstructionsText.LoadText());
        break;
    }
}

void Menu::ResetToMainMenu()
{
    currentState = MenuState::MainMenu;
    selectedMainOption = 0;
    UpdateMainMenuColors();
}

void Menu::ToggleFullscreen()
{
    // If fullscreen status is set to off, change the window state to windowed mode
    if (fullscreenStatus[currentFullscreenStatus] == "Off")
    {
        Engine::Instance()->GetWindow()->close();
        Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Game", sf::State::Windowed);
    }

    // If fullscreen status is set to on, change the window state to fullscreen mode
    else if (fullscreenStatus[currentFullscreenStatus] == "On")
    {
        Engine::Instance()->GetWindow()->close();
        Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Game", sf::State::Fullscreen);
    }
}