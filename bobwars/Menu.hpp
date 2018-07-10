#ifndef MENU_HPP
#define MENU_HPP

#include <SFUI/Layouts/Menu.hpp>
#include <SFML/Graphics.hpp>

#include <vector>
#include <string>

class Menu : public sf::Drawable
{
public:
	Menu(sf::RenderWindow* targetWindow, std::string menuTitle, std::vector<std::pair<std::string, int>> options);
	~Menu();

	void Update();
	void HandleEvents(sf::Event event);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	std::vector<std::pair<std::string, int>> options;
	std::string title;
	sf::Vector2f windowDimensions;

	SFUI::Menu* menu;

	int selectedOption;
	bool done;

private:
	sf::RenderWindow* window;
	sf::RectangleShape background;
};

#endif // !MENU_HPP
