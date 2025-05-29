#include "DialoguePanel.h"

DialoguePanel::DialoguePanel()
{
}

DialoguePanel::~DialoguePanel()
{
}

void DialoguePanel::InitializeDialoguePanel(const char* filePath_, Vector2f position, Vector2f scale)
{
	texture.loadFromFile(filePath_);

	dialoguePanel.setTexture(&texture);
	dialoguePanel.setSize(scale);
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
