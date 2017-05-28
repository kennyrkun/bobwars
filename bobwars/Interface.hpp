#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <SFML\Graphics.hpp>
#include "BaseEntity.hpp"
#include "Button.hpp"

class Interface
{
public:
	Interface(sf::RenderWindow *_targetWindow, sf::View *_viewAnchor, sf::View *_mainView);
	~Interface();

	void setTargetWindow(sf::RenderWindow *target_window);
	sf::RenderWindow* getTargetWindow();
	void setViewAnchor(sf::View *viewAnchor);
	sf::View* getViewAnchor();

	void Render();
	void Update();

	Button create_ent_button;
	Button delete_ent_button;
	sf::Text unitCounterText;
	BaseEntity selectedEntity;
	bool hasSelected;
		
private:
	sf::RenderWindow *targetWindow;
	sf::View *viewAnchor;
	sf::View *mainView;
	sf::Font arial;

	sf::RectangleShape bottomui_background;
	sf::RectangleShape topui_background;

	sf::RectangleShape unitCounterIcon;
	sf::Texture unitCounterIcon_tex;
	sf::RectangleShape unitCounterBackground;

	void reanchor();
};

#endif /* INTERFACE_HPP */
