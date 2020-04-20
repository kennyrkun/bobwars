#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "BaseEntity.hpp"
#include "ResourceCounter.hpp"

#include <SFML/Graphics.hpp>

#include <SFUI/Layouts/Menu.hpp>
#include <SFUI/Layouts/HorizontalBoxLayout.hpp>
#include <SFUI/Layouts/HorizontalBoxLayout.hpp>

/*
class IInterface
{
public:
	IInterface(sf::RenderTarget *target);
	~IInterface();

	void addInterface(Interface* _interface);
	void popInterface(Interface* _interface);

private:
	sf::RenderTarget *target;
};
*/

enum MENU_CALLBACKS
{
	DELETE_SELECTION = -2,
	NULL_CALLBACK = -1,
};

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

	bool createEnabled = true;
	bool deleteEnabled = false;

	SFUI::Menu* unitActionMenu;
	SFUI::Menu* unitInformationMenu;
	SFUI::Menu* gameInformationMenu;

	// TODO: Separate these into "interface components" in other classes so that there isn't so much mangling going on
	sf::RectangleShape topBar;
	ResourceCounter* memesCounter;
	ResourceCounter* unitCounter;

	// TODO: Separate these into "interface components" in other classes so that there isn't so much mangling going on
	sf::RectangleShape bottomBar;

	void updateSelectionInfo(const std::vector<BaseEntity*>& entities);

private:
	sf::RenderWindow *targetWindow;
	sf::View *viewAnchor;
	sf::View *mainView;
	sf::Font arial;

	// TODO: replace these counters with ResourceCounter class
};

#endif // !INTERFACE_HPP

