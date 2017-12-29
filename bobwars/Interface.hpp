#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <SFML\Graphics.hpp>
#include "BaseEntity.hpp"
#include "Button.hpp"

class Interface
{
public:
	Interface(sf::RenderWindow *_targetWindow, sf::View *_mainView);
	~Interface();

	void setTargetWindow(sf::RenderWindow *target_window);
	sf::RenderWindow* getTargetWindow() { return targetWindow; }
	void setViewAnchor(sf::View *viewAnchor);
	sf::View* getViewAnchor() { return viewAnchor; }

	void Draw();
	void Update();

	Button create_ent_button;
	Button delete_ent_button;
	// TODO: replace these with SFUI::Button

	sf::Text unitCounterText;
		
	sf::RectangleShape bottomBar;
	sf::RectangleShape topBar;

private:
	sf::RenderWindow *targetWindow;
	sf::View *viewAnchor;
	sf::View *mainView;
	sf::Font arial;

	sf::RectangleShape unitCounterIcon;
	sf::Texture unitCounterIcon_tex;
	sf::RectangleShape unitCounterBackground;

	void reanchor();
};

#endif /* INTERFACE_HPP */
