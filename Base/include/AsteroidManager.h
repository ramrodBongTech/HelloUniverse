#ifndef ASTEROIDMANAGER_H
#define ASTEROIDMANAGER_H

#include "stdafx.h"
#include "Asteroid.h"
#include "CollisionManager.h"
#include "FireworksEmitter.h"
//#include "Audio.h"

//FMOD includes
#pragma comment(lib,"fmodex_vc.lib")
#include "fmod.hpp"
#include "fmod_errors.h"

class AsteroidManager
{
public:
	AsteroidManager();
	
	AsteroidManager(thor::ParticleSystem* pSystem, FMOD::System* FMODsys);
	
	//static AsteroidManager* instance();
	
	void update(sf::Time& dt);
	
	void draw(sf::RenderWindow* window);
	
	std::vector<Asteroid *> getAsteroids() const;

protected:
	void createAsteroids(int asteroids);
	
	void eraseFromList();

	void playExplosion();

	void checkDestroyed(Asteroid* a);

	void checkCollisions();

	// Member variables...
	std::vector<Asteroid *> m_asteroids;				
	int m_numAsteroids, m_numDestroyed;				
	int m_numSmall, m_numMedium, m_numLarge;
	thor::ParticleSystem* m_pSystem;
	FMOD::System* m_FMODsys;
	FMOD::Sound* m_explosionSound;
	FMOD::Channel* m_channel;
	sf::Vector2f m_lastDestroyedPosition;
	sf::Texture m_smallTex, m_mediumTex, m_largeTex;

	// Const member variables
	const int RESPAWN = 3;
	const int MAX_LARGE = 16;
	const int MAX_MEDIUM = 32;
	const int MAX_SMALL = 64;

	//Audio *audio;
};
// Static member variable
static AsteroidManager* m_asteroidInstance = 0;

#endif