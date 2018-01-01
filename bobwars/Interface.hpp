#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <SFUI/TextButton.hpp>

#include <SFML/Graphics.hpp>

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
	void HandleEvents();

	SFUI::TextButton create_ent_button;
	SFUI::TextButton delete_ent_button;
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
};

#endif /* INTERFACE_HPP */
