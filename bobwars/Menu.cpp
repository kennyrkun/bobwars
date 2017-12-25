#include "Menu.hpp"

#include <ENGINE\Logger.hpp>
#include <ENGINE\Engine.hpp>

// HACK: this is only here because the EXACT SAME THING in the engine won't work. /shrug
bool mouseIsOver(sf::Shape &object, sf::RenderWindow &window)
{
	if (object.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
		return true;
	else
		return false;
}

Menu::Menu(sf::RenderWindow* window_, std::string title_, std::vector<SFUI::TextButton*> options_) : window(window_), title(title_), options(options_)
{
	logger::INFO("Creating " + title + " Menu with " + std::to_string(options.size()) + " options");

	int largest = options.front()->m_shape.getLocalBounds().width;
	for (size_t i = 0; i < options.size(); i++)
		if (options[i]->m_shape.getLocalBounds().width > largest)
			largest = options[i]->m_shape.getLocalBounds().width;

	background.setSize(sf::Vector2f(largest + 10, options.size() * 54));
	background.setOrigin(sf::Vector2f(background.getGlobalBounds().width / 2, background.getGlobalBounds().height / 2));
	background.setPosition(sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2));
	background.setOutlineColor(sf::Color::Black);
	background.setOutlineThickness(4);

	options[0]->setPosition(sf::Vector2f(background.getPosition().x, (background.getPosition().y - background.getGlobalBounds().height / 2) + (options[0]->m_shape.getLocalBounds().height / 2) + 10));

	for (size_t i = 1; i < options.size(); i++)
	{
		options[i]->setPosition(sf::Vector2f(background.getPosition().x, (options[i - 1]->getPosition().y + (options[i - 1]->m_shape.getLocalBounds().height / 2)) + (options[i]->m_shape.getLocalBounds().height / 2) + 10));
	}

	logger::INFO("Created " + title + " Menu");
}

Menu::~Menu()
{
	for (size_t i = 0; i < options.size(); i++)
		delete options[i];

	options.clear();

	logger::INFO("PauseMenu destroyed");
}

void Menu::HandleEvents(sf::Event& event)
{
	if (event.type == sf::Event::EventType::MouseButtonPressed)
	{
		for (size_t i = 0; i < options.size(); i++)
		{
			if (mouseIsOver(options[i]->m_shape, *window))
			{
				options[i]->select();
			}
		}
	}
	else if (event.type == sf::Event::EventType::MouseButtonReleased)
	{
		for (size_t i = 0; i < options.size(); i++)
		{
			if (mouseIsOver(options[i]->m_shape, *window))
			{
				options[i]->select();
//				options[i]->callback();
			}
		}
	}
}

void Menu::Draw()
{
	window->draw(background);

	for (size_t i = 0; i < options.size(); i++)
		window->draw(*options[i]);
}
