#include "stdafx.h"
#include "Bullet.h"

// Default & Overloaded constructor
Bullet::Bullet():
m_direction(sf::Vector2f(0,0)),
m_position(sf::Vector2f(-9999,-9999)),
m_speed(0),
m_ttl(0),
m_radius(0),
m_alive(false)
{
	sf::Texture tex;
	if (!tex.loadFromFile("laserBlue03.png"))
	{
	}
	m_sprite = sf::Sprite(tex);

	m_radius = tex.getSize().y / 2;

	m_sprite.setPosition(sf::Vector2f(0, 0));
	m_sprite.setOrigin(sf::Vector2f(m_radius, m_radius));
	m_sprite.setPosition(m_position);
	m_sprite.setRotation(90);
}

Bullet::Bullet(sf::Vector2f& dir, sf::Vector2f& pos, float speed, int rad, sf::Texture& tex) 
:	m_direction(dir),
	m_position(pos),
	m_speed(speed*2),
	m_ttl(0),
	m_rotation(0),
	m_radius(rad),
	m_alive(false),
	m_sprite(tex)
{
	m_sprite.setPosition(sf::Vector2f(0, 0));
	m_sprite.setOrigin(sf::Vector2f(m_radius, m_radius));
	m_sprite.setPosition(pos);
	m_sprite.setRotation(90);
}

// Destructor function
Bullet::~Bullet(){
}

// Update method for moving the bullet
void Bullet::update(sf::Time& dt){
	if (m_alive){
		m_position += m_direction * m_speed;
		m_sprite.setPosition(m_position);

		m_ttl += dt.asMilliseconds();
		if (sf::milliseconds(m_ttl) >= MAX_TTL){
			reset();
		}
	}
}

// Draw method to draw the bullet
void Bullet::draw(sf::RenderWindow* window){
	if (m_alive){
		window->draw(m_sprite);
	}
}

// Getter method for the direction
sf::Vector2f Bullet::getDirection() const{
	return m_direction;
}

// Getter method for the position
sf::Vector2f Bullet::getPosition()const{
	return m_position;
}

// Getter method for the speed
float Bullet::getSpeed() const{
	return m_speed;
}

// Getter method for the radius
int Bullet::getRadius() const{
	return m_radius;
}

// Getter method for m_alive
bool Bullet::getAlive() const{
	return m_alive;
}

// Getter method for the sprite
sf::Sprite* Bullet::getSprite(){
	return &m_sprite;
}

// Setter method for the direction
void Bullet::setDirection(sf::Vector2f& dir){
	m_direction = dir;
}

// Setter method for the position
void Bullet::setPosition(sf::Vector2f& pos){
	m_position = pos;
	m_sprite.setPosition(pos);
}

// Setter method for the speed
void Bullet::setSpeed(float& s){
	m_speed = s;
}

// Setter method for the rotation of the sprite
void Bullet::setRotation(float& rot){
	m_rotation = rot;
	m_sprite.setRotation(rot+90);
}

// Getter method for m_alive
void Bullet::setAlive(bool a){
	m_alive = a;
}

// Method for reseting the bullet
void Bullet::reset(){
	m_direction = sf::Vector2f((0) / 100.0f, (0) / 100.0f);
	m_position = sf::Vector2f(-9999, -9999);
	m_ttl = 0;
	m_alive = false;
}