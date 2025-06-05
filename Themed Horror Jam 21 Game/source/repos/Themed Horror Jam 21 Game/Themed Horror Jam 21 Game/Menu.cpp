#include "Menu.h"
#include "Engine.h"

int Menu::volume = 100;
std::vector<std::string> Menu::difficulty;
std::vector<std::string> Menu::fullscreenStatus;
std::vector<Vector2u> Menu::resolutionSize;

int Menu::currentDifficulty = 1;
int Menu::currentFullscreenStatus = 1;
int Menu::currentResolutionSize = 0;

float ABOUT_CONTENT_CHARACTER_SIZE;
float MAIN_MENU_TITLE_TEXT_CHARACTER_SIZE;
float MAIN_MENU_TEXT_CHARACTER_SIZE;
float SETTINGS_MENU_TEXT_CHARACTER_SIZE;
float SETTINGS_TITLE_TEXT_CHARACTER_SIZE;
float MAIN_MENU_INSTRUCTION_TEXT_CHARACTER_SIZE;
float SETTINGS_INSTRUCTION_TEXT_CHARACTER_SIZE;
float ABOUT_BACK_TEXT_CHARACTER_SIZE;

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
    , settingsBackButton(nullptr)
{
}

Menu::~Menu()
{
    delete settingsBackButton;
    settingsBackButton = nullptr;
}

void Menu::Initialize(Vector2u screenResolution)
{
    resolution = screenResolution;

    ABOUT_CONTENT_CHARACTER_SIZE = 35.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);
    MAIN_MENU_TITLE_TEXT_CHARACTER_SIZE = 80.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);
    MAIN_MENU_TEXT_CHARACTER_SIZE = 50.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);
    SETTINGS_MENU_TEXT_CHARACTER_SIZE = 50.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);
    SETTINGS_TITLE_TEXT_CHARACTER_SIZE = 35.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);
    MAIN_MENU_INSTRUCTION_TEXT_CHARACTER_SIZE = 25.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);
    SETTINGS_INSTRUCTION_TEXT_CHARACTER_SIZE = 25.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);
    ABOUT_BACK_TEXT_CHARACTER_SIZE = 30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

    CreateMainMenuButtons();
    CreateMainMenuTexts();
    CreateAboutText();
    CreateSettingsButtons();
    CreateSettingsText();
}

void Menu::CreateMainMenuTexts()
{
    // Create title
    titleText.InitializeText("Fonts/Roboto-Regular.ttf", "HORROR GAME", MAIN_MENU_TITLE_TEXT_CHARACTER_SIZE, true,
        false,titleColor, Vector2f(resolution.x / 2.0f, resolution.y / 5.0f));

    // Create main menu options
    mainMenuTexts.clear();
    mainMenuTexts.resize(maxMainOptions);

    std::vector<std::string> menuOptions = { "Start Game", "Settings", "About", "Exit" };

    for (int i = 0; i < maxMainOptions; i++)
    {
        Color textColor = (i == selectedMainOption) ? selectedColor : normalColor;
        mainMenuTexts[i].InitializeText("Fonts/Roboto-Regular.ttf", menuOptions[i], MAIN_MENU_TEXT_CHARACTER_SIZE, true,
            false, textColor, Vector2f(mainMenuButtons[i].GetPosition() +
                mainMenuButtons[i].GetSize() / 2.0f));
    }

    // Instructions
    instructionsText.InitializeText("Fonts/Roboto-Regular.ttf",
        "Use UP/DOWN arrows to navigate • ENTER to select • ESC to exit",
        MAIN_MENU_INSTRUCTION_TEXT_CHARACTER_SIZE, true, false, Color({ 128, 128,128 }),
        Vector2f(resolution.x / 2.0f, resolution.y - 80.0f));
}
void Menu::CreateMainMenuButtons()
{
    // Clear existing buttons
    mainMenuButtons.clear();
    mainMenuButtons.reserve(maxMainOptions);

    float buttonWidth = 300.0f;
    float buttonHeight = 60.0f;
    float startY = resolution.y / 2.5f;  // Slightly offset to align with text
    float spacing = 120.0f * (resolution.y / 1080.0f);

    for (int i = 0; i < maxMainOptions; i++)
    {
        float buttonX = resolution.x / 2.35f;
        float buttonY = startY + (i * spacing);

        mainMenuButtons.emplace_back(buttonX, buttonY, buttonWidth * (resolution.x / 1920.0f),
            buttonHeight * (resolution.y / 1080.0f), buttonIdleColor, buttonHoverColor, buttonActiveColor);
    }

    // Initialize back button (positioned at bottom of screen)
    float backButtonX = (resolution.x - 200.0f) / 2.0f;
    float backButtonY = resolution.y - 150.0f;
}

void Menu::CreateSettingsButtons()
{
    // Clear existing buttons
    settingMenuButtons.clear();
    settingMenuButtons.reserve(maxSettingsOptions);

    float buttonWidth = 700.0f;
    float buttonHeight = 65.0f;
    float startY = resolution.y / 3.4f;
    float spacing = 100.0f * (resolution.y / 1080.0f);

    for (int i = 0; i < maxSettingsOptions; i++)
    {
        float buttonX = resolution.x / 3.0f;
        float buttonY = startY + (i * spacing);

        settingMenuButtons.emplace_back(buttonX, buttonY, buttonWidth * (resolution.x / 1920.0f),
            buttonHeight * (resolution.y / 1080.0f), buttonIdleColor, buttonHoverColor, buttonActiveColor);
    }

    // Initialize back button (positioned at bottom of screen)
    float backButtonX = (resolution.x - 100.0f) / 2.0f;
    float backButtonY = resolution.y - 200.0f;

    if (!settingsBackButton)
    {
        settingsBackButton = new Button(backButtonX, backButtonY, 200.0f * (resolution.x / 1920.0f), 
            50.0f * (resolution.y / 1080.0f), buttonIdleColor, buttonHoverColor, buttonActiveColor);
    }
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
        "Game Design/Writing by fauxdude97\n"
        "Music/SFX by Seangle75\n";

    aboutContentText.InitializeText("Fonts/Roboto-Regular.ttf", aboutContent, ABOUT_CONTENT_CHARACTER_SIZE, true, false,
        normalColor, Vector2f(resolution.x / 2.0f, resolution.y / 2.0f));

    backInstructionText.InitializeText("Fonts/Roboto-Regular.ttf",
        "Press ESC to return to main menu",
        ABOUT_BACK_TEXT_CHARACTER_SIZE, true, false, Color({ 128, 128,128 }),
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

    difficulty = { "Easy", "Normal", "Hard" };
    fullscreenStatus = { "Off", "On" };

    InitializeScreenResolution(); // Found in Resolution.cpp

    if (resolution != Engine::Instance()->GetResolution()) resolution = Engine::Instance()->GetResolution();

    settingsContentText.InitializeText("Fonts/Roboto-Regular.ttf", "SETTINGS\n\n", SETTINGS_TITLE_TEXT_CHARACTER_SIZE,
        true, false, normalColor, Vector2f(resolution.x / 2.0f, resolution.y / 5.0f));

    std::vector<std::string> settingTexts = { "Volume", "Difficulty", "Fullscreen", "Resolution" };

    std::vector<std::string> settingOptions = { std::to_string(volume) + "%", difficulty[currentDifficulty],
        fullscreenStatus[currentFullscreenStatus], std::to_string(resolutionSize[currentResolutionSize].x) + "x" +
        std::to_string(resolutionSize[currentResolutionSize].y) };

    for (int i = 0; i < maxSettingsOptions; i++)
    {
        Color textColor = (i == selectedSettingsOption) ? selectedColor : normalColor;

        settingMenuTexts[i].InitializeText("Fonts/Roboto-Regular.ttf", settingTexts[i], SETTINGS_MENU_TEXT_CHARACTER_SIZE,
            false, true, textColor, 
            Vector2f(settingMenuButtons[i].GetPosition().x + settingMenuButtons[i].GetSize().x / 2.4f,
                settingMenuButtons[i].GetPosition().y + settingMenuButtons[i].GetSize().y / 1.3f));

        settingMenuOptionsTexts[i].InitializeText("Fonts/Roboto-Regular.ttf", settingOptions[i], 
            SETTINGS_MENU_TEXT_CHARACTER_SIZE, false, false, textColor,
            Vector2f(settingMenuButtons[i].GetPosition().x + settingMenuButtons[i].GetSize().x / 1.7f,
                settingMenuButtons[i].GetPosition().y + settingMenuButtons[i].GetSize().y / 3.8f));

        settingsBackText.InitializeText("Fonts/Roboto-Regular.ttf", "Back", SETTINGS_MENU_TEXT_CHARACTER_SIZE, false,
            false, textColor, Vector2f(settingsBackButton->GetPosition().x + settingsBackButton->GetSize().x / 3.8f,
                settingsBackButton->GetPosition().y + settingsBackButton->GetSize().y / 7.6f));
    }

    // Instructions for settings menu
    settingsInstructionsText.InitializeText("Fonts/Roboto-Regular.ttf",
        "Use UP/DOWN arrows to navigate • Use LEFT/RIGHT arrows to modify setting • ENTER to select • ESC to go back to main menu",
        SETTINGS_INSTRUCTION_TEXT_CHARACTER_SIZE, true, false, Color({ 128, 128,128 }),
        Vector2f(resolution.x / 2.0f, resolution.y - 80.0f));
}

void Menu::UpdateMainMenuColors()
{
    std::vector<std::string> menuOptions = { "Start Game", "Settings", "About", "Exit" };

    // Main menu texts
    float startY = resolution.y / 2.0f;
    float spacing = 80.0f;

    for (int i = 0; i < maxMainOptions; i++)
    {
        Color textColor = (i == selectedMainOption) ? selectedColor : normalColor;
        mainMenuTexts[i].InitializeText("Fonts/Roboto-Regular.ttf", menuOptions[i], MAIN_MENU_TEXT_CHARACTER_SIZE, true,
            false, textColor, Vector2f(resolution.x / 2.0f, startY + (i * spacing)));

        if (mainMenuTexts[i].GetTextPosition() != mainMenuButtons[i].GetPosition() + mainMenuButtons[i].GetSize() / 2.0f)
        {
            // Center the main menu texts on the buttons
            mainMenuTexts[i].SetTextPosition(Vector2f(mainMenuButtons[i].GetPosition() + 
                mainMenuButtons[i].GetSize() / 2.0f));
        }
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

    for (int i = 0; i < maxSettingsOptions; i++)
    {
        Color textColor = (i == selectedSettingsOption) ? selectedColor : normalColor;

        settingMenuTexts[i].InitializeText("Fonts/Roboto-Regular.ttf", settingTexts[i], SETTINGS_MENU_TEXT_CHARACTER_SIZE,
            false, true, textColor, 
            Vector2f(settingMenuButtons[i].GetPosition().x + settingMenuButtons[i].GetSize().x / 2.4f,
                settingMenuButtons[i].GetPosition().y + settingMenuButtons[i].GetSize().y / 1.3f));

        settingMenuOptionsTexts[i].InitializeText("Fonts/Roboto-Regular.ttf", settingOptions[i], 
            SETTINGS_MENU_TEXT_CHARACTER_SIZE, false, false, textColor,
            Vector2f(settingMenuButtons[i].GetPosition().x + settingMenuButtons[i].GetSize().x / 1.7f,
                settingMenuButtons[i].GetPosition().y + settingMenuButtons[i].GetSize().y / 3.8f));

        settingsBackText.InitializeText("Fonts/Roboto-Regular.ttf", "Back", SETTINGS_MENU_TEXT_CHARACTER_SIZE, false,
            false, sf::Color::White, Vector2f(settingsBackButton->GetPosition().x + settingsBackButton->GetSize().x / 3.8f,
                settingsBackButton->GetPosition().y + settingsBackButton->GetSize().y / 7.6f));
    }
}

MenuAction Menu::Update(float deltaTime, Vector2f mousePos)
{
    //if (resolution != Engine::Instance()->GetResolution()) resolution = Engine::Instance()->GetResolution();

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
        for (auto& button : settingMenuButtons)
        {
            button.update(mousePos);
        }

        settingsBackButton->update(mousePos);

        action = (inputCooldown <= 0.0f) ? HandleSettingsInput(mousePos) : MenuAction::None;
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
MenuAction Menu::HandleSettingsInput(Vector2f mousePos)
{
    MenuAction action = MenuAction::None;

    // Track current input states
    bool isUpPressed = Keyboard::isKeyPressed(Keyboard::Key::Up);
    bool isDownPressed = Keyboard::isKeyPressed(Keyboard::Key::Down);
    bool isLeftPressed = Keyboard::isKeyPressed(Keyboard::Key::Left);
    bool isRightPressed = Keyboard::isKeyPressed(Keyboard::Key::Right);
    bool isEnterPressed = Keyboard::isKeyPressed(Keyboard::Key::Enter);
    bool isEscapePressed = Keyboard::isKeyPressed(Keyboard::Key::Escape);

    // Check for button clicks first
    for (int i = 0; i < maxSettingsOptions; i++)
    {
        if (settingMenuButtons[i].isHovered() && settingsAction == SettingsMenuAction::ModifyNone)
        {
            switch (i)
            {
            case 0: // Volume
                if (selectedSettingsOption != 0) selectedSettingsOption = 0;
                UpdateSettingsMenuColors();
                break;
            case 1: // Difficulty
                if (selectedSettingsOption != 1) selectedSettingsOption = 1;
                UpdateSettingsMenuColors();
                break;
            case 2: // Fullscreen
                if (selectedSettingsOption != 2) selectedSettingsOption = 2;
                UpdateSettingsMenuColors();
                break;
            case 3: // Resolution
                if (selectedSettingsOption != 3) selectedSettingsOption = 3;
                UpdateSettingsMenuColors();
                break;
            }
        }
        else if (settingMenuButtons[i].isPressed() && inputCooldown <= 0.0f && 
            settingsAction == SettingsMenuAction::ModifyNone)
        {
            switch (i)
            {
            case 0: // Volume
                settingsAction = SettingsMenuAction::ModifyVolume;
                UpdateSettingsMenuColors();
                inputCooldown = INPUT_DELAY;
                break;
            case 1: // Difficulty
                settingsAction = SettingsMenuAction::ModifyDifficulty;
                UpdateSettingsMenuColors();
                inputCooldown = INPUT_DELAY;
                break;
            case 2: // Fullscreen
                settingsAction = SettingsMenuAction::ModifyFullscreen;
                UpdateSettingsMenuColors();
                inputCooldown = INPUT_DELAY;
                break;
            case 3: // Resolution
                settingsAction = SettingsMenuAction::ModifyResolution;
                UpdateSettingsMenuColors();
                inputCooldown = INPUT_DELAY;
                break;
            }
        }

        // If the mouse was pressed and the settings action isn't set to none, set it to none
        else if (settingMenuButtons[i].isPressed() && inputCooldown <= 0.0f && 
            settingsAction != SettingsMenuAction::ModifyNone)
        {
            settingsAction = SettingsMenuAction::ModifyNone;
            UpdateSettingsMenuColors();
            inputCooldown = INPUT_DELAY;
        }
    }

    // If settings back button is pressed and settings action is set to none, go back to the main menu
    if (settingsBackButton->isPressed() && inputCooldown <= 0.0f && settingsAction == SettingsMenuAction::ModifyNone)
    {
        currentState = MenuState::MainMenu;
        action = MenuAction::BackToMain;
        inputCooldown = INPUT_DELAY;
    }

    // If settings back button is pressed and settings action is NOT set to none, set it to none
    else if (settingsBackButton->isPressed() && inputCooldown <= 0.0f && settingsAction != SettingsMenuAction::ModifyNone)
    {
        settingsAction = SettingsMenuAction::ModifyNone;
        UpdateSettingsMenuColors();
        inputCooldown = INPUT_DELAY;
    }

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
        if (currentFullscreenStatus > 0) 
        { 
            currentFullscreenStatus--;
            ToggleFullscreen();
        }

        else if (currentFullscreenStatus <= 0) currentFullscreenStatus = 0;

        UpdateSettingsMenuColors();
        inputCooldown = INPUT_DELAY;
    }
    else if (isRightPressed && !wasRightPressed && inputCooldown <= 0.0f && 
        settingsAction == SettingsMenuAction::ModifyFullscreen)
    {
        if (currentFullscreenStatus < fullscreenStatus.size() - 1)
        {
            currentFullscreenStatus++;
            ToggleFullscreen();
        }

        else if (currentFullscreenStatus >= fullscreenStatus.size() - 1) currentFullscreenStatus = fullscreenStatus.size() - 1;

        UpdateSettingsMenuColors();
        inputCooldown = INPUT_DELAY;
    }

    // Modify resolution
    else if (isLeftPressed && !wasLeftPressed && inputCooldown <= 0.0f && 
        settingsAction == SettingsMenuAction::ModifyResolution)
    {
        if (currentResolutionSize > 0)
        {
            currentResolutionSize--;
            ChangeResolution();
        }

        else if (currentResolutionSize <= 0) currentResolutionSize = 0;

        UpdateSettingsMenuColors();
        inputCooldown = INPUT_DELAY;
    }
    else if (isRightPressed && !wasRightPressed && inputCooldown <= 0.0f && 
        settingsAction == SettingsMenuAction::ModifyResolution)
    {
        if (currentResolutionSize < resolutionSize.size() - 1)
        {
            currentResolutionSize++;
            ChangeResolution();
        }

        else if (currentResolutionSize >= resolutionSize.size() - 1) currentResolutionSize = resolutionSize.size() - 1;

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

        for (auto& button : settingMenuButtons)
        {
            button.draw(&window);
        }

        settingsBackButton->draw(&window);
        window.draw(settingsBackText.LoadText());

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
        Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game", sf::State::Windowed);
    }

    // If fullscreen status is set to on, change the window state to fullscreen mode
    else if (fullscreenStatus[currentFullscreenStatus] == "On")
    {
        Engine::Instance()->GetWindow()->close();
        Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game", sf::State::Fullscreen);
    }
}