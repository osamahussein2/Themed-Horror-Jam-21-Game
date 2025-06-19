#include "TextbookPages.h"

TextbookPages::TextbookPages() : textbookSprite(textbookTexture), GroundSprite(GroundTexture), mouseClicked(false),
pageNumber(0)
{
}

TextbookPages::~TextbookPages()
{
}

void TextbookPages::Initialize(const char* filePath_, Vector2f position_, Vector2f scale_, bool centerTexture_, 
    const char* fontPath_, Vector2f leftTextPosition_, Vector2f rightTextPosition_, float characterSize_,
    Vector2f buttonSize_, Vector2f leftButtonPosition_, Vector2f rightButtonPosition_)
{
    textbookTexture.loadFromFile(filePath_);

    // Create a sprite from the texture
    textbookSprite = sf::Sprite(textbookTexture);

    if (centerTexture_)
    {
        textbookSprite.setOrigin(Vector2f(textbookSprite.getPosition().x + textbookSprite.getScale().x / 2.0f,
            textbookSprite.getPosition().y + textbookSprite.getScale().y / 2.0f));
    }

    else
    {
        textbookSprite.setOrigin(Vector2f(0.0f, 0.0f));
    }

    textbookSprite.setTexture(textbookTexture);
    textbookSprite.setScale(scale_);
    textbookSprite.setPosition(position_);

    GroundTexture.loadFromFile("Art Assets/SurgeryRoom/ground.png");
    GroundSprite = sf::Sprite(GroundTexture);
    GroundSprite.setTexture(GroundTexture);
    GroundSprite.setScale(scale_);
    float GroundX = 0;
    float GroundY = 0;
    GroundSprite.setPosition({ GroundX,GroundY });

    leftPageTexts = { "Fever\n\nThe best way to cure a fever is to perform bloodletting…\n"
        "…but some folk tales say it can be treated by pouring\nwater on the skin.", 
        "Headache\n\nThe best way to cure a headache is to mix rose\nand lavender with mortar and pestle… …but some folk\n"
        "tales say it can be treated by placing warm wet hay on\nthe head", 
        "Gangrene\n\nThe best way to cure gangrene and rot is to place\nmaggots on the infected areas… …but some folk tales\n"
        "say it can be treated by placing a cobweb on the skin.", 
        "Blood Vomiting\n\nThe best way to cure vomiting of blood is to mix mint\nand betony with mortar and pestle… …but "
        "some folk\ntales say it can be treated by drinking a cup of antimony.", 
        "Chills\n\nThe best way to cure chills is to simmer honey to make\na cough draught… …but some folk tales say it can "
        "be\ntreated by pouring water on the skin."};

    rightPageTexts = { "Vomiting\n\nThe best way to cure vomiting is to mix wormwood\nand mint with "
        "mortar and pestle…\n…but some folk tales say it can be treated by drinking a\ncup of antimony.", 
        "Buboes\n\nThe best way to cure buboes is to lance the boils with\na scalpel… …but some folk tales say it can be "
        "treated by\nplacing a plucked chicken on the boils.", 
        "Inflammation\n\nThe best way to cure inflamed skin is to spread\nointment on the inflamed area… …but some folk tales"
        "\nsay it can be treated by piercing the skin with a scalpel.", 
        "Muscle Cramps\n\nThe best way to cure muscle cramps is to mix henbane\nand hemlock with mortar and pestle… …but some" 
        " folk\ntales say it can be treated by placing a cramp - ring on\nthe finger.", 
        "Possession\n\nThe best way to cure possession of the muscles is to\nswing a crucifix over the possessed patient… …but"
        "\nsome folk tales say it can be treated by placing a cramp\n- ring on the finger."};

    leftPageText.InitializeText(fontPath_, leftPageTexts[pageNumber], characterSize_, false, false, Color::Black,
        leftTextPosition_);

    rightPageText.InitializeText(fontPath_, rightPageTexts[pageNumber], characterSize_, false, false, Color::Black,
        rightTextPosition_);

    leftButtonTexture.loadFromFile("Art Assets/Ui/left_button_arrow.png");

    turnPageLeftButton.setTexture(&leftButtonTexture);
    turnPageLeftButton.setSize(buttonSize_);
    turnPageLeftButton.setFillColor(Color::White);
    turnPageLeftButton.setPosition(leftButtonPosition_);

    rightButtonTexture.loadFromFile("Art Assets/Ui/right_button_arrow.png");

    turnPageRightButton.setTexture(&rightButtonTexture);
    turnPageRightButton.setSize(buttonSize_);
    turnPageRightButton.setFillColor(Color::White);
    turnPageRightButton.setPosition(rightButtonPosition_);
}

void TextbookPages::UpdateTextbookPages(Vector2f mousePos)
{
    // Check if the mouse is hovering over the turn left page button 
    if (turnPageLeftButton.getGlobalBounds().contains(mousePos))
    {
        if (turnPageLeftButton.getFillColor() != Color::Red) turnPageLeftButton.setFillColor(Color::Red);

        if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked)
        {
            if (pageNumber > 0) pageNumber--;
            mouseClicked = true;
        }

        else if (!Mouse::isButtonPressed(Mouse::Button::Left) && mouseClicked) mouseClicked = false;
    }

    else if (!turnPageLeftButton.getGlobalBounds().contains(mousePos))
    {
        if (turnPageLeftButton.getFillColor() != Color::White) turnPageLeftButton.setFillColor(Color::White);
    }

    // Check if the mouse is hovering over the turn right page button 
    if (turnPageRightButton.getGlobalBounds().contains(mousePos))
    {
        if (turnPageRightButton.getFillColor() != Color::Red) turnPageRightButton.setFillColor(Color::Red);

        if (Mouse::isButtonPressed(Mouse::Button::Left) && !mouseClicked)
        {
            if (pageNumber < rightPageTexts.size() - 1) pageNumber++;
            mouseClicked = true;
        }

        else if (!Mouse::isButtonPressed(Mouse::Button::Left) && mouseClicked) mouseClicked = false;
    }

    else if (!turnPageRightButton.getGlobalBounds().contains(mousePos))
    {
        if (turnPageRightButton.getFillColor() != Color::White) turnPageRightButton.setFillColor(Color::White);
    }

    if (leftPageText.GetText() != leftPageTexts[pageNumber]) leftPageText.SetText(leftPageTexts[pageNumber]);
    if (rightPageText.GetText() != rightPageTexts[pageNumber]) rightPageText.SetText(rightPageTexts[pageNumber]);
}

void TextbookPages::ClearTextVector()
{
    leftPageTexts.clear();
    rightPageTexts.clear();
}

void TextbookPages::Draw(sf::RenderWindow& window)
{
    window.draw(GroundSprite);
    window.draw(textbookSprite);
    window.draw(leftPageText.LoadText());
    window.draw(rightPageText.LoadText());
    window.draw(turnPageLeftButton);
    window.draw(turnPageRightButton);
}

void TextbookPages::SetPosition(sf::Vector2f position)
{
    textbookSprite.setPosition(position);
}

void TextbookPages::Move(sf::Vector2f offset)
{
    textbookSprite.move(offset);
}

void TextbookPages::SetScale(sf::Vector2f scale)
{
    textbookSprite.setScale(scale);
}

Sprite& TextbookPages::GetSprite()
{
    return textbookSprite;
}

const char* TextbookPages::GetTexture()
{
    return textbookSpriteTexture.GetTexture();
}