#ifndef ASTEROID_H
#define ASTEROID_H

#include "stdafx.h" 
#include "FireworksEmitter.h"

class Asteroid{
public:
	enum class AsteroidType { LARGE, MEDIUM, SMALL };

	Asteroid();
	Asteroid(float speed, sf::Vector2f& pos, sf::Vector2f& dir, sf::Texture* tex, AsteroidType t, thor::ParticleSystem* pSystem);

	~Asteroid();

	void update(sf::Time& dt);

	void draw(sf::RenderWindow* window);

	void updatePosition(sf::Time& dt);

	void wallBounce();
	
	sf::Vector2f getPosition() const;
	void setPosition(sf::Vector2f& pos);

	sf::Vector2f getDirection() const;
	void setDirection(sf::Vector2f& dir);

	bool getAlive() const;
	void setAlive(bool a);

	float getSpeed() const;
	int getRadius() const;

	AsteroidType getType() const;
	std::vector<sf::Vector2f>* getAxes();

	void rotate();

	void makeLargeAsteroid();
	void makeMediumAsteroid();
	void makeSmallAsteroid();

	void destroyed();

	void reset();

	void project(sf::Vector2f* axis, float* min, float* max);

private:
	// Member variable for the asteroid
	int m_radius;
	float m_speed, m_tts, m_rotation;
	sf::Vector2f m_position, m_direction;
	sf::Sprite* m_sprite;
	AsteroidType m_type;
	bool m_alive;
	thor::ParticleSystem* m_pSystem;

	std::vector<sf::Vertex> m_points;
	std::vector<sf::Vector2f> m_axes;

	const float ROTATION_SPEED = 0.576f;
	const int MAX_TTS = 1000;
};

#endif