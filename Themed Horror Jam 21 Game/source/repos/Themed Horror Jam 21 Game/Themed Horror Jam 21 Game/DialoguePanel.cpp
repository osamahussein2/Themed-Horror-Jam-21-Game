#include "DialoguePanel.h"

DialoguePanel::DialoguePanel()
{
}

DialoguePanel::~DialoguePanel()
{
}

void DialoguePanel::InitializeDialoguePanel(const char* filePath_, Vector2f position, Vector2f size)
{
	filePath = filePath_;

	texture.loadFromFile(filePath_);

	dialoguePanel.setTexture(&texture);
	dialoguePanel.setSize(size);
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

Vector2f DialoguePanel::GetSize() const
{
	return dialoguePanel.getSize();
}

void DialoguePanel::SetSize(Vector2f size)
{
	return dialoguePanel.setSize(size);
}