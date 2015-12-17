#include "stdafx.h" 

//FMOD includes
#pragma comment(lib,"fmodex_vc.lib")
#include "fmod.hpp"
#include "fmod_errors.h"

#include "SFML/Graphics.hpp" 
#include "SFML/OpenGL.hpp" 
#include <iostream> 

class Audio
{
public:
	Audio();
	~Audio();

	void update();

	void left();
	void right();

	void backgroundMusic();
	void pauseBackgroundMusic(bool);

	void sound3DSpace();
	void sound3DSpaceUpdate(sf::Vector2f, sf::Vector2f, sf::Vector2f);
	void pause3DSound(bool);

	void reverbManager();
	void pauseReverb(bool);

private:
	//setup FMOD
	FMOD::System *FMODsys; //will point to the FMOD system
	FMOD_RESULT result;

	FMOD::Channel *channel;

	FMOD::Sound *leftClick;
	FMOD::Sound *rightClick;

	FMOD::Sound *music;
	FMOD::Channel* musicChannel;

	FMOD::Sound *sound3D;
	FMOD::Channel *channel3D;

	FMOD::Reverb *reverb;


	FMOD::Sound *explosionSound;
};



