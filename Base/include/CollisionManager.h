#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "stdafx.h"
#include "Asteroid.h"
#include "Bullet.h"



class CollisionManager
{
public:
	static CollisionManager* instance();

	bool checkAsteroidPlayerBullet(Asteroid* a, Bullet* b);

	bool checkCircular(Asteroid* c1, Asteroid* c2);

	bool checkSAT(Asteroid* c1, Asteroid* c2, float& d, sf::Vector2f* minAxis);

	void handleCollisions(Asteroid* c1, Asteroid* c2);

	void handleMTV(Asteroid* c1, Asteroid* c2, float& d, sf::Vector2f* minAxis);

protected:
	CollisionManager();

};

static CollisionManager* m_instance = 0;

#endif