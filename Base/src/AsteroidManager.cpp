#include "stdafx.h"
#include "AsteroidManager.h"

AsteroidManager::AsteroidManager() 
{
	if (!m_smallTex.loadFromFile("meteorBrown_tiny1.png") || 
		!m_mediumTex.loadFromFile("meteorBrown_med1.png") || 
		!m_largeTex.loadFromFile("meteorBrown_big4.png"))
	{
	}
	m_numAsteroids = 9;
	m_numDestroyed = 0;
	m_numSmall = 0;
	m_numMedium = 0;
	m_numLarge = 0;
	createAsteroids(m_numAsteroids);
}

AsteroidManager::AsteroidManager(thor::ParticleSystem* pSystem, FMOD::System* FMODsys)
	: m_pSystem(pSystem),
	m_FMODsys(FMODsys)
{
	if (!m_smallTex.loadFromFile("meteorBrown_tiny1.png") ||
		!m_mediumTex.loadFromFile("meteorBrown_med1.png") ||
		!m_largeTex.loadFromFile("meteorBrown_big4.png"))
	{
	}
	m_numAsteroids = 56;
	m_numDestroyed = 0;
	m_numSmall = 0;
	m_numMedium = 0;
	m_numLarge = 0;
	createAsteroids(m_numAsteroids);
}

void AsteroidManager::createAsteroids(int asteroids)
{
	for (int i = 0; i < asteroids; i++)
	{
		if (i % 3 == 0 && m_numLarge != MAX_LARGE)
		{
			m_asteroids.push_back(new Asteroid((rand() % 5 + 5), sf::Vector2f(rand() % 2560 - 1280 - (m_largeTex.getSize().x / 2), rand() % 1440 - 720 - (m_largeTex.getSize().y / 2)), sf::Vector2f((rand() % 20 - 10) / 100.0f, (rand() % 20 - 10) / 100.0f), &m_largeTex, Asteroid::AsteroidType::LARGE, m_pSystem));
			m_numLarge++;
		}
		else if (i % 3 == 1 && m_numMedium != MAX_MEDIUM)
		{
			m_asteroids.push_back(new Asteroid((rand() % 5 + 5), sf::Vector2f(rand() % 2560 - 1280 - (m_mediumTex.getSize().x / 2), rand() % 1440 - 720 - (m_mediumTex.getSize().y / 2)), sf::Vector2f((rand() % 20 - 10) / 100.0f, (rand() % 20 - 10) / 100.0f), &m_mediumTex, Asteroid::AsteroidType::MEDIUM, m_pSystem));
			m_numMedium++;
		}
		else
		{
			m_asteroids.push_back(new Asteroid((rand() % 5 + 5), sf::Vector2f(rand() % 2560 - 1280 - (m_smallTex.getSize().x / 2), rand() % 1440 - 720 - (m_smallTex.getSize().y / 2)), sf::Vector2f((rand() % 20 - 10) / 100.0f, (rand() % 20 - 10) / 100.0f), &m_smallTex, Asteroid::AsteroidType::SMALL, m_pSystem));
			m_numSmall++;
		}
	}
}

void AsteroidManager::update(sf::Time& dt)
{
	for (int i = 0; i < m_asteroids.size(); i++)
	{
		m_asteroids[i]->update(dt);

		checkDestroyed(m_asteroids[i]);
	}

	if (m_numDestroyed == RESPAWN && m_numLarge < MAX_LARGE)
	{
		m_numDestroyed = 0;
		m_asteroids.push_back(new Asteroid(5, sf::Vector2f(rand() % 1280 - (m_largeTex.getSize().x / 2), rand() % 720 - (m_largeTex.getSize().y / 2)), sf::Vector2f((rand() % 20 - 10) / 100.0f, (rand() % 20 - 10) / 100.0f), &m_largeTex, Asteroid::AsteroidType::LARGE, m_pSystem));
		m_numAsteroids++;
		m_numLarge++;
	}

	checkCollisions();

	////update position of sound
	//if (m_channel)
	//{
	//	FMOD_VECTOR  sourcePos = { m_lastDestroyedPosition.x, m_lastDestroyedPosition.y };
	//	//source is fixed so velocity is zero
	//	m_channel->set3DAttributes(&sourcePos, 0);
	//}
}

void AsteroidManager::draw(sf::RenderWindow* window)
{
	std::vector<Asteroid *>::iterator curr = m_asteroids.begin();
	std::vector<Asteroid *>::iterator end = m_asteroids.end();

	for (; curr != end; ++curr)
	{
		(*curr)->draw(window);
	}
}

// Getter for the vector of asteroids
std::vector<Asteroid *> AsteroidManager::getAsteroids() const
{
	return m_asteroids;
}

// Method for erasing an asteroid from the vector
void AsteroidManager::eraseFromList()
{
	std::vector<Asteroid *>::iterator erase;
	std::vector<Asteroid *>::iterator curr = m_asteroids.begin();
	std::vector<Asteroid *>::iterator end = m_asteroids.end();

	for (; curr != end; ++curr)
	{
		if (!(*curr)->getAlive())
		{
			erase = curr;
			curr = end-1;
		}
	}

	m_asteroids.erase(erase);
}

void AsteroidManager::playExplosion()
{
	//FMOD_RESULT result = m_FMODsys->createSound("explosion.mp3", FMOD_DEFAULT, 0, &m_explosionSound);
	//if (result != FMOD_OK)
	//{
	//	std::cout << "FMOD error! (%d) %s\n" << result;
	//	exit(-1);
	//}

	//m_FMODsys->playSound(FMOD_CHANNEL_FREE, m_explosionSound, true, &m_channel);
	//m_channel->setVolume(0.25f);
	//m_channel->set3DMinMaxDistance(10, 100);
	//m_channel->setPaused(false);
	//audio->sound3DSpace();
}

void AsteroidManager::checkDestroyed(Asteroid* a)
{
	if (!a->getAlive() && a->getType() == Asteroid::AsteroidType::LARGE)
	{
		m_numLarge--;
		m_numMedium += 2;
		m_numAsteroids++;
		m_numDestroyed++;

		m_pSystem->addEmitter(ExplosionEmitter(a->getPosition()), explosionDuration);

		playExplosion();
		m_lastDestroyedPosition = a->getPosition();

		if (m_numMedium + 1 < MAX_MEDIUM)
		{
			m_asteroids.push_back(new Asteroid(5, a->getPosition() + (a->getDirection()*400.0f),a->getDirection(), &m_mediumTex, Asteroid::AsteroidType::MEDIUM, m_pSystem));
			m_asteroids.push_back(new Asteroid(5, a->getPosition() - (a->getDirection()*400.0f), -a->getDirection(), &m_mediumTex, Asteroid::AsteroidType::MEDIUM, m_pSystem));
		}

		eraseFromList();
	}
	else if (!a->getAlive() && a->getType() == Asteroid::AsteroidType::MEDIUM)
	{
		m_numMedium--;
		m_numSmall += 2;
		m_numAsteroids++;
		m_numDestroyed++;

		m_pSystem->addEmitter(ExplosionEmitter(a->getPosition()), explosionDuration);

		playExplosion();
		m_lastDestroyedPosition = a->getPosition();

		if (m_numSmall + 1 < MAX_SMALL)
		{
			m_asteroids.push_back(new Asteroid(5, a->getPosition() + (a->getDirection()*100.0f), a->getDirection(), &m_smallTex, Asteroid::AsteroidType::SMALL, m_pSystem));
			m_asteroids.push_back(new Asteroid(5, a->getPosition() - (a->getDirection()*100.0f), -a->getDirection(), &m_smallTex, Asteroid::AsteroidType::SMALL, m_pSystem));
		}

		eraseFromList();
	}
	else if (!a->getAlive() && a->getType() == Asteroid::AsteroidType::SMALL)
	{
		m_numSmall--;
		m_numAsteroids--;
		m_numDestroyed++;

		m_pSystem->addEmitter(ExplosionEmitter(a->getPosition()), explosionDuration);

		playExplosion();
		m_lastDestroyedPosition = a->getPosition();

		eraseFromList();
	}
}

void AsteroidManager::checkCollisions()
{
	for (int i = 0; i < m_asteroids.size(); i++)
	{
		float d = 100000;
		sf::Vector2f* minAxis = &sf::Vector2f(0, 0);

		for (int j = i + 1; j < m_asteroids.size(); j++)
		{
			if (CollisionManager::instance()->checkCircular(m_asteroids[i], m_asteroids[j]))
			{
				if (CollisionManager::instance()->checkSAT(m_asteroids[i], m_asteroids[j], d, minAxis))
				{
					CollisionManager::instance()->handleMTV(m_asteroids[i], m_asteroids[j], d, minAxis);
					CollisionManager::instance()->handleCollisions(m_asteroids[i], m_asteroids[j]);
				}
			}
		}
	}
}