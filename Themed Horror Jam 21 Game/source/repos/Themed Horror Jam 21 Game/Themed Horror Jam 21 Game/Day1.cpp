#include "GameScene.h"
#include "Engine.h"

float DIALOGUE_TEXT_CHARACTER_SIZE;

void GameScene::InitializeDay1()
{
    if (timeRanOut != false) timeRanOut = false;

    // Reset to initial state
    currentGameState = GameState::DIALOGUE_ACTIVE;

    // Clear previous dialogue texts
    for (int i = 0; i < dialogueTexts.size(); i++)
    {
        delete dialogueTexts[i];
        dialogueTexts[i] = nullptr;
    }
    dialogueTexts.clear();

    // Stop any running timer when reinitializing
    if (surgeryRoom.IsLoaded())
    {
        surgeryRoom.StopTimer();
    }

    if (inputCooldown != INPUT_DELAY) inputCooldown = INPUT_DELAY;
    if (currentDialogueIndex != 0) currentDialogueIndex = 0;
    if (typeTextTime != 0.0f) typeTextTime = 0.0f;
    if (skippedTypewriting != false) skippedTypewriting = false;

    if (!gameBackground.IsLoaded())
    {
        gameBackground.Initialize("Art Assets/Background.jpg", resolution);
    }

    if (!dialoguePanel)
    {
        dialoguePanel = new DialoguePanel();

        float panelWidth = 1820.0f * (resolution.x / 1920.0f);
        float panelHeight = 700.0f * (resolution.y / 1080.0f);

        float panelX = resolution.x / 35.0f;
        float panelY = resolution.y / 3.0f;

        dialoguePanelTextures = { "Art Assets/Ui/chat_box_0.png", "Art Assets/Ui/chat_box_1.png",
            "Art Assets/Ui/chat_box_2.png", "Art Assets/Ui/chat_box_3.png", "Art Assets/Ui/chat_box_4.png" };

        dialoguePanel->InitializeDialoguePanel("Art Assets/Ui/chat_box_3.png", Vector2f(panelX, panelY),
            Vector2f(panelWidth, panelHeight));
    }

    dialogueTexts.clear();
    dialogueTexts.resize(maxDialogueTexts);

    for (int i = 0; i < dialogueTexts.size(); i++)
    {
        if (!dialogueTexts[i])
        {
            dialogueTexts[i] = new Game::Text();
        }
    }

    if (!dialogueSystemInitialized)
    {
        InitializeDialogueSystem();
        dialogueSystemInitialized = true;
    }

    float textPanelX = resolution.x / 25.0f; // Move text panels slightly to the right from the dialogue panel
    float textPanelY = resolution.y / 1.4f; // Move text panels slightly below from the dialogue panel

    DIALOGUE_TEXT_CHARACTER_SIZE = 50.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

    dialogueTexts[0]->InitializeText("Fonts/Roboto-Regular.ttf", DIALOGUE_TEXT_CHARACTER_SIZE, false, false,
        sf::Color::White, Vector2f(textPanelX, textPanelY));

    dialogueTexts[1]->InitializeText("Fonts/Roboto-Regular.ttf", DIALOGUE_TEXT_CHARACTER_SIZE, false, false,
        sf::Color::White, Vector2f(textPanelX, textPanelY));

    dialogueTexts[2]->InitializeText("Fonts/Roboto-Regular.ttf", DIALOGUE_TEXT_CHARACTER_SIZE, false, false,
        sf::Color::White, Vector2f(textPanelX, textPanelY));
}

void GameScene::UpdateDay1(float deltaTime)
{
    // Update input cooldown
    if (inputCooldown > 0.0f)
    {
        inputCooldown -= deltaTime;
    }

    // Debugging purposes
    /*if (Keyboard::isKeyPressed(Keyboard::Key::P))
    {
        sceneManager->ChangeScene("Menu");
        currentDay = 2;
        return;
    }*/

    // Check for escape key to return to menu
    if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
    {
        typewriterEffect.Reset();
        sceneManager->ChangeScene("Menu");
        return;
    }

    // Check for inventory toggle (I key)
    if (Keyboard::isKeyPressed(Keyboard::Key::I) && inputCooldown <= 0.0f && !timeRanOut)
    {
        inputCooldown = INPUT_DELAY;
        if (bag.IsVisible())
        {
            bag.Hide();
        }
        else
        {
            bag.Show();
        }
    }

    // Update the timer in all relevant states (once the surgery room is loaded and timer is running)
    if (surgeryRoom.IsLoaded() && surgeryRoom.IsTimerRunning() && !timeRanOut)
    {
        if (failedTextAlpha != 255.0f) failedTextAlpha = 255.0f;
        if (failedTimer != 0.0f) failedTimer = 0.0f;

        surgeryRoom.UpdateTimer(deltaTime);

        if (surgeryRoom.GetTimeRemaining() <= 0.0f)
        {
            if (timeRanOut != true) timeRanOut = true;
        }
    }

    // Else if time runs out
    if (timeRanOut == true)
    {
        float dayFailedCharacterSize = 40.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

        // If failed timer is less than 1 second, initialize the failed text and increment the failed timer for text fade
        if (failedTimer < 1.0f) 
        { 
            failedTimer += deltaTime; 

            dayFailedText.InitializeText("Fonts/Roboto-Regular.ttf", "You failed!", dayFailedCharacterSize,
                true, false, Color(255, 0, 0, failedTextAlpha), Vector2f(resolution.x / 2.0f, resolution.y / 2.0f));
        }

        // If failed timer is around 1 second or so, make the text fade away overtime
        else if (failedTimer >= 1.0f)
        {
            failedTextAlpha -= deltaTime * 100.0f;

            if (dayFailedText.GetTextColor() != Color(255, 0, 0, failedTextAlpha))
                dayFailedText.SetTextColor(Color(255, 0, 0, failedTextAlpha));
        }

        //printf("failed timer: %f\n", failedTextAlpha);

        // If failed text's alpha value goes down to 0, go back to main menu
        if (failedTextAlpha <= 0.0f)
        {
            typewriterEffect.Reset();
            sceneManager->ChangeScene("Menu");
            return;
        }
    }

    // Get mouse position for click detection
    Vector2i mousePixelPos = Mouse::getPosition(*Engine::Instance()->GetWindow());
    Vector2f mousePos = Engine::Instance()->GetWindow()->mapPixelToCoords(mousePixelPos);

    // Check for bag sprite click (only when surgery room is active)
    if (currentGameState == GameState::SURGERY_ROOM_ACTIVE && !timeRanOut || 
        currentGameState == GameState::OPERATION_ACTIVE && !timeRanOut)
    {
        if (surgeryRoom.BagSprite.getGlobalBounds().contains(mousePos))
        {
            if (surgeryRoom.BagSprite.getColor() != Color::Red)
                surgeryRoom.BagSprite.setColor(Color::Red);

            if (Mouse::isButtonPressed(Mouse::Button::Left) && inputCooldown <= 0.0f)
            {
                if (bag.IsVisible())
                {
                    bag.Hide();
                }
                else
                {
                    bag.Show();
                }
                inputCooldown = INPUT_DELAY;
            }
        }
        else
        {
            // Reset color when not hovering
            if (surgeryRoom.BagSprite.getColor() != Color::White)
                surgeryRoom.BagSprite.setColor(Color::White);
        }
    }

    if (!timeRanOut)
    {
        // Update based on current game state
        switch (currentGameState)
        {
        case GameState::DIALOGUE_ACTIVE:
        {
            // Update the typewriter effect and dialogue panel textures
            typewriterEffect.Update(deltaTime);
            UpdateDialoguePanelTexture();

            if (currentDialogueIndex >= 0 && currentDialogueIndex < static_cast<int>(dialogueTexts.size())
                && dialogueTexts[currentDialogueIndex])
            {
                std::string currentText = typewriterEffect.GetCurrentText();
                dialogueTexts[currentDialogueIndex]->SetTypewriterString(sf::String(currentText));
            }

            // Handle input for dialogue
            if (Keyboard::isKeyPressed(Keyboard::Key::Enter) && inputCooldown <= 0.0f)
            {
                if (!typewriterEffect.IsCurrentDialogueComplete())
                {
                    typewriterEffect.Skip();
                }
                else if (typewriterEffect.HasNextDialogue())
                {
                    typewriterEffect.NextDialogue();
                    currentDialogueIndex = typewriterEffect.GetCurrentDialogueIndex();
                }
                else
                {
                    currentGameState = GameState::DIALOGUE_HIDDEN;
                    // Activate surgery room when dialogue ends
                    gameBackground.Unload();

                    if (!surgeryRoom.IsLoaded())
                    {
                        // Initialize surgery room with your asset paths
                        surgeryRoom.Initialize(
                            "Art Assets/SurgeryRoom/Background.png",
                            "Art Assets/SurgeryRoom/BottomUI.png",
                            "Art Assets/SurgeryRoom/TopUI.png",
                            resolution,
                            sf::Vector2f(resolution.x / 1920.0f, resolution.y / 1080.0f),//size
                            sf::Vector2f(0.0f, resolution.y / 1.35f), // bottom UI
                            sf::Vector2f(resolution.x / 2.0f, resolution.y / 10.0f), // top UI
                            sf::Vector2f(resolution.x / 7.0f, resolution.y / 1.15f), // life sprite 0 position
                            sf::Vector2f(resolution.x / 5.5f, resolution.y / 1.15f), // life sprite 1 position
                            sf::Vector2f(resolution.x / 4.5f, resolution.y / 1.15f), // life sprite 2 position
                            sf::Vector2f(resolution.x / 7.0f, resolution.y / 1.15f), // death sprite 0 position
                            sf::Vector2f(resolution.x / 5.5f, resolution.y / 1.15f), // death sprite 1 position
                            sf::Vector2f(resolution.x / 4.5f, resolution.y / 1.15f), // death sprite 2 position
                            sf::Vector2f(resolution.x / 25.0f, resolution.y / 1.235f), // timer sprite position
                            sf::Vector2f(resolution.x / 1.27f, resolution.y / 1.15f),// notes sprite position
                            sf::Vector2f(resolution.x / 1.43f, resolution.y / 1.15f), // bag sprite position
                            sf::Vector2f(resolution.x / 1.13f, resolution.y / 1.15f),// table UI sprite position
                            sf::Vector2f(resolution.x / 1.53f, resolution.y / 1.22f));// OperationTableSprite UI sprite position

                        if (!surgeryRoom.IsTimerRunning())
                        {
                            // Start the timer when operation scene becomes active
                            surgeryRoom.StartTimer(57.0f); // Start with 57 seconds, adjust as needed
                        }
                    }

                    operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                        Vector2f(resolution.x / 2.8f, 0.0f),
                        Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                        true);

                    operationScene.maxDots = 4;

                    operationScene.InitializeDot(Vector2f(resolution.x / 2.25f, resolution.y / 4.0f),
                        10.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2), Color::Red, Color::Red,
                        5.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2),
                        50.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2),
                        0.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2));

                    person.InitializeSprite("Art Assets/SurgeryRoom/sickness/basebody.png", Vector2f(resolution.x / 2.238f,
                        resolution.y / 2.5f), sf::Vector2f(resolution.x / 1920.0f, resolution.y / 1080.0f));

                    // Initialize the bag when surgery room becomes active
                    InitializeBag();

                    currentGameState = GameState::SURGERY_ROOM_ACTIVE;
                    alpha = 255.0f;
                }

                inputCooldown = INPUT_DELAY;
            }
            break;
        }

        case GameState::SURGERY_ROOM_ACTIVE:
        {
            // Handle bag clicks when visible
            if (bag.IsVisible())
            {
                HandleBagClicks(mousePos);
            }

            if (!alphaIncrease) alpha -= 50.0f * deltaTime;
            else if (alphaIncrease) alpha += 50.0f * deltaTime;

            if (alpha >= 255.0f) alphaIncrease = false;
            else if (alpha <= 125.0f) alphaIncrease = true;

            if (surgeryRoom.TableUISprite.getGlobalBounds().contains(mousePos))
            {
                if (surgeryRoom.TableUISprite.getColor() != Color::Red)
                    surgeryRoom.TableUISprite.setColor(Color::Red);

                // Add input cooldown check here
                if (Mouse::isButtonPressed(Mouse::Button::Left) && inputCooldown <= 0.0f)
                {
                    std::cout << "TopUI clicked! Changing to ITEM_TABLE_ACTIVE" << std::endl;

                    itemTable.Initialize("Art Assets/SurgeryRoom/items_table/table.png",
                        Vector2f((resolution.x / 2.0f) - 990.0f, (resolution.y / 2.0f) - 500.0f),
                        Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                        true);

                    // Change to ITEM_TABLE_ACTIVE
                    currentGameState = GameState::ITEM_TABLE_ACTIVE;
                    inputCooldown = INPUT_DELAY;
                    std::cout << "State changed to: " << static_cast<int>(currentGameState) << std::endl;
                }
            }
            else
            {
                // Reset color when not hovering
                if (surgeryRoom.TableUISprite.getColor() != Color::White)
                    surgeryRoom.TableUISprite.setColor(Color::White);
            }

            // Handle operation table clicks
            if (surgeryRoom.OperationTableSprite.getGlobalBounds().contains(mousePos))
            {
                if (alpha != 255.0f) alpha = 255.0f;
                if (surgeryRoom.OperationTableSprite.getColor() != Color::Red)
                    surgeryRoom.OperationTableSprite.setColor(Color::Red);

                // Set up the operation scene after clicking the left mouse button
                if (Mouse::isButtonPressed(Mouse::Button::Left) && inputCooldown <= 0.0f)
                {
                    currentGameState = GameState::OPERATION_ACTIVE;

                    operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                        Vector2f(resolution.x / 2.8f, 0.0f),
                        Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                        true);
                    if (!surgeryRoom.IsTimerRunning())
                    {
                        // Start the timer when operation scene becomes active
                        surgeryRoom.StartTimer(57.0f); // Start with 57 seconds, adjust as needed
                    }
                    inputCooldown = INPUT_DELAY;
                }
            }
            // Otherwise, reset the sprite's color back to white once the mouse is no longer hovering on the sprite
            else if (!surgeryRoom.OperationTableSprite.getGlobalBounds().contains(mousePos))
            {
                if (surgeryRoom.OperationTableSprite.getColor() != Color(255, 255, 255, alpha))
                    surgeryRoom.OperationTableSprite.setColor(Color(255, 255, 255, alpha));
            }
            break;
        }

        case GameState::OPERATION_ACTIVE:
        {
            // Handle bag clicks when visible
            if (bag.IsVisible())
            {
                HandleBagClicks(mousePos);
            }

            // Handle input for operation scene
            if (Keyboard::isKeyPressed(Keyboard::Key::Enter) && inputCooldown <= 0.0f)
            {
                // Return to surgery room when Enter is pressed
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
                inputCooldown = INPUT_DELAY;
            }

            // Also allow clicking outside to close (optional)
            if (Mouse::isButtonPressed(Mouse::Button::Right) && inputCooldown <= 0.0f)
            {
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
                inputCooldown = INPUT_DELAY;
            }

            for (int i = 0; i < operationScene.maxDots; i++)
            {
                if (operationScene.dotCircleShape[i].getGlobalBounds().contains(mousePos))
                {
                    // Set up the operation scene after clicking the left mouse button
                    if (Mouse::isButtonPressed(Mouse::Button::Left))
                    {
                        if (operationScene.dotCircleShape[i].getFillColor() != Color::Green)
                            operationScene.dotCircleShape[i].setFillColor(Color::Green);
                    }
                }
            }

            if (operationScene.dotCircleShape[0].getFillColor() == Color::Green &&
                operationScene.dotCircleShape[1].getFillColor() == Color::Green &&
                operationScene.dotCircleShape[2].getFillColor() == Color::Green &&
                operationScene.dotCircleShape[3].getFillColor() == Color::Green)
            {
                successfulOperationTime += deltaTime;

                float successfulCharacterSize = 30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

                operationScene.InitializeSuccessPanel(Vector2(resolution.x / 1.95f, resolution.y / 2.375f),
                    Vector2f(335.0f * (resolution.x / 1920.0f), 50.0f * (resolution.y / 1080.0f)), Color::Black, true);

                successfulText.InitializeText("Fonts/Roboto-Regular.ttf", "Operation Successful!",
                    successfulCharacterSize, true, false,
                    Color::Green, Vector2(resolution.x / 1.95f, resolution.y / 2.4f));

                if (successfulOperationTime >= 1.0f)
                {
                    successfulOperationTime = 0.0f;
                    currentGameState = GameState::SURGERY_ROOM_ACTIVE;
                }
            }
            break;
        }

        case GameState::DIALOGUE_HIDDEN:
        {
            // Handle input for dialogue hidden state
            if (Keyboard::isKeyPressed(Keyboard::Key::Enter) && inputCooldown <= 0.0f)
            {
                // Return to surgery room when Enter is pressed
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
                inputCooldown = INPUT_DELAY;
            }

            // Also allow clicking outside to close (optional)
            if (Mouse::isButtonPressed(Mouse::Button::Right) && inputCooldown <= 0.0f)
            {
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
                inputCooldown = INPUT_DELAY;
            }
            break;
        }

        case GameState::ITEM_TABLE_ACTIVE:
        {
            // Handle bag clicks when visible
            if (bag.IsVisible())
            {
                HandleBagClicks(mousePos);
            }

            // Handle item table clicks
            HandleItemTableClicks(mousePos);

            // Handle input for item table
            if (Keyboard::isKeyPressed(Keyboard::Key::Enter) && inputCooldown <= 0.0f)
            {
                // Return to surgery room when Enter is pressed
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
                inputCooldown = INPUT_DELAY;
            }

            // Also allow clicking outside to close (optional)
            if (Mouse::isButtonPressed(Mouse::Button::Right) && inputCooldown <= 0.0f)
            {
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
                inputCooldown = INPUT_DELAY;
            }
            break;
        }

        default:
            break;
        }
    }
}

/*
    Render Day 1 here
*/

void GameScene::RenderDay1(RenderWindow& window)
{
    switch (currentGameState)
    {
    case GameState::DIALOGUE_ACTIVE:
    {
        // Draw normal game background
        gameBackground.Draw(window);

        if (dialogueTexts[currentDialogueIndex])
        {
            if (dialoguePanel)
            {
                dialoguePanel->DrawDialoguePanel(window);
            }
            window.draw(dialogueTexts[currentDialogueIndex]->LoadText());
        }
        break;
    }

    case GameState::SURGERY_ROOM_ACTIVE:
    {
        if (!timeRanOut)
        {
            // Draw surgery room when active
            surgeryRoom.Draw(window, person.LoadSprite());

            // Draw bag if visible
            if (bag.IsVisible())
            {
                bag.Draw(window);
            }
        }

        if (surgeryRoom.GetTimeRemaining() <= 0.0f)
        {
            window.draw(dayFailedText.LoadText());
        }

        break;
    }

    case GameState::OPERATION_ACTIVE:
    {
        if (!timeRanOut)
        {
            // Draw operation scene and overlay the UI from surgery room
            operationScene.Draw(window);
            surgeryRoom.DrawUI(window);  // Draw the UI elements on top of operation scene

            // Draw bag if visible
            if (bag.IsVisible())
            {
                bag.Draw(window);
            }

            if (successfulOperationTime > 0.0f)
            {
                window.draw(operationScene.successPanel);
                window.draw(successfulText.LoadText());
            }
        }

        if (surgeryRoom.GetTimeRemaining() <= 0.0f)
        {
            window.draw(dayFailedText.LoadText());
        }

        break;
    }

    case GameState::ITEM_TABLE_ACTIVE:
    {
        if (!timeRanOut)
        {
            itemTable.Draw(window);
            surgeryRoom.DrawUI(window);  // Draw the UI elements on top of operation scene

            // Draw bag if visible
            if (bag.IsVisible())
            {
                bag.Draw(window);
            }
        }

        if (surgeryRoom.GetTimeRemaining() <= 0.0f)
        {
            window.draw(dayFailedText.LoadText());
        }

        break;
    }

    case GameState::DIALOGUE_HIDDEN:
    {
        // Clean up dialogue texts when hidden
        for (int i = 0; i < dialogueTexts.size(); i++)
        {
            delete dialogueTexts[i];
            dialogueTexts[i] = nullptr;
        }
        break;
    }
    }
}