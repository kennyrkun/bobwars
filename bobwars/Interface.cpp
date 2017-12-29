#include "Interface.hpp"

#include <ENGINE\Logger.hpp>

Interface::Interface(sf::RenderWindow *_targetWindow, sf::View *_mainView)
{
//	Button create_ent_button(sf::Vector2f(50, 25), "create");
//	Button delete_ent_button(sf::Vector2f(50, 25), "delete");

	targetWindow = _targetWindow;
	mainView = _mainView;

	viewAnchor = new sf::View(mainView->getCenter(), sf::Vector2f(targetWindow->getSize().x, targetWindow->getSize().y));

	topBar.setSize(sf::Vector2f(targetWindow->getSize().x, 40));
	bottomBar.setSize(sf::Vector2f(targetWindow->getSize().x, 150));

	float leftX = viewAnchor->getCenter().x - targetWindow->getSize().x / 2;
	float rightX = 0;
	float topMiddleY = viewAnchor->getCenter().y - targetWindow->getSize().y / 2 + topBar.getSize().y / 2;
	float bottomMiddleY = viewAnchor->getCenter().y + targetWindow->getSize().y / 2 - bottomBar.getSize().y / 2;

	arial.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf");

	topBar.setFillColor(sf::Color(100, 100, 100));
	topBar.setOrigin(topBar.getLocalBounds().width / 2, topBar.getLocalBounds().height / 2);
	topBar.setPosition(sf::Vector2f(viewAnchor->getCenter().x, topMiddleY));

	bottomBar.setFillColor(sf::Color(100, 100, 100));
	bottomBar.setOrigin(bottomBar.getLocalBounds().width / 2, bottomBar.getLocalBounds().height / 2);
	bottomBar.setPosition(sf::Vector2f(viewAnchor->getCenter().x, bottomMiddleY));

	unitCounterBackground.setFillColor(sf::Color(70, 70, 70));
	float padding = 3.0f;
	unitCounterBackground.setSize(sf::Vector2f(46 - padding, 26 - padding));
	unitCounterBackground.setOrigin(sf::Vector2f(unitCounterBackground.getSize().x / 2, unitCounterBackground.getSize().y / 2));
	unitCounterBackground.setOutlineThickness(padding);
	unitCounterBackground.setOutlineColor(sf::Color(50, 50, 50));
	unitCounterBackground.setPosition(sf::Vector2f(leftX + 125, topMiddleY));

	unitCounterIcon_tex.loadFromFile("resource\\textures\\silk\\user.png");
	unitCounterIcon.setTexture(&unitCounterIcon_tex);
	unitCounterIcon.setSize(sf::Vector2f(20, 20));
	unitCounterIcon.setOrigin(sf::Vector2f(unitCounterIcon.getLocalBounds().width / 2, unitCounterIcon.getLocalBounds().height / 2));
	unitCounterIcon.setPosition(sf::Vector2f(unitCounterBackground.getPosition().x - 10, unitCounterBackground.getPosition().y));

	unitCounterText.setFont(arial);
	unitCounterText.setCharacterSize(22);
	unitCounterText.setString("0");
	unitCounterText.setOrigin(sf::Vector2f(unitCounterText.getLocalBounds().width / 2, unitCounterText.getLocalBounds().height / 2));
	unitCounterText.setPosition(sf::Vector2f(unitCounterBackground.getPosition().x + 10, unitCounterBackground.getPosition().y - 6));

	create_ent_button.setString("create");
	create_ent_button.setPosition(sf::Vector2f(leftX + 50, topMiddleY));

	delete_ent_button.setString("delete");
	delete_ent_button.disable();
	delete_ent_button.setPosition(sf::Vector2f(leftX + 50, bottomMiddleY - 55));

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

// sf::RenderWindow* Interface::getTargetWindow()

void Interface::setViewAnchor(sf::View *_viewAnchor)
{
	viewAnchor = _viewAnchor;
}

// sf::View* Interface::getViewAnchor()

void Interface::Draw()
{
	Update();
	
	targetWindow->draw(topBar);

	create_ent_button.draw(targetWindow);

	targetWindow->draw(unitCounterBackground);
	targetWindow->draw(unitCounterIcon);
	targetWindow->draw(unitCounterText);

	targetWindow->draw(bottomBar);

	delete_ent_button.draw(targetWindow);
}

void Interface::Update()
{
	unitCounterBackground.setSize(sf::Vector2f(unitCounterText.getGlobalBounds().width + 35, unitCounterBackground.getSize().y));
}

// private:

void Interface::reanchor()
{
	/*
	float leftX = viewAnchor->getCenter().x - targetWindow->getSize().x / 2;
	float rightX = 0;
	float topMiddleY = viewAnchor->getCenter().y - targetWindow->getSize().y / 2 + topBar.getSize().y / 2;
	float bottomMiddleY = viewAnchor->getCenter().y + targetWindow->getSize().y / 2 - bottomBar.getSize().y / 2;

	topBar.setPosition(sf::Vector2f(viewAnchor->getCenter().x, topMiddleY));

	create_ent_button.setPosition(sf::Vector2f(leftX + 50, topMiddleY));

	unitCounterBackground.setPosition(sf::Vector2f(leftX + 125, topMiddleY));
	unitCounterIcon.setPosition(sf::Vector2f(unitCounterBackground.getPosition().x - 10, unitCounterBackground.getPosition().y));
	unitCounterText.setPosition(sf::Vector2f(unitCounterBackground.getPosition().x + 10, unitCounterBackground.getPosition().y - 6));

	bottomBar.setPosition(sf::Vector2f(viewAnchor->getCenter().x, bottomMiddleY));

	delete_ent_button.setPosition(sf::Vector2f(leftX + 50, bottomMiddleY - 55));
	*/
}
