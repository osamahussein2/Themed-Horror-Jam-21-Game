// TypewriterEffect.h
#ifndef TYPEWRITEREFFECT_H
#define TYPEWRITEREFFECT_H

#include <string>
#include <vector>

class TypewriterEffect
{
public:
    struct DialogueData
    {
        std::string fullText;
        float charactersPerSecond;

        DialogueData(const std::string& text, float speed = 5.0f)
            : fullText(text), charactersPerSecond(speed) {
        }
    };

private:
    std::vector<DialogueData> dialogues;
    int currentDialogueIndex;
    float currentCharacterTime;
    int currentCharacterIndex;
    bool isComplete;
    bool isSkipped;

public:
    TypewriterEffect();

    // Initialize with dialogue data
    void Initialize(const std::vector<DialogueData>& dialogueList);

    // Update the typewriter effect
    void Update(float deltaTime);

    // Get the current visible text
    std::string GetCurrentText() const;

    // Navigation
    void NextDialogue();
    void PreviousDialogue();
    void SetDialogue(int index);

    // Control
    void Skip();
    void Reset();
    void ResetCurrentDialogue();

    // State queries
    bool IsCurrentDialogueComplete() const { return isComplete; }
    bool IsSkipped() const { return isSkipped; }
    bool HasNextDialogue() const { return currentDialogueIndex < static_cast<int>(dialogues.size()) - 1; }
    bool HasPreviousDialogue() const { return currentDialogueIndex > 0; }
    int GetCurrentDialogueIndex() const { return currentDialogueIndex; }
    int GetDialogueCount() const { return static_cast<int>(dialogues.size()); }

    // Utility
    void SetTypingSpeed(float charactersPerSecond);
    void SetCurrentDialogueSpeed(float charactersPerSecond);
};

#endif
