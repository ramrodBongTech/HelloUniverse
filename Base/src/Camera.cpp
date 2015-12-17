#include "stdafx.h"
#include "Camera.h"

Camera::Camera(float w, float h, sf::Texture& texture, Player* p)
{
	m_width = w;
	m_height = h;

	m_sprite.setTexture(texture);
	m_sprite.setPosition(-m_width, -m_height);
	//sprite.setOrigin(sf::Vector2f(width / 2, height / 2));
	m_sprite.scale(3, 3);

	m_radius = p->getRadius();
	m_player = p;
	m_player->setPosition(sf::Vector2f(m_width / 2, m_height / 2));

	view.setSize(m_width, m_height);
	view.setCenter(m_player->getPosition());
}

Camera::~Camera()
{

}

void Camera::update(sf::RenderWindow* window)
{
	window->setView(view);
}

void Camera::draw(sf::RenderWindow* window)
{
	window->draw(m_sprite);
	m_player->draw(window);
}

void Camera::move()
{
	if (m_player->getPosition().x > (-m_width + m_width / 2) // left side
		&& m_player->getPosition().x  < ((m_width * 2) - (m_width / 2))) // right side
	{
		view.move(m_player->getVelocity().x, 0);
	}

	if (m_player->getPosition().y  >(-m_height + m_height / 2) // top side
		&& m_player->getPosition().y  < ((m_height * 2) - (m_height / 2))) // bottom side
	{
		view.move(0, m_player->getVelocity().y);
	}

	if (m_player->getPosition().x < -m_width)
	{
		m_player->setPosition(sf::Vector2f((m_width * 2), m_player->getPosition().y));
		view.setCenter(m_player->getPosition().x - m_width / 2, view.getCenter().y);
	}

	if (m_player->getPosition().x >(m_width * 2))
	{
		m_player->setPosition(sf::Vector2f(-m_width, m_player->getPosition().y));
		view.setCenter(m_player->getPosition().x + m_width / 2, view.getCenter().y);
	}

	if (m_player->getPosition().y < -m_height)
	{
		m_player->setPosition(sf::Vector2f(m_player->getPosition().x, (m_height * 2)));
		view.setCenter(view.getCenter().x, m_player->getPosition().y - m_height / 2);
	}

	if (m_player->getPosition().y >(m_height * 2))
	{
		m_player->setPosition(sf::Vector2f(m_player->getPosition().x, -m_height));
		view.setCenter(view.getCenter().x, m_player->getPosition().y + m_height / 2);
	}
}