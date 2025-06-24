#include "GameScene.h"
#include "Engine.h"

float DIALOGUE_TEXT5_CHARACTER_SIZE;

void GameScene::InitializeDay5()
{
    if (Menu::nextDayUnlocked != false) Menu::nextDayUnlocked = false;

    // Play the game music
    gameMusic.InitializeAudio("Audio/Music/videogame2_horror_5_gameplay.mp3", true);

    successfulDaySound[0].InitializeAudio("Audio/Sounds/fx 6 good ending_n.wav");
    successfulDaySound[1].InitializeAudio("Audio/Sounds/fx 6 good ending2_n-2.wav");

    failedDaySound[0].InitializeAudio("Audio/Sounds/fx 6 bad ending_n.wav");
    failedDaySound[1].InitializeAudio("Audio/Sounds/fx 6 bad ending2_n.wav");

    // Make sure the game music is equal to the volume variable set in the options menu
    if (gameMusic.GetVolume() != Menu::GetVolume()) gameMusic.SetVolume(Menu::GetVolume());

    for (int i = 0; i < successfulDaySound.size(); i++)
        if (successfulDaySound[i].GetVolume() != Menu::GetVolume()) successfulDaySound[i].SetVolume(Menu::GetVolume());

    for (int i = 0; i < failedDaySound.size(); i++)
        if (failedDaySound[i].GetVolume() != Menu::GetVolume()) failedDaySound[i].SetVolume(Menu::GetVolume());

    // Reset to initial state
    currentGameState = GameState::DIALOGUE_ACTIVE;

    if (isInputEnabled != true) isInputEnabled = true;
    if (mouseClicked != false) mouseClicked = false;

    maxPatients = 20;
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

    if (successfulOperationTime != 0.0f) successfulOperationTime = 0.0f;

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

    DIALOGUE_TEXT5_CHARACTER_SIZE = 40.0f * (resolution.x / 1920.0f);

    for (int i = 0; i < maxDialogueTexts; i++)
    {
        dialogueTexts[i].InitializeText("Fonts/Roboto-Regular.ttf", DIALOGUE_TEXT5_CHARACTER_SIZE, false, false,
            sf::Color::White, Vector2f(textPanelX, textPanelY));
    }
}

void GameScene::UpdateDay5(float deltaTime)
{
    if (gameMusic.GetVolume() != Menu::GetVolume()) gameMusic.SetVolume(Menu::GetVolume());

    for (int i = 0; i < successfulDaySound.size(); i++)
        if (successfulDaySound[i].GetVolume() != Menu::GetVolume()) successfulDaySound[i].SetVolume(Menu::GetVolume());

    for (int i = 0; i < failedDaySound.size(); i++)
        if (failedDaySound[i].GetVolume() != Menu::GetVolume()) failedDaySound[i].SetVolume(Menu::GetVolume());

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
    /*if (Keyboard::isKeyPressed(Keyboard::Key::I) && inputCooldown <= 0.0f && currentGameState != GameState::FAILURE_ACTIVE
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
    }*/

    // Update the timer in all relevant states (once the surgery room is loaded and timer is running)
    if (surgeryRoom.IsLoaded() && surgeryRoom.IsTimerRunning() && currentGameState != GameState::FAILURE_ACTIVE &&
        successfulOperationTime <= 0.0f || surgeryRoom.IsLoaded() && surgeryRoom.IsTimerRunning() &&
        currentGameState != GameState::SUCCESSFUL_DAY_ACTIVE && successfulOperationTime <= 0.0f)
    {
        surgeryRoom.UpdateTimer(deltaTime);

        // Once timer runs out, change the current game state to show the fail state
        if (surgeryRoom.GetTimeRemaining() <= 0.0f && surgeryRoom.GetMinutesRemaining() <= 0)
        {
            surgeryRoom.StopLowTimeAudio();
            failedDaySoundIndex = rand() % failedDaySound.size();
            currentGameState = GameState::FAILURE_ACTIVE;
        }
    }

    // Check for bag sprite click (only when surgery room is active)
    /*if (currentGameState == GameState::SURGERY_ROOM_ACTIVE && currentGameState != GameState::FAILURE_ACTIVE ||
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
    }*/

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
                        sf::Vector2f(resolution.x / 1.53f, resolution.y / 1.22f), // OperationTableSprite UI sprite position
                        sf::Vector2f(resolution.x / 8.0f, resolution.y / 1.04f)); // timer text position
                }

                operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                    Vector2f(resolution.x / 2.8f, 0.0f),
                    Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                    true);

                operationScene.maxDots = 10;

                operationScene.InitializeDot({ Vector2f(resolution.x / 1.89f, resolution.y / 7.77f),
                    Vector2f(resolution.x / 2.03f, resolution.y / 29.19f),
                    Vector2f(resolution.x / 2.02f, resolution.y / 4.58f),
                    Vector2f(resolution.x / 1.71f, resolution.y / 3.42f),
                    Vector2f(resolution.x / 2.36f, resolution.y / 2.10f),
                    Vector2f(resolution.x / 1.74f, resolution.y / 1.47f),
                    Vector2f(resolution.x / 2.17f, resolution.y / 2.09f),
                    Vector2f(resolution.x / 1.95f, resolution.y / 2.69f),
                    Vector2f(resolution.x / 2.22f, resolution.y / 1.30f),
                    Vector2f(resolution.x / 2.01f, resolution.y / 1.73f) },
                    6.0f * (resolution.x / 1920.0f), Color::Red, Color::Red,
                    3.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2));

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

        // Check mouse input for dialogue
        if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
            Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
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
                        sf::Vector2f(resolution.x / 1.53f, resolution.y / 1.22f), // OperationTableSprite UI sprite position
                        sf::Vector2f(resolution.x / 8.0f, resolution.y / 1.04f)); // timer text position
                }

                operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                    Vector2f(resolution.x / 2.8f, 0.0f),
                    Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                    true);

                operationScene.maxDots = 10;

                operationScene.InitializeDot({ Vector2f(resolution.x / 1.89f, resolution.y / 7.77f),
                    Vector2f(resolution.x / 2.03f, resolution.y / 29.19f),
                    Vector2f(resolution.x / 2.02f, resolution.y / 4.58f),
                    Vector2f(resolution.x / 1.71f, resolution.y / 3.42f),
                    Vector2f(resolution.x / 2.36f, resolution.y / 2.10f),
                    Vector2f(resolution.x / 1.74f, resolution.y / 1.47f),
                    Vector2f(resolution.x / 2.17f, resolution.y / 2.09f),
                    Vector2f(resolution.x / 1.95f, resolution.y / 2.69f),
                    Vector2f(resolution.x / 2.22f, resolution.y / 1.30f),
                    Vector2f(resolution.x / 2.01f, resolution.y / 1.73f) },
                    6.0f * (resolution.x / 1920.0f), Color::Red, Color::Red,
                    3.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2));

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

            if (mouseClicked != true) mouseClicked = true;
        }

        else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
            && mouseClicked)
        {
            if (mouseClicked != false) mouseClicked = false;
        }

        break;
    }

    case GameState::SURGERY_ROOM_ACTIVE:
    {
        // Handle bag clicks when visible
        /*if (bag.IsVisible())
        {
            HandleBagClicks(mousePos);
        }*/

        if (!alphaIncrease) alpha -= 50.0f * deltaTime;
        else if (alphaIncrease) alpha += 50.0f * deltaTime;

        if (alpha >= 255.0f) alphaIncrease = false;
        else if (alpha <= 125.0f) alphaIncrease = true;

        if (surgeryRoom.TableUISprite.getGlobalBounds().contains(mousePos) && isInputEnabled)
        {
            if (surgeryRoom.TableUISprite.getColor() != Color::Red)
                surgeryRoom.TableUISprite.setColor(Color::Red);

            // Add input cooldown check here
            if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
            {
                std::cout << "TopUI clicked! Changing to ITEM_TABLE_ACTIVE" << std::endl;

                previousGameState = GameState::SURGERY_ROOM_ACTIVE;

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

            else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                && mouseClicked) mouseClicked = false;
        }
        else if (!surgeryRoom.TableUISprite.getGlobalBounds().contains(mousePos))
        {
            // Reset color when not hovering
            if (surgeryRoom.TableUISprite.getColor() != Color::White)
                surgeryRoom.TableUISprite.setColor(Color::White);
        }

        // Check if mouse is inside the notes sprite
        if (surgeryRoom.NotesSprite.getGlobalBounds().contains(mousePos) && isInputEnabled)
        {
            if (surgeryRoom.NotesSprite.getColor() != Color::Red)
                surgeryRoom.NotesSprite.setColor(Color::Red);

            // Add input cooldown check here
            if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
            {
                previousGameState = GameState::SURGERY_ROOM_ACTIVE;

                textbookPages.Initialize("Art Assets/SurgeryRoom/notes.png",
                    Vector2f(resolution.x / resolution.x, (resolution.y / resolution.y) - 70.0f),
                    Vector2f(12.5f * (resolution.x / 1920.0f), 9.0f * (resolution.y / 1080.0f)), false,
                    "Fonts/Roboto-Regular.ttf", Vector2f(resolution.x / 9.7f, resolution.y / 4.5f),
                    Vector2f(resolution.x / 1.9f, resolution.y / 4.5f), 30.0f * (resolution.x / 1920.0f),
                    Vector2(300.0f * (resolution.x / 1920.0f), 200.0f * (resolution.y / 1080.0f)),
                    Vector2f(resolution.x / 5.0f, resolution.y / 2.0f), Vector2f(resolution.x / 1.6f, resolution.y / 2.0f));

                // Change to TEXTBOOK_ACTIVE
                currentGameState = GameState::TEXTBOOK_ACTIVE;
                mouseClicked = true;
            }

            else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                && mouseClicked) mouseClicked = false;
        }
        else if (!surgeryRoom.NotesSprite.getGlobalBounds().contains(mousePos))
        {
            // Reset color when not hovering
            if (surgeryRoom.NotesSprite.getColor() != Color::White)
                surgeryRoom.NotesSprite.setColor(Color::White);
        }

        // Handle operation table clicks
        if (surgeryRoom.OperationTableSprite.getGlobalBounds().contains(mousePos) && isInputEnabled)
        {
            if (alpha != 255.0f) alpha = 255.0f;
            if (surgeryRoom.OperationTableSprite.getColor() != Color::Red)
                surgeryRoom.OperationTableSprite.setColor(Color::Red);

            // Set up the operation scene after clicking the left mouse button
            if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
            {
                currentGameState = GameState::OPERATION_ACTIVE;

                operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                    Vector2f(resolution.x / 2.8f, 0.0f),
                    Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                    true);

                if (!surgeryRoom.IsTimerRunning())
                {
                    // Start the timer depending on current difficulty when operation scene becomes active
                    if (Menu::GetDifficulty() == "Easy") surgeryRoom.StartTimer(9, 1.0f);
                    else if (Menu::GetDifficulty() == "Normal") surgeryRoom.StartTimer(8, 1.0f);
                    else if (Menu::GetDifficulty() == "Hard") surgeryRoom.StartTimer(7, 1.0f);
                }
                mouseClicked = true;
            }

            else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                && mouseClicked) mouseClicked = false;
        }
        // Otherwise, reset the sprite's color back to white once the mouse is no longer hovering on the sprite
        else if (!surgeryRoom.OperationTableSprite.getGlobalBounds().contains(mousePos))
        {
            if (surgeryRoom.OperationTableSprite.getColor() != Color(255, 255, 255, alpha))
                surgeryRoom.OperationTableSprite.setColor(Color(255, 255, 255, alpha));
        }

        // Switch between patients
        UpdateDay5Patients();

        break;
    }

    case GameState::OPERATION_ACTIVE:
    {
        // Handle bag clicks when visible
        /*if (bag.IsVisible())
        {
            HandleBagClicks(mousePos);
        }*/

        // Handle input for operation scene
        if (Keyboard::isKeyPressed(Keyboard::Key::Enter) && inputCooldown <= 0.0f && isInputEnabled)
        {
            // Return to surgery room when Enter is pressed
            currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            inputCooldown = INPUT_DELAY;
        }

        // Check if mouse is hovered over table UI in operation scene
        if (surgeryRoom.TableUISprite.getGlobalBounds().contains(mousePos) && isInputEnabled)
        {
            if (surgeryRoom.TableUISprite.getColor() != Color::Red)
                surgeryRoom.TableUISprite.setColor(Color::Red);

            if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
            {
                previousGameState = GameState::OPERATION_ACTIVE;

                itemTable.Initialize("Art Assets/SurgeryRoom/items_table/table.png",
                    Vector2f(resolution.x / -64.0f, resolution.y / 27.0f),
                    Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                    Vector2f(resolution.x / 1920.0f, resolution.y / 1080.0f),
                    true);

                currentGameState = GameState::ITEM_TABLE_ACTIVE;
                mouseClicked = true;
            }

            else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                && mouseClicked) mouseClicked = false;
        }
        else if (!surgeryRoom.TableUISprite.getGlobalBounds().contains(mousePos))
        {
            // Reset color when not hovering
            if (surgeryRoom.TableUISprite.getColor() != Color::White)
                surgeryRoom.TableUISprite.setColor(Color::White);
        }

        // Check if mouse is inside the notes sprite
        if (surgeryRoom.NotesSprite.getGlobalBounds().contains(mousePos) && isInputEnabled)
        {
            if (surgeryRoom.NotesSprite.getColor() != Color::Red)
                surgeryRoom.NotesSprite.setColor(Color::Red);

            // Add input cooldown check here
            if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
            {
                previousGameState = GameState::OPERATION_ACTIVE;

                textbookPages.Initialize("Art Assets/SurgeryRoom/notes.png",
                    Vector2f(resolution.x / resolution.x, (resolution.y / resolution.y) - 70.0f),
                    Vector2f(12.5f * (resolution.x / 1920.0f), 9.0f * (resolution.y / 1080.0f)), false,
                    "Fonts/Roboto-Regular.ttf", Vector2f(resolution.x / 9.7f, resolution.y / 4.5f),
                    Vector2f(resolution.x / 1.9f, resolution.y / 4.5f), 30.0f * (resolution.x / 1920.0f),
                    Vector2(300.0f * (resolution.x / 1920.0f), 200.0f * (resolution.y / 1080.0f)),
                    Vector2f(resolution.x / 5.0f, resolution.y / 2.0f), Vector2f(resolution.x / 1.6f, resolution.y / 2.0f));

                // Change to TEXTBOOK_ACTIVE
                currentGameState = GameState::TEXTBOOK_ACTIVE;
                mouseClicked = true;
            }

            else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                && mouseClicked) mouseClicked = false;
        }
        else if (!surgeryRoom.NotesSprite.getGlobalBounds().contains(mousePos))
        {
            // Reset color when not hovering
            if (surgeryRoom.NotesSprite.getColor() != Color::White)
                surgeryRoom.NotesSprite.setColor(Color::White);
        }

        // Handle operation table clicks
        if (surgeryRoom.OperationTableSprite.getGlobalBounds().contains(mousePos) && isInputEnabled)
        {
            if (surgeryRoom.OperationTableSprite.getColor() != Color::Red)
                surgeryRoom.OperationTableSprite.setColor(Color::Red);

            // Go back to surgery room after clicking the left mouse button
            if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
            {
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;

                mouseClicked = true;
            }

            else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right) 
                && mouseClicked) mouseClicked = false;
        }
        // Otherwise, reset the sprite's color back to white once the mouse is no longer hovering on the sprite
        else if (!surgeryRoom.OperationTableSprite.getGlobalBounds().contains(mousePos))
        {
            if (surgeryRoom.OperationTableSprite.getColor() != Color::White)
                surgeryRoom.OperationTableSprite.setColor(Color::White);
        }

        UpdateDay5OperationScene(deltaTime);
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
        break;
    }

    case GameState::ITEM_TABLE_ACTIVE:
    {
        // Handle bag clicks when visible
        /*if (bag.IsVisible())
        {
            HandleBagClicks(mousePos);
        }*/

        // Handle item table clicks
        HandleItemTableClicks(mousePos);

        if (!alphaIncrease) alpha -= 50.0f * deltaTime;
        else if (alphaIncrease) alpha += 50.0f * deltaTime;

        if (alpha >= 255.0f) alphaIncrease = false;
        else if (alpha <= 125.0f) alphaIncrease = true;

        // Handle input for item table
        if (Keyboard::isKeyPressed(Keyboard::Key::Enter) && inputCooldown <= 0.0f && isInputEnabled)
        {
            // Return to surgery room or operation scene depending on previous game state when Enter is pressed
            if (previousGameState == GameState::SURGERY_ROOM_ACTIVE) currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            else if (previousGameState == GameState::OPERATION_ACTIVE) currentGameState = GameState::OPERATION_ACTIVE;
            inputCooldown = INPUT_DELAY;
        }

        // Check if mouse is inside the notes sprite
        if (surgeryRoom.NotesSprite.getGlobalBounds().contains(mousePos) && isInputEnabled)
        {
            if (surgeryRoom.NotesSprite.getColor() != Color::Red)
                surgeryRoom.NotesSprite.setColor(Color::Red);

            // Add input cooldown check here
            if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
            {
                textbookPages.Initialize("Art Assets/SurgeryRoom/notes.png",
                    Vector2f(resolution.x / resolution.x, (resolution.y / resolution.y) - 70.0f),
                    Vector2f(12.5f * (resolution.x / 1920.0f), 9.0f * (resolution.y / 1080.0f)), false,
                    "Fonts/Roboto-Regular.ttf", Vector2f(resolution.x / 9.7f, resolution.y / 4.5f),
                    Vector2f(resolution.x / 1.9f, resolution.y / 4.5f), 30.0f * (resolution.x / 1920.0f),
                    Vector2(300.0f * (resolution.x / 1920.0f), 200.0f * (resolution.y / 1080.0f)),
                    Vector2f(resolution.x / 5.0f, resolution.y / 2.0f), Vector2f(resolution.x / 1.6f, resolution.y / 2.0f));

                // Change to TEXTBOOK_ACTIVE
                currentGameState = GameState::TEXTBOOK_ACTIVE;
                mouseClicked = true;
            }

            else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                && mouseClicked) mouseClicked = false;
        }
        else if (!surgeryRoom.NotesSprite.getGlobalBounds().contains(mousePos))
        {
            // Reset color when not hovering
            if (surgeryRoom.NotesSprite.getColor() != Color::White)
                surgeryRoom.NotesSprite.setColor(Color::White);
        }

        // Handle operation table clicks
        if (surgeryRoom.OperationTableSprite.getGlobalBounds().contains(mousePos) && isInputEnabled)
        {
            if (alpha != 255.0f) alpha = 255.0f;
            if (surgeryRoom.OperationTableSprite.getColor() != Color::Red)
                surgeryRoom.OperationTableSprite.setColor(Color::Red);

            // Set up the operation scene after clicking the left mouse button
            if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
            {
                currentGameState = GameState::OPERATION_ACTIVE;

                operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                    Vector2f(resolution.x / 2.8f, 0.0f),
                    Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                    true);

                if (!surgeryRoom.IsTimerRunning())
                {
                    // Start the timer depending on current difficulty when operation scene becomes active
                    if (Menu::GetDifficulty() == "Easy") surgeryRoom.StartTimer(9, 1.0f);
                    else if (Menu::GetDifficulty() == "Normal") surgeryRoom.StartTimer(8, 1.0f);
                    else if (Menu::GetDifficulty() == "Hard") surgeryRoom.StartTimer(7, 1.0f);
                }
                mouseClicked = true;
            }

            else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                && mouseClicked) mouseClicked = false;
        }
        // Otherwise, reset the sprite's color back to white once the mouse is no longer hovering on the sprite
        else if (!surgeryRoom.OperationTableSprite.getGlobalBounds().contains(mousePos))
        {
            if (surgeryRoom.OperationTableSprite.getColor() != Color(255, 255, 255, alpha))
                surgeryRoom.OperationTableSprite.setColor(Color(255, 255, 255, alpha));
        }

        // Check if the mouse is hovering over the table UI
        if (surgeryRoom.TableUISprite.getGlobalBounds().contains(mousePos) && isInputEnabled)
        {
            if (surgeryRoom.TableUISprite.getColor() != Color::Red)
                surgeryRoom.TableUISprite.setColor(Color::Red);

            // Check for left mouse press
            if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
            {
                if (previousGameState == GameState::SURGERY_ROOM_ACTIVE) currentGameState = GameState::SURGERY_ROOM_ACTIVE;
                else if (previousGameState == GameState::OPERATION_ACTIVE) currentGameState = GameState::OPERATION_ACTIVE;
                mouseClicked = true;
            }

            else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right) 
                && mouseClicked) mouseClicked = false;
        }
        else if (!surgeryRoom.TableUISprite.getGlobalBounds().contains(mousePos))
        {
            // Reset color when not hovering
            if (surgeryRoom.TableUISprite.getColor() != Color::White)
                surgeryRoom.TableUISprite.setColor(Color::White);
        }

        break;
    }

    case GameState::TEXTBOOK_ACTIVE:
    {
        textbookPages.UpdateTextbookPages(mousePos);

        if (!alphaIncrease) alpha -= 50.0f * deltaTime;
        else if (alphaIncrease) alpha += 50.0f * deltaTime;

        if (alpha >= 255.0f) alphaIncrease = false;
        else if (alpha <= 125.0f) alphaIncrease = true;

        // Handle input for textbook active
        if (Keyboard::isKeyPressed(Keyboard::Key::Enter) && inputCooldown <= 0.0f && isInputEnabled)
        {
            // Return to last previous game state
            if (previousGameState == GameState::SURGERY_ROOM_ACTIVE)
            {
                textbookPages.ClearTextVector();
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            }
            else if (previousGameState == GameState::OPERATION_ACTIVE)
            {
                textbookPages.ClearTextVector();
                currentGameState = GameState::OPERATION_ACTIVE;
            }
            inputCooldown = INPUT_DELAY;
        }

        if (surgeryRoom.TableUISprite.getGlobalBounds().contains(mousePos) && isInputEnabled)
        {
            if (surgeryRoom.TableUISprite.getColor() != Color::Red)
                surgeryRoom.TableUISprite.setColor(Color::Red);

            // Add input cooldown check here
            if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
            {
                itemTable.Initialize("Art Assets/SurgeryRoom/items_table/table.png",
                    Vector2f(resolution.x / -64.0f, resolution.y / 27.0f),
                    Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                    Vector2f(resolution.x / 1920.0f, resolution.y / 1080.0f),
                    true);

                // Change to ITEM_TABLE_ACTIVE
                currentGameState = GameState::ITEM_TABLE_ACTIVE;
                mouseClicked = true;
            }

            else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                && mouseClicked) mouseClicked = false;
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
            if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
            {
                currentGameState = GameState::OPERATION_ACTIVE;

                operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                    Vector2f(resolution.x / 2.8f, 0.0f),
                    Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                    true);

                if (!surgeryRoom.IsTimerRunning())
                {
                    // Start the timer depending on current difficulty when operation scene becomes active
                    if (Menu::GetDifficulty() == "Easy") surgeryRoom.StartTimer(9, 1.0f);
                    else if (Menu::GetDifficulty() == "Normal") surgeryRoom.StartTimer(8, 1.0f);
                    else if (Menu::GetDifficulty() == "Hard") surgeryRoom.StartTimer(7, 1.0f);
                }
                mouseClicked = true;
            }

            else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                && mouseClicked) mouseClicked = false;
        }
        // Otherwise, reset the sprite's color back to white once the mouse is no longer hovering on the sprite
        else if (!surgeryRoom.OperationTableSprite.getGlobalBounds().contains(mousePos))
        {
            if (surgeryRoom.OperationTableSprite.getColor() != Color(255, 255, 255, alpha))
                surgeryRoom.OperationTableSprite.setColor(Color(255, 255, 255, alpha));
        }

        // Check if mouse is inside the notes sprite
        if (surgeryRoom.NotesSprite.getGlobalBounds().contains(mousePos) && isInputEnabled)
        {
            if (surgeryRoom.NotesSprite.getColor() != Color::Red)
                surgeryRoom.NotesSprite.setColor(Color::Red);

            // Check for mouse press
            if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
            {
                if (previousGameState == GameState::SURGERY_ROOM_ACTIVE)
                {
                    textbookPages.ClearTextVector();
                    currentGameState = GameState::SURGERY_ROOM_ACTIVE;
                }
                else if (previousGameState == GameState::OPERATION_ACTIVE)
                {
                    textbookPages.ClearTextVector();
                    currentGameState = GameState::OPERATION_ACTIVE;
                }
                mouseClicked = true;
            }

            else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                && mouseClicked) mouseClicked = false;
        }
        else if (!surgeryRoom.NotesSprite.getGlobalBounds().contains(mousePos))
        {
            // Reset color when not hovering
            if (surgeryRoom.NotesSprite.getColor() != Color::White)
                surgeryRoom.NotesSprite.setColor(Color::White);
        }

        break;
    }

    case GameState::FAILURE_ACTIVE:
    {
        float dayFailedCharacterSize = 40.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);
        if (isInputEnabled != false) isInputEnabled = false;

        // If the game music is playing, stop the music and set the bool to false
        if (musicPlaying)
        {
            gameMusic.StopAudio();
            musicPlaying = false;
        }

        if (failedTimer <= 0.0f) failedDaySound[failedDaySoundIndex].PlayAudio();

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
            for (int i = 0; i < failedDaySound.size(); i++) failedDaySound[i].StopAudio();

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

        // If the game music is playing, stop the music and set the bool to false
        if (musicPlaying)
        {
            gameMusic.StopAudio();
            musicPlaying = false;
        }

        if (daySuccessfulTimer <= 0.0f) successfulDaySound[successfulDaySoundIndex].PlayAudio();

        /* If day successful timer is less than 1 second, initialize the day successful text and increment the day
        successful timer for text fade */
        if (daySuccessfulTimer < 2.0f)
        {
            daySuccessfulTimer += deltaTime;

            daySuccessfulText.InitializeText("Fonts/Roboto-Regular.ttf", "You passed Day 5! That's it, thanks for playing!", 
                daySuccessfulCharacterSize, true, false, Color(daySuccessfulRedValue, daySuccessfulGreenValue, 
                    daySuccessfulBlueValue, daySuccessfulTextAlpha), Vector2f(resolution.x / 2.0f, resolution.y / 2.0f));
        }

        // If day successful timer is around 1 second or so, make the text fade away overtime
        else if (daySuccessfulTimer >= 2.0f)
        {
            daySuccessfulTextAlpha -= deltaTime * 100.0f;

            if (daySuccessfulText.GetTextColor() != Color(daySuccessfulRedValue, daySuccessfulGreenValue, daySuccessfulBlueValue, daySuccessfulTextAlpha))
                daySuccessfulText.SetTextColor(Color(daySuccessfulRedValue, daySuccessfulGreenValue, daySuccessfulBlueValue, daySuccessfulTextAlpha));
        }

        //printf("day successful timer: %f\n", daySuccessfulTimer);

        // If day successful text's alpha value goes down to 0, go back to main menu and switch to the next day
        if (daySuccessfulTextAlpha <= 0.0f)
        {
            for (int i = 0; i < successfulDaySound.size(); i++) successfulDaySound[i].StopAudio();

            typewriterEffect.Reset();
            itemTable.ResetCollectedItems();
            bag.ClearBag();
            if (completedDays <= 5) completedDays++;
            currentDay = 1;
            Menu::nextDayUnlocked = false;
            sceneManager->ChangeScene("Menu");
            return;
        }

        break;
    }
    default:
        break;
    }
}

void GameScene::RenderDay5(RenderWindow& window)
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
        /*if (bag.IsVisible())
        {
            bag.Draw(window);
        }*/

        break;
    }

    case GameState::OPERATION_ACTIVE:
    {
        // Draw operation scene and overlay the UI from surgery room
        operationScene.Draw(window);
        surgeryRoom.DrawUI(window);  // Draw the UI elements on top of operation scene

        // Draw bag if visible
        /*if (bag.IsVisible())
        {
            bag.Draw(window);
        }*/

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
        /*if (bag.IsVisible())
        {
            bag.Draw(window);
        }*/

        break;
    }

    case GameState::TEXTBOOK_ACTIVE:
    {
        textbookPages.Draw(window);
        surgeryRoom.DrawUI(window);  // Draw the UI elements on top of textbook
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

void GameScene::UpdateDay5Patients()
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

            operationScene.maxDots = 12;

            operationScene.InitializeDot({ Vector2f(resolution.x / 1.56f, resolution.y / 1.48f),
                Vector2f(resolution.x / 2.49f, resolution.y / 1.43f),
                Vector2f(resolution.x / 2.46f, resolution.y / 1.73f),
                Vector2f(resolution.x / 2.33f, resolution.y / 2.37f),
                Vector2f(resolution.x / 1.62f, resolution.y / 1.67f),
                Vector2f(resolution.x / 1.66f, resolution.y / 1.97f),
                Vector2f(resolution.x / 1.69f, resolution.y / 2.58f),
                Vector2f(resolution.x / 1.90f, resolution.y / 5.51f),
                Vector2f(resolution.x / 2.00f, resolution.y / 5.51f),
                Vector2f(resolution.x / 2.15f, resolution.y / 3.56f),
                Vector2f(resolution.x / 1.79f, resolution.y / 3.44f),
                Vector2f(resolution.x / 1.84f, resolution.y / 2.88f) },
                6.0f * (resolution.x / 1920.0f), Color::Red, Color::Red,
                3.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2));

            operationSceneChanged = true;
        }

        break;

    case 2: // 3rd patient
        if (!operationSceneChanged)
        {
            operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                Vector2f(resolution.x / 2.8f, 0.0f),
                Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                true);

            operationScene.maxDots = 14;

            operationScene.InitializeDot({ Vector2f(resolution.x / 1.75f, resolution.y / 1.26f),
                Vector2f(resolution.x / 1.79f, resolution.y / 1.26f),
                Vector2f(resolution.x / 1.85f, resolution.y / 1.26f),
                Vector2f(resolution.x / 1.91f, resolution.y / 1.26f),
                Vector2f(resolution.x / 2.00f, resolution.y / 1.26f),
                Vector2f(resolution.x / 2.06f, resolution.y / 1.26f),
                Vector2f(resolution.x / 2.14f, resolution.y / 1.26f),
                Vector2f(resolution.x / 2.24f, resolution.y / 1.26f),
                Vector2f(resolution.x / 2.17f, resolution.y / 2.07f),
                Vector2f(resolution.x / 2.09f, resolution.y / 2.07f),
                Vector2f(resolution.x / 2.00f, resolution.y / 2.07f),
                Vector2f(resolution.x / 1.91f, resolution.y / 2.07f),
                Vector2f(resolution.x / 1.84f, resolution.y / 2.07f),
                Vector2f(resolution.x / 1.78f, resolution.y / 2.07f) },
                6.0f * (resolution.x / 1920.0f), Color::Red, Color::Red,
                3.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2));

            operationSceneChanged = true;
        }
        break;

    case 3: // 4th patient
        if (!operationSceneChanged)
        {
            operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                Vector2f(resolution.x / 2.8f, 0.0f),
                Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                true);

            operationScene.maxDots = 15;

            operationScene.InitializeDot({ Vector2f(resolution.x / 1.71f, resolution.y / 3.70f),
                Vector2f(resolution.x / 1.73f, resolution.y / 3.38f),
                Vector2f(resolution.x / 1.77f, resolution.y / 3.07f),
                Vector2f(resolution.x / 1.81f, resolution.y / 2.76f),
                Vector2f(resolution.x / 1.88f, resolution.y / 2.41f),
                Vector2f(resolution.x / 1.95f, resolution.y / 2.17f),
                Vector2f(resolution.x / 2.01f, resolution.y / 2.35f),
                Vector2f(resolution.x / 2.09f, resolution.y / 2.55f),
                Vector2f(resolution.x / 2.17f, resolution.y / 2.86f),
                Vector2f(resolution.x / 2.25f, resolution.y / 3.29f),
                Vector2f(resolution.x / 2.32f, resolution.y / 3.83f),
                Vector2f(resolution.x / 2.22f, resolution.y / 4.06f),
                Vector2f(resolution.x / 2.12f, resolution.y / 4.32f),
                Vector2f(resolution.x / 2.01f, resolution.y / 4.68f),
                Vector2f(resolution.x / 1.87f, resolution.y / 4.64f) },
                6.0f * (resolution.x / 1920.0f), Color::Red, Color::Red,
                3.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2));

            operationSceneChanged = true;
        }
        break;

    case 4: // 5th patient
        if (!operationSceneChanged)
        {
            operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                Vector2f(resolution.x / 2.8f, 0.0f),
                Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                true);

            operationScene.maxDots = 15;

            operationScene.InitializeDot({ Vector2f(resolution.x / 1.90f, resolution.y / 5.51f),
                Vector2f(resolution.x / 2.01f, resolution.y / 5.51f),
                Vector2f(resolution.x / 2.07f, resolution.y / 4.50f),
                Vector2f(resolution.x / 2.14f, resolution.y / 4.17f),
                Vector2f(resolution.x / 2.23f, resolution.y / 3.90f),
                Vector2f(resolution.x / 2.33f, resolution.y / 3.46f),
                Vector2f(resolution.x / 2.36f, resolution.y / 3.07f),
                Vector2f(resolution.x / 2.36f, resolution.y / 2.78f),
                Vector2f(resolution.x / 2.37f, resolution.y / 2.52f),
                Vector2f(resolution.x / 2.26f, resolution.y / 2.52f),
                Vector2f(resolution.x / 2.25f, resolution.y / 2.86f),
                Vector2f(resolution.x / 2.24f, resolution.y / 3.26f),
                Vector2f(resolution.x / 2.18f, resolution.y / 3.46f),
                Vector2f(resolution.x / 2.09f, resolution.y / 3.64f),
                Vector2f(resolution.x / 1.95f, resolution.y / 3.69f) },
                6.0f * (resolution.x / 1920.0f), Color::Red, Color::Red,
                3.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2));

            operationSceneChanged = true;
        }
        break;

    case 5: // 6th patient
        if (!operationSceneChanged)
        {
            operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                Vector2f(resolution.x / 2.8f, 0.0f),
                Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                true);

            operationScene.maxDots = 17;

            operationScene.InitializeDot({ Vector2f(resolution.x / 1.96f, resolution.y / 4.58f),
                Vector2f(resolution.x / 2.03f, resolution.y / 3.94f),
                Vector2f(resolution.x / 2.11f, resolution.y / 3.36f),
                Vector2f(resolution.x / 2.20f, resolution.y / 2.93f),
                Vector2f(resolution.x / 2.32f, resolution.y / 2.54f),
                Vector2f(resolution.x / 2.42f, resolution.y / 2.28f),
                Vector2f(resolution.x / 2.32f, resolution.y / 2.03f),
                Vector2f(resolution.x / 2.21f, resolution.y / 1.82f),
                Vector2f(resolution.x / 2.13f, resolution.y / 1.64f),
                Vector2f(resolution.x / 2.06f, resolution.y / 1.50f),
                Vector2f(resolution.x / 1.99f, resolution.y / 1.37f),
                Vector2f(resolution.x / 1.87f, resolution.y / 1.43f),
                Vector2f(resolution.x / 1.78f, resolution.y / 1.52f),
                Vector2f(resolution.x / 1.72f, resolution.y / 1.63f),
                Vector2f(resolution.x / 1.70f, resolution.y / 2.23f),
                Vector2f(resolution.x / 1.66f, resolution.y / 2.54f),
                Vector2f(resolution.x / 1.71f, resolution.y / 2.88f) },
                6.0f * (resolution.x / 1920.0f), Color::Red, Color::Red,
                3.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2));

            operationSceneChanged = true;
        }
        break;

    case 6: // 7th patient
        if (!operationSceneChanged)
        {
            operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                Vector2f(resolution.x / 2.8f, 0.0f),
                Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                true);

            operationScene.maxDots = 17;

            operationScene.InitializeDot({ Vector2f(resolution.x / 1.96f, resolution.y / 5.48f),
                Vector2f(resolution.x / 2.00f, resolution.y / 4.80f),
                Vector2f(resolution.x / 2.07f, resolution.y / 4.12f),
                Vector2f(resolution.x / 2.16f, resolution.y / 3.70f),
                Vector2f(resolution.x / 2.26f, resolution.y / 3.29f),
                Vector2f(resolution.x / 2.36f, resolution.y / 3.02f),
                Vector2f(resolution.x / 1.88f, resolution.y / 4.62f),
                Vector2f(resolution.x / 1.82f, resolution.y / 4.00f),
                Vector2f(resolution.x / 1.77f, resolution.y / 3.52f),
                Vector2f(resolution.x / 1.73f, resolution.y / 3.10f),
                Vector2f(resolution.x / 1.68f, resolution.y / 2.76f),
                Vector2f(resolution.x / 1.95f, resolution.y / 2.04f),
                Vector2f(resolution.x / 1.95f, resolution.y / 4.08f),
                Vector2f(resolution.x / 1.96f, resolution.y / 3.40f),
                Vector2f(resolution.x / 1.96f, resolution.y / 2.90f),
                Vector2f(resolution.x / 1.96f, resolution.y / 2.58f),
                Vector2f(resolution.x / 1.96f, resolution.y / 2.24f) },
                6.0f * (resolution.x / 1920.0f), Color::Red, Color::Red,
                3.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2));

            operationSceneChanged = true;
        }
        break;

    case 7: // 8th patient
        if (!operationSceneChanged)
        {
            operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                Vector2f(resolution.x / 2.8f, 0.0f),
                Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                true);

            operationScene.maxDots = 18;

            operationScene.InitializeDot({ Vector2f(resolution.x / 2.21f, resolution.y / 1.32f),
                Vector2f(resolution.x / 2.11f, resolution.y / 1.53f),
                Vector2f(resolution.x / 1.80f, resolution.y / 1.34f),
                Vector2f(resolution.x / 1.77f, resolution.y / 1.67f),
                Vector2f(resolution.x / 2.20f, resolution.y / 1.77f),
                Vector2f(resolution.x / 2.54f, resolution.y / 1.42f),
                Vector2f(resolution.x / 1.94f, resolution.y / 37.24f),
                Vector2f(resolution.x / 2.02f, resolution.y / 7.66f),
                Vector2f(resolution.x / 1.91f, resolution.y / 4.98f),
                Vector2f(resolution.x / 1.74f, resolution.y / 3.54f),
                Vector2f(resolution.x / 1.88f, resolution.y / 2.80f),
                Vector2f(resolution.x / 1.65f, resolution.y / 2.08f),
                Vector2f(resolution.x / 1.63f, resolution.y / 1.77f),
                Vector2f(resolution.x / 1.94f, resolution.y / 2.05f),
                Vector2f(resolution.x / 2.11f, resolution.y / 2.35f),
                Vector2f(resolution.x / 1.84f, resolution.y / 2.64f),
                Vector2f(resolution.x / 2.11f, resolution.y / 1.66f),
                Vector2f(resolution.x / 1.84f, resolution.y / 2.00f) },
                6.0f * (resolution.x / 1920.0f), Color::Red, Color::Red,
                3.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2));

            operationSceneChanged = true;
        }
        break;

    case 8: // 9th patient
        if (!operationSceneChanged)
        {
            operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                Vector2f(resolution.x / 2.8f, 0.0f),
                Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                true);

            operationScene.maxDots = 18;

            operationScene.InitializeDot({ Vector2f(resolution.x / 2.01f, resolution.y / 4.98f),
                Vector2f(resolution.x / 2.07f, resolution.y / 4.64f),
                Vector2f(resolution.x / 2.13f, resolution.y / 4.34f),
                Vector2f(resolution.x / 2.21f, resolution.y / 4.03f),
                Vector2f(resolution.x / 2.28f, resolution.y / 3.67f),
                Vector2f(resolution.x / 2.34f, resolution.y / 3.27f),
                Vector2f(resolution.x / 2.35f, resolution.y / 3.02f),
                Vector2f(resolution.x / 1.97f, resolution.y / 4.98f),
                Vector2f(resolution.x / 1.89f, resolution.y / 4.89f),
                Vector2f(resolution.x / 1.82f, resolution.y / 4.56f),
                Vector2f(resolution.x / 1.76f, resolution.y / 4.22f),
                Vector2f(resolution.x / 1.70f, resolution.y / 3.75f),
                Vector2f(resolution.x / 1.67f, resolution.y / 3.18f),
                Vector2f(resolution.x / 1.71f, resolution.y / 2.83f),
                Vector2f(resolution.x / 2.19f, resolution.y / 3.05f),
                Vector2f(resolution.x / 2.11f, resolution.y / 2.65f),
                Vector2f(resolution.x / 1.81f, resolution.y / 2.63f),
                Vector2f(resolution.x / 1.97f, resolution.y / 2.28f) },
                6.0f * (resolution.x / 1920.0f), Color::Red, Color::Red,
                3.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2));

            operationSceneChanged = true;
        }
        break;

    case 9: // 10th patient
        if (!operationSceneChanged)
        {
            operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                Vector2f(resolution.x / 2.8f, 0.0f),
                Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                true);

            operationScene.maxDots = 20;

            operationScene.InitializeDot({ Vector2f(resolution.x / 2.03f, resolution.y / 5.51f),
                Vector2f(resolution.x / 1.95f, resolution.y / 5.51f),
                Vector2f(resolution.x / 1.88f, resolution.y / 5.57f),
                Vector2f(resolution.x / 2.03f, resolution.y / 4.62f),
                Vector2f(resolution.x / 1.94f, resolution.y / 4.62f),
                Vector2f(resolution.x / 1.87f, resolution.y / 4.62f),
                Vector2f(resolution.x / 2.07f, resolution.y / 4.14f),
                Vector2f(resolution.x / 1.96f, resolution.y / 4.08f),
                Vector2f(resolution.x / 1.86f, resolution.y / 4.08f),
                Vector2f(resolution.x / 2.02f, resolution.y / 3.54f),
                Vector2f(resolution.x / 2.04f, resolution.y / 1.88f),
                Vector2f(resolution.x / 1.77f, resolution.y / 1.79f),
                Vector2f(resolution.x / 1.93f, resolution.y / 2.36f),
                Vector2f(resolution.x / 1.56f, resolution.y / 1.48f),
                Vector2f(resolution.x / 2.17f, resolution.y / 1.34f),
                Vector2f(resolution.x / 1.84f, resolution.y / 1.26f),
                Vector2f(resolution.x / 1.72f, resolution.y / 1.46f),
                Vector2f(resolution.x / 2.24f, resolution.y / 1.77f),
                Vector2f(resolution.x / 2.16f, resolution.y / 2.06f),
                Vector2f(resolution.x / 1.79f, resolution.y / 2.11f) },
                6.0f * (resolution.x / 1920.0f), Color::Red, Color::Red,
                3.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2));

            operationSceneChanged = true;
        }
        break;

    case 10: // 11th patient
        if (!operationSceneChanged)
        {
            operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                Vector2f(resolution.x / 2.8f, 0.0f),
                Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                true);

            operationScene.maxDots = 22;

            operationScene.InitializeDot({ Vector2f(resolution.x / 1.74f, resolution.y / 1.26f),
                Vector2f(resolution.x / 1.80f, resolution.y / 1.32f),
                Vector2f(resolution.x / 1.93f, resolution.y / 1.32f),
                Vector2f(resolution.x / 2.06f, resolution.y / 1.31f),
                Vector2f(resolution.x / 2.19f, resolution.y / 1.30f),
                Vector2f(resolution.x / 2.26f, resolution.y / 1.26f),
                Vector2f(resolution.x / 2.06f, resolution.y / 1.36f),
                Vector2f(resolution.x / 1.99f, resolution.y / 1.44f),
                Vector2f(resolution.x / 2.10f, resolution.y / 4.48f),
                Vector2f(resolution.x / 1.88f, resolution.y / 1.42f),
                Vector2f(resolution.x / 1.95f, resolution.y / 1.49f),
                Vector2f(resolution.x / 1.95f, resolution.y / 1.58f),
                Vector2f(resolution.x / 1.95f, resolution.y / 1.71f),
                Vector2f(resolution.x / 1.96f, resolution.y / 1.86f),
                Vector2f(resolution.x / 1.95f, resolution.y / 2.05f),
                Vector2f(resolution.x / 2.01f, resolution.y / 2.25f),
                Vector2f(resolution.x / 1.89f, resolution.y / 2.25f),
                Vector2f(resolution.x / 1.82f, resolution.y / 2.46f),
                Vector2f(resolution.x / 2.09f, resolution.y / 2.47f),
                Vector2f(resolution.x / 1.78f, resolution.y / 3.45f),
                Vector2f(resolution.x / 2.12f, resolution.y / 3.50f),
                Vector2f(resolution.x / 1.81f, resolution.y / 4.48f) },
                6.0f * (resolution.x / 1920.0f), Color::Red, Color::Red,
                3.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2));

            operationSceneChanged = true;
        }
        break;

    case 11: // 12th patient
        if (!operationSceneChanged)
        {
            operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                Vector2f(resolution.x / 2.8f, 0.0f),
                Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                true);

            operationScene.maxDots = 23;

            operationScene.InitializeDot({ Vector2f(resolution.x / 2.56f, resolution.y / 1.33f),
                Vector2f(resolution.x / 2.56f, resolution.y / 1.42f),
                Vector2f(resolution.x / 2.51f, resolution.y / 1.54f),
                Vector2f(resolution.x / 2.45f, resolution.y / 1.72f),
                Vector2f(resolution.x / 2.41f, resolution.y / 1.94f),
                Vector2f(resolution.x / 2.35f, resolution.y / 2.18f),
                Vector2f(resolution.x / 2.32f, resolution.y / 2.53f),
                Vector2f(resolution.x / 2.22f, resolution.y / 2.93f),
                Vector2f(resolution.x / 2.08f, resolution.y / 2.92f),
                Vector2f(resolution.x / 1.96f, resolution.y / 2.93f),
                Vector2f(resolution.x / 1.85f, resolution.y / 2.86f),
                Vector2f(resolution.x / 1.75f, resolution.y / 2.78f),
                Vector2f(resolution.x / 1.68f, resolution.y / 2.59f),
                Vector2f(resolution.x / 1.67f, resolution.y / 2.28f),
                Vector2f(resolution.x / 1.66f, resolution.y / 2.03f),
                Vector2f(resolution.x / 1.64f, resolution.y / 1.85f),
                Vector2f(resolution.x / 1.62f, resolution.y / 1.68f),
                Vector2f(resolution.x / 1.60f, resolution.y / 1.53f),
                Vector2f(resolution.x / 1.58f, resolution.y / 1.43f),
                Vector2f(resolution.x / 1.57f, resolution.y / 1.34f),
                Vector2f(resolution.x / 2.35f, resolution.y / 3.29f),
                Vector2f(resolution.x / 1.68f, resolution.y / 3.25f),
                Vector2f(resolution.x / 1.96f, resolution.y / 54.00f) },
                6.0f * (resolution.x / 1920.0f), Color::Red, Color::Red,
                3.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2));

            operationSceneChanged = true;
        }
        break;

    case 12: // 13th patient
        if (!operationSceneChanged)
        {
            operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                Vector2f(resolution.x / 2.8f, 0.0f),
                Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                true);

            operationScene.maxDots = 24;

            operationScene.InitializeDot({ Vector2f(resolution.x / 1.86f, resolution.y / 24.00f),
                Vector2f(resolution.x / 1.91f, resolution.y / 43.20f),
                Vector2f(resolution.x / 1.98f, resolution.y / 43.20f),
                Vector2f(resolution.x / 2.03f, resolution.y / 27.69f),
                Vector2f(resolution.x / 2.06f, resolution.y / 15.65f),
                Vector2f(resolution.x / 2.06f, resolution.y / 10.38f),
                Vector2f(resolution.x / 2.04f, resolution.y / 8.00f),
                Vector2f(resolution.x / 1.95f, resolution.y / 7.06f),
                Vector2f(resolution.x / 1.86f, resolution.y / 7.88f),
                Vector2f(resolution.x / 1.85f, resolution.y / 11.74f),
                Vector2f(resolution.x / 2.05f, resolution.y / 4.60f),
                Vector2f(resolution.x / 1.96f, resolution.y / 4.56f),
                Vector2f(resolution.x / 1.86f, resolution.y / 4.52f),
                Vector2f(resolution.x / 1.77f, resolution.y / 4.19f),
                Vector2f(resolution.x / 2.12f, resolution.y / 4.35f),
                Vector2f(resolution.x / 2.25f, resolution.y / 3.83f),
                Vector2f(resolution.x / 1.70f, resolution.y / 3.74f),
                Vector2f(resolution.x / 1.95f, resolution.y / 4.03f),
                Vector2f(resolution.x / 1.95f, resolution.y / 3.54f),
                Vector2f(resolution.x / 1.95f, resolution.y / 3.00f),
                Vector2f(resolution.x / 1.95f, resolution.y / 2.65f),
                Vector2f(resolution.x / 1.95f, resolution.y / 2.30f),
                Vector2f(resolution.x / 1.95f, resolution.y / 2.07f),
                Vector2f(resolution.x / 1.95f, resolution.y / 1.88f) },
                6.0f * (resolution.x / 1920.0f), Color::Red, Color::Red,
                3.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2));

            operationSceneChanged = true;
        }
        break;

    case 13: // 14th patient
        if (!operationSceneChanged)
        {
            operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                Vector2f(resolution.x / 2.8f, 0.0f),
                Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                true);

            operationScene.maxDots = 24;

            operationScene.InitializeDot({ Vector2f(resolution.x / 2.12f, resolution.y / 1.26f),
                Vector2f(resolution.x / 2.12f, resolution.y / 1.34f),
                Vector2f(resolution.x / 2.13f, resolution.y / 1.44f),
                Vector2f(resolution.x / 2.14f, resolution.y / 1.58f),
                Vector2f(resolution.x / 2.14f, resolution.y / 1.74f),
                Vector2f(resolution.x / 2.15f, resolution.y / 1.95f),
                Vector2f(resolution.x / 2.15f, resolution.y / 2.11f),
                Vector2f(resolution.x / 2.15f, resolution.y / 2.33f),
                Vector2f(resolution.x / 2.15f, resolution.y / 2.54f),
                Vector2f(resolution.x / 2.15f, resolution.y / 2.86f),
                Vector2f(resolution.x / 1.82f, resolution.y / 1.26f),
                Vector2f(resolution.x / 1.82f, resolution.y / 1.33f),
                Vector2f(resolution.x / 1.83f, resolution.y / 1.42f),
                Vector2f(resolution.x / 1.83f, resolution.y / 1.51f),
                Vector2f(resolution.x / 1.83f, resolution.y / 1.61f),
                Vector2f(resolution.x / 1.84f, resolution.y / 1.76f),
                Vector2f(resolution.x / 1.84f, resolution.y / 1.97f),
                Vector2f(resolution.x / 1.84f, resolution.y / 2.16f),
                Vector2f(resolution.x / 1.84f, resolution.y / 2.40f),
                Vector2f(resolution.x / 1.85f, resolution.y / 2.65f),
                Vector2f(resolution.x / 2.00f, resolution.y / 5.45f),
                Vector2f(resolution.x / 1.89f, resolution.y / 5.45f),
                Vector2f(resolution.x / 1.95f, resolution.y / 12.56f),
                Vector2f(resolution.x / 1.94f, resolution.y / 29.19f) },
                6.0f * (resolution.x / 1920.0f), Color::Red, Color::Red,
                3.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2));

            operationSceneChanged = true;
        }
        break;

    case 14: // 15th patient
        if (!operationSceneChanged)
        {
            operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                Vector2f(resolution.x / 2.8f, 0.0f),
                Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                true);

            operationScene.maxDots = 25;

            operationScene.InitializeDot({ Vector2f(resolution.x / 1.86f, resolution.y / 3.47f),
                Vector2f(resolution.x / 1.82f, resolution.y / 3.25f),
                Vector2f(resolution.x / 1.76f, resolution.y / 3.28f),
                Vector2f(resolution.x / 1.74f, resolution.y / 3.61f),
                Vector2f(resolution.x / 1.79f, resolution.y / 3.84f),
                Vector2f(resolution.x / 1.84f, resolution.y / 3.83f),
                Vector2f(resolution.x / 2.12f, resolution.y / 2.75f),
                Vector2f(resolution.x / 2.10f, resolution.y / 2.42f),
                Vector2f(resolution.x / 2.09f, resolution.y / 2.16f),
                Vector2f(resolution.x / 2.15f, resolution.y / 1.91f),
                Vector2f(resolution.x / 2.20f, resolution.y / 1.72f),
                Vector2f(resolution.x / 1.74f, resolution.y / 1.77f),
                Vector2f(resolution.x / 1.77f, resolution.y / 1.91f),
                Vector2f(resolution.x / 1.79f, resolution.y / 2.11f),
                Vector2f(resolution.x / 1.79f, resolution.y / 2.39f),
                Vector2f(resolution.x / 1.76f, resolution.y / 2.71f),
                Vector2f(resolution.x / 1.70f, resolution.y / 2.61f),
                Vector2f(resolution.x / 1.66f, resolution.y / 2.61f),
                Vector2f(resolution.x / 2.27f, resolution.y / 2.73f),
                Vector2f(resolution.x / 2.36f, resolution.y / 2.71f),
                Vector2f(resolution.x / 2.37f, resolution.y / 2.39f),
                Vector2f(resolution.x / 2.28f, resolution.y / 2.38f),
                Vector2f(resolution.x / 1.71f, resolution.y / 2.38f),
                Vector2f(resolution.x / 1.65f, resolution.y / 2.43f),
                Vector2f(resolution.x / 2.02f, resolution.y / 5.29f) },
                6.0f * (resolution.x / 1920.0f), Color::Red, Color::Red,
                3.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2));

            operationSceneChanged = true;
        }
        break;

    case 15: // 16th patient
        if (!operationSceneChanged)
        {
            operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                Vector2f(resolution.x / 2.8f, 0.0f),
                Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                true);

            operationScene.maxDots = 26;

            operationScene.InitializeDot({ Vector2f(resolution.x / 2.37f, resolution.y / 2.57f),
                Vector2f(resolution.x / 2.31f, resolution.y / 2.86f),
                Vector2f(resolution.x / 2.28f, resolution.y / 3.20f),
                Vector2f(resolution.x / 2.23f, resolution.y / 3.74f),
                Vector2f(resolution.x / 2.18f, resolution.y / 4.25f),
                Vector2f(resolution.x / 2.08f, resolution.y / 4.22f),
                Vector2f(resolution.x / 1.99f, resolution.y / 4.22f),
                Vector2f(resolution.x / 1.91f, resolution.y / 4.22f),
                Vector2f(resolution.x / 1.84f, resolution.y / 4.17f),
                Vector2f(resolution.x / 1.76f, resolution.y / 4.12f),
                Vector2f(resolution.x / 1.71f, resolution.y / 4.09f),
                Vector2f(resolution.x / 1.74f, resolution.y / 3.59f),
                Vector2f(resolution.x / 1.77f, resolution.y / 3.16f),
                Vector2f(resolution.x / 1.80f, resolution.y / 2.78f),
                Vector2f(resolution.x / 1.83f, resolution.y / 2.45f),
                Vector2f(resolution.x / 1.91f, resolution.y / 2.44f),
                Vector2f(resolution.x / 2.00f, resolution.y / 2.43f),
                Vector2f(resolution.x / 2.11f, resolution.y / 2.43f),
                Vector2f(resolution.x / 2.18f, resolution.y / 2.42f),
                Vector2f(resolution.x / 2.16f, resolution.y / 2.25f),
                Vector2f(resolution.x / 2.10f, resolution.y / 2.24f),
                Vector2f(resolution.x / 2.01f, resolution.y / 2.24f),
                Vector2f(resolution.x / 1.95f, resolution.y / 2.23f),
                Vector2f(resolution.x / 1.89f, resolution.y / 2.22f),
                Vector2f(resolution.x / 1.84f, resolution.y / 2.21f),
                Vector2f(resolution.x / 1.78f, resolution.y / 2.20f) },
                6.0f * (resolution.x / 1920.0f), Color::Red, Color::Red,
                3.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2));

            operationSceneChanged = true;
        }
        break;

    case 16: // 17th patient
        if (!operationSceneChanged)
        {
            operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                Vector2f(resolution.x / 2.8f, 0.0f),
                Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                true);

            operationScene.maxDots = 26;

            operationScene.InitializeDot({ Vector2f(resolution.x / 1.88f, resolution.y / 24.00f),
                Vector2f(resolution.x / 2.06f, resolution.y / 11.87f),
                Vector2f(resolution.x / 1.95f, resolution.y / 7.45f),
                Vector2f(resolution.x / 2.01f, resolution.y / 5.29f),
                Vector2f(resolution.x / 1.89f, resolution.y / 4.72f),
                Vector2f(resolution.x / 1.97f, resolution.y / 4.20f),
                Vector2f(resolution.x / 2.10f, resolution.y / 4.00f),
                Vector2f(resolution.x / 2.29f, resolution.y / 3.80f),
                Vector2f(resolution.x / 2.34f, resolution.y / 3.24f),
                Vector2f(resolution.x / 2.17f, resolution.y / 3.38f),
                Vector2f(resolution.x / 2.07f, resolution.y / 3.56f),
                Vector2f(resolution.x / 2.07f, resolution.y / 3.08f),
                Vector2f(resolution.x / 1.98f, resolution.y / 2.74f),
                Vector2f(resolution.x / 1.82f, resolution.y / 2.61f),
                Vector2f(resolution.x / 1.68f, resolution.y / 2.36f),
                Vector2f(resolution.x / 2.39f, resolution.y / 1.97f),
                Vector2f(resolution.x / 2.20f, resolution.y / 1.84f),
                Vector2f(resolution.x / 2.25f, resolution.y / 1.62f),
                Vector2f(resolution.x / 2.08f, resolution.y / 1.58f),
                Vector2f(resolution.x / 2.01f, resolution.y / 1.82f),
                Vector2f(resolution.x / 2.08f, resolution.y / 2.16f),
                Vector2f(resolution.x / 1.99f, resolution.y / 2.51f),
                Vector2f(resolution.x / 1.84f, resolution.y / 2.43f),
                Vector2f(resolution.x / 1.84f, resolution.y / 1.75f),
                Vector2f(resolution.x / 1.83f, resolution.y / 1.36f),
                Vector2f(resolution.x / 2.07f, resolution.y / 1.45f) },
                6.0f * (resolution.x / 1920.0f), Color::Red, Color::Red,
                3.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2));

            operationSceneChanged = true;
        }
        break;

    case 17: // 18th patient
        if (!operationSceneChanged)
        {
            operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                Vector2f(resolution.x / 2.8f, 0.0f),
                Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                true);

            operationScene.maxDots = 28;

            operationScene.InitializeDot({ Vector2f(resolution.x / 2.21f, resolution.y / 1.27f),
                Vector2f(resolution.x / 2.03f, resolution.y / 1.30f),
                Vector2f(resolution.x / 1.90f, resolution.y / 1.28f),
                Vector2f(resolution.x / 1.75f, resolution.y / 1.26f),
                Vector2f(resolution.x / 1.83f, resolution.y / 1.32f),
                Vector2f(resolution.x / 1.75f, resolution.y / 1.40f),
                Vector2f(resolution.x / 2.26f, resolution.y / 1.36f),
                Vector2f(resolution.x / 2.10f, resolution.y / 1.35f),
                Vector2f(resolution.x / 1.97f, resolution.y / 1.39f),
                Vector2f(resolution.x / 1.87f, resolution.y / 1.46f),
                Vector2f(resolution.x / 2.16f, resolution.y / 1.47f),
                Vector2f(resolution.x / 1.78f, resolution.y / 1.60f),
                Vector2f(resolution.x / 2.28f, resolution.y / 1.61f),
                Vector2f(resolution.x / 1.73f, resolution.y / 1.73f),
                Vector2f(resolution.x / 2.19f, resolution.y / 1.89f),
                Vector2f(resolution.x / 2.01f, resolution.y / 1.82f),
                Vector2f(resolution.x / 1.91f, resolution.y / 1.91f),
                Vector2f(resolution.x / 1.91f, resolution.y / 1.71f),
                Vector2f(resolution.x / 1.87f, resolution.y / 2.05f),
                Vector2f(resolution.x / 2.03f, resolution.y / 2.06f),
                Vector2f(resolution.x / 2.12f, resolution.y / 2.37f),
                Vector2f(resolution.x / 1.93f, resolution.y / 2.37f),
                Vector2f(resolution.x / 1.78f, resolution.y / 2.36f),
                Vector2f(resolution.x / 2.36f, resolution.y / 2.26f),
                Vector2f(resolution.x / 2.42f, resolution.y / 2.04f),
                Vector2f(resolution.x / 1.67f, resolution.y / 2.10f),
                Vector2f(resolution.x / 1.64f, resolution.y / 1.89f),
                Vector2f(resolution.x / 1.62f, resolution.y / 1.69f) },
                6.0f * (resolution.x / 1920.0f), Color::Red, Color::Red,
                3.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2));

            operationSceneChanged = true;
        }
        break;

    case 18: // 19th patient
        if (!operationSceneChanged)
        {
            operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                Vector2f(resolution.x / 2.8f, 0.0f),
                Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                true);

            operationScene.maxDots = 28;

            operationScene.InitializeDot({ Vector2f(resolution.x / 1.96f, resolution.y / 5.43f),
                Vector2f(resolution.x / 2.02f, resolution.y / 4.62f),
                Vector2f(resolution.x / 1.88f, resolution.y / 3.78f),
                Vector2f(resolution.x / 2.13f, resolution.y / 3.28f),
                Vector2f(resolution.x / 2.02f, resolution.y / 8.00f),
                Vector2f(resolution.x / 1.93f, resolution.y / 8.24f),
                Vector2f(resolution.x / 1.86f, resolution.y / 11.13f),
                Vector2f(resolution.x / 2.07f, resolution.y / 13.50f),
                Vector2f(resolution.x / 1.88f, resolution.y / 26.34f),
                Vector2f(resolution.x / 1.95f, resolution.y / 83.08f),
                Vector2f(resolution.x / 2.00f, resolution.y / 31.76f),
                Vector2f(resolution.x / 1.82f, resolution.y / 3.04f),
                Vector2f(resolution.x / 1.99f, resolution.y / 2.81f),
                Vector2f(resolution.x / 2.25f, resolution.y / 2.86f),
                Vector2f(resolution.x / 2.32f, resolution.y / 2.45f),
                Vector2f(resolution.x / 2.37f, resolution.y / 2.18f),
                Vector2f(resolution.x / 2.40f, resolution.y / 1.96f),
                Vector2f(resolution.x / 2.46f, resolution.y / 1.78f),
                Vector2f(resolution.x / 2.50f, resolution.y / 1.63f),
                Vector2f(resolution.x / 2.53f, resolution.y / 1.50f),
                Vector2f(resolution.x / 2.55f, resolution.y / 1.37f),
                Vector2f(resolution.x / 1.59f, resolution.y / 1.36f),
                Vector2f(resolution.x / 1.59f, resolution.y / 1.48f),
                Vector2f(resolution.x / 1.61f, resolution.y / 1.61f),
                Vector2f(resolution.x / 1.63f, resolution.y / 1.81f),
                Vector2f(resolution.x / 1.65f, resolution.y / 1.99f),
                Vector2f(resolution.x / 1.67f, resolution.y / 2.33f),
                Vector2f(resolution.x / 1.69f, resolution.y / 2.64f) },
                6.0f * (resolution.x / 1920.0f), Color::Red, Color::Red,
                3.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2));

            operationSceneChanged = true;
        }
        break;

    case 19: // 20th patient
        if (!operationSceneChanged)
        {
            operationScene.Initialize("Art Assets/SurgeryRoom/sickness/basebody.png",
                Vector2f(resolution.x / 2.8f, 0.0f),
                Vector2f(3.0f * (resolution.x / 1920.0f), 3.0f * (resolution.y / 1080.0f)),
                true);

            operationScene.maxDots = 30;

            operationScene.InitializeDot({ Vector2f(resolution.x / 2.04f, resolution.y / 24.55f),
                Vector2f(resolution.x / 2.04f, resolution.y / 14.40f),
                Vector2f(resolution.x / 2.04f, resolution.y / 9.82f),
                Vector2f(resolution.x / 1.95f, resolution.y / 7.40f),
                Vector2f(resolution.x / 1.87f, resolution.y / 9.39f),
                Vector2f(resolution.x / 1.86f, resolution.y / 13.85f),
                Vector2f(resolution.x / 1.86f, resolution.y / 24.00f),
                Vector2f(resolution.x / 2.02f, resolution.y / 5.45f),
                Vector2f(resolution.x / 1.88f, resolution.y / 5.40f),
                Vector2f(resolution.x / 1.94f, resolution.y / 4.43f),
                Vector2f(resolution.x / 2.01f, resolution.y / 3.86f),
                Vector2f(resolution.x / 2.10f, resolution.y / 3.27f),
                Vector2f(resolution.x / 1.98f, resolution.y / 2.67f),
                Vector2f(resolution.x / 1.81f, resolution.y / 2.22f),
                Vector2f(resolution.x / 1.99f, resolution.y / 1.88f),
                Vector2f(resolution.x / 2.15f, resolution.y / 2.08f),
                Vector2f(resolution.x / 1.94f, resolution.y / 2.22f),
                Vector2f(resolution.x / 2.19f, resolution.y / 1.70f),
                Vector2f(resolution.x / 2.27f, resolution.y / 1.53f),
                Vector2f(resolution.x / 2.13f, resolution.y / 1.52f),
                Vector2f(resolution.x / 2.10f, resolution.y / 1.63f),
                Vector2f(resolution.x / 1.97f, resolution.y / 1.67f),
                Vector2f(resolution.x / 1.89f, resolution.y / 1.49f),
                Vector2f(resolution.x / 1.85f, resolution.y / 1.34f),
                Vector2f(resolution.x / 1.77f, resolution.y / 1.28f),
                Vector2f(resolution.x / 2.11f, resolution.y / 1.29f),
                Vector2f(resolution.x / 2.03f, resolution.y / 1.37f),
                Vector2f(resolution.x / 1.76f, resolution.y / 1.49f),
                Vector2f(resolution.x / 1.73f, resolution.y / 1.66f),
                Vector2f(resolution.x / 1.77f, resolution.y / 1.95f) },
                6.0f * (resolution.x / 1920.0f), Color::Red, Color::Red,
                3.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2));

            operationSceneChanged = true;
        }
        break;

    default:
        break;
    }
}

void GameScene::UpdateDay5OperationScene(float deltaTime)
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
                if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                    Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
                {
                    if (operationScene.dotCircleShape[i].getFillColor() != Color::Green)
                        operationScene.dotCircleShape[i].setFillColor(Color::Green);

                    mouseClicked = true;
                }

                else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                    && mouseClicked)
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
            operationScene.dotCircleShape[7].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[8].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[9].getFillColor() == Color::Green)
        {
            successfulOperationTime += deltaTime;
            if (successfulOperations != 1) successfulOperations = 1;

            if (isInputEnabled != false) isInputEnabled = false;

            if (operationSceneChanged != false) operationSceneChanged = false;

            float successfulCharacterSize = 30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

            operationScene.InitializeSuccessPanel(Vector2(resolution.x / 1.95f, resolution.y / 2.375f),
                Vector2f(420.0f * (resolution.x / 1920.0f), 50.0f * (resolution.y / 1080.0f)), Color::Black, true);

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
                if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                    Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
                {
                    if (operationScene.dotCircleShape[i].getFillColor() != Color::Green)
                        operationScene.dotCircleShape[i].setFillColor(Color::Green);

                    mouseClicked = true;
                }

                else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                    && mouseClicked)
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
            operationScene.dotCircleShape[7].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[8].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[9].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[10].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[11].getFillColor() == Color::Green)
        {
            successfulOperationTime += deltaTime;
            if (successfulOperations != 2) successfulOperations = 2;

            if (operationSceneChanged != false) operationSceneChanged = false;
            if (isInputEnabled != false) isInputEnabled = false;

            float successfulCharacterSize = 30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

            operationScene.InitializeSuccessPanel(Vector2(resolution.x / 1.95f, resolution.y / 2.375f),
                Vector2f(420.0f * (resolution.x / 1920.0f), 50.0f * (resolution.y / 1080.0f)), Color::Black, true);

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

    case 2: // 3rd patient
        for (int i = 0; i < operationScene.maxDots; i++)
        {
            if (operationScene.dotCircleShape[i].getGlobalBounds().contains(mousePos) && isInputEnabled)
            {
                // Set up the operation scene after clicking the left mouse button
                if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                    Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
                {
                    if (operationScene.dotCircleShape[i].getFillColor() != Color::Green)
                        operationScene.dotCircleShape[i].setFillColor(Color::Green);

                    mouseClicked = true;
                }

                else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                    && mouseClicked)
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
            operationScene.dotCircleShape[7].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[8].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[9].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[10].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[11].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[12].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[13].getFillColor() == Color::Green)
        {
            successfulOperationTime += deltaTime;
            if (successfulOperations != 3) successfulOperations = 3;

            if (operationSceneChanged != false) operationSceneChanged = false;
            if (isInputEnabled != false) isInputEnabled = false;

            float successfulCharacterSize = 30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

            operationScene.InitializeSuccessPanel(Vector2(resolution.x / 1.95f, resolution.y / 2.375f),
                Vector2f(420.0f * (resolution.x / 1920.0f), 50.0f * (resolution.y / 1080.0f)), Color::Black, true);

            successfulText.InitializeText("Fonts/Roboto-Regular.ttf", std::to_string(successfulOperations) + "/" +
                std::to_string(maxPatients) + " operations successful!",
                successfulCharacterSize, true, false,
                Color::Green, Vector2(resolution.x / 1.95f, resolution.y / 2.4f));

            if (successfulOperationTime >= 1.0f)
            {
                isInputEnabled = true;

                successfulOperationTime = 0.0f;
                currentPatientIndex = 3;
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            }
        }
        break;

    case 3: // 4th patient
        for (int i = 0; i < operationScene.maxDots; i++)
        {
            if (operationScene.dotCircleShape[i].getGlobalBounds().contains(mousePos) && isInputEnabled)
            {
                // Set up the operation scene after clicking the left mouse button
                if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                    Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
                {
                    if (operationScene.dotCircleShape[i].getFillColor() != Color::Green)
                        operationScene.dotCircleShape[i].setFillColor(Color::Green);

                    mouseClicked = true;
                }

                else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                    && mouseClicked)
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
            operationScene.dotCircleShape[7].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[8].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[9].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[10].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[11].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[12].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[13].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[14].getFillColor() == Color::Green)
        {
            successfulOperationTime += deltaTime;
            if (successfulOperations != 4) successfulOperations = 4;

            if (operationSceneChanged != false) operationSceneChanged = false;
            if (isInputEnabled != false) isInputEnabled = false;

            float successfulCharacterSize = 30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

            operationScene.InitializeSuccessPanel(Vector2(resolution.x / 1.95f, resolution.y / 2.375f),
                Vector2f(420.0f * (resolution.x / 1920.0f), 50.0f * (resolution.y / 1080.0f)), Color::Black, true);

            successfulText.InitializeText("Fonts/Roboto-Regular.ttf", std::to_string(successfulOperations) + "/" +
                std::to_string(maxPatients) + " operations successful!",
                successfulCharacterSize, true, false,
                Color::Green, Vector2(resolution.x / 1.95f, resolution.y / 2.4f));

            if (successfulOperationTime >= 1.0f)
            {
                isInputEnabled = true;

                successfulOperationTime = 0.0f;
                currentPatientIndex = 4;
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            }
        }
        break;

    case 4: // 5th patient
        for (int i = 0; i < operationScene.maxDots; i++)
        {
            if (operationScene.dotCircleShape[i].getGlobalBounds().contains(mousePos) && isInputEnabled)
            {
                // Set up the operation scene after clicking the left mouse button
                if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                    Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
                {
                    if (operationScene.dotCircleShape[i].getFillColor() != Color::Green)
                        operationScene.dotCircleShape[i].setFillColor(Color::Green);

                    mouseClicked = true;
                }

                else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                    && mouseClicked)
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
            operationScene.dotCircleShape[7].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[8].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[9].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[10].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[11].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[12].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[13].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[14].getFillColor() == Color::Green)
        {
            successfulOperationTime += deltaTime;
            if (successfulOperations != 5) successfulOperations = 5;

            if (operationSceneChanged != false) operationSceneChanged = false;
            if (isInputEnabled != false) isInputEnabled = false;

            float successfulCharacterSize = 30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

            operationScene.InitializeSuccessPanel(Vector2(resolution.x / 1.95f, resolution.y / 2.375f),
                Vector2f(420.0f * (resolution.x / 1920.0f), 50.0f * (resolution.y / 1080.0f)), Color::Black, true);

            successfulText.InitializeText("Fonts/Roboto-Regular.ttf", std::to_string(successfulOperations) + "/" +
                std::to_string(maxPatients) + " operations successful!",
                successfulCharacterSize, true, false,
                Color::Green, Vector2(resolution.x / 1.95f, resolution.y / 2.4f));

            if (successfulOperationTime >= 1.0f)
            {
                isInputEnabled = true;

                successfulOperationTime = 0.0f;
                currentPatientIndex = 5;
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            }
        }
        break;

    case 5: // 6th patient
        for (int i = 0; i < operationScene.maxDots; i++)
        {
            if (operationScene.dotCircleShape[i].getGlobalBounds().contains(mousePos) && isInputEnabled)
            {
                // Set up the operation scene after clicking the left mouse button
                if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                    Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
                {
                    if (operationScene.dotCircleShape[i].getFillColor() != Color::Green)
                        operationScene.dotCircleShape[i].setFillColor(Color::Green);

                    mouseClicked = true;
                }

                else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                    && mouseClicked)
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
            operationScene.dotCircleShape[7].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[8].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[9].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[10].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[11].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[12].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[13].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[14].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[15].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[16].getFillColor() == Color::Green)
        {
            successfulOperationTime += deltaTime;
            if (successfulOperations != 6) successfulOperations = 6;

            if (operationSceneChanged != false) operationSceneChanged = false;
            if (isInputEnabled != false) isInputEnabled = false;

            float successfulCharacterSize = 30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

            operationScene.InitializeSuccessPanel(Vector2(resolution.x / 1.95f, resolution.y / 2.375f),
                Vector2f(420.0f * (resolution.x / 1920.0f), 50.0f * (resolution.y / 1080.0f)), Color::Black, true);

            successfulText.InitializeText("Fonts/Roboto-Regular.ttf", std::to_string(successfulOperations) + "/" +
                std::to_string(maxPatients) + " operations successful!",
                successfulCharacterSize, true, false,
                Color::Green, Vector2(resolution.x / 1.95f, resolution.y / 2.4f));

            if (successfulOperationTime >= 1.0f)
            {
                isInputEnabled = true;

                successfulOperationTime = 0.0f;
                currentPatientIndex = 6;
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            }
        }
        break;

    case 6: // 7th patient
        for (int i = 0; i < operationScene.maxDots; i++)
        {
            if (operationScene.dotCircleShape[i].getGlobalBounds().contains(mousePos) && isInputEnabled)
            {
                // Set up the operation scene after clicking the left mouse button
                if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                    Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
                {
                    if (operationScene.dotCircleShape[i].getFillColor() != Color::Green)
                        operationScene.dotCircleShape[i].setFillColor(Color::Green);

                    mouseClicked = true;
                }

                else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                    && mouseClicked)
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
            operationScene.dotCircleShape[7].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[8].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[9].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[10].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[11].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[12].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[13].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[14].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[15].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[16].getFillColor() == Color::Green)
        {
            successfulOperationTime += deltaTime;
            if (successfulOperations != 7) successfulOperations = 7;

            if (operationSceneChanged != false) operationSceneChanged = false;
            if (isInputEnabled != false) isInputEnabled = false;

            float successfulCharacterSize = 30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

            operationScene.InitializeSuccessPanel(Vector2(resolution.x / 1.95f, resolution.y / 2.375f),
                Vector2f(420.0f * (resolution.x / 1920.0f), 50.0f * (resolution.y / 1080.0f)), Color::Black, true);

            successfulText.InitializeText("Fonts/Roboto-Regular.ttf", std::to_string(successfulOperations) + "/" +
                std::to_string(maxPatients) + " operations successful!",
                successfulCharacterSize, true, false,
                Color::Green, Vector2(resolution.x / 1.95f, resolution.y / 2.4f));

            if (successfulOperationTime >= 1.0f)
            {
                isInputEnabled = true;

                successfulOperationTime = 0.0f;
                currentPatientIndex = 7;
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            }
        }
        break;

    case 7: // 8th patient
        for (int i = 0; i < operationScene.maxDots; i++)
        {
            if (operationScene.dotCircleShape[i].getGlobalBounds().contains(mousePos) && isInputEnabled)
            {
                // Set up the operation scene after clicking the left mouse button
                if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                    Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
                {
                    if (operationScene.dotCircleShape[i].getFillColor() != Color::Green)
                        operationScene.dotCircleShape[i].setFillColor(Color::Green);

                    mouseClicked = true;
                }

                else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                    && mouseClicked)
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
            operationScene.dotCircleShape[7].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[8].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[9].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[10].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[11].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[12].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[13].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[14].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[15].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[16].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[17].getFillColor() == Color::Green)
        {
            successfulOperationTime += deltaTime;
            if (successfulOperations != 8) successfulOperations = 8;

            if (operationSceneChanged != false) operationSceneChanged = false;
            if (isInputEnabled != false) isInputEnabled = false;

            float successfulCharacterSize = 30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

            operationScene.InitializeSuccessPanel(Vector2(resolution.x / 1.95f, resolution.y / 2.375f),
                Vector2f(420.0f * (resolution.x / 1920.0f), 50.0f * (resolution.y / 1080.0f)), Color::Black, true);

            successfulText.InitializeText("Fonts/Roboto-Regular.ttf", std::to_string(successfulOperations) + "/" +
                std::to_string(maxPatients) + " operations successful!",
                successfulCharacterSize, true, false,
                Color::Green, Vector2(resolution.x / 1.95f, resolution.y / 2.4f));

            if (successfulOperationTime >= 1.0f)
            {
                isInputEnabled = true;

                successfulOperationTime = 0.0f;
                currentPatientIndex = 8;
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            }
        }
        break;

    case 8: // 9th patient
        for (int i = 0; i < operationScene.maxDots; i++)
        {
            if (operationScene.dotCircleShape[i].getGlobalBounds().contains(mousePos) && isInputEnabled)
            {
                // Set up the operation scene after clicking the left mouse button
                if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                    Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
                {
                    if (operationScene.dotCircleShape[i].getFillColor() != Color::Green)
                        operationScene.dotCircleShape[i].setFillColor(Color::Green);

                    mouseClicked = true;
                }

                else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                    && mouseClicked)
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
            operationScene.dotCircleShape[7].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[8].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[9].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[10].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[11].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[12].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[13].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[14].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[15].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[16].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[17].getFillColor() == Color::Green)
        {
            successfulOperationTime += deltaTime;
            if (successfulOperations != 9) successfulOperations = 9;

            if (operationSceneChanged != false) operationSceneChanged = false;
            if (isInputEnabled != false) isInputEnabled = false;

            float successfulCharacterSize = 30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

            operationScene.InitializeSuccessPanel(Vector2(resolution.x / 1.95f, resolution.y / 2.375f),
                Vector2f(420.0f * (resolution.x / 1920.0f), 50.0f * (resolution.y / 1080.0f)), Color::Black, true);

            successfulText.InitializeText("Fonts/Roboto-Regular.ttf", std::to_string(successfulOperations) + "/" +
                std::to_string(maxPatients) + " operations successful!",
                successfulCharacterSize, true, false,
                Color::Green, Vector2(resolution.x / 1.95f, resolution.y / 2.4f));

            if (successfulOperationTime >= 1.0f)
            {
                isInputEnabled = true;

                successfulOperationTime = 0.0f;
                currentPatientIndex = 9;
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            }
        }
        break;

    case 9: // 10th patient
        for (int i = 0; i < operationScene.maxDots; i++)
        {
            if (operationScene.dotCircleShape[i].getGlobalBounds().contains(mousePos) && isInputEnabled)
            {
                // Set up the operation scene after clicking the left mouse button
                if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                    Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
                {
                    if (operationScene.dotCircleShape[i].getFillColor() != Color::Green)
                        operationScene.dotCircleShape[i].setFillColor(Color::Green);

                    mouseClicked = true;
                }

                else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                    && mouseClicked)
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
            operationScene.dotCircleShape[7].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[8].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[9].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[10].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[11].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[12].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[13].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[14].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[15].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[16].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[17].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[18].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[19].getFillColor() == Color::Green)
        {
            successfulOperationTime += deltaTime;
            if (successfulOperations != 10) successfulOperations = 10;

            if (operationSceneChanged != false) operationSceneChanged = false;
            if (isInputEnabled != false) isInputEnabled = false;

            float successfulCharacterSize = 30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

            operationScene.InitializeSuccessPanel(Vector2(resolution.x / 1.95f, resolution.y / 2.375f),
                Vector2f(430.0f * (resolution.x / 1920.0f), 50.0f * (resolution.y / 1080.0f)), Color::Black, true);

            successfulText.InitializeText("Fonts/Roboto-Regular.ttf", std::to_string(successfulOperations) + "/" +
                std::to_string(maxPatients) + " operations successful!",
                successfulCharacterSize, true, false,
                Color::Green, Vector2(resolution.x / 1.95f, resolution.y / 2.4f));

            if (successfulOperationTime >= 1.0f)
            {
                isInputEnabled = true;

                successfulOperationTime = 0.0f;
                currentPatientIndex = 10;
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            }
        }
        break;

    case 10: // 11th patient
        for (int i = 0; i < operationScene.maxDots; i++)
        {
            if (operationScene.dotCircleShape[i].getGlobalBounds().contains(mousePos) && isInputEnabled)
            {
                // Set up the operation scene after clicking the left mouse button
                if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                    Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
                {
                    if (operationScene.dotCircleShape[i].getFillColor() != Color::Green)
                        operationScene.dotCircleShape[i].setFillColor(Color::Green);

                    mouseClicked = true;
                }

                else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                    && mouseClicked)
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
            operationScene.dotCircleShape[7].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[8].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[9].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[10].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[11].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[12].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[13].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[14].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[15].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[16].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[17].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[18].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[19].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[20].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[21].getFillColor() == Color::Green)
        {
            successfulOperationTime += deltaTime;
            if (successfulOperations != 11) successfulOperations = 11;

            if (operationSceneChanged != false) operationSceneChanged = false;
            if (isInputEnabled != false) isInputEnabled = false;

            float successfulCharacterSize = 30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

            operationScene.InitializeSuccessPanel(Vector2(resolution.x / 1.95f, resolution.y / 2.375f),
                Vector2f(430.0f * (resolution.x / 1920.0f), 50.0f * (resolution.y / 1080.0f)), Color::Black, true);

            successfulText.InitializeText("Fonts/Roboto-Regular.ttf", std::to_string(successfulOperations) + "/" +
                std::to_string(maxPatients) + " operations successful!",
                successfulCharacterSize, true, false,
                Color::Green, Vector2(resolution.x / 1.95f, resolution.y / 2.4f));

            if (successfulOperationTime >= 1.0f)
            {
                isInputEnabled = true;

                successfulOperationTime = 0.0f;
                currentPatientIndex = 11;
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            }
        }
        break;

    case 11: // 12th patient
        for (int i = 0; i < operationScene.maxDots; i++)
        {
            if (operationScene.dotCircleShape[i].getGlobalBounds().contains(mousePos) && isInputEnabled)
            {
                // Set up the operation scene after clicking the left mouse button
                if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                    Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
                {
                    if (operationScene.dotCircleShape[i].getFillColor() != Color::Green)
                        operationScene.dotCircleShape[i].setFillColor(Color::Green);

                    mouseClicked = true;
                }

                else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                    && mouseClicked)
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
            operationScene.dotCircleShape[7].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[8].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[9].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[10].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[11].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[12].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[13].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[14].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[15].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[16].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[17].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[18].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[19].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[20].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[21].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[22].getFillColor() == Color::Green)
        {
            successfulOperationTime += deltaTime;
            if (successfulOperations != 12) successfulOperations = 12;

            if (operationSceneChanged != false) operationSceneChanged = false;
            if (isInputEnabled != false) isInputEnabled = false;

            float successfulCharacterSize = 30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

            operationScene.InitializeSuccessPanel(Vector2(resolution.x / 1.95f, resolution.y / 2.375f),
                Vector2f(430.0f * (resolution.x / 1920.0f), 50.0f * (resolution.y / 1080.0f)), Color::Black, true);

            successfulText.InitializeText("Fonts/Roboto-Regular.ttf", std::to_string(successfulOperations) + "/" +
                std::to_string(maxPatients) + " operations successful!",
                successfulCharacterSize, true, false,
                Color::Green, Vector2(resolution.x / 1.95f, resolution.y / 2.4f));

            if (successfulOperationTime >= 1.0f)
            {
                isInputEnabled = true;

                successfulOperationTime = 0.0f;
                currentPatientIndex = 12;
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            }
        }
        break;

    case 12: // 13th patient
        for (int i = 0; i < operationScene.maxDots; i++)
        {
            if (operationScene.dotCircleShape[i].getGlobalBounds().contains(mousePos) && isInputEnabled)
            {
                // Set up the operation scene after clicking the left mouse button
                if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                    Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
                {
                    if (operationScene.dotCircleShape[i].getFillColor() != Color::Green)
                        operationScene.dotCircleShape[i].setFillColor(Color::Green);

                    mouseClicked = true;
                }

                else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                    && mouseClicked)
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
            operationScene.dotCircleShape[7].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[8].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[9].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[10].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[11].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[12].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[13].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[14].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[15].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[16].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[17].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[18].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[19].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[20].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[21].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[22].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[23].getFillColor() == Color::Green)
        {
            successfulOperationTime += deltaTime;
            if (successfulOperations != 13) successfulOperations = 13;

            if (operationSceneChanged != false) operationSceneChanged = false;
            if (isInputEnabled != false) isInputEnabled = false;

            float successfulCharacterSize = 30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

            operationScene.InitializeSuccessPanel(Vector2(resolution.x / 1.95f, resolution.y / 2.375f),
                Vector2f(430.0f * (resolution.x / 1920.0f), 50.0f * (resolution.y / 1080.0f)), Color::Black, true);

            successfulText.InitializeText("Fonts/Roboto-Regular.ttf", std::to_string(successfulOperations) + "/" +
                std::to_string(maxPatients) + " operations successful!",
                successfulCharacterSize, true, false,
                Color::Green, Vector2(resolution.x / 1.95f, resolution.y / 2.4f));

            if (successfulOperationTime >= 1.0f)
            {
                isInputEnabled = true;

                successfulOperationTime = 0.0f;
                currentPatientIndex = 13;
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            }
        }
        break;

    case 13: // 14th patient
        for (int i = 0; i < operationScene.maxDots; i++)
        {
            if (operationScene.dotCircleShape[i].getGlobalBounds().contains(mousePos) && isInputEnabled)
            {
                // Set up the operation scene after clicking the left mouse button
                if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                    Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
                {
                    if (operationScene.dotCircleShape[i].getFillColor() != Color::Green)
                        operationScene.dotCircleShape[i].setFillColor(Color::Green);

                    mouseClicked = true;
                }

                else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                    && mouseClicked)
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
            operationScene.dotCircleShape[7].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[8].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[9].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[10].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[11].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[12].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[13].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[14].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[15].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[16].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[17].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[18].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[19].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[20].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[21].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[22].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[23].getFillColor() == Color::Green)
        {
            successfulOperationTime += deltaTime;
            if (successfulOperations != 14) successfulOperations = 14;

            if (operationSceneChanged != false) operationSceneChanged = false;
            if (isInputEnabled != false) isInputEnabled = false;

            float successfulCharacterSize = 30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

            operationScene.InitializeSuccessPanel(Vector2(resolution.x / 1.95f, resolution.y / 2.375f),
                Vector2f(430.0f * (resolution.x / 1920.0f), 50.0f * (resolution.y / 1080.0f)), Color::Black, true);

            successfulText.InitializeText("Fonts/Roboto-Regular.ttf", std::to_string(successfulOperations) + "/" +
                std::to_string(maxPatients) + " operations successful!",
                successfulCharacterSize, true, false,
                Color::Green, Vector2(resolution.x / 1.95f, resolution.y / 2.4f));

            if (successfulOperationTime >= 1.0f)
            {
                isInputEnabled = true;

                successfulOperationTime = 0.0f;
                currentPatientIndex = 14;
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            }
        }
        break;

    case 14: // 15th patient
        for (int i = 0; i < operationScene.maxDots; i++)
        {
            if (operationScene.dotCircleShape[i].getGlobalBounds().contains(mousePos) && isInputEnabled)
            {
                // Set up the operation scene after clicking the left mouse button
                if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                    Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
                {
                    if (operationScene.dotCircleShape[i].getFillColor() != Color::Green)
                        operationScene.dotCircleShape[i].setFillColor(Color::Green);

                    mouseClicked = true;
                }

                else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                    && mouseClicked)
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
            operationScene.dotCircleShape[7].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[8].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[9].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[10].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[11].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[12].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[13].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[14].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[15].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[16].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[17].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[18].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[19].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[20].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[21].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[22].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[23].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[24].getFillColor() == Color::Green)
        {
            successfulOperationTime += deltaTime;
            if (successfulOperations != 15) successfulOperations = 15;

            if (operationSceneChanged != false) operationSceneChanged = false;
            if (isInputEnabled != false) isInputEnabled = false;

            float successfulCharacterSize = 30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

            operationScene.InitializeSuccessPanel(Vector2(resolution.x / 1.95f, resolution.y / 2.375f),
                Vector2f(430.0f * (resolution.x / 1920.0f), 50.0f * (resolution.y / 1080.0f)), Color::Black, true);

            successfulText.InitializeText("Fonts/Roboto-Regular.ttf", std::to_string(successfulOperations) + "/" +
                std::to_string(maxPatients) + " operations successful!",
                successfulCharacterSize, true, false,
                Color::Green, Vector2(resolution.x / 1.95f, resolution.y / 2.4f));

            if (successfulOperationTime >= 1.0f)
            {
                isInputEnabled = true;

                successfulOperationTime = 0.0f;
                currentPatientIndex = 15;
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            }
        }
        break;

    case 15: // 16th patient
        for (int i = 0; i < operationScene.maxDots; i++)
        {
            if (operationScene.dotCircleShape[i].getGlobalBounds().contains(mousePos) && isInputEnabled)
            {
                // Set up the operation scene after clicking the left mouse button
                if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                    Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
                {
                    if (operationScene.dotCircleShape[i].getFillColor() != Color::Green)
                        operationScene.dotCircleShape[i].setFillColor(Color::Green);

                    mouseClicked = true;
                }

                else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                    && mouseClicked)
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
            operationScene.dotCircleShape[7].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[8].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[9].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[10].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[11].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[12].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[13].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[14].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[15].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[16].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[17].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[18].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[19].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[20].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[21].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[22].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[23].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[24].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[25].getFillColor() == Color::Green)
        {
            successfulOperationTime += deltaTime;
            if (successfulOperations != 16) successfulOperations = 16;

            if (operationSceneChanged != false) operationSceneChanged = false;
            if (isInputEnabled != false) isInputEnabled = false;

            float successfulCharacterSize = 30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

            operationScene.InitializeSuccessPanel(Vector2(resolution.x / 1.95f, resolution.y / 2.375f),
                Vector2f(430.0f * (resolution.x / 1920.0f), 50.0f * (resolution.y / 1080.0f)), Color::Black, true);

            successfulText.InitializeText("Fonts/Roboto-Regular.ttf", std::to_string(successfulOperations) + "/" +
                std::to_string(maxPatients) + " operations successful!",
                successfulCharacterSize, true, false,
                Color::Green, Vector2(resolution.x / 1.95f, resolution.y / 2.4f));

            if (successfulOperationTime >= 1.0f)
            {
                isInputEnabled = true;

                successfulOperationTime = 0.0f;
                currentPatientIndex = 16;
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            }
        }
        break;

    case 16: // 17th patient
        for (int i = 0; i < operationScene.maxDots; i++)
        {
            if (operationScene.dotCircleShape[i].getGlobalBounds().contains(mousePos) && isInputEnabled)
            {
                // Set up the operation scene after clicking the left mouse button
                if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                    Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
                {
                    if (operationScene.dotCircleShape[i].getFillColor() != Color::Green)
                        operationScene.dotCircleShape[i].setFillColor(Color::Green);

                    mouseClicked = true;
                }

                else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                    && mouseClicked)
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
            operationScene.dotCircleShape[7].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[8].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[9].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[10].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[11].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[12].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[13].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[14].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[15].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[16].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[17].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[18].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[19].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[20].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[21].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[22].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[23].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[24].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[25].getFillColor() == Color::Green)
        {
            successfulOperationTime += deltaTime;
            if (successfulOperations != 17) successfulOperations = 17;

            if (operationSceneChanged != false) operationSceneChanged = false;
            if (isInputEnabled != false) isInputEnabled = false;

            float successfulCharacterSize = 30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

            operationScene.InitializeSuccessPanel(Vector2(resolution.x / 1.95f, resolution.y / 2.375f),
                Vector2f(430.0f * (resolution.x / 1920.0f), 50.0f * (resolution.y / 1080.0f)), Color::Black, true);

            successfulText.InitializeText("Fonts/Roboto-Regular.ttf", std::to_string(successfulOperations) + "/" +
                std::to_string(maxPatients) + " operations successful!",
                successfulCharacterSize, true, false,
                Color::Green, Vector2(resolution.x / 1.95f, resolution.y / 2.4f));

            if (successfulOperationTime >= 1.0f)
            {
                isInputEnabled = true;

                successfulOperationTime = 0.0f;
                currentPatientIndex = 17;
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            }
        }
        break;

    case 17: // 18th patient
        for (int i = 0; i < operationScene.maxDots; i++)
        {
            if (operationScene.dotCircleShape[i].getGlobalBounds().contains(mousePos) && isInputEnabled)
            {
                // Set up the operation scene after clicking the left mouse button
                if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                    Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
                {
                    if (operationScene.dotCircleShape[i].getFillColor() != Color::Green)
                        operationScene.dotCircleShape[i].setFillColor(Color::Green);

                    mouseClicked = true;
                }

                else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                    && mouseClicked)
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
            operationScene.dotCircleShape[7].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[8].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[9].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[10].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[11].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[12].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[13].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[14].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[15].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[16].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[17].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[18].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[19].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[20].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[21].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[22].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[23].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[24].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[25].getFillColor() == Color::Green && 
            operationScene.dotCircleShape[26].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[27].getFillColor() == Color::Green)
        {
            successfulOperationTime += deltaTime;
            if (successfulOperations != 18) successfulOperations = 18;

            if (operationSceneChanged != false) operationSceneChanged = false;
            if (isInputEnabled != false) isInputEnabled = false;

            float successfulCharacterSize = 30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

            operationScene.InitializeSuccessPanel(Vector2(resolution.x / 1.95f, resolution.y / 2.375f),
                Vector2f(430.0f * (resolution.x / 1920.0f), 50.0f * (resolution.y / 1080.0f)), Color::Black, true);

            successfulText.InitializeText("Fonts/Roboto-Regular.ttf", std::to_string(successfulOperations) + "/" +
                std::to_string(maxPatients) + " operations successful!",
                successfulCharacterSize, true, false,
                Color::Green, Vector2(resolution.x / 1.95f, resolution.y / 2.4f));

            if (successfulOperationTime >= 1.0f)
            {
                isInputEnabled = true;

                successfulOperationTime = 0.0f;
                currentPatientIndex = 18;
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            }
        }
        break;

    case 18: // 19th patient
        for (int i = 0; i < operationScene.maxDots; i++)
        {
            if (operationScene.dotCircleShape[i].getGlobalBounds().contains(mousePos) && isInputEnabled)
            {
                // Set up the operation scene after clicking the left mouse button
                if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                    Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
                {
                    if (operationScene.dotCircleShape[i].getFillColor() != Color::Green)
                        operationScene.dotCircleShape[i].setFillColor(Color::Green);

                    mouseClicked = true;
                }

                else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                    && mouseClicked)
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
            operationScene.dotCircleShape[7].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[8].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[9].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[10].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[11].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[12].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[13].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[14].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[15].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[16].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[17].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[18].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[19].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[20].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[21].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[22].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[23].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[24].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[25].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[26].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[27].getFillColor() == Color::Green)
        {
            successfulOperationTime += deltaTime;
            if (successfulOperations != 19) successfulOperations = 19;

            if (operationSceneChanged != false) operationSceneChanged = false;
            if (isInputEnabled != false) isInputEnabled = false;

            float successfulCharacterSize = 30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

            operationScene.InitializeSuccessPanel(Vector2(resolution.x / 1.95f, resolution.y / 2.375f),
                Vector2f(430.0f * (resolution.x / 1920.0f), 50.0f * (resolution.y / 1080.0f)), Color::Black, true);

            successfulText.InitializeText("Fonts/Roboto-Regular.ttf", std::to_string(successfulOperations) + "/" +
                std::to_string(maxPatients) + " operations successful!",
                successfulCharacterSize, true, false,
                Color::Green, Vector2(resolution.x / 1.95f, resolution.y / 2.4f));

            if (successfulOperationTime >= 1.0f)
            {
                isInputEnabled = true;

                successfulOperationTime = 0.0f;
                currentPatientIndex = 19;
                currentGameState = GameState::SURGERY_ROOM_ACTIVE;
            }
        }
        break;

    case 19: // 20th patient
        for (int i = 0; i < operationScene.maxDots; i++)
        {
            if (operationScene.dotCircleShape[i].getGlobalBounds().contains(mousePos) && isInputEnabled)
            {
                // Set up the operation scene after clicking the left mouse button
                if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked ||
                    Mouse::isButtonPressed(Mouse::Button::Right) && !mouseClicked)
                {
                    if (operationScene.dotCircleShape[i].getFillColor() != Color::Green)
                        operationScene.dotCircleShape[i].setFillColor(Color::Green);

                    mouseClicked = true;
                }

                else if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)
                    && mouseClicked)
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
            operationScene.dotCircleShape[7].getFillColor() == Color::Green && 
            operationScene.dotCircleShape[8].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[9].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[10].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[11].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[12].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[13].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[14].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[15].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[16].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[17].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[18].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[19].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[20].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[21].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[22].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[23].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[24].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[25].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[26].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[27].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[28].getFillColor() == Color::Green &&
            operationScene.dotCircleShape[29].getFillColor() == Color::Green)
        {
            successfulOperationTime += deltaTime;
            if (successfulOperations != 20) successfulOperations = 20;

            if (operationSceneChanged != false) operationSceneChanged = false;
            if (isInputEnabled != false) isInputEnabled = false;

            float successfulCharacterSize = 30.0f * (((resolution.x / 1920.0f) + (resolution.y / 1080.0f)) / 2);

            operationScene.InitializeSuccessPanel(Vector2(resolution.x / 1.95f, resolution.y / 2.375f),
                Vector2f(430.0f * (resolution.x / 1920.0f), 50.0f * (resolution.y / 1080.0f)), Color::Black, true);

            successfulText.InitializeText("Fonts/Roboto-Regular.ttf", std::to_string(successfulOperations) + "/" +
                std::to_string(maxPatients) + " operations successful!",
                successfulCharacterSize, true, false,
                Color::Green, Vector2(resolution.x / 1.95f, resolution.y / 2.4f));

            if (successfulOperationTime >= 1.0f)
            {
                isInputEnabled = true;
                successfulOperationTime = 0.0f;
                successfulDaySoundIndex = rand() % successfulDaySound.size();
                currentGameState = GameState::SUCCESSFUL_DAY_ACTIVE;
            }
        }
        break;

    default:
        break;
    }
}