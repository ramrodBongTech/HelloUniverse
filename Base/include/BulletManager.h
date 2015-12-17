#ifndef BULLETMANAGER_H
#define BULLETMANAGER_H

#include "stdafx.h"
#include "Bullet.h"


class BulletManager
{
public:
	static BulletManager* instance();

	void update(sf::Time& dt);
	void draw(sf::RenderWindow* window);

	Bullet* getNextBullet();

	std::vector<Bullet *>& getUsedBullets();
	std::vector<Bullet *>& getFreeBullets();

protected:
	BulletManager();

	void createBullets(int bullets);

	// Member variables...
	std::vector<Bullet *> m_freeBullets;				
	std::vector<Bullet *> m_usedBullets;				
	int m_bullets;										
	sf::Texture m_tex;

};

static BulletManager* m_bulletInstance = 0;

#endif