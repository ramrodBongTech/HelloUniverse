// HelloUniverse.cpp : Defines the entry point for the console application.
/////////////////////////////////////////////////////////// 
// Headers 
//////////////////////////////////////////////////////////// 
#include "stdafx.h"
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#pragma comment(lib,"thor-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 

#include "Player.h"
#include "AsteroidManager.h"
#include "CollisionManager.h"
#include "Menu.h"
#include "Camera.h"
#include "FireworksEmitter.h"
#include "Audio.h"
#include "Controller.h"

////////////////////////////////////////////////////////////
///Entrypoint of application 
//////////////////////////////////////////////////////////// 
sf::Clock myClock;
float rof;
sf::Font menuFont;
sf::Texture menuTexture, playerTexture, cameraTexture, fireTexture;

Player* p;
Camera* camera;
AsteroidManager* asteroidMgr;

thor::ParticleSystem pSystem;

FMOD::System* FMODsys;							// Will point to the FMOD system
FMOD_RESULT result;
bool backgroundSoundOn;
FMOD::Channel* backgroundChannel;
FMOD::Sound* backgroundSound;
Asteroid* lastDestroyed;

Audio audio;
Controller controller;
sf::CircleShape reverbCircle(350);

const int MENU_STATE = 0, GAME_STATE = 1;
int state = 0;

void loadAssets()
{
	// Font
	if (!menuFont.loadFromFile("C:\\Windows\\Fonts\\arial.ttf"))
	{
		std::cout << "Font Error" << std::endl;
	}

	// Art
	if (!menuTexture.loadFromFile("Black Hole.png") || !playerTexture.loadFromFile("playerShip1_blue.png") ||
		!cameraTexture.loadFromFile("grid.png") || !fireTexture.loadFromFile("fireball.png"))
	{
		std::cout << "Art error" << std::endl;
	}

	// Loading a sound file
	result = FMODsys->createSound("background.ogg", FMOD_DEFAULT, 0, &backgroundSound);
	if (result != FMOD_OK)
	{
		std::cout << "FMOD error! (%d) %s\n" << result;
		exit(-1);
	}

	backgroundSoundOn = true;

	reverbCircle.setOrigin(350, 350);
	reverbCircle.setPosition(640, 720);
	reverbCircle.setFillColor(sf::Color(0, 0, 255, 100));
}

void getUserInput(sf::Time& dt)
{
	if (sf::Joystick::isConnected(0) == false)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && sf::milliseconds(rof) >= p->getFiringDelay())
		{
			p->fire(dt);
			rof = 0;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			if (backgroundSoundOn)
			{
				backgroundChannel->setPaused(true);
				backgroundSoundOn = false;
			}
			else
			{
				backgroundChannel->setPaused(false);
				backgroundSoundOn = true;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
		{
			if (p->getLaserSound())
			{
				p->setLaserSound(false);
			}
			else
			{
				p->setLaserSound(true);
			}
		}
	}

	else if (sf::Joystick::isConnected(0))
	{
		p->setVelocity(controller.getLeftStickVelocity());
		p->setRotation(controller.getRightStickByAngle());
		p->setDirection(controller.getRightStickByVector());

		if (controller.RightThumbStickDirection.x >= 50 || controller.RightThumbStickDirection.x <= -50
			|| controller.RightThumbStickDirection.y >= 50 || controller.RightThumbStickDirection.y <= -50)
		{
			if (controller.rTriggerState == controller.RTHARD)
			{
				p->fire(dt);
			}
		}

		if (controller.rTriggerState == controller.RTHARD && sf::milliseconds(rof) >= p->getFiringDelay())
		{
			p->fire(dt);
			rof = 0;
		}
	}
}

void checkCollisions()
{
	std::vector<Bullet *> playerBullets = p->getBullets();

	for (int i = 0; i < playerBullets.size(); i++)
	{
		std::vector<Asteroid *> asteroids = asteroidMgr->getAsteroids();
		for (int j = 0; j < asteroids.size(); j++)
		{
			if (CollisionManager::instance()->checkAsteroidPlayerBullet(asteroids[j], playerBullets[i]))
			{
				asteroids[j]->setAlive(false);
				playerBullets[i]->setAlive(false);
				lastDestroyed = asteroids[j];
				audio.sound3DSpace();
			}
		}
	}
}


void updateGame(sf::Time& dt, sf::RenderWindow* window)
{
	window->draw(reverbCircle);

	getUserInput(dt);

	p->update(dt);

	camera->update(window);
	camera->move();

	BulletManager::instance()->update(dt);
	asteroidMgr->update(dt);

	checkCollisions();

	if (lastDestroyed != nullptr)
	{
		audio.sound3DSpaceUpdate(p->getPosition(), p->getVelocity(), lastDestroyed->getPosition());
	}

	audio.reverbManager();
}

int main() 
{ 
    // Create the main window 
    sf::RenderWindow window(sf::VideoMode(1280, 720, 32), "Hello Universe"); 

	// Seed the random generator
	srand((unsigned)time(NULL));

	window.setFramerateLimit(30);

	lastDestroyed = nullptr;

	bool part1 = true;
	bool part2 = true;
	bool part3 = true;
	bool part4 = true;

	// Setup FMOD
	result = FMOD::System_Create(&FMODsys);         // Create the main system object.
	if (result != FMOD_OK)
	{
		std::cout << "FMOD error!" << result << FMOD_ErrorString(result);
		exit(-1);
	}

	result = FMODsys->init(100, FMOD_INIT_NORMAL, 0);   // Initialize FMOD.
	if (result != FMOD_OK)

	{
		std::cout << "FMOD error!" << result << FMOD_ErrorString(result);
		exit(-1);
	}

	loadAssets();

	pSystem.setTexture(fireTexture);
	//system.addAffector(FireworkAffector());
	asteroidMgr = new AsteroidManager(&pSystem, FMODsys);

	/***********************************************************************************************************************************************************/
	// Create timer that can be connected to callbacks. Initial time limit is 1 second, timer immediately starts
	//thor::CallbackTimer explosionTimer;
	//explosionTimer.restart(sf::seconds(1.f));
	//// Connect timer to a lambda expression which restarts the timer every time it expires
	////explosionTimer.connect([](thor::CallbackTimer& trigger)
	////{
	////	trigger.restart(explosionInterval);
	////});

	//// Connect timer to a lambda expression that creates an explosion at expiration
	//explosionTimer.connect0([&system]()
	//{
	//	// Compute random position on screen
	//	sf::Vector2f position(thor::randomDev(400.f, 300.f), thor::randomDev(300.f, 200.f));

	//	// Add a temporary emitter to the particle system
	//	system.addEmitter(FireworkEmitter(position), explosionDuration);
	//});

	/*thor::UniversalEmitter emitter;
	emitter.setEmissionRate(30);
	emitter.setParticleLifetime(sf::seconds(5));
	emitter.setParticlePosition(sf::Vector2f(640, 360));
	system.addEmitter(thor::refEmitter(emitter));*/
	/***********************************************************************************************************************************************************/
	/**************************************************************************************************************************************************************/

	Menu menu(window.getSize().x, window.getSize().y, menuTexture, &menuFont);

	BulletManager::instance();
	CollisionManager::instance();

	p = new Player(playerTexture.getSize().x / 2, 0, sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2), sf::Vector2f((100) / 100.0f, (0) / 100.0f), &playerTexture, FMODsys, &pSystem, sf::Vector2f(0, 0));
	camera = new Camera(window.getSize().x, window.getSize().y, cameraTexture, p);

	// Create a channel to play the sound on
	FMODsys->playSound(FMOD_CHANNEL_FREE, backgroundSound, true, &backgroundChannel);
	backgroundChannel->setVolume(0.25f);
	backgroundChannel->setPaused(false);
	backgroundChannel->set3DMinMaxDistance(1, 50);

	//update position of sound
	if (backgroundChannel)
	{
		FMOD_VECTOR  sourcePos = { 1280/2, 720/2 };
		//source is fixed so velocity is zero
		backgroundChannel->set3DAttributes(&sourcePos, 0);
	}

    // Start game loop 
    while (window.isOpen()) 
    {
		sf::Time dt = myClock.restart();

		pSystem.update(dt);

		audio.update();

        // Process events 
        sf::Event Event; 
        while (window.pollEvent(Event)) 
        { 
            // Close window : exit 
            if (Event.type == sf::Event::Closed) 
                window.close(); 
   
            // Escape key : exit 
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape)) 
                window.close(); 
			switch (state)
			{
			case MENU_STATE:
				// Move UI Up
				if ((Event.type == sf::Event::KeyReleased) && (Event.key.code == sf::Keyboard::Up))
				{
					menu.moveUp();
				}

				// Move UI Down
				if ((Event.type == sf::Event::KeyReleased) && (Event.key.code == sf::Keyboard::Down))
				{
					menu.moveDown();
				}

				if ((Event.type == sf::Event::KeyReleased) && (Event.key.code == sf::Keyboard::Z))
				{
					if (backgroundSoundOn)
					{
						backgroundChannel->setPaused(true);
						backgroundSoundOn = false;
					}
					else
					{
						backgroundChannel->setPaused(false);
						backgroundSoundOn = true;
					}
				}

				// Select
				if ((Event.type == sf::Event::KeyReleased) && (Event.key.code == sf::Keyboard::Return))
					//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
				{
					switch (menu.getPressedItem())
					{
					case 0:
						state = GAME_STATE;
						break;
					case 1:
						state = GAME_STATE;
						break;
					case 2:
						std::cout << "High Score button has been pressed!" << std::endl;
						break;
					case 3:
						std::cout << "Options button has been pressed!" << std::endl;
						break;
					case 4:
						std::cout << "Quit button has been pressed!" << std::endl;
						window.close();
						break;
					}
				}
				// FMOD
				//Part 1
				if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Num1)) { part1 = !part1; }
				if (part1 == true)
				{
					if ((Event.type == sf::Event::MouseButtonPressed) && (Event.key.code == sf::Mouse::Left)){ audio.left(); }
					if ((Event.type == sf::Event::MouseButtonPressed) && (Event.key.code == sf::Mouse::Right)){ audio.right(); }
				}

				// Part 2
				if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Num2)) { part2 = !part2; }
				if (part2 == true)
				{
					audio.pauseBackgroundMusic(false);
				}
				else { audio.pauseBackgroundMusic(true); }

				// Part 3
				if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Num3)) { part3 = !part3; }
				if (part3 == true)
				{
					audio.pause3DSound(false);
				}
				else { audio.pause3DSound(true); }

				// Part 4
				if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Num4)) { part4 = !part4; }
				if (part4 == true)
				{
					audio.pauseReverb(true);
				}
				else { audio.pauseReverb(false); }

				break;
			}
		}

		// Update particle system and timer
		pSystem.update(dt);
		//explosionTimer.update();

		switch (state)
		{
		case MENU_STATE:
			menu.update();
			break;
		case GAME_STATE:
			updateGame(dt, &window);
			controller.update(Event);
			break;
		}
             
			/*if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::W)){ camera.move(sf::Vector2f(0, -5)); }
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::A)){ camera.move(sf::Vector2f(-5, 0)); }
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::S)){ camera.move(sf::Vector2f(0, 5)); }
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::D)){ camera.move(sf::Vector2f(5, 0)); }*/
         
		//prepare frame
        window.clear();

		switch (state)
		{
		case MENU_STATE:
			menu.draw(&window);
			break;
		case GAME_STATE:
			window.draw(pSystem, sf::BlendAdd);
			BulletManager::instance()->draw(&window);
			asteroidMgr->draw(&window);
			camera->draw(&window);
			break;
		}

        // Finally, display rendered frame on screen 
        window.display(); 
    } //loop back for next frame
   
    return EXIT_SUCCESS; 
}


