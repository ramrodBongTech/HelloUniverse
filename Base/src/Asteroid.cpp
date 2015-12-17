#include "stdafx.h" 
#include "Asteroid.h" 

Asteroid::Asteroid() :
m_radius(0),
m_speed(0),
m_position(sf::Vector2f(-9999, -9999)),
m_direction(sf::Vector2f(0, 0)),
m_type(AsteroidType::SMALL),
m_alive(false),
m_tts(0)
{
	sf::Texture tex;
	if (!tex.loadFromFile("meteorBrown_tiny1.png"))
	{
	}
	m_sprite = new sf::Sprite(tex);

	m_radius = tex.getSize().x / 2;

	m_sprite->setPosition(sf::Vector2f(0, 0));
	m_sprite->setOrigin(sf::Vector2f(m_radius, m_radius));
	m_sprite->setPosition(m_position);
	m_sprite->setRotation(rand() % 360 + 1);
}

Asteroid::Asteroid(float speed, sf::Vector2f& pos, sf::Vector2f& dir, sf::Texture* tex, AsteroidType t, thor::ParticleSystem* pSystem)
:	m_radius(tex->getSize().x/2),
	m_speed(speed),
	m_position(pos),
	m_direction(dir),
	m_type(t),
	m_alive(true),
	m_tts(0),
	m_pSystem(pSystem)
{
	m_sprite = new sf::Sprite(*tex);
	m_sprite->setPosition(sf::Vector2f(0, 0));
	m_sprite->setOrigin(sf::Vector2f(m_radius, m_radius));
	m_sprite->setPosition(pos);
	m_rotation = 0;


	for (float angle = 0.0f; angle < (2.0f * M_PI); angle += (2.0f * M_PI) / 6)
	{
		m_points.push_back(sf::Vertex(sf::Vector2f(m_position.x + (m_radius * cos(angle)), m_position.y + (m_radius * sin(angle)))));
	}

	for (int i = 0; i < m_points.size(); i++)
	{
		sf::Vector2f a = m_points[i].position;
		sf::Vector2f b = m_points[(i + 1) % m_points.size()].position;
		sf::Vector2f edge = b - a;
		sf::Vector2f ax = sf::Vector2f(-edge.y, edge.x);

		m_axes.push_back(ax);
	}
}

Asteroid::~Asteroid(){
}

void Asteroid::update(sf::Time& dt){
	if (m_alive){
		updatePosition(dt);
		rotate();
		wallBounce();
	}
	/*else{
		m_tts += dt;
		if (m_tts >= MAX_TTS){
			reset();
		}
	}*/
}

void Asteroid::draw(sf::RenderWindow* window){
	if (m_alive){
		window->draw(*m_sprite);
	}
}

void Asteroid::updatePosition(sf::Time& dt){
	m_position += m_direction * (m_speed * 10);
	m_sprite->setPosition(m_position);

	for (int i = 0; i < m_points.size(); i++)
	{
		m_points[i].position += m_direction * (m_speed * 10);
	}
}

void Asteroid::wallBounce(){
	if (m_position.x + m_radius <= -1280){				// From left to right
		m_position.x = 2560;
		m_sprite->setPosition(m_position);
	}
	if (m_position.x - m_radius >= 2560){			// From right to left
		m_position.x = -1280;
		m_sprite->setPosition(m_position);
	}
	if (m_position.y + m_radius <= -720){				// From top to bottom
		m_position.y = 1440;
		m_sprite->setPosition(m_position);
	}
	if (m_position.y - m_radius >= 1440){			// From bottom to top
		m_position.y = -720;
		m_sprite->setPosition(m_position);
	}
}

sf::Vector2f Asteroid::getPosition() const{
	return m_position;
}

sf::Vector2f Asteroid::getDirection() const{
	return m_direction;
}

float Asteroid::getSpeed() const{
	return m_speed;
}

int Asteroid::getRadius() const {
	return m_radius;
}

bool Asteroid::getAlive() const{
	return m_alive;
}

void Asteroid::setPosition(sf::Vector2f& pos){
	m_position = pos;
	m_sprite->setPosition(pos);
}

void Asteroid::setDirection(sf::Vector2f& dir){
	m_direction = dir;
}

void Asteroid::setAlive(bool a){
	m_alive = a;
}

void Asteroid::rotate(){
	m_sprite->rotate(ROTATION_SPEED);
	m_rotation += ROTATION_SPEED;
	m_points.clear();
	
	for (float angle = 0.0f; angle < (2.0f * M_PI); angle += (2.0f * M_PI) / 6)
	{
		m_points.push_back(sf::Vertex(sf::Vector2f(m_position.x + (m_radius * cos(angle + m_rotation)), m_position.y + (m_radius * sin(angle + m_rotation)))));
	}

	for (int i = 0; i < m_points.size(); i++)
	{
		sf::Vector2f a = m_points[i].position;
		sf::Vector2f b = m_points[(i + 1) % m_points.size()].position;
		sf::Vector2f edge = b - a;
		sf::Vector2f ax = sf::Vector2f(-edge.y, edge.x);
		//ax = ax / sqrt(ax.x*ax.x + ax.y*ax.y);

		m_axes[i] = ax;
	}
}

Asteroid::AsteroidType Asteroid::getType() const{
	return m_type;
}

void Asteroid::destroyed(){
	m_alive = false;
	m_speed = 0;
	m_position = sf::Vector2f(-9999, -9999);
	m_direction = sf::Vector2f(0, 0);
}

void Asteroid::reset(){
	m_speed = 5;
	m_position = sf::Vector2f(rand() % 1280 - m_radius, rand() % 720 - m_radius);
	m_direction = sf::Vector2f((rand() % 20 - 10) / 100.0f, (rand() % 20 - 10) / 100.0f);
	m_alive = true;
	m_tts = 0;
}

std::vector<sf::Vector2f>* Asteroid::getAxes()
{
	return &m_axes;
}

// Method for projecting the points onto an axis
void Asteroid::project(sf::Vector2f* axis, float* min, float* max)
{
	float d = (axis->x * m_points[0].position.x + axis->y * m_points[0].position.y);

	float _min = d;
	float _max = d;

	for (int i = 1; i < m_points.size(); i++)
	{
		d = (axis->x * m_points[i].position.x + axis->y * m_points[i].position.y);
		if (_min > d)
		{
			_min = d;
		}

		if (_max < d)
		{
			_max = d;
		}
	}
	*min = _min;
	*max = _max;
}