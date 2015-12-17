#ifndef BULLET_H
#define BULLET_H

#include "stdafx.h" 

#include "SFML/Graphics.hpp" 
#include "SFML/OpenGL.hpp" 
#include <iostream> 
#define _USE_MATH_DEFINES
#include <math.h>

class Bullet{
public:
	Bullet();
	Bullet(sf::Vector2f& dir, sf::Vector2f& pos, float speed, int rad, sf::Texture& tex);

	~Bullet();

	void update(sf::Time& dt);

	void draw(sf::RenderWindow* window);

	sf::Vector2f getDirection() const;
	void setDirection(sf::Vector2f& dir);

	sf::Vector2f getPosition() const;
	void setPosition(sf::Vector2f& pos);

	float getSpeed() const;
	void setSpeed(float& s);

	bool getAlive() const;
	void setAlive(bool a);

	int getRadius() const;
	void setRotation(float& rot);

	sf::Sprite* getSprite();

	void reset();

private:
	// Member variables for the bullet
	sf::Vector2f m_direction, m_position;
	float m_speed, m_ttl, m_rotation;
	int m_radius;
	bool m_alive;
	sf::Sprite m_sprite;								// or use a particle instead??

	// Constant variables that we will use
	const sf::Time MAX_TTL = sf::milliseconds(1000);
};

#endif