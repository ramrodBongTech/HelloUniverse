#ifndef PLAYER_H
#define PLAYER_H

#include "stdafx.h" 
#include "BulletManager.h" 
#include "FireworksEmitter.h"

//FMOD includes
#pragma comment(lib,"fmodex_vc.lib")
#include "fmod.hpp"
#include "fmod_errors.h"


class Player{
public:

	Player();
	Player(int rad, float speed, sf::Vector2f& pos, sf::Vector2f& dir, sf::Texture* tex, FMOD::System* FMODsys, thor::ParticleSystem* pSystem, sf::Vector2f& vel);
	~Player();
	void update(sf::Time& dt);
	
	void draw(sf::RenderWindow* window);

	sf::Vector2f getPosition() const;
	void setPosition(sf::Vector2f& pos);

	sf::Vector2f getDirection() const;
	void setDirection(sf::Vector2f& dir);

	sf::Vector2f getVelocity() const;
	void setVelocity(sf::Vector2f& vel);

	float getSpeed() const;
	int getRadius() const;

	void setRotation(float);
	void rotateDirection();

	void fire(sf::Time& dt);
	std::vector<Bullet *>& getBullets();

	void setRateOfFire(float rof);
	float getRateOfFire() const;
	sf::Time const getFiringDelay() const;

	void setLaserSound(bool sound);
	bool getLaserSound();

	void setMoving(bool move);
	bool getMoving();

private:
	// Member variables for the player
	int m_radius;
	float m_speed, m_rotation, m_rof, m_acceleration;
	sf::Vector2f m_position, m_direction, m_initialDirection;
	sf::Vector2f m_velocity;
	sf::Sprite* m_sprite;
	// A vector of bullets
	std::vector<Bullet *> m_bullets;
	thor::ParticleSystem* m_pSystem;
	FMOD::System* m_FMODsys;
	FMOD::Sound* m_laserSound;
	FMOD::Channel* m_channel;
	bool m_laserSoundOn, m_isMoving;

	void playLaser();
	void updateListener();

	// Constant member variables we will use
	//const float SPRITE_ROTATION_SPEED = 0.576f, ROTATION_SPEED = 0.01f;
	const float SPRITE_ROTATION_SPEED = 1.43f, ROTATION_SPEED = 0.025f;
	const int MAX_BULLETS = 20, MAX_SPEED = 10;
	const sf::Time FIRING_DELAY = sf::milliseconds(1);
};

#endif