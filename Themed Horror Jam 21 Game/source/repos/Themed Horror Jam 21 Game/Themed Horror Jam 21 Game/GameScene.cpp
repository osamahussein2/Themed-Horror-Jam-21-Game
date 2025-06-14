#include "GameScene.h"
#include "Engine.h"
#include "SceneManager.h"

// Initialize current day to 1 to prevent errors in-game
int GameScene::currentDay = 1;

GameScene::GameScene()
    : currentDialogueIndex(0)
    , currentGameState()
    , previousGameState(GameState::SURGERY_ROOM_ACTIVE)
    , skippedTypewriting(false)
    , inputCooldown(INPUT_DELAY)
    , dialogueSystemInitialized(false)
    , typeTextTime(0.0f)
{
}

GameScene::~GameScene()
{
    if (!dialogueTexts.empty())
    {
        dialogueTexts.clear();
    }

    if (!dialoguePanelTextures.empty()) dialoguePanelTextures.clear();
}

void GameScene::Initialize()
{
    resolution = Engine::Instance()->GetResolution();
}

void GameScene::Update(float deltaTime)
{
    switch (currentDay)
    {
    case 1:
        UpdateDay1(deltaTime);
        break;

    case 2:
        UpdateDay2(deltaTime);
        break;

    case 3:
        UpdateDay3(deltaTime);
        break;

    case 4:
        UpdateDay4(deltaTime);
        break;

    case 5:
        UpdateDay5(deltaTime);
        break;

    default:
        break;
    }
}

// Update your Render method to include the new case:
void GameScene::Render(RenderWindow& window)
{
    window.clear();

    switch (currentDay)
    {
    case 1:
        RenderDay1(window);
        break;

    case 2:
        RenderDay2(window);
        break;

    case 3:
        RenderDay3(window);
        break;

    case 4:
        RenderDay4(window);
        break;

    case 5:
        RenderDay5(window);
        break;

    default:
        break;
    }
}

void GameScene::Cleanup()
{
}

void GameScene::OnEnter()
{
    InitializeGame();
}

void GameScene::OnExit()
{
}

std::string GameScene::GetSceneName() const
{
    return "Game";
}

void GameScene::InitializeGame()
{
    switch (currentDay)
    {
    case 1:
        InitializeDay1();
        break;

    case 2:
        InitializeDay2();
        break;

    case 3:
        InitializeDay3();
        break;

    case 4:
        InitializeDay4();
        break;

    case 5:
        InitializeDay5();
        break;

    default:
        break;
    }
}

void GameScene::InitializeDialogueSystem()
{
    std::vector<TypewriterEffect::DialogueData> allDialogueData;

    switch (currentDay)
    {
    case 1:
        allDialogueData = {
             TypewriterEffect::DialogueData("Dies Mercurii, May, the year of our Lord 1346", 8.0f),
             TypewriterEffect::DialogueData("At last, a chance to prove my worth! The magistrates of Assisi have called upon my services. A\ndisease spreads through their citizens, and they desire my medical aid. They report the disease has\nbasic symptoms: fevers, headaches, and vomiting. It sounds like a simple imbalance of yellow bile.\nEasy work, with good pay.", 12.0f),
             TypewriterEffect::DialogueData("The magisters have asked me to handle the symptoms of 3 patients today. If I fail, they will send me\nhome without pay. I can use my textbook to diagnose symptoms, and use tools in my doctor’s bag\nto treat them.", 10.0f),
             TypewriterEffect::DialogueData("The official scientific methods provide the best cures, but I can also use traditional folk methods.\nThey aren’t as well-tested, but they take much less time to perform. I doubt the peasants will notice\na difference.", 10.0f),
             TypewriterEffect::DialogueData("Enough writing. I have a duty to perform!", 10.0f)
        };

        typewriterEffect.Initialize(allDialogueData);

        break;

    case 2:
        allDialogueData = {
             TypewriterEffect::DialogueData("Dies Jovis, May, the year of our Lord 1346", 8.0f),
             TypewriterEffect::DialogueData("I’ve never seen anything like it! Boils the size of my hand, bubbling from the skin like liquid in a\ncauldron. When lanced, they burst with warm pus. It seems to help the patient recover, but oh, how\nthey scream!", 12.0f),
             TypewriterEffect::DialogueData("This is a severe imbalance of the humors. I’m certain more symptoms will emerge that I have yet to\nsee. I must consult my textbook on anything strange and new. This is uncharted territory!", 10.0f),
             TypewriterEffect::DialogueData("People continue to grow sick. The magistrates have asked me to handle 5 patients today. I will do\nmy duty.", 10.0f),
             TypewriterEffect::DialogueData("I only hope what I saw yesterday was an isolated incident…", 10.0f)
        };

        typewriterEffect.Initialize(allDialogueData);

        break;

    case 3:
        allDialogueData = {
             TypewriterEffect::DialogueData("Dies Veneris, May, the year of our Lord 1346", 8.0f),
             TypewriterEffect::DialogueData("The symptoms worsen! Boils, inflammation, and more! I try my best to save these poor people, but I\nhave nothing to help with the pain. I should write to my teachers in Florence about this, but their reply\nwould take months! I do not have that time.", 12.0f),
             TypewriterEffect::DialogueData("The magistrates are flooded with worry. They’ve demanded that I handle 10 patients today. I’ll treat\nas many as I can, but time works against me.", 10.0f),
             TypewriterEffect::DialogueData("I may encounter more new symptoms today. I must be ready for them!", 10.0f)
        };

        typewriterEffect.Initialize(allDialogueData);

        break;

    case 4:
        allDialogueData = {
             TypewriterEffect::DialogueData("Dies Saturni, May, the year of our Lord 1346", 8.0f),
             TypewriterEffect::DialogueData("I see carts in the street, stacked with dead bodies. Some of my patients are among them. My\ntreatments are sound, yet still they perish. If all I can do is ease their suffering, then I must continue\nmy work.", 12.0f),
             TypewriterEffect::DialogueData("I’m beginning to doubt this is an issue with humours, but what else could it be?! I must have faith in\nGod and in my treatments. If this is His will, then so be it. I will try my hardest to shepherd these\npoor souls to Him.", 10.0f),
             TypewriterEffect::DialogueData("One of the magistrates took ill this morning. I asked if there was something I could do, but they’ve\nalready taken him to Florence. I wonder if he’ll survive the trip… They asked me to treat 15 patients\ntoday. I must not let them down.", 10.0f),
             TypewriterEffect::DialogueData("I fear the symptoms will increase in intensity today. They may become more difficult to treat…", 10.0f)
        };

        typewriterEffect.Initialize(allDialogueData);

        break;

    case 5:
        allDialogueData = {
             TypewriterEffect::DialogueData("Dies Solis, May, the year of our Lord 1346", 8.0f),
             TypewriterEffect::DialogueData("The streets are deserted. People hide in their homes, or in the churches. I can hear them praying as\nI walk through the town. Some people slam their doors as I approach. They think I am an angel of\ndeath, spreading the plague with each step. I wonder if they are right…", 12.0f),
             TypewriterEffect::DialogueData("The magistrates told me today would be my final day. They’ve called for more doctors from\nFlorence. I begged them to keep me on, but they refused. They said I’ve done more than enough.\nThese poor naive doctors will be walking into Death’s domain! I must help them lest they be eaten\nalive!", 10.0f),
             TypewriterEffect::DialogueData("They demanded that I treat 20 patients today. If I finish, they’ll pay me, but this is beyond money\nnow. I know now what I must do.", 10.0f)
        };

        typewriterEffect.Initialize(allDialogueData);

        break;

    default:
        break;
    }
}

void GameScene::InitializeBag()
{
    // Initialize the bag with your bag sprite
    Vector2f bagPosition(resolution.x / 2.0f, resolution.y / 2.0f); // Position in center of screen
    Vector2f bagScale(1.0f, 1.0f);

    bag.Initialize("Art Assets/SurgeryRoom/bagInvtory.png", bagPosition, bagScale); // Replace with your bag texture path
    bag.Hide(); // Start hidden

    std::cout << "Bag initialized at position: (" << bagPosition.x << ", " << bagPosition.y << ")" << std::endl;
}

void GameScene::HandleItemTableClicks(Vector2f mousePos)
{
    if (bag.IsVisible())
    {
        return; // Don't handle table clicks when bag is visible
    }

    if (Mouse::isButtonPressed(Mouse::Button::Left) && inputCooldown <= 0.0f)
    {
        ItemType clickedItem = itemTable.GetClickedItem(mousePos);

        if (clickedItem != ItemType::NONE)
        {
            // Try to add the item to the bag
            std::string itemName = itemTable.GetItemName(clickedItem);
            std::string itemTexturePath = itemTable.GetItemTexturePath(clickedItem);

            if (bag.AddItem(clickedItem, itemName, itemTexturePath))
            {
                // Successfully added to bag, collect the item from table
                itemTable.CollectItem(clickedItem);
                std::cout << "Added " << itemName << " to bag!" << std::endl;
            }
            else
            {
                std::cout << "Could not add " << itemName << " to bag (already have it or bag is full)" << std::endl;
            }

            inputCooldown = INPUT_DELAY;
        }
    }
}

void GameScene::HandleBagClicks(Vector2f mousePos)
{
    if (Mouse::isButtonPressed(Mouse::Button::Left) && inputCooldown <= 0.0f)
    {
        ItemType clickedBagItem = bag.GetClickedItem(mousePos);

        if (clickedBagItem != ItemType::NONE)
        {
            // Handle using item from bag (in operation scene)
            if (currentGameState == GameState::OPERATION_ACTIVE)
            {
                // Find the item name for display
                std::string itemName = "";
                for (const auto& item : bag.GetInventory())
                {
                    if (item.type == clickedBagItem)
                    {
                        itemName = item.name;
                        break;
                    }
                }

                std::cout << "Using item: " << itemName << " in operation!" << std::endl;
                // Here you can add logic for using the item in the operation
                // For example, remove the item from bag after use:
                // bag.RemoveItem(clickedBagItem);
            }

            inputCooldown = INPUT_DELAY;
        }
    }
}

void GameScene::UpdateDialoguePanelTexture()
{
    switch (currentDay)
    {
    case 1:
        switch (currentDialogueIndex)
        {
        case 0:
            if (dialoguePanel.GetDialoguePanelTexture() != dialoguePanelTextures[3].c_str())
            {
                dialoguePanel.SetDialoguePanelTexture(dialoguePanelTextures[3].c_str());
            }
            break;

        case 1:
            if (dialoguePanel.GetDialoguePanelTexture() != dialoguePanelTextures[2].c_str())
            {
                dialoguePanel.SetDialoguePanelTexture(dialoguePanelTextures[2].c_str());
            }
            break;

        case 2:
            if (dialoguePanel.GetDialoguePanelTexture() != dialoguePanelTextures[1].c_str())
            {
                dialoguePanel.SetDialoguePanelTexture(dialoguePanelTextures[1].c_str());
            }
            break;

        default:
            break;
        }

        break;

    case 2:
        switch (currentDialogueIndex)
        {
        case 0:
            if (dialoguePanel.GetDialoguePanelTexture() != dialoguePanelTextures[3].c_str())
            {
                dialoguePanel.SetDialoguePanelTexture(dialoguePanelTextures[3].c_str());
            }
            break;

        case 1:
            if (dialoguePanel.GetDialoguePanelTexture() != dialoguePanelTextures[2].c_str())
            {
                dialoguePanel.SetDialoguePanelTexture(dialoguePanelTextures[2].c_str());
            }
            break;

        case 2:
            if (dialoguePanel.GetDialoguePanelTexture() != dialoguePanelTextures[1].c_str())
            {
                dialoguePanel.SetDialoguePanelTexture(dialoguePanelTextures[1].c_str());
            }
            break;

        default:
            break;
        }

        break;

    case 3:
        switch (currentDialogueIndex)
        {
        case 0:
            if (dialoguePanel.GetDialoguePanelTexture() != dialoguePanelTextures[3].c_str())
            {
                dialoguePanel.SetDialoguePanelTexture(dialoguePanelTextures[3].c_str());
            }
            break;

        case 1:
            if (dialoguePanel.GetDialoguePanelTexture() != dialoguePanelTextures[2].c_str())
            {
                dialoguePanel.SetDialoguePanelTexture(dialoguePanelTextures[2].c_str());
            }
            break;

        case 2:
            if (dialoguePanel.GetDialoguePanelTexture() != dialoguePanelTextures[1].c_str())
            {
                dialoguePanel.SetDialoguePanelTexture(dialoguePanelTextures[1].c_str());
            }
            break;

        default:
            break;
        }

        break;

    case 4:
        switch (currentDialogueIndex)
        {
        case 0:
            if (dialoguePanel.GetDialoguePanelTexture() != dialoguePanelTextures[3].c_str())
            {
                dialoguePanel.SetDialoguePanelTexture(dialoguePanelTextures[3].c_str());
            }
            break;

        case 1:
            if (dialoguePanel.GetDialoguePanelTexture() != dialoguePanelTextures[2].c_str())
            {
                dialoguePanel.SetDialoguePanelTexture(dialoguePanelTextures[2].c_str());
            }
            break;

        case 2:
            if (dialoguePanel.GetDialoguePanelTexture() != dialoguePanelTextures[1].c_str())
            {
                dialoguePanel.SetDialoguePanelTexture(dialoguePanelTextures[1].c_str());
            }
            break;

        default:
            break;
        }

        break;

    case 5:
        switch (currentDialogueIndex)
        {
        case 0:
            if (dialoguePanel.GetDialoguePanelTexture() != dialoguePanelTextures[3].c_str())
            {
                dialoguePanel.SetDialoguePanelTexture(dialoguePanelTextures[3].c_str());
            }
            break;

        case 1:
            if (dialoguePanel.GetDialoguePanelTexture() != dialoguePanelTextures[2].c_str())
            {
                dialoguePanel.SetDialoguePanelTexture(dialoguePanelTextures[2].c_str());
            }
            break;

        case 2:
            if (dialoguePanel.GetDialoguePanelTexture() != dialoguePanelTextures[1].c_str())
            {
                dialoguePanel.SetDialoguePanelTexture(dialoguePanelTextures[1].c_str());
            }
            break;

        default:
            break;
        }

        break;

    default:
        break;
    }
}