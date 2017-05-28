#include "Interface.hpp"

#include <ENGINE\Logger.hpp>

Interface::Interface(sf::RenderWindow *_targetWindow, sf::View *_viewAnchor, sf::View *_mainView)
{
//	Button create_ent_button(sf::Vector2f(50, 25), "create");
//	Button delete_ent_button(sf::Vector2f(50, 25), "delete");

	targetWindow = _targetWindow;
	viewAnchor = _viewAnchor;
	mainView = _mainView;

	arial.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf");

	topui_background.setFillColor(sf::Color(100, 100, 100));
	topui_background.setSize(sf::Vector2f(mainView->getSize().x, 20));
	topui_background.setOrigin(topui_background.getLocalBounds().width / 2, topui_background.getLocalBounds().height / 2);

	bottomui_background.setFillColor(sf::Color(100, 100, 100));
	bottomui_background.setSize(sf::Vector2f(mainView->getSize().x, 60));
	bottomui_background.setOrigin(bottomui_background.getLocalBounds().width / 2, bottomui_background.getLocalBounds().height / 2);

	unitCounterIcon_tex.loadFromFile("resource\\textures\\silk\\user.png");
	unitCounterIcon.setTexture(&unitCounterIcon_tex);
	unitCounterIcon.setSize(sf::Vector2f(10, 10));
	unitCounterIcon.setOrigin(sf::Vector2f(unitCounterIcon.getLocalBounds().width / 2, unitCounterIcon.getLocalBounds().height / 2));

	unitCounterBackground.setFillColor(sf::Color(70, 70, 70));
	float padding = 1.5f;
	unitCounterBackground.setSize(sf::Vector2f(28 - padding, 13 - padding));
	unitCounterBackground.setOutlineThickness(padding);
	unitCounterBackground.setOutlineColor(sf::Color(50, 50, 50));
	unitCounterText.setFont(arial);

	unitCounterText.setFont(arial);
	unitCounterText.setCharacterSize(34);
	unitCounterText.setScale(sf::Vector2f(.25f, .25f));
	unitCounterText.setString("0");
	unitCounterText.setOrigin(sf::Vector2f(unitCounterText.getLocalBounds().width / 2, unitCounterText.getLocalBounds().height / 2));

	logger::INFO("New interface created.");
}

Interface::~Interface()
{
	logger::INFO("Interface destroyed.");
}

// public:

void Interface::setTargetWindow(sf::RenderWindow *_targetWindow)
{
	targetWindow = _targetWindow;
}

sf::RenderWindow* Interface::getTargetWindow()
{
	return this->targetWindow;
}

void Interface::setViewAnchor(sf::View *_viewAnchor)
{
	viewAnchor = _viewAnchor;
}

sf::View* Interface::getViewAnchor()
{
	return this->viewAnchor;
}

void Interface::Render()
{
	Update();
	
	targetWindow->draw(topui_background);

	targetWindow->draw(unitCounterBackground);
	targetWindow->draw(unitCounterIcon);
	targetWindow->draw(unitCounterText);

//	targetWindow->draw(bottomui_background);

	create_ent_button.draw(targetWindow);
	delete_ent_button.draw(targetWindow);
}

void Interface::Update()
{
	unitCounterText.setOrigin(sf::Vector2f(unitCounterText.getLocalBounds().width / 2, unitCounterText.getLocalBounds().height / 2));

	reanchor();
}

// private:

void Interface::reanchor()
{
	topui_background.setPosition(sf::Vector2f(mainView->getCenter().x, mainView->getCenter().y - 140));
	unitCounterBackground.setPosition(sf::Vector2f(mainView->getCenter().x - 115, mainView->getCenter().y - 145.5f));
	unitCounterIcon.setPosition(sf::Vector2f((mainView->getCenter().x - 114) + (unitCounterIcon.getLocalBounds().width / 2), (mainView->getCenter().y - 145) + (unitCounterIcon.getLocalBounds().height / 2)));
	unitCounterText.setPosition(sf::Vector2f((mainView->getCenter().x - 102) + (unitCounterIcon.getLocalBounds().width / 2), (mainView->getCenter().y - 147) + (unitCounterIcon.getLocalBounds().height / 2)));

	bottomui_background.setPosition(sf::Vector2f(mainView->getCenter().x, mainView->getCenter().y + 120));

	create_ent_button.setPosition(sf::Vector2f(mainView->getCenter().x - 179, mainView->getCenter().y - 140));
	delete_ent_button.setPosition(sf::Vector2f(mainView->getCenter().x - 139, mainView->getCenter().y - 140));
}
