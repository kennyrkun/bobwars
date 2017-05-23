#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <SFML\Graphics.hpp>
#include "Button.hpp"

class Interface
{
public:
	Interface(sf::RenderWindow *target_window);
	Interface();
	~Interface();

	void setTargetWindow(sf::RenderWindow *target_window);
	sf::RenderWindow* getTargetWindow();

	void draw();
	void clicked();

	Button create_ent_button;
	Button delete_ent_button;
	sf::RectangleShape bottomui_background;
	sf::RectangleShape topui_background;
		
private:
	sf::RenderWindow *targetWindow;
};

#endif /* INTERFACE_HPP */
