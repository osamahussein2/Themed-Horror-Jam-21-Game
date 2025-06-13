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
    std::vector<TypewriterEffect::DialogueData> allDialogueData = {
         TypewriterEffect::DialogueData("Welcome to game!", 8.0f),
         TypewriterEffect::DialogueData("Play!", 12.0f),
         TypewriterEffect::DialogueData("Go!", 10.0f)
    };

    typewriterEffect.Initialize(allDialogueData);
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
}