#include "Button.h"


Button::Button(float x, float y, float width, float height, 
	Color idleColor, Color hoverColor, Color activeColor)
{
	this->buttonState = BTN_IDLE;
	this->shape.setPosition(sf::Vector2f(x, y));
	this->shape.setSize(sf::Vector2f(width, height));

	this->idleColor = idleColor;
	this->hoverColor = hoverColor;
	this->activeColor = activeColor;

	this->shape.setFillColor(this->idleColor);
}

Button::~Button()
{
}
const bool Button::isPressed() const {
	return this->buttonState == BTN_Active;
}

const bool Button::isHovered() const
{
	return buttonState == BTN_HOVER;
}

void Button::update(Vector2f mousePos)
{
	this->buttonState = BTN_IDLE;

	if (this->shape.getGlobalBounds().contains(mousePos)) 
	{
		this->buttonState = BTN_HOVER;

		if (Mouse::isButtonPressed(Mouse::Button::Left))
		{
			this->buttonState = BTN_Active;
		}
	}

	switch (this->buttonState)
	{
	case BTN_IDLE:
		this->shape.setFillColor(this->idleColor);
		break;
	case BTN_HOVER:
		this->shape.setFillColor(this->hoverColor);
		break;
	case BTN_Active:
		this->shape.setFillColor(this->activeColor);
		break;
	default:
		this->shape.setFillColor(this->idleColor);
		break;
	}
	
}

void Button::draw(sf::RenderTarget* target)
{
	target->draw(this->shape);

}
