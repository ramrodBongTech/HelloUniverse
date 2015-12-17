#include "stdafx.h"
#include "BulletManager.h"

// Protected constructor
BulletManager::BulletManager(){
	if (!m_tex.loadFromFile("laserBlue04.png")){}
	createBullets(100);
}

BulletManager * BulletManager::instance() {
	if (m_bulletInstance == 0)  {
		m_bulletInstance = new BulletManager();
	}

	return m_bulletInstance;
}

void BulletManager::createBullets(int bullets){
	int size = m_tex.getSize().x;
	for (int i = 0; i < bullets; i++)
	{
		m_freeBullets.push_back(new Bullet(sf::Vector2f((0) / 100.0f, (0) / 100.0f), sf::Vector2f(-9999, -9999), 20.0f, size / 2, m_tex));
	}
}

void BulletManager::update(sf::Time& dt){
	for (int i = 0; i < m_usedBullets.size(); i++){
		m_usedBullets[i]->update(dt);

		if (!(m_usedBullets[i]->getAlive()) && m_usedBullets.size() > 0){
			Bullet* b = m_usedBullets[i];
			m_freeBullets.push_back(b);
			m_usedBullets[i] = m_usedBullets.back();
			m_usedBullets.pop_back();
		}
	}
}

void BulletManager::draw(sf::RenderWindow* window){
	std::vector<Bullet *>::iterator curr = m_usedBullets.begin();
	std::vector<Bullet *>::iterator end = m_usedBullets.end();

	for (; curr != end; ++curr)
	{
		(*curr)->draw(window);
	}
}

Bullet* BulletManager::getNextBullet(){
	Bullet* b = 0;

	if (m_freeBullets.size() > 0)
	{
		b = m_freeBullets.at(m_freeBullets.size() - 1);
		m_usedBullets.push_back(b);
		m_freeBullets.pop_back();
	}

	return b;
}

std::vector<Bullet *>& BulletManager::getUsedBullets(){
	return m_usedBullets;
}

std::vector<Bullet *>& BulletManager::getFreeBullets(){
	return m_freeBullets;
}
