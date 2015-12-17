#include "stdafx.h"
#include "CollisionManager.h"

CollisionManager::CollisionManager() {
}

CollisionManager * CollisionManager::instance() {
	if (m_instance == 0)  {
		m_instance = new CollisionManager();
	}
	return m_instance;
}

bool CollisionManager::checkAsteroidPlayerBullet(Asteroid* a, Bullet* b){
	sf::Vector2f ba = b->getPosition() - a->getPosition();
	float dis = std::sqrt(ba.x*ba.x + ba.y*ba.y);
	float rad = b->getRadius() + a->getRadius();

	if (dis < rad){
		return true;
	}
	else{
		return false;
	}
}

bool CollisionManager::checkCircular(Asteroid* c1, Asteroid* c2)
{
	sf::Vector2f c2c1 = c2->getPosition() - c1->getPosition();
	float dis = std::sqrt(c2c1.x*c2c1.x + c2c1.y*c2c1.y);
	float rad = c2->getRadius() + c1->getRadius();

	if (dis < rad){
		return true;
	}
	else{
		return false;
	}
}

bool CollisionManager::checkSAT(Asteroid* c1, Asteroid* c2, float& d, sf::Vector2f* minAxis)
{
	std::vector<sf::Vector2f>* c1Axes = c1->getAxes();
	std::vector<sf::Vector2f>* c2Axes = c2->getAxes();

	for (int i = 0; i < c2Axes->size() / 2; i++)
	{
		float minA, minB, maxA, maxB;

		sf::Vector2f ax = c2Axes->at(i);
		ax = ax / sqrt(ax.x*ax.x + ax.y*ax.y);

		c1->project(&ax, &minA, &maxA);
		c2->project(&ax, &minB, &maxB);

		if (minA > maxB || minB > maxA)
		{
			d = 100000;
			*minAxis = sf::Vector2f(0, 0);
			return false;
		}

		if (maxB - minA < d)
		{
			d = maxB - minA;
			*minAxis = c2Axes->at(i);
		}
	}

	for (int i = 0; i < c1Axes->size() / 2; i++)
	{
		float minA, minB, maxA, maxB;

		sf::Vector2f ax = c1Axes->at(i);
		ax = ax / sqrt(ax.x*ax.x + ax.y*ax.y);

		c2->project(&ax, &minA, &maxA);
		c1->project(&ax, &minB, &maxB);

		if (minA > maxB || minB > maxA)
		{
			d = 100000;
			*minAxis = sf::Vector2f(0, 0);
			return false;
		}

		if (maxB - minA < d)
		{
			d = maxB - minA;
			*minAxis = c1Axes->at(i);
		}
	}
	
	return true;
}

void CollisionManager::handleCollisions(Asteroid* c1, Asteroid* c2)
{
	sf::Vector2f collisionVectorA = c2->getPosition() - c1->getPosition();
	float dotProdA = (collisionVectorA.x * c1->getDirection().x + collisionVectorA.y * c1->getDirection().y);

	sf::Vector2f collisionVectorB = c1->getPosition() - c2->getPosition();
	float dotProdB = (collisionVectorB.x * c2->getDirection().x + collisionVectorB.y * c2->getDirection().y);

	float dis = std::sqrt(collisionVectorA.x*collisionVectorA.x + collisionVectorA.y*collisionVectorA.y);

	float lenA = dotProdA / dis;
	float lenB = dotProdB / dis;

	sf::Vector2f vecA = (collisionVectorA / dis) * lenA;
	sf::Vector2f vecB = (collisionVectorB / dis) * lenB;

	c1->setDirection(c1->getDirection() - vecA + vecB);
	c2->setDirection(c2->getDirection() - vecB + vecA);
}

void CollisionManager::handleMTV(Asteroid* c1, Asteroid* c2, float& d, sf::Vector2f* minAxis)
{
	float divX = minAxis->x / sqrt(minAxis->x * minAxis->x + minAxis->y * minAxis->y);
	float divY = minAxis->y / sqrt(minAxis->x * minAxis->x + minAxis->y * minAxis->y);
	sf::Vector2f MTV = sf::Vector2f(divX * d, divY * d);

	sf::Vector2f s1 = c2->getPosition() - c1->getPosition();
	float t1 = (MTV.x * s1.x + MTV.y * s1.y);

	if (t1 > 0)
	{
		c1->setPosition(c1->getPosition() - MTV);
	}
	else if (t1 < 0)
	{
		c1->setPosition(c1->getPosition() + MTV);
	}
}