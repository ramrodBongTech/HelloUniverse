#include "stdafx.h"
#include "Menu.h"

Menu::Menu(float width, float height, sf::Texture& texture, sf::Font* font)
{
	// Menu Items
	m_text[0].setFont(*font);
	m_text[0].setColor(sf::Color::Red);
	m_text[0].setString("Continue");
	m_text[0].setPosition(sf::Vector2f(width / 1.5f, height / (MAX_NUMBER_OF_ITEMS + 1) * 1));

	m_text[1].setFont(*font);
	m_text[1].setColor(sf::Color::White);
	m_text[1].setString("New Game");
	m_text[1].setPosition(sf::Vector2f(width / 1.5f, height / (MAX_NUMBER_OF_ITEMS + 1) * 2));

	m_text[2].setFont(*font);
	m_text[2].setColor(sf::Color::White);
	m_text[2].setString("High Score");
	m_text[2].setPosition(sf::Vector2f(width / 1.5f, height / (MAX_NUMBER_OF_ITEMS + 1) * 3));

	m_text[3].setFont(*font);
	m_text[3].setColor(sf::Color::White);
	m_text[3].setString("Options");
	m_text[3].setPosition(sf::Vector2f(width / 1.5f, height / (MAX_NUMBER_OF_ITEMS + 1) * 4));

	m_text[4].setFont(*font);
	m_text[4].setColor(sf::Color::White);
	m_text[4].setString("Quit!");
	m_text[4].setPosition(sf::Vector2f(width / 1.5f, height / (MAX_NUMBER_OF_ITEMS + 1) * 5));

	m_selectedItemIndex = 0;

	// Audio
	if (!m_buffer.loadFromFile("content\\audio\\rollover1.wav")){ std::cout << "Buffer error" << std::endl; }
	m_moveSound.setBuffer(m_buffer);

	if (!m_music.openFromFile("content\\audio\\background.ogg")){ std::cout << "Music error" << std::endl; }
	m_music.play();
	m_music.setLoop(true);

	m_sprite.setTexture(texture);
	m_sprite.setTextureRect(sf::IntRect(0, 0, 512, 512));
	m_sprite.setOrigin(sf::Vector2f(256, 256));
	m_sprite.setPosition(sf::Vector2f(width / 3, height / 2));
	m_angle = 0;
}

Menu::~Menu()
{

}

void Menu::update()
{
	m_angle += 1.0f / 12.0f;
	m_sprite.setRotation(m_angle);
	if (m_angle >= 360)
	{ 
		m_angle = 0;
	}
}

void Menu::draw(sf::RenderWindow* window)
{
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
	{
		window->draw(m_text[i]);
	}

	window->draw(m_sprite);
}
void Menu::moveUp()
{
	if (m_selectedItemIndex - 1 >= 0)
	{
		m_text[m_selectedItemIndex].setColor(sf::Color::White);
		m_selectedItemIndex--;
		m_text[m_selectedItemIndex].setColor(sf::Color::Red);
		m_moveSound.play();
	}
}

void Menu::moveDown()
{
	if (m_selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS)
	{
		m_text[m_selectedItemIndex].setColor(sf::Color::White);
		m_selectedItemIndex++;
		m_text[m_selectedItemIndex].setColor(sf::Color::Red);
		m_moveSound.play();
	}
}

int Menu::getPressedItem()
{
	return m_selectedItemIndex;
}