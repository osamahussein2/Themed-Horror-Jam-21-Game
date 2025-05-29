// TypewriterEffect.cpp
#include "TypewriterEffect.h"
#include <algorithm>

TypewriterEffect::TypewriterEffect()
    : currentDialogueIndex(0)
    , currentCharacterTime(0.0f)
    , currentCharacterIndex(0)
    , isComplete(false)
    , isSkipped(false)
{
}

void TypewriterEffect::Initialize(const std::vector<DialogueData>& dialogueList)
{
    dialogues = dialogueList;
    Reset();
}

void TypewriterEffect::Update(float deltaTime)
{
    if (currentDialogueIndex >= static_cast<int>(dialogues.size()) || isComplete)
        return;

    const DialogueData& currentDialogue = dialogues[currentDialogueIndex];

    if (isSkipped)
    {
        currentCharacterIndex = static_cast<int>(currentDialogue.fullText.length());
        isComplete = true;
        return;
    }

    currentCharacterTime += deltaTime;

    float timePerCharacter = 1.0f / currentDialogue.charactersPerSecond;

    while (currentCharacterTime >= timePerCharacter && currentCharacterIndex < static_cast<int>(currentDialogue.fullText.length()))
    {
        currentCharacterTime -= timePerCharacter;
        currentCharacterIndex++;
    }

    if (currentCharacterIndex >= static_cast<int>(currentDialogue.fullText.length()))
    {
        isComplete = true;
    }
}

std::string TypewriterEffect::GetCurrentText() const
{
    if (currentDialogueIndex >= static_cast<int>(dialogues.size()))
        return "";

    const DialogueData& currentDialogue = dialogues[currentDialogueIndex];

    if (isSkipped || isComplete)
        return currentDialogue.fullText;

    return currentDialogue.fullText.substr(0, currentCharacterIndex);
}

void TypewriterEffect::NextDialogue()
{
    if (HasNextDialogue())
    {
        currentDialogueIndex++;
        ResetCurrentDialogue();
    }
}

void TypewriterEffect::PreviousDialogue()
{
    if (HasPreviousDialogue())
    {
        currentDialogueIndex--;
        ResetCurrentDialogue();
    }
}

void TypewriterEffect::SetDialogue(int index)
{
    if (index >= 0 && index < static_cast<int>(dialogues.size()))
    {
        currentDialogueIndex = index;
        ResetCurrentDialogue();
    }
}

void TypewriterEffect::Skip()
{
    isSkipped = true;
}

void TypewriterEffect::Reset()
{
    currentDialogueIndex = 0;
    ResetCurrentDialogue();
}

void TypewriterEffect::ResetCurrentDialogue()
{
    currentCharacterTime = 0.0f;
    currentCharacterIndex = 0;
    isComplete = false;
    isSkipped = false;
}

void TypewriterEffect::SetTypingSpeed(float charactersPerSecond)
{
    for (auto& dialogue : dialogues)
    {
        dialogue.charactersPerSecond = charactersPerSecond;
    }
}

void TypewriterEffect::SetCurrentDialogueSpeed(float charactersPerSecond)
{
    if (currentDialogueIndex >= 0 && currentDialogueIndex < static_cast<int>(dialogues.size()))
    {
        dialogues[currentDialogueIndex].charactersPerSecond = charactersPerSecond;
    }
}
