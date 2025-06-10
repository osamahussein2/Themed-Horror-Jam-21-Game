#include "GameScene.h"
#include "Engine.h"

void GameScene::InitializeDay2()
{
    // Reset to initial state
    currentGameState = GameState::DIALOGUE_ACTIVE;

    if (isInputEnabled != true) isInputEnabled = true;
}

void GameScene::UpdateDay2(float deltaTime)
{
    // Update input cooldown
    if (inputCooldown > 0.0f)
    {
        inputCooldown -= deltaTime;
    }

    // Get mouse position for click detection
    Vector2i mousePixelPos = Mouse::getPosition(*Engine::Instance()->GetWindow());
    Vector2f mousePos = Engine::Instance()->GetWindow()->mapPixelToCoords(mousePixelPos);

    // Check for escape key to return to menu
    if (Keyboard::isKeyPressed(Keyboard::Key::Escape) && isInputEnabled)
    {
        //typewriterEffect.Reset();
        //itemTable.ResetCollectedItems();
        //bag.ClearBag();
        sceneManager->ChangeScene("Menu");
        return;
    }
}

void GameScene::RenderDay2(RenderWindow& window)
{

}