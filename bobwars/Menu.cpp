#include "Menu.hpp"

#include <ENGINE/Logger.hpp>
#include <ENGINE/Engine.hpp>

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

	float largest = options.front()->m_shape.getLocalBounds().width;
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

	selectedButton = options[0];
	selectedButton->select();
}

Menu::~Menu()
{
	for (size_t i = 0; i < options.size(); i++)
		delete options[i];

	options.clear();

	logger::INFO("PauseMenu destroyed");
}

void Menu::HandleEvents(sf::Event event)
{
	if (event.type == sf::Event::EventType::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Key::Up || event.key.code == sf::Keyboard::Key::Left)
		{
			if (selectedButton != nullptr)
				selectedButton->deselect();

			if (selectedButtonNum > 0)
			{
				selectedButton = options[selectedButtonNum - 1];
				selectedButton->select();
				selectedButtonNum -= 1;
			}
			else
			{
				selectedButton = options[options.size() - 1];
				selectedButton->select();
				selectedButtonNum = options.size() - 1;
			}
		}
		else if (event.key.code == sf::Keyboard::Key::Down || event.key.code == sf::Keyboard::Key::Right)
		{
			if (selectedButton != nullptr)
				selectedButton->deselect();

			if (selectedButtonNum < options.size() - 1)
			{
				selectedButton = options[selectedButtonNum + 1];
				selectedButton->select();
				selectedButtonNum += 1;
			}
			else
			{
				selectedButton = options[0];
				selectedButton->select();
				selectedButtonNum = 0;
			}
		}
		else if (event.key.code == sf::Keyboard::Key::Return)
		{
			logger::INFO("selected option " + std::to_string(selectedButtonNum));

			selectedOption = selectedButtonNum;
			done = true;
		}
		else if (event.key.code == sf::Keyboard::Key::Return)
		{
			logger::INFO("dont fuck me");
		}

		logger::INFO("key: " + std::to_string(event.key.code));
		logger::INFO("return: " + std::to_string(sf::Keyboard::Key::Return));
	}
}

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(background);

	for (size_t i = 0; i < options.size(); i++)
		target.draw(*options[i]);
}
