#include "stdafx.h" 
#include "Audio.h"

#include <iostream> 
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

Audio::Audio()
{
	////setup FMOD
	//FMOD::System *FMODsys; //will point to the FMOD system
	//FMOD_RESULT result;
	result = FMOD::System_Create(&FMODsys);         // Create the main system object.

	if (result != FMOD_OK)
	{
		cout << "FMOD error!" << result << FMOD_ErrorString(result);
		//exit(-1);
	}

	result = FMODsys->init(100, FMOD_INIT_NORMAL, 0);   // Initialize FMOD.

	if (result != FMOD_OK)
	{
		cout << "FMOD Error!" << result << FMOD_ErrorString(result);
		//exit(-1);
	}

	// Load Sounds 
	result = FMODsys->createSound("Content\\Audio\\highDown.mp3", FMOD_DEFAULT, 0, &leftClick);
	result = FMODsys->createSound("Content\\Audio\\highUp.mp3", FMOD_DEFAULT, 0, &rightClick);

	result = FMODsys->createStream("Content\\Audio\\background.ogg", FMOD_SOFTWARE | FMOD_LOOP_NORMAL, 0, &music);

	result = FMODsys->createSound("Content\\Audio\\noise.ogg", FMOD_LOOP_NORMAL | FMOD_3D, 0, &sound3D);

	result = FMODsys->createSound("explosion.mp3", FMOD_DEFAULT, 0, &explosionSound);

	if (result != FMOD_OK)
	{
		std::cout << "FMOD error! (%d) %s\n" << result;
		//exit(-1);
	}
}

Audio::~Audio()
{
}

void Audio::update()
{
	FMODsys->update();
}

void Audio::left()
{
	FMODsys->playSound(FMOD_CHANNEL_FREE, leftClick, false, 0);
}
void Audio::right()
{
	FMODsys->playSound(FMOD_CHANNEL_FREE, rightClick, false, 0);
}

void Audio::backgroundMusic()
{
	FMODsys->playSound(FMOD_CHANNEL_FREE, music, false, &musicChannel);
}

void Audio::pauseBackgroundMusic(bool pause)
{
	musicChannel->setPaused(pause);
}

void Audio::sound3DSpace()
{
	FMODsys->playSound(FMOD_CHANNEL_FREE, explosionSound, false, &channel3D);
	result = channel3D->setVolume(0.125f);
	//need this for sound fall off
	channel3D->set3DMinMaxDistance(50, 500);
}

void Audio::sound3DSpaceUpdate(sf::Vector2f playerPos, sf::Vector2f playerVel, sf::Vector2f objectPos)
{
	FMOD_VECTOR  listenervel = { playerVel.x, playerVel.y, 0.0f };
	FMOD_VECTOR  listenerpos = { playerPos.x, playerPos.y, 0.0f };
	//final pair of parameters are forward direction and up direction of listener (not needed in 2D)
	FMODsys->set3DListenerAttributes(0, &listenerpos, &listenervel, 0, 0);

	//update position of sound
	if (channel3D){
		FMOD_VECTOR  sourcePos = { objectPos.x, objectPos.y, 0.0f };
		//source is fixed so velocity is zero
		channel3D->set3DAttributes(&sourcePos, 0);
	}
}

void Audio::pause3DSound(bool pause)
{
	channel3D->setPaused(pause);
}

void Audio::reverbManager()
{
	result = FMODsys->createReverb(&reverb);
	FMOD_REVERB_PROPERTIES prop = FMOD_PRESET_SEWERPIPE;
	reverb->setProperties(&prop);
	FMOD_VECTOR pos = { 0, 0, 0 };

	reverb->set3DAttributes(&pos, 150, 350);
}

void Audio::pauseReverb(bool pause)
{
	reverb->setActive(pause);
}
