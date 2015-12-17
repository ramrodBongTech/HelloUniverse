#ifndef MENU_H
#define MENU_H

#include "stdafx.h" 

#include "SFML/Graphics.hpp" 
#include "SFML/OpenGL.hpp" 
#include "SFML/Audio.hpp"
#include <iostream> 
#define _USE_MATH_DEFINES
#include <math.h>
#define MAX_NUMBER_OF_ITEMS 5

class Menu
{
public:
	Menu(float width, float height, sf::Texture& texture, sf::Font* font);
	~Menu();
	void update();
	void draw(sf::RenderWindow* window);
	void moveUp();
	void moveDown();
	int getPressedItem();

private:
	int m_selectedItemIndex;
	sf::Text m_text[MAX_NUMBER_OF_ITEMS];

	sf::SoundBuffer m_buffer;
	sf::Sound m_moveSound;
	sf::Music m_music;

	sf::Sprite m_sprite;
	float m_angle;
};

#endif