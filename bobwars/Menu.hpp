#ifndef MENU_HPP
#define MENU_HPP

#include <SFUI/TextButton.hpp>
#include <SFML/Graphics.hpp>

#include <vector>
#include <string>

class Menu : public sf::Drawable
{
public:
	Menu(sf::RenderWindow* window_, std::string title_, std::vector<SFUI::TextButton*> options_);
	~Menu();

	void HandleEvents(sf::Event event);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	std::vector<SFUI::TextButton*> options;
	std::string title;
	sf::Vector2f windowDimensions;

	int selectedOption;
	bool done;

private:
	sf::RenderWindow* window;
	sf::RectangleShape background;

	SFUI::TextButton *selectedButton;
	int selectedButtonNum;

};

#endif // !MENU_HPP
