#include "stdafx.h" 
#include "Player.h" 

Player::Player() :
m_radius(0),
m_speed(5),
m_rotation(0),
m_position(sf::Vector2f(640, 360)),
m_direction(sf::Vector2f((10) / 100.0f, (0) / 100.0f))
{
	sf::Texture tex;
	if (!tex.loadFromFile("playerShip1_blue.png"))
	{
	}
	m_sprite = new sf::Sprite(tex);

	m_radius = tex.getSize().x / 2;

	m_initialDirection = m_direction;

	m_sprite->setPosition(sf::Vector2f(0, 0));
	m_sprite->setOrigin(sf::Vector2f(m_radius, m_radius));
	m_sprite->setPosition(m_position);
	m_sprite->setRotation(90);
}

Player::Player(int rad, float speed, sf::Vector2f& pos, sf::Vector2f& dir, sf::Texture* tex, FMOD::System* FMODsys, thor::ParticleSystem* pSystem, sf::Vector2f& vel)
	: m_radius(rad),
	m_speed(speed),
	m_rotation(0),
	m_position(pos),
	m_direction(dir),
	m_acceleration(0.2f),
	m_velocity(vel),
	m_FMODsys(FMODsys),
	m_laserSoundOn(true),
	m_isMoving(false),
	m_pSystem(pSystem)
{
	m_initialDirection = dir;

	m_sprite = new sf::Sprite(*tex),
	m_sprite->setPosition(sf::Vector2f(0, 0));
	m_sprite->setOrigin(sf::Vector2f(m_radius, m_radius));
	m_sprite->setPosition(pos);
	m_sprite->setRotation(90);

	m_velocity = sf::Vector2f(0, 0);
}

Player::~Player(){

}

void Player::update(sf::Time& dt){
	for (int i = 0; i < m_bullets.size(); i++)
	{
		if (!m_bullets[i]->getAlive()){
  			m_bullets[i] = m_bullets.back();
   			m_bullets.pop_back();
		}
	}

	m_position = m_position + m_velocity;
	m_sprite->setPosition(m_position);

	if (m_speed > 0 && !m_isMoving)
	{
		m_speed -= m_acceleration;
	}
	else if (!m_isMoving)
	{
		m_speed = 0;
	}

	setRotation(m_rotation);
}

void Player::setRotation(float ang){
	m_rotation = ang;
	m_sprite->setRotation(m_rotation);
}

void Player::draw(sf::RenderWindow* window){
	window->draw(*m_sprite);
}

sf::Vector2f Player::getPosition() const{
	return m_position;
}

sf::Vector2f Player::getDirection() const{
	return m_direction;
}

float Player::getSpeed() const{
	return m_speed;
}

int Player::getRadius() const{
	return m_radius;
}

void Player::setPosition(sf::Vector2f& pos){
	m_position = pos;
	m_sprite->setPosition(pos);
}

void Player::setDirection(sf::Vector2f& dir){
	m_direction = dir;
}

void Player::rotateDirection(){
	m_direction.x = cosf(m_rotation);
	m_direction.y = sinf(m_rotation);

	m_direction.x = m_direction.x / std::sqrt(m_direction.x * m_direction.x + m_direction.y * m_direction.y);
	m_direction.y = m_direction.y / std::sqrt(m_direction.x * m_direction.x + m_direction.y * m_direction.y);
}

void Player::fire(sf::Time& dt){
	if (m_bullets.size() < MAX_BULLETS)
	{
		Bullet* b = BulletManager::instance()->getNextBullet();
		b->setPosition(sf::Vector2f(m_position.x + (m_direction.x * 50), m_position.y + (m_direction.y * 51)));
		b->setDirection(m_direction);
		float rot = m_rotation * 180 / M_PI;
		b->setRotation(m_rotation);
		b->setAlive(true);
		m_bullets.push_back(b);
	}

	if (m_laserSoundOn)
	{
		playLaser();
	}
}

std::vector<Bullet *>& Player::getBullets(){
	return m_bullets;
}

void Player::setRateOfFire(float rof)
{
	m_rof = rof;
}

float Player::getRateOfFire() const
{
	return m_rof;
}

sf::Time const Player::getFiringDelay() const
{
	return FIRING_DELAY;
}

void Player::playLaser()
{
	FMOD_RESULT result = m_FMODsys->createSound("Laser Blaster.mp3", FMOD_DEFAULT, 0, &m_laserSound);
	if (result != FMOD_OK)
	{
		std::cout << "FMOD error! (%d) %s\n" << result;
		exit(-1);
	}
	
	m_FMODsys->playSound(FMOD_CHANNEL_FREE, m_laserSound, true, &m_channel);
	m_channel->setVolume(0.1f);
	m_channel->setPaused(false);
}

void Player::setLaserSound(bool sound)
{
	m_laserSoundOn = sound;
}

bool Player::getLaserSound()
{
	return m_laserSoundOn;
}

void Player::updateListener()
{
	FMOD_VECTOR  listenervel = { m_velocity.x, 0.0f, m_velocity.y };
	FMOD_VECTOR  listenerpos = { m_position.x, 0.0f, m_position.y };

	m_FMODsys->set3DListenerAttributes(0, &listenerpos, &listenervel, 0, 0);
}

void Player::setMoving(bool move)
{
	m_isMoving = move;
}

bool Player::getMoving()
{
	return m_isMoving;
}

sf::Vector2f Player::getVelocity() const
{
	return m_velocity;
}

void Player::setVelocity(sf::Vector2f& vel)
{
	m_velocity = vel;
}