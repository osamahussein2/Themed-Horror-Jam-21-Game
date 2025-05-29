#include "DialoguePanel.h"

DialoguePanel::DialoguePanel()
{
}

DialoguePanel::~DialoguePanel()
{
}

void DialoguePanel::InitializeDialoguePanel(const char* filePath_, Vector2f position, Vector2f scale)
{
	filePath = filePath_;

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

const char* DialoguePanel::GetDialoguePanelTexture()
{
	return filePath;
}

void DialoguePanel::SetDialoguePanelTexture(const char* filePath_)
{
	texture.loadFromFile(filePath_);

	dialoguePanel.setTexture(&texture);
}
