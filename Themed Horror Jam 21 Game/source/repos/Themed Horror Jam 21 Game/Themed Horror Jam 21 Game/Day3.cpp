#include "GameScene.h"
#include "Engine.h"

float DIALOGUE_TEXT3_CHARACTER_SIZE;

void GameScene::InitializeDay3()
{
    // Play the game music
    gameMusic.InitializeAudio("Audio/Music/videogame2_horror_5_gameplay.mp3", true);

    // Make sure the game music is equal to the volume variable set in the options menu
    if (gameMusic.GetVolume() != Menu::GetVolume()) gameMusic.SetVolume(Menu::GetVolume());

    // Reset to initial state
    currentGameState = GameState::DIALOGUE_ACTIVE;

    if (isInputEnabled != true) isInputEnabled = true;
    if (mouseClicked != false) mouseClicked = false;

    maxPatients = 3;
    currentPatientIndex = 0;

    // Stop any running timer when reinitializing
    if (surgeryRoom.IsLoaded())
    {
        surgeryRoom.ResetToStartTimeTexture();
        surgeryRoom.StopTimer();
    }

    if (inputCooldown != INPUT_DELAY) inputCooldown = INPUT_DELAY;
    if (currentDialogueIndex != 0) currentDialogueIndex = 0;
    if (typeTextTime != 0.0f) typeTextTime = 0.0f;
    if (skippedTypewriting != false) skippedTypewriting = false;
    if (successfulOperations != 0) successfulOperations = 0;
    if (dialogueSystemInitialized != false) dialogueSystemInitialized = false;

    // Timer related text initialization
    if (failedTextAlpha != 255.0f) failedTextAlpha = 255.0f;
    if (failedTimer != 0.0f) failedTimer = 0.0f;

    if (daySuccessfulTextAlpha != 255.0f) daySuccessfulTextAlpha = 255.0f;
    if (daySuccessfulTimer != 0.0f) daySuccessfulTimer = 0.0f;

    if (!gameBackground.IsLoaded())
    {
        gameBackground.Initialize("Art Assets/Background.jpg", resolution);
    }
    float panelWidth = 1820.0f * (resolution.x / 1920.0f);
    float panelHeight = 700.0f * (resolution.y / 1080.0f);

    float panelX = resolution.x / 35.0f;
    float panelY = resolution.y / 3.0f;

    dialoguePanelTextures = { "Art Assets/Ui/chat_box_0.png", "Art Assets/Ui/chat_box_1.png",
        "Art Assets/Ui/chat_box_2.png", "Art Assets/Ui/chat_box_3.png", "Art Assets/Ui/chat_box_4.png" };

    dialoguePanel.InitializeDialoguePanel("Art Assets/Ui/chat_box_3.png", Vector2f(panelX, panelY),
        Vector2f(panelWidth, panelHeight));

    maxDialogueTexts = 4;

    dialogueTexts.clear();
    dialogueTexts.resize(maxDialogueTexts);

    if (!dialogueSystemInitialized)
    {
        InitializeDialogueSystem();
        dialogueSystemInitialized = true;
    }

    float textPanelX = resolution.x / 25.0f; // Move text panels slightly to the right from the dialogue panel
    float textPanelY = resolution.y / 1.4f; // Move text panels slightly below from the dialogue panel

    DIALOGUE_TEXT3_CHARACTER_SIZE = 40.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

    for (int i = 0; i < maxDialogueTexts; i++)
    {
        dialogueTexts[i].InitializeText("Fonts/Roboto-Regular.ttf", DIALOGUE_TEXT3_CHARACTER_SIZE, false, false,
            sf::Color::White, Vector2f(textPanelX, textPanelY));
    }
}

void GameScene::UpdateDay3(float deltaTime)
{
    // Update input cooldown
    if (inputCooldown > 0.0f)
    {
        inputCooldown -= deltaTime;
    }

    // Play game music if music isn't playing yet
    if (!musicPlaying)
    {
        gameMusic.PlayAudio();
        musicPlaying = true;
    }

    // Get mouse position for click detection
    Vector2i mousePixelPos = Mouse::getPosition(*Engine::Instance()->GetWindow());
    Vector2f mousePos = Engine::Instance()->GetWindow()->mapPixelToCoords(mousePixelPos);

    // Check for escape key to return to menu
    if (Keyboard::isKeyPressed(Keyboard::Key::Escape) && isInputEnabled)
    {
        // If the game music is playing, stop the music and set the bool to false
        if (musicPlaying)
        {
            gameMusic.StopAudio();
            musicPlaying = false;
        }

        typewriterEffect.Reset();
        itemTable.ResetCollectedItems();
        bag.ClearBag();
        sceneManager->ChangeScene("Menu");
        return;
    }

    // Check for inventory toggle (I key)
    if (Keyboard::isKeyPressed(Keyboard::Key::I) && inputCooldown <= 0.0f && currentGameState != GameState::FAILURE_ACTIVE
        && isInputEnabled)
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
    if (surgeryRoom.IsLoaded() && surgeryRoom.IsTimerRunning() && currentGameState != GameState::FAILURE_ACTIVE ||
        surgeryRoom.IsLoaded() && surgeryRoom.IsTimerRunning() && currentGameState != GameState::SUCCESSFUL_DAY_ACTIVE)
    {
        surgeryRoom.UpdateTimer(deltaTime);

        // Once timer runs out, change the current game state to show the fail state
        if (surgeryRoom.GetTimeRemaining() <= 0.0f)
            currentGameState = GameState::FAILURE_ACTIVE;
    }

    // Check for bag sprite click (only when surgery room is active)
    if (currentGameState == GameState::SURGERY_ROOM_ACTIVE && currentGameState != GameState::FAILURE_ACTIVE ||
        currentGameState == GameState::OPERATION_ACTIVE && currentGameState != GameState::FAILURE_ACTIVE)
    {
        if (surgeryRoom.BagSprite.getGlobalBounds().contains(mousePos) && isInputEnabled)
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
        else if (!surgeryRoom.BagSprite.getGlobalBounds().contains(mousePos))
        {
            // Reset color when not hovering
            if (surgeryRoom.BagSprite.getColor() != Color::White)
                surgeryRoom.BagSprite.setColor(Color::White);
        }
    }

    // Update based on current game state
    switch (currentGameState)
    {
    case GameState::DIALOGUE_ACTIVE:
    {
        // Update the typewriter effect and dialogue panel textures
        typewriterEffect.Update(deltaTime);
        UpdateDialoguePanelTexture();

        if (currentDialogueIndex >= 0 && currentDialogueIndex < static_cast<int>(dialogueTexts.size()))
        {
            std::string currentText = typewriterEffect.GetCurrentText();
            dialogueTexts[currentDialogueIndex].SetTypewriterString(sf::String(currentText));
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

                person.resize(maxPatients);

                for (int i = 0; i < maxPatients; i++)
                {
                    person[i].InitializeSprite("Art Assets/SurgeryRoom/sickness/basebody.png",
                        Vector2f(resolution.x / 2.238f, resolution.y / 2.5f),
                        Vector2f(resolution.x / 1920.0f, resolution.y / 1080.0f));
                }

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

        if (surgeryRoom.TableUISprite.getGlobalBounds().contains(mousePos) && isInputEnabled)
        {
            if (surgeryRoom.TableUISprite.getColor() != Color::Red)
                surgeryRoom.TableUISprite.setColor(Color::Red);

            // Add input cooldown check here
            if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked)
            {
                std::cout << "TopUI clicked! Changing to ITEM_TABLE_ACTIVE" << std::endl;

                itemTable.Initialize("Art Assets/SurgeryRoom/items_table/table.png",
                    Vector2f(resolution.x / -64.0f, resolution.y / 27.0f),
                    Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                    Vector2f(resolution.x / 1920.0f, resolution.y / 1080.0f),
                    true);

                // Change to ITEM_TABLE_ACTIVE
                currentGameState = GameState::ITEM_TABLE_ACTIVE;
                mouseClicked = true;

                std::cout << "State changed to: " << static_cast<int>(currentGameState) << std::endl;
            }

            else if (!Mouse::isButtonPressed(Mouse::Button::Left) && mouseClicked) mouseClicked = false;
        }
        else if (!surgeryRoom.TableUISprite.getGlobalBounds().contains(mousePos))
        {
            // Reset color when not hovering
            if (surgeryRoom.TableUISprite.getColor() != Color::White)
                surgeryRoom.TableUISprite.setColor(Color::White);
        }

        // Handle operation table clicks
        if (surgeryRoom.OperationTableSprite.getGlobalBounds().contains(mousePos) && isInputEnabled)
        {
            if (alpha != 255.0f) alpha = 255.0f;
            if (surgeryRoom.OperationTableSprite.getColor() != Color::Red)
                surgeryRoom.OperationTableSprite.setColor(Color::Red);

            // Set up the operation scene after clicking the left mouse button
            if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked)
            {
                currentGameState = GameState::OPERATION_ACTIVE;

                operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                    Vector2f(resolution.x / 2.8f, 0.0f),
                    Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                    true);

                if (!surgeryRoom.IsTimerRunning())
                {
                    // Start the timer depending on current difficulty when operation scene becomes active
                    if (Menu::GetDifficulty() == "Easy") surgeryRoom.StartTimer(57.0f);
                    else if (Menu::GetDifficulty() == "Normal") surgeryRoom.StartTimer(50.0f);
                    else if (Menu::GetDifficulty() == "Hard") surgeryRoom.StartTimer(40.0f);
                }
                mouseClicked = true;
            }

            else if (!Mouse::isButtonPressed(Mouse::Button::Left) && mouseClicked) mouseClicked = false;
        }
        // Otherwise, reset the sprite's color back to white once the mouse is no longer hovering on the sprite
        else if (!surgeryRoom.OperationTableSprite.getGlobalBounds().contains(mousePos))
        {
            if (surgeryRoom.OperationTableSprite.getColor() != Color(255, 255, 255, alpha))
                surgeryRoom.OperationTableSprite.setColor(Color(255, 255, 255, alpha));
        }

        // Switch between patients
        UpdateDay3Patients();

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
        if (Keyboard::isKeyPressed(Keyboard::Key::Enter) && inputCooldown <= 0.0f && isInputEnabled)
        {
            // Return to surgery room when Enter is pressed
            currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            inputCooldown = INPUT_DELAY;
        }

        // Also allow clicking outside to close (optional)
        if (Mouse::isButtonPressed(Mouse::Button::Right) && inputCooldown <= 0.0f && isInputEnabled)
        {
            currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            inputCooldown = INPUT_DELAY;
        }

        // Check if mouse is hovered over table UI in operation scene
        if (surgeryRoom.TableUISprite.getGlobalBounds().contains(mousePos) && isInputEnabled)
        {
            if (surgeryRoom.TableUISprite.getColor() != Color::Red)
                surgeryRoom.TableUISprite.setColor(Color::Red);

            if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked)
            {
                currentGameState = GameState::ITEM_TABLE_ACTIVE;
                mouseClicked = true;
            }

            else if (!Mouse::isButtonPressed(Mouse::Button::Left) && mouseClicked) mouseClicked = false;
        }
        else if (!surgeryRoom.TableUISprite.getGlobalBounds().contains(mousePos))
        {
            // Reset color when not hovering
            if (surgeryRoom.TableUISprite.getColor() != Color::White)
                surgeryRoom.TableUISprite.setColor(Color::White);
        }

        // Handle operation table clicks
        if (surgeryRoom.OperationTableSprite.getGlobalBounds().contains(mousePos) && isInputEnabled)
        {
            if (surgeryRoom.OperationTableSprite.getColor() != Color::Red)
                surgeryRoom.OperationTableSprite.setColor(Color::Red);

            // Go back to surgery room after clicking the left mouse button
            if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked)
            {
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;

                mouseClicked = true;
            }

            else if (!Mouse::isButtonPressed(Mouse::Button::Left) && mouseClicked) mouseClicked = false;
        }
        // Otherwise, reset the sprite's color back to white once the mouse is no longer hovering on the sprite
        else if (!surgeryRoom.OperationTableSprite.getGlobalBounds().contains(mousePos))
        {
            if (surgeryRoom.OperationTableSprite.getColor() != Color::White)
                surgeryRoom.OperationTableSprite.setColor(Color::White);
        }

        UpdateDay3OperationScene(deltaTime);
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
        if (Keyboard::isKeyPressed(Keyboard::Key::Enter) && inputCooldown <= 0.0f && isInputEnabled)
        {
            // Return to surgery room when Enter is pressed
            currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            inputCooldown = INPUT_DELAY;
        }

        // Also allow clicking outside to close (optional)
        if (Mouse::isButtonPressed(Mouse::Button::Right) && inputCooldown <= 0.0f && isInputEnabled)
        {
            currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            inputCooldown = INPUT_DELAY;
        }

        // Check if the mouse is hovering over the table UI
        if (surgeryRoom.TableUISprite.getGlobalBounds().contains(mousePos) && isInputEnabled)
        {
            if (surgeryRoom.TableUISprite.getColor() != Color::Red)
                surgeryRoom.TableUISprite.setColor(Color::Red);

            // Check for left mouse press
            if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked)
            {
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
                mouseClicked = true;
            }

            else if (!Mouse::isButtonPressed(Mouse::Button::Left) && mouseClicked) mouseClicked = false;
        }
        else if (!surgeryRoom.TableUISprite.getGlobalBounds().contains(mousePos))
        {
            // Reset color when not hovering
            if (surgeryRoom.TableUISprite.getColor() != Color::White)
                surgeryRoom.TableUISprite.setColor(Color::White);
        }

        break;
    }

    case GameState::FAILURE_ACTIVE:
    {
        float dayFailedCharacterSize = 40.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);
        if (isInputEnabled != false) isInputEnabled = false;

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
            // If the game music is playing, stop the music and set the bool to false
            if (musicPlaying)
            {
                gameMusic.StopAudio();
                musicPlaying = false;
            }

            typewriterEffect.Reset();
            itemTable.ResetCollectedItems();
            bag.ClearBag();
            sceneManager->ChangeScene("Menu");
            return;
        }

        break;
    }
    case GameState::SUCCESSFUL_DAY_ACTIVE:
    {
        float daySuccessfulCharacterSize = 40.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);
        if (isInputEnabled != false) isInputEnabled = false;

        // Modify day successful text colors
        uint8_t daySuccessfulRedValue{ 255 };
        uint8_t daySuccessfulGreenValue{ 255 };
        uint8_t daySuccessfulBlueValue{ 0 };

        /* If day successful timer is less than 1 second, initialize the day successful text and increment the day
        successful timer for text fade */
        if (daySuccessfulTimer < 1.0f)
        {
            daySuccessfulTimer += deltaTime;

            daySuccessfulText.InitializeText("Fonts/Roboto-Regular.ttf", "You passed Day 3!", daySuccessfulCharacterSize,
                true, false, Color(daySuccessfulRedValue, daySuccessfulGreenValue, daySuccessfulBlueValue, daySuccessfulTextAlpha),
                Vector2f(resolution.x / 2.0f, resolution.y / 2.0f));
        }

        // If day successful timer is around 1 second or so, make the text fade away overtime
        else if (daySuccessfulTimer >= 1.0f)
        {
            daySuccessfulTextAlpha -= deltaTime * 100.0f;

            if (daySuccessfulText.GetTextColor() != Color(daySuccessfulRedValue, daySuccessfulGreenValue, daySuccessfulBlueValue, daySuccessfulTextAlpha))
                daySuccessfulText.SetTextColor(Color(daySuccessfulRedValue, daySuccessfulGreenValue, daySuccessfulBlueValue, daySuccessfulTextAlpha));
        }

        //printf("day successful timer: %f\n", daySuccessfulTimer);

        // If day successful text's alpha value goes down to 0, go back to main menu and switch to the next day
        if (daySuccessfulTextAlpha <= 0.0f)
        {
            // If the game music is playing, stop the music and set the bool to false
            if (musicPlaying)
            {
                gameMusic.StopAudio();
                musicPlaying = false;
            }

            typewriterEffect.Reset();
            itemTable.ResetCollectedItems();
            bag.ClearBag();
            currentDay = 4;
            Menu::nextDayUnlocked = true;
            sceneManager->ChangeScene("Menu");
            return;
        }

        break;
    }
    default:
        break;
    }
}

void GameScene::RenderDay3(RenderWindow& window)
{
    switch (currentGameState)
    {
    case GameState::DIALOGUE_ACTIVE:
    {
        // Draw normal game background
        gameBackground.Draw(window);
        dialoguePanel.DrawDialoguePanel(window);
        window.draw(dialogueTexts[currentDialogueIndex].LoadText());
        break;
    }

    case GameState::SURGERY_ROOM_ACTIVE:
    {
        // Draw surgery room when active
        surgeryRoom.Draw(window, person[currentPatientIndex].LoadSprite());

        // Draw bag if visible
        if (bag.IsVisible())
        {
            bag.Draw(window);
        }

        break;
    }

    case GameState::OPERATION_ACTIVE:
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

        break;
    }

    case GameState::ITEM_TABLE_ACTIVE:
    {
        itemTable.Draw(window);
        surgeryRoom.DrawUI(window);  // Draw the UI elements on top of operation scene

        // Draw bag if visible
        if (bag.IsVisible())
        {
            bag.Draw(window);
        }

        break;
    }

    case GameState::DIALOGUE_HIDDEN:
    {
        break;
    }

    case GameState::FAILURE_ACTIVE:
    {
        window.draw(dayFailedText.LoadText());
        break;
    }

    case GameState::SUCCESSFUL_DAY_ACTIVE:
    {
        window.draw(daySuccessfulText.LoadText());
        break;
    }

    default:
        break;
    }
}

// Update patient here
void GameScene::UpdateDay3Patients()
{
    switch (currentPatientIndex)
    {
    case 0: // 1st patient (nothing happens here because operation scene is already initialized)
        break;

    case 1: // 2nd patient
        if (!operationSceneChanged)
        {
            operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                Vector2f(resolution.x / 2.8f, 0.0f),
                Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                true);

            operationScene.maxDots = 6;

            operationScene.InitializeDot(Vector2f(resolution.x / 2.25f, resolution.y / 4.0f),
                10.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2), Color::Red, Color::Red,
                5.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2),
                50.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2),
                0.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2));

            operationSceneChanged = true;
        }

        break;

    case 2: // 3rd and final patient
        if (!operationSceneChanged)
        {
            operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                Vector2f(resolution.x / 2.8f, 0.0f),
                Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                true);

            operationScene.maxDots = 8;

            operationScene.InitializeDot(Vector2f(resolution.x / 2.25f, resolution.y / 4.0f),
                10.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2), Color::Red, Color::Red,
                5.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2),
                30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2),
                50.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2));

            operationSceneChanged = true;
        }
        break;

    default:
        break;
    }
}

// Update Day 3's operation scene logic
void GameScene::UpdateDay3OperationScene(float deltaTime)
{
    // Get mouse position for click detection
    Vector2i mousePixelPos = Mouse::getPosition(*Engine::Instance()->GetWindow());
    Vector2f mousePos = Engine::Instance()->GetWindow()->mapPixelToCoords(mousePixelPos);

    switch (currentPatientIndex)
    {
    case 0: // 1st patient
        for (int i = 0; i < operationScene.maxDots; i++)
        {
            if (operationScene.dotCircleShape[i].getGlobalBounds().contains(mousePos) && isInputEnabled)
            {
                // Set up the operation scene after clicking the left mouse button
                if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked)
                {
                    if (operationScene.dotCircleShape[i].getFillColor() != Color::Green)
                        operationScene.dotCircleShape[i].setFillColor(Color::Green);

                    mouseClicked = true;
                }

                else if (!Mouse::isButtonPressed(Mouse::Button::Left) && mouseClicked)
                    mouseClicked = false;
            }
        }

        if (operationScene.dotCircleShape[0].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[1].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[2].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[3].getFillColor() == Color::Green)
        {
            successfulOperationTime += deltaTime;
            if (successfulOperations != 1) successfulOperations = 1;

            if (isInputEnabled != false) isInputEnabled = false;

            if (operationSceneChanged != false) operationSceneChanged = false;

            float successfulCharacterSize = 30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

            operationScene.InitializeSuccessPanel(Vector2(resolution.x / 1.95f, resolution.y / 2.375f),
                Vector2f(410.0f * (resolution.x / 1920.0f), 50.0f * (resolution.y / 1080.0f)), Color::Black, true);

            successfulText.InitializeText("Fonts/Roboto-Regular.ttf", std::to_string(successfulOperations) + "/" +
                std::to_string(maxPatients) + " operations successful!",
                successfulCharacterSize, true, false,
                Color::Green, Vector2(resolution.x / 1.95f, resolution.y / 2.4f));

            if (successfulOperationTime >= 1.0f)
            {
                isInputEnabled = true;

                successfulOperationTime = 0.0f;
                currentPatientIndex = 1;
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            }
        }

        break;

    case 1: // 2nd patient
        for (int i = 0; i < operationScene.maxDots; i++)
        {
            if (operationScene.dotCircleShape[i].getGlobalBounds().contains(mousePos) && isInputEnabled)
            {
                // Set up the operation scene after clicking the left mouse button
                if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked)
                {
                    if (operationScene.dotCircleShape[i].getFillColor() != Color::Green)
                        operationScene.dotCircleShape[i].setFillColor(Color::Green);

                    mouseClicked = true;
                }

                else if (!Mouse::isButtonPressed(Mouse::Button::Left) && mouseClicked)
                    mouseClicked = false;
            }
        }

        if (operationScene.dotCircleShape[0].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[1].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[2].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[3].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[4].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[5].getFillColor() == Color::Green)
        {
            successfulOperationTime += deltaTime;
            if (successfulOperations != 2) successfulOperations = 2;

            if (operationSceneChanged != false) operationSceneChanged = false;
            if (isInputEnabled != false) isInputEnabled = false;

            float successfulCharacterSize = 30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

            operationScene.InitializeSuccessPanel(Vector2(resolution.x / 1.95f, resolution.y / 2.375f),
                Vector2f(410.0f * (resolution.x / 1920.0f), 50.0f * (resolution.y / 1080.0f)), Color::Black, true);

            successfulText.InitializeText("Fonts/Roboto-Regular.ttf", std::to_string(successfulOperations) + "/" +
                std::to_string(maxPatients) + " operations successful!",
                successfulCharacterSize, true, false,
                Color::Green, Vector2(resolution.x / 1.95f, resolution.y / 2.4f));

            if (successfulOperationTime >= 1.0f)
            {
                isInputEnabled = true;

                successfulOperationTime = 0.0f;
                currentPatientIndex = 2;
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            }
        }

        break;

    case 2: // Third and final patient
        for (int i = 0; i < operationScene.maxDots; i++)
        {
            if (operationScene.dotCircleShape[i].getGlobalBounds().contains(mousePos) && isInputEnabled)
            {
                // Set up the operation scene after clicking the left mouse button
                if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked)
                {
                    if (operationScene.dotCircleShape[i].getFillColor() != Color::Green)
                        operationScene.dotCircleShape[i].setFillColor(Color::Green);

                    mouseClicked = true;
                }

                else if (!Mouse::isButtonPressed(Mouse::Button::Left) && mouseClicked)
                    mouseClicked = false;
            }
        }

        if (operationScene.dotCircleShape[0].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[1].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[2].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[3].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[4].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[5].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[6].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[7].getFillColor() == Color::Green)
        {
            successfulOperationTime += deltaTime;
            if (successfulOperations != 3) successfulOperations = 3;

            if (operationSceneChanged != false) operationSceneChanged = false;
            if (isInputEnabled != false) isInputEnabled = false;

            float successfulCharacterSize = 30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

            operationScene.InitializeSuccessPanel(Vector2(resolution.x / 1.95f, resolution.y / 2.375f),
                Vector2f(410.0f * (resolution.x / 1920.0f), 50.0f * (resolution.y / 1080.0f)), Color::Black, true);

            successfulText.InitializeText("Fonts/Roboto-Regular.ttf", std::to_string(successfulOperations) + "/" +
                std::to_string(maxPatients) + " operations successful!",
                successfulCharacterSize, true, false,
                Color::Green, Vector2(resolution.x / 1.95f, resolution.y / 2.4f));

            if (successfulOperationTime >= 1.0f)
            {
                isInputEnabled = true;
                successfulOperationTime = 0.0f;
                currentGameState = GameState::SUCCESSFUL_DAY_ACTIVE;
            }
        }
        break;

    default:
        break;
    }
}