#include "DialoguePanel.h"

DialoguePanel::DialoguePanel()
{
}

DialoguePanel::~DialoguePanel()
{
}

void DialoguePanel::InitializeDialoguePanel(Vector2f position, Vector2f scale, Color outlineColor_, Color fillColor_,
	float outlineThickness_)
{
	dialoguePanel.setSize(scale);
	dialoguePanel.setFillColor(fillColor_);
	dialoguePanel.setOutlineColor(outlineColor_);
	dialoguePanel.setOutlineThickness(outlineThickness_);
	dialoguePanel.setPosition(position);
}

void DialoguePanel::DrawDialoguePanel(RenderWindow& window)
{
	window.draw(dialoguePanel);
}

Vector2f DialoguePanel::GetPosition() const
{
	return dialoguePanel.getPosition();
}

void DialoguePanel::SetPosition(Vector2f position)
{
	dialoguePanel.setPosition(position);
}
