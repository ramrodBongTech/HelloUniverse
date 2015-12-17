#ifndef CAMERA_H
#define CAMERA_H

#include "stdafx.h" 
#include "Player.h"

class Camera
{
public:
	Camera(float w, float h, sf::Texture& texture, Player* p);
	~Camera();

	void update(sf::RenderWindow* window);
	void draw(sf::RenderWindow* window);
	void move();
	sf::View view;

private:
	sf::Sprite m_sprite;

	Player* m_player;
	float m_radius;

	int m_width;
	int m_height;
};

#endif