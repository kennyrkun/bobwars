#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "BaseEntity.hpp"

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

	void addInterface(Interface *_interface);
	void popInterface(Interface* _interface);

private:
	sf::RenderTarget *target;
};
*/

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
//	SFUI::Button* create_ent_button;

	bool deleteEnabled = false;
//	SFUI::Button* delete_ent_button;
	// TODO: replace these with SFUI::Button

	SFUI::Menu* menu;

	sf::Texture bobIcon;

	sf::Text unitCounterText;
	sf::Text memesCounterText;
	
	sf::RectangleShape bottomBar;
	sf::RectangleShape topBar;

	enum class State
	{
		NoEntitiesSelected,
		SingleEntitySelected,
		MultipleEntitiesSelected
//		SelectedCommentSection
	};

	void updateUnitInfo(State state, BaseEntity *entitiy);

private:
	sf::RenderWindow *targetWindow;
	sf::View *viewAnchor;
	sf::View *mainView;
	sf::Font arial;

	// TODO: replace these counters with ResourceCounter class

	sf::RectangleShape unitCounterIcon;
	sf::Texture unitCounterIcon_tex;
	sf::RectangleShape unitCounterBackground;

	sf::RectangleShape memesCounterIcon;
	sf::Texture memesCounterIcon_tex;
	sf::RectangleShape memesCounterBackground;
};

#endif /* INTERFACE_HPP */
