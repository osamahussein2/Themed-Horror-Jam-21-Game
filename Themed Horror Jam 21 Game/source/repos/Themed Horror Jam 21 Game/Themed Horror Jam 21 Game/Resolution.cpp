#include "Menu.h"
#include "Engine.h"

void Menu::InitializeScreenResolution()
{
    // 1920 x 1080
    if (Engine::Instance()->GetResolution() == NINETEEN_TWENTY_BY_TEN_EIGHTY)
    {
        resolutionSize.resize(16);
        resolutionSize = { EIGHT_HUNDRED_BY_SIX_HUNDRED, TEN_HUNDRED_TWENTY_FOUR_BY_SEVEN_SIXTY_EIGHT,
            ELEVEN_HUNDRED_FIFTY_TWO_BY_EIGHT_SIXTY_FOUR, TWELEVE_HUNDRED_EIGHTY_BY_SIX_HUNDRED,
            TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_TWENTY, TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_SIXTY_EIGHT,
            TWELEVE_HUNDRED_EIGHTY_BY_EIGHT_HUNDRED, TWELEVE_HUNDRED_EIGHTY_BY_NINE_SIXTY,
            TWELEVE_HUNDRED_EIGHTY_BY_TEN_HUNDRED_TWENTY_FOUR, THIRTEEN_HUNDRED_SIXTY_BY_SEVEN_SIXTY_EIGHT,
            THIRTEEN_HUNDRED_SIXTY_SIX_BY_SEVEN_SIXTY_EIGHT, FOURTEEN_HUNDRED_BY_TEN_FIFTY, FOURTEEN_FORTY_BY_NINE_HUNDRED,
            SIXTEEN_HUNDRED_BY_NINE_HUNDRED, SIXTEEN_EIGHTY_BY_TEN_FIFTY,
            Vector2u(Engine::Instance()->GetResolution().x, Engine::Instance()->GetResolution().y) };

        currentResolutionSize = 15;
    }

    // 1680 x 1050
    else if (Engine::Instance()->GetResolution() == SIXTEEN_EIGHTY_BY_TEN_FIFTY)
    {
        resolutionSize.resize(15);
        resolutionSize = { EIGHT_HUNDRED_BY_SIX_HUNDRED, TEN_HUNDRED_TWENTY_FOUR_BY_SEVEN_SIXTY_EIGHT,
            ELEVEN_HUNDRED_FIFTY_TWO_BY_EIGHT_SIXTY_FOUR, TWELEVE_HUNDRED_EIGHTY_BY_SIX_HUNDRED,
            TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_TWENTY, TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_SIXTY_EIGHT,
            TWELEVE_HUNDRED_EIGHTY_BY_EIGHT_HUNDRED, TWELEVE_HUNDRED_EIGHTY_BY_NINE_SIXTY,
            TWELEVE_HUNDRED_EIGHTY_BY_TEN_HUNDRED_TWENTY_FOUR, THIRTEEN_HUNDRED_SIXTY_BY_SEVEN_SIXTY_EIGHT,
            THIRTEEN_HUNDRED_SIXTY_SIX_BY_SEVEN_SIXTY_EIGHT, FOURTEEN_HUNDRED_BY_TEN_FIFTY, FOURTEEN_FORTY_BY_NINE_HUNDRED,
            SIXTEEN_HUNDRED_BY_NINE_HUNDRED, Vector2u(Engine::Instance()->GetResolution().x,
                Engine::Instance()->GetResolution().y) };

        currentResolutionSize = 14;
    }

    // 1600 x 900
    else if (Engine::Instance()->GetResolution() == SIXTEEN_HUNDRED_BY_NINE_HUNDRED)
    {
        resolutionSize.resize(14);
        resolutionSize = { EIGHT_HUNDRED_BY_SIX_HUNDRED, TEN_HUNDRED_TWENTY_FOUR_BY_SEVEN_SIXTY_EIGHT,
            ELEVEN_HUNDRED_FIFTY_TWO_BY_EIGHT_SIXTY_FOUR, TWELEVE_HUNDRED_EIGHTY_BY_SIX_HUNDRED,
            TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_TWENTY, TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_SIXTY_EIGHT,
            TWELEVE_HUNDRED_EIGHTY_BY_EIGHT_HUNDRED, TWELEVE_HUNDRED_EIGHTY_BY_NINE_SIXTY,
            TWELEVE_HUNDRED_EIGHTY_BY_TEN_HUNDRED_TWENTY_FOUR, THIRTEEN_HUNDRED_SIXTY_BY_SEVEN_SIXTY_EIGHT,
            THIRTEEN_HUNDRED_SIXTY_SIX_BY_SEVEN_SIXTY_EIGHT, FOURTEEN_HUNDRED_BY_TEN_FIFTY, FOURTEEN_FORTY_BY_NINE_HUNDRED,
            Vector2u(Engine::Instance()->GetResolution().x, Engine::Instance()->GetResolution().y) };

        currentResolutionSize = 13;
    }

    // 1440 x 900
    else if (Engine::Instance()->GetResolution() == FOURTEEN_FORTY_BY_NINE_HUNDRED)
    {
        resolutionSize.resize(13);
        resolutionSize = { EIGHT_HUNDRED_BY_SIX_HUNDRED, TEN_HUNDRED_TWENTY_FOUR_BY_SEVEN_SIXTY_EIGHT,
            ELEVEN_HUNDRED_FIFTY_TWO_BY_EIGHT_SIXTY_FOUR, TWELEVE_HUNDRED_EIGHTY_BY_SIX_HUNDRED,
            TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_TWENTY, TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_SIXTY_EIGHT,
            TWELEVE_HUNDRED_EIGHTY_BY_EIGHT_HUNDRED, TWELEVE_HUNDRED_EIGHTY_BY_NINE_SIXTY,
            TWELEVE_HUNDRED_EIGHTY_BY_TEN_HUNDRED_TWENTY_FOUR, THIRTEEN_HUNDRED_SIXTY_BY_SEVEN_SIXTY_EIGHT,
            THIRTEEN_HUNDRED_SIXTY_SIX_BY_SEVEN_SIXTY_EIGHT, FOURTEEN_HUNDRED_BY_TEN_FIFTY,
            Vector2u(Engine::Instance()->GetResolution().x, Engine::Instance()->GetResolution().y) };

        currentResolutionSize = 12;
    }

    // 1400 x 1050
    else if (Engine::Instance()->GetResolution() == FOURTEEN_HUNDRED_BY_TEN_FIFTY)
    {
        resolutionSize.resize(12);
        resolutionSize = { EIGHT_HUNDRED_BY_SIX_HUNDRED, TEN_HUNDRED_TWENTY_FOUR_BY_SEVEN_SIXTY_EIGHT,
            ELEVEN_HUNDRED_FIFTY_TWO_BY_EIGHT_SIXTY_FOUR, TWELEVE_HUNDRED_EIGHTY_BY_SIX_HUNDRED,
            TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_TWENTY, TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_SIXTY_EIGHT,
            TWELEVE_HUNDRED_EIGHTY_BY_EIGHT_HUNDRED, TWELEVE_HUNDRED_EIGHTY_BY_NINE_SIXTY,
            TWELEVE_HUNDRED_EIGHTY_BY_TEN_HUNDRED_TWENTY_FOUR, THIRTEEN_HUNDRED_SIXTY_BY_SEVEN_SIXTY_EIGHT,
            THIRTEEN_HUNDRED_SIXTY_SIX_BY_SEVEN_SIXTY_EIGHT,
            Vector2u(Engine::Instance()->GetResolution().x, Engine::Instance()->GetResolution().y) };

        currentResolutionSize = 11;
    }

    // 1366 x 758
    else if (Engine::Instance()->GetResolution() == THIRTEEN_HUNDRED_SIXTY_SIX_BY_SEVEN_SIXTY_EIGHT)
    {
        resolutionSize.resize(11);
        resolutionSize = { EIGHT_HUNDRED_BY_SIX_HUNDRED, TEN_HUNDRED_TWENTY_FOUR_BY_SEVEN_SIXTY_EIGHT,
            ELEVEN_HUNDRED_FIFTY_TWO_BY_EIGHT_SIXTY_FOUR, TWELEVE_HUNDRED_EIGHTY_BY_SIX_HUNDRED,
            TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_TWENTY, TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_SIXTY_EIGHT,
            TWELEVE_HUNDRED_EIGHTY_BY_EIGHT_HUNDRED, TWELEVE_HUNDRED_EIGHTY_BY_NINE_SIXTY,
            TWELEVE_HUNDRED_EIGHTY_BY_TEN_HUNDRED_TWENTY_FOUR, THIRTEEN_HUNDRED_SIXTY_BY_SEVEN_SIXTY_EIGHT,
            Vector2u(Engine::Instance()->GetResolution().x, Engine::Instance()->GetResolution().y) };

        currentResolutionSize = 10;
    }

    // 1360 x 768
    else if (Engine::Instance()->GetResolution() == THIRTEEN_HUNDRED_SIXTY_BY_SEVEN_SIXTY_EIGHT)
    {
        resolutionSize.resize(10);
        resolutionSize = { EIGHT_HUNDRED_BY_SIX_HUNDRED, TEN_HUNDRED_TWENTY_FOUR_BY_SEVEN_SIXTY_EIGHT,
            ELEVEN_HUNDRED_FIFTY_TWO_BY_EIGHT_SIXTY_FOUR, TWELEVE_HUNDRED_EIGHTY_BY_SIX_HUNDRED,
            TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_TWENTY, TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_SIXTY_EIGHT,
            TWELEVE_HUNDRED_EIGHTY_BY_EIGHT_HUNDRED, TWELEVE_HUNDRED_EIGHTY_BY_NINE_SIXTY,
            TWELEVE_HUNDRED_EIGHTY_BY_TEN_HUNDRED_TWENTY_FOUR,
            Vector2u(Engine::Instance()->GetResolution().x, Engine::Instance()->GetResolution().y) };

        currentResolutionSize = 9;
    }

    // 1280 x 1024
    else if (Engine::Instance()->GetResolution() == TWELEVE_HUNDRED_EIGHTY_BY_TEN_HUNDRED_TWENTY_FOUR)
    {
        resolutionSize.resize(9);
        resolutionSize = { EIGHT_HUNDRED_BY_SIX_HUNDRED, TEN_HUNDRED_TWENTY_FOUR_BY_SEVEN_SIXTY_EIGHT,
            ELEVEN_HUNDRED_FIFTY_TWO_BY_EIGHT_SIXTY_FOUR, TWELEVE_HUNDRED_EIGHTY_BY_SIX_HUNDRED,
            TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_TWENTY, TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_SIXTY_EIGHT,
            TWELEVE_HUNDRED_EIGHTY_BY_EIGHT_HUNDRED, TWELEVE_HUNDRED_EIGHTY_BY_NINE_SIXTY,
            Vector2u(Engine::Instance()->GetResolution().x, Engine::Instance()->GetResolution().y) };

        currentResolutionSize = 8;
    }

    // 1280 x 960
    else if (Engine::Instance()->GetResolution() == TWELEVE_HUNDRED_EIGHTY_BY_NINE_SIXTY)
    {
        resolutionSize.resize(8);
        resolutionSize = { EIGHT_HUNDRED_BY_SIX_HUNDRED, TEN_HUNDRED_TWENTY_FOUR_BY_SEVEN_SIXTY_EIGHT,
            ELEVEN_HUNDRED_FIFTY_TWO_BY_EIGHT_SIXTY_FOUR, TWELEVE_HUNDRED_EIGHTY_BY_SIX_HUNDRED,
            TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_TWENTY, TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_SIXTY_EIGHT,
            TWELEVE_HUNDRED_EIGHTY_BY_EIGHT_HUNDRED,
            Vector2u(Engine::Instance()->GetResolution().x, Engine::Instance()->GetResolution().y) };

        currentResolutionSize = 7;
    }

    // 1280 x 800
    else if (Engine::Instance()->GetResolution() == TWELEVE_HUNDRED_EIGHTY_BY_EIGHT_HUNDRED)
    {
        resolutionSize.resize(7);
        resolutionSize = { EIGHT_HUNDRED_BY_SIX_HUNDRED, TEN_HUNDRED_TWENTY_FOUR_BY_SEVEN_SIXTY_EIGHT,
            ELEVEN_HUNDRED_FIFTY_TWO_BY_EIGHT_SIXTY_FOUR, TWELEVE_HUNDRED_EIGHTY_BY_SIX_HUNDRED,
            TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_TWENTY, TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_SIXTY_EIGHT,
            Vector2u(Engine::Instance()->GetResolution().x, Engine::Instance()->GetResolution().y) };

        currentResolutionSize = 6;
    }

    // 1280 x 768
    else if (Engine::Instance()->GetResolution() == TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_SIXTY_EIGHT)
    {
        resolutionSize.resize(6);
        resolutionSize = { EIGHT_HUNDRED_BY_SIX_HUNDRED, TEN_HUNDRED_TWENTY_FOUR_BY_SEVEN_SIXTY_EIGHT,
            ELEVEN_HUNDRED_FIFTY_TWO_BY_EIGHT_SIXTY_FOUR, TWELEVE_HUNDRED_EIGHTY_BY_SIX_HUNDRED,
            TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_TWENTY,
            Vector2u(Engine::Instance()->GetResolution().x, Engine::Instance()->GetResolution().y) };

        currentResolutionSize = 5;
    }

    // 1280 x 720
    else if (Engine::Instance()->GetResolution() == TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_TWENTY)
    {
        resolutionSize.resize(5);
        resolutionSize = { EIGHT_HUNDRED_BY_SIX_HUNDRED, TEN_HUNDRED_TWENTY_FOUR_BY_SEVEN_SIXTY_EIGHT,
            ELEVEN_HUNDRED_FIFTY_TWO_BY_EIGHT_SIXTY_FOUR, TWELEVE_HUNDRED_EIGHTY_BY_SIX_HUNDRED,
            Vector2u(Engine::Instance()->GetResolution().x, Engine::Instance()->GetResolution().y) };

        currentResolutionSize = 4;
    }

    // 1280 x 600
    else if (Engine::Instance()->GetResolution() == TWELEVE_HUNDRED_EIGHTY_BY_SIX_HUNDRED)
    {
        resolutionSize.resize(4);
        resolutionSize = { EIGHT_HUNDRED_BY_SIX_HUNDRED, TEN_HUNDRED_TWENTY_FOUR_BY_SEVEN_SIXTY_EIGHT,
            ELEVEN_HUNDRED_FIFTY_TWO_BY_EIGHT_SIXTY_FOUR,
            Vector2u(Engine::Instance()->GetResolution().x, Engine::Instance()->GetResolution().y) };

        currentResolutionSize = 3;
    }

    // 1152 x 864
    else if (Engine::Instance()->GetResolution() == ELEVEN_HUNDRED_FIFTY_TWO_BY_EIGHT_SIXTY_FOUR)
    {
        resolutionSize.resize(3);
        resolutionSize = { EIGHT_HUNDRED_BY_SIX_HUNDRED, TEN_HUNDRED_TWENTY_FOUR_BY_SEVEN_SIXTY_EIGHT,
            Vector2u(Engine::Instance()->GetResolution().x, Engine::Instance()->GetResolution().y) };

        currentResolutionSize = 2;
    }

    // 1024 x 768
    else if (Engine::Instance()->GetResolution() == TEN_HUNDRED_TWENTY_FOUR_BY_SEVEN_SIXTY_EIGHT)
    {
        resolutionSize.resize(2);
        resolutionSize = { EIGHT_HUNDRED_BY_SIX_HUNDRED,
            Vector2u(Engine::Instance()->GetResolution().x, Engine::Instance()->GetResolution().y) };

        currentResolutionSize = 1;
    }

    // 800 x 600
    else if (Engine::Instance()->GetResolution() == EIGHT_HUNDRED_BY_SIX_HUNDRED)
    {
        resolutionSize.resize(1);
        resolutionSize = { Vector2u(Engine::Instance()->GetResolution().x, Engine::Instance()->GetResolution().y) };

        currentResolutionSize = 0;
    }

    // If not equal to any of the screen resolutions set above
    else if (Engine::Instance()->GetResolution() != NINETEEN_TWENTY_BY_TEN_EIGHTY &&
        Engine::Instance()->GetResolution() != SIXTEEN_EIGHTY_BY_TEN_FIFTY &&
        Engine::Instance()->GetResolution() != SIXTEEN_HUNDRED_BY_NINE_HUNDRED &&
        Engine::Instance()->GetResolution() != FOURTEEN_FORTY_BY_NINE_HUNDRED &&
        Engine::Instance()->GetResolution() != FOURTEEN_HUNDRED_BY_TEN_FIFTY &&
        Engine::Instance()->GetResolution() != THIRTEEN_HUNDRED_SIXTY_SIX_BY_SEVEN_SIXTY_EIGHT &&
        Engine::Instance()->GetResolution() != THIRTEEN_HUNDRED_SIXTY_BY_SEVEN_SIXTY_EIGHT &&
        Engine::Instance()->GetResolution() != TWELEVE_HUNDRED_EIGHTY_BY_TEN_HUNDRED_TWENTY_FOUR &&
        Engine::Instance()->GetResolution() != TWELEVE_HUNDRED_EIGHTY_BY_NINE_SIXTY &&
        Engine::Instance()->GetResolution() != TWELEVE_HUNDRED_EIGHTY_BY_EIGHT_HUNDRED &&
        Engine::Instance()->GetResolution() != TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_SIXTY_EIGHT &&
        Engine::Instance()->GetResolution() != TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_TWENTY &&
        Engine::Instance()->GetResolution() != TWELEVE_HUNDRED_EIGHTY_BY_SIX_HUNDRED &&
        Engine::Instance()->GetResolution() != ELEVEN_HUNDRED_FIFTY_TWO_BY_EIGHT_SIXTY_FOUR &&
        Engine::Instance()->GetResolution() != TEN_HUNDRED_TWENTY_FOUR_BY_SEVEN_SIXTY_EIGHT &&
        Engine::Instance()->GetResolution() != EIGHT_HUNDRED_BY_SIX_HUNDRED)
    {
        resolutionSize.resize(1);
        resolutionSize = { Vector2u(Engine::Instance()->GetResolution().x, Engine::Instance()->GetResolution().y) };

        currentResolutionSize = 0;
    }
}

void Menu::ChangeResolution()
{
    // 800 x 600
    if (resolutionSize[currentResolutionSize] == EIGHT_HUNDRED_BY_SIX_HUNDRED)
    {
        resolution = EIGHT_HUNDRED_BY_SIX_HUNDRED;

        Engine::Instance()->SetResolution(resolution);
        Engine::Instance()->SetMainMenuView(Vector2f(0.0f, 0.0f), Vector2f(resolution));

        UpdateMainMenuColors();
        UpdateSettingsMenuColors();

        if (fullscreenStatus[currentFullscreenStatus] == "On")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Fullscreen);
        }

        else if (fullscreenStatus[currentFullscreenStatus] == "Off")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Windowed);
        }
    }

    // 1024 x 768
    else if (resolutionSize[currentResolutionSize] == TEN_HUNDRED_TWENTY_FOUR_BY_SEVEN_SIXTY_EIGHT)
    {
        resolution = TEN_HUNDRED_TWENTY_FOUR_BY_SEVEN_SIXTY_EIGHT;

        Engine::Instance()->SetResolution(resolution);
        Engine::Instance()->SetMainMenuView(Vector2f(0.0f, 0.0f), Vector2f(resolution));

        UpdateMainMenuColors();
        UpdateSettingsMenuColors();

        if (fullscreenStatus[currentFullscreenStatus] == "On")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Fullscreen);
        }

        else if (fullscreenStatus[currentFullscreenStatus] == "Off")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Windowed);
        }
    }

    // 1152 x 864
    else if (resolutionSize[currentResolutionSize] == ELEVEN_HUNDRED_FIFTY_TWO_BY_EIGHT_SIXTY_FOUR)
    {
        resolution = ELEVEN_HUNDRED_FIFTY_TWO_BY_EIGHT_SIXTY_FOUR;

        Engine::Instance()->SetResolution(resolution);
        Engine::Instance()->SetMainMenuView(Vector2f(0.0f, 0.0f), Vector2f(resolution));

        UpdateMainMenuColors();
        UpdateSettingsMenuColors();

        if (fullscreenStatus[currentFullscreenStatus] == "On")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Fullscreen);
        }

        else if (fullscreenStatus[currentFullscreenStatus] == "Off")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Windowed);
        }
    }

    // 1280 x 600
    else if (resolutionSize[currentResolutionSize] == TWELEVE_HUNDRED_EIGHTY_BY_SIX_HUNDRED)
    {
        resolution = TWELEVE_HUNDRED_EIGHTY_BY_SIX_HUNDRED;

        Engine::Instance()->SetResolution(resolution);
        Engine::Instance()->SetMainMenuView(Vector2f(0.0f, 0.0f), Vector2f(resolution));

        UpdateMainMenuColors();
        UpdateSettingsMenuColors();

        if (fullscreenStatus[currentFullscreenStatus] == "On")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Fullscreen);
        }

        else if (fullscreenStatus[currentFullscreenStatus] == "Off")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Windowed);
        }
    }

    // 1280 x 720
    else if (resolutionSize[currentResolutionSize] == TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_TWENTY)
    {
        resolution = TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_TWENTY;

        Engine::Instance()->SetResolution(resolution);
        Engine::Instance()->SetMainMenuView(Vector2f(0.0f, 0.0f), Vector2f(resolution));

        UpdateMainMenuColors();
        UpdateSettingsMenuColors();

        if (fullscreenStatus[currentFullscreenStatus] == "On")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Fullscreen);
        }

        else if (fullscreenStatus[currentFullscreenStatus] == "Off")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Windowed);
        }
    }

    // 1280 x 768
    else if (resolutionSize[currentResolutionSize] == TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_SIXTY_EIGHT)
    {
        resolution = TWELEVE_HUNDRED_EIGHTY_BY_SEVEN_SIXTY_EIGHT;

        Engine::Instance()->SetResolution(resolution);
        Engine::Instance()->SetMainMenuView(Vector2f(0.0f, 0.0f), Vector2f(resolution));

        UpdateMainMenuColors();
        UpdateSettingsMenuColors();

        if (fullscreenStatus[currentFullscreenStatus] == "On")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Fullscreen);
        }

        else if (fullscreenStatus[currentFullscreenStatus] == "Off")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Windowed);
        }
    }

    // 1280 x 800
    else if (resolutionSize[currentResolutionSize] == TWELEVE_HUNDRED_EIGHTY_BY_EIGHT_HUNDRED)
    {
        resolution = TWELEVE_HUNDRED_EIGHTY_BY_EIGHT_HUNDRED;

        Engine::Instance()->SetResolution(resolution);
        Engine::Instance()->SetMainMenuView(Vector2f(0.0f, 0.0f), Vector2f(resolution));

        UpdateMainMenuColors();
        UpdateSettingsMenuColors();

        if (fullscreenStatus[currentFullscreenStatus] == "On")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Fullscreen);
        }

        else if (fullscreenStatus[currentFullscreenStatus] == "Off")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Windowed);
        }
    }

    // 1280 x 960
    else if (resolutionSize[currentResolutionSize] == TWELEVE_HUNDRED_EIGHTY_BY_NINE_SIXTY)
    {
        resolution = TWELEVE_HUNDRED_EIGHTY_BY_NINE_SIXTY;

        Engine::Instance()->SetResolution(resolution);
        Engine::Instance()->SetMainMenuView(Vector2f(0.0f, 0.0f), Vector2f(resolution));

        UpdateMainMenuColors();
        UpdateSettingsMenuColors();

        if (fullscreenStatus[currentFullscreenStatus] == "On")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Fullscreen);
        }

        else if (fullscreenStatus[currentFullscreenStatus] == "Off")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Windowed);
        }
    }

    // 1280 x 1024
    else if (resolutionSize[currentResolutionSize] == TWELEVE_HUNDRED_EIGHTY_BY_TEN_HUNDRED_TWENTY_FOUR)
    {
        resolution = TWELEVE_HUNDRED_EIGHTY_BY_TEN_HUNDRED_TWENTY_FOUR;

        Engine::Instance()->SetResolution(resolution);
        Engine::Instance()->SetMainMenuView(Vector2f(0.0f, 0.0f), Vector2f(resolution));

        UpdateMainMenuColors();
        UpdateSettingsMenuColors();

        if (fullscreenStatus[currentFullscreenStatus] == "On")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Fullscreen);
        }

        else if (fullscreenStatus[currentFullscreenStatus] == "Off")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Windowed);
        }
    }

    // 1360 x 768
    else if (resolutionSize[currentResolutionSize] == THIRTEEN_HUNDRED_SIXTY_BY_SEVEN_SIXTY_EIGHT)
    {
        resolution = THIRTEEN_HUNDRED_SIXTY_BY_SEVEN_SIXTY_EIGHT;

        Engine::Instance()->SetResolution(resolution);
        Engine::Instance()->SetMainMenuView(Vector2f(0.0f, 0.0f), Vector2f(resolution));

        UpdateMainMenuColors();
        UpdateSettingsMenuColors();

        if (fullscreenStatus[currentFullscreenStatus] == "On")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Fullscreen);
        }

        else if (fullscreenStatus[currentFullscreenStatus] == "Off")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Windowed);
        }
    }

    // 1366 x 768
    else if (resolutionSize[currentResolutionSize] == THIRTEEN_HUNDRED_SIXTY_SIX_BY_SEVEN_SIXTY_EIGHT)
    {
        resolution = THIRTEEN_HUNDRED_SIXTY_SIX_BY_SEVEN_SIXTY_EIGHT;

        Engine::Instance()->SetResolution(resolution);
        Engine::Instance()->SetMainMenuView(Vector2f(0.0f, 0.0f), Vector2f(resolution));

        UpdateMainMenuColors();
        UpdateSettingsMenuColors();

        if (fullscreenStatus[currentFullscreenStatus] == "On")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Fullscreen);
        }

        else if (fullscreenStatus[currentFullscreenStatus] == "Off")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Windowed);
        }
    }

    // 1400 x 1050
    else if (resolutionSize[currentResolutionSize] == FOURTEEN_HUNDRED_BY_TEN_FIFTY)
    {
        resolution = FOURTEEN_HUNDRED_BY_TEN_FIFTY;

        Engine::Instance()->SetResolution(resolution);
        Engine::Instance()->SetMainMenuView(Vector2f(0.0f, 0.0f), Vector2f(resolution));

        UpdateMainMenuColors();
        UpdateSettingsMenuColors();

        if (fullscreenStatus[currentFullscreenStatus] == "On")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Fullscreen);
        }

        else if (fullscreenStatus[currentFullscreenStatus] == "Off")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Windowed);
        }
    }

    // 1440 x 900
    else if (resolutionSize[currentResolutionSize] == FOURTEEN_FORTY_BY_NINE_HUNDRED)
    {
        resolution = FOURTEEN_FORTY_BY_NINE_HUNDRED;

        Engine::Instance()->SetResolution(resolution);
        Engine::Instance()->SetMainMenuView(Vector2f(0.0f, 0.0f), Vector2f(resolution));

        UpdateMainMenuColors();
        UpdateSettingsMenuColors();

        if (fullscreenStatus[currentFullscreenStatus] == "On")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Fullscreen);
        }

        else if (fullscreenStatus[currentFullscreenStatus] == "Off")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Windowed);
        }
    }

    // 1600 x 900
    else if (resolutionSize[currentResolutionSize] == SIXTEEN_HUNDRED_BY_NINE_HUNDRED)
    {
        resolution = SIXTEEN_HUNDRED_BY_NINE_HUNDRED;

        Engine::Instance()->SetResolution(resolution);
        Engine::Instance()->SetMainMenuView(Vector2f(0.0f, 0.0f), Vector2f(resolution));

        UpdateMainMenuColors();
        UpdateSettingsMenuColors();

        if (fullscreenStatus[currentFullscreenStatus] == "On")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Fullscreen);
        }

        else if (fullscreenStatus[currentFullscreenStatus] == "Off")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Windowed);
        }
    }

    // 1680 x 1050
    else if (resolutionSize[currentResolutionSize] == SIXTEEN_EIGHTY_BY_TEN_FIFTY)
    {
        resolution = SIXTEEN_EIGHTY_BY_TEN_FIFTY;

        Engine::Instance()->SetResolution(resolution);
        Engine::Instance()->SetMainMenuView(Vector2f(0.0f, 0.0f), Vector2f(resolution));

        UpdateMainMenuColors();
        UpdateSettingsMenuColors();

        if (fullscreenStatus[currentFullscreenStatus] == "On")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Fullscreen);
        }

        else if (fullscreenStatus[currentFullscreenStatus] == "Off")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Windowed);
        }
    }

    // 1920 x 1080
    else if (resolutionSize[currentResolutionSize] == NINETEEN_TWENTY_BY_TEN_EIGHTY)
    {
        resolution = NINETEEN_TWENTY_BY_TEN_EIGHTY;

        Engine::Instance()->SetResolution(resolution);
        Engine::Instance()->SetMainMenuView(Vector2f(0.0f, 0.0f), Vector2f(resolution));

        UpdateMainMenuColors();
        UpdateSettingsMenuColors();

        if (fullscreenStatus[currentFullscreenStatus] == "On")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Fullscreen);
        }

        else if (fullscreenStatus[currentFullscreenStatus] == "Off")
        {
            Engine::Instance()->GetWindow()->close();

            Engine::Instance()->GetWindow()->create(sf::VideoMode(resolution), "Themed Horror Jam 21 Game",
                sf::State::Windowed);
        }
    }
}