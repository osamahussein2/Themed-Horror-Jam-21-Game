#ifndef DIALOGUEPANEL_H
#define DIALOGUEPANEL_H

#include <SFML/Graphics.hpp>

using namespace sf;

class DialoguePanel
{
private:
    RectangleShape dialoguePanel;

public:
    DialoguePanel();
    ~DialoguePanel();

    void InitializeDialoguePanel(Vector2f position, Vector2f scale, Color outlineColor_, Color fillColor_, 
        float outlineThickness_);

    void DrawDialoguePanel(RenderWindow& window);

    Vector2f GetPosition() const;
    void SetPosition(Vector2f position);
};

#endif