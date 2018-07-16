#include "Menu.hpp"

#include "Util/Logger.hpp"
#include "Util/Util.hpp"

// HACK: this is only here because the EXACT SAME THING in the util won't work. /shrug
bool mouseIsOver(sf::Shape &object, sf::RenderWindow &window)
{
	if (object.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
		return true;
	else
		return false;
}

Menu::Menu(sf::RenderWindow* window_, std::string title_, std::vector<std::pair<std::string, int>> options_) : window(window_), title(title_), options(options_)
{
	logger::INFO("Creating " + title + " Menu with " + std::to_string(options.size()) + " options");

	menu = new SFUI::Menu(*window);

	for (size_t i = 0; i < options.size(); i++)
		menu->addButton(options[i].first, options[i].second);

	background.setSize(sf::Vector2f(menu->getSize().x + 10, menu->getSize().y + 10));
	background.setPosition(sf::Vector2f((window->getSize().x / 2) - (background.getSize().x / 2), (window->getSize().y / 2) - (background.getSize().x / 2)));
	background.setOutlineColor(sf::Color::Black);
	background.setOutlineThickness(4);

	menu->setPosition(sf::Vector2f(background.getPosition().x + 5, background.getPosition().y + 5));

	logger::INFO("Created " + title + " Menu");
}

Menu::~Menu()
{
	delete menu;

	logger::INFO("PauseMenu destroyed.");
}

void Menu::Update()
{
}

void Menu::HandleEvents(sf::Event event)
{
	int id = menu->onEvent(event);

	if (event.type == sf::Event::EventType::KeyPressed || event.type == sf::Event::EventType::MouseButtonReleased)
	{
		if (event.key.code == sf::Keyboard::Key::Return || event.key.code == sf::Mouse::Left)
		{
			selectedOption = id;

			if (id == -1)
			{
				logger::INFO("No widget ID was returned, skipping. (" + std::to_string(id) + ")");
			}
			else
			{
				std::cout << "option: " << options[id].first << " (" << id << ")" << std::endl;

				std::cout << "options" << std::endl;
				done = true;
			}
		}
	}
}

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(background);

	target.draw(*menu);
}
