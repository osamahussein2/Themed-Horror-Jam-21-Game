#ifndef DIALOGUEPANEL_H
#define DIALOGUEPANEL_H

#include <SFML/Graphics.hpp>

using namespace sf;

class DialoguePanel
{
private:
    RectangleShape dialoguePanel;
    Texture texture;

    const char* filePath;

public:
    DialoguePanel();
    ~DialoguePanel();

    void InitializeDialoguePanel(const char* filePath_, Vector2f position, Vector2f size);

    void DrawDialoguePanel(RenderWindow& window);

    Vector2f GetPosition() const;
    void SetPosition(Vector2f position);

    Vector2f GetSize() const;
    void SetSize(Vector2f size);

    const char* GetDialoguePanelTexture();
    void SetDialoguePanelTexture(const char* filePath_);
};

#endif