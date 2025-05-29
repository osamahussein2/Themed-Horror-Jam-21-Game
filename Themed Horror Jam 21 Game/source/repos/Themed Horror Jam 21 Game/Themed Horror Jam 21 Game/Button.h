#ifndef BUTTON_H
#define BUTTON_H
#include<iostream>
#include<ctime>
#include<cstdlib>
#include<sstream>
#include <format>

#include"SFML\System.hpp"
#include"SFML\Window.hpp"
#include"SFML\Graphics.hpp"
#include"SFML\Audio.hpp"

using namespace sf;
enum buttonState {
	BTN_IDLE = 0,
	BTN_HOVER,
	BTN_Active
};
class Button
{
	private:
		short unsigned buttonState;
		sf::RectangleShape shape;

		sf::Color idleColor;
		sf::Color hoverColor;
		sf::Color activeColor;
	public:
		Button(float x, float y, float width, float height, 
			sf::Color idleColor, sf::Color hoverColor , sf::Color activeColor);
		~Button();
		//accessors
		const bool isPressed() const ;
		const bool isHovered() const;
		//functions
		void update(sf::Vector2f mousePos);
		void draw(sf::RenderTarget* target);
};

#endif