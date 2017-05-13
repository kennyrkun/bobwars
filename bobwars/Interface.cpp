#include "Interface.hpp"

#include <ENGINE\Logger.hpp>

Interface::Interface(sf::RenderWindow *target_window)
{
//	Button create_ent_button(sf::Vector2f(50, 25), "create");
//	Button delete_ent_button(sf::Vector2f(50, 25), "delete");

	targetWindow = target_window;

	logger::INFO("New interface created.");
}

Interface::~Interface()
{
	logger::INFO("Interface destroyed.");
}

// public:

void Interface::setTargetWindow(sf::RenderWindow *target_window)
{
	targetWindow = target_window;
}

void Interface::draw()
{
	create_ent_button.draw(targetWindow);
	create_ent_button.draw(targetWindow);
}
