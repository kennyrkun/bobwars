#ifndef MENU_HPP
#define MENU_HPP

#include <SFML\Graphics.hpp>
#include <vector>
#include <string>
#include <SFUI\TextButton.hpp>

class Menu
{
public:
	Menu(sf::RenderWindow* window_, std::string title_, std::vector<SFUI::TextButton*> options_);
	~Menu();

	void HandleEvents(sf::Event& event);
	void Draw();

	std::vector<SFUI::TextButton*> options;
	std::string title;
	sf::Vector2f windowDimensions;

private:
	sf::RectangleShape background;

	sf::RenderWindow* window;
};

#endif // !MENU_HPP
