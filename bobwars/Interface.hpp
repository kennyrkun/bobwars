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

	void HandleEvents(sf::Event& event);
	void Update();
	void Draw();

	SFUI::TextButton create_ent_button;
	SFUI::TextButton delete_ent_button;
	// TODO: replace these with SFUI::Button

	sf::Text unitCounterText;
	sf::Text memesCounterText;
		
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

	sf::RectangleShape memesCounterIcon;
	sf::Texture memesCounterIcon_tex;
	sf::RectangleShape memesCounterBackground;
};

#endif /* INTERFACE_HPP */
