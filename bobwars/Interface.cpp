#include "Interface.hpp"

#include <ENGINE/Logger.hpp>

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

	arial.loadFromFile("C://Windows//Fonts//Arial.ttf");

	topBar.setFillColor(sf::Color(100, 100, 100));
	topBar.setOrigin(topBar.getLocalBounds().width / 2, topBar.getLocalBounds().height / 2);
	topBar.setPosition(sf::Vector2f(viewAnchor->getCenter().x, topMiddleY));

	bottomBar.setFillColor(sf::Color(100, 100, 100));
	bottomBar.setOrigin(bottomBar.getLocalBounds().width / 2, bottomBar.getLocalBounds().height / 2);
	bottomBar.setPosition(sf::Vector2f(viewAnchor->getCenter().x, bottomMiddleY));

	float padding = 3.0f; // for the counters

	// gold
	memesCounterBackground.setFillColor(sf::Color(70, 70, 70));
	memesCounterBackground.setSize(sf::Vector2f(46 - padding, 26 - padding));
	memesCounterBackground.setOrigin(sf::Vector2f(memesCounterBackground.getSize().x / 2, memesCounterBackground.getSize().y / 2));
	memesCounterBackground.setOutlineThickness(padding);
	memesCounterBackground.setOutlineColor(sf::Color(50, 50, 50));
	memesCounterBackground.setPosition(sf::Vector2f(leftX + 30, topMiddleY));

	memesCounterIcon_tex.loadFromFile("resource//textures//silk//heart.png");
	memesCounterIcon.setTexture(&memesCounterIcon_tex);
	memesCounterIcon.setSize(sf::Vector2f(16, 16));
	memesCounterIcon.setOrigin(sf::Vector2f(8, 8));
	memesCounterIcon.setPosition(sf::Vector2f(memesCounterBackground.getPosition().x - 10, memesCounterBackground.getPosition().y));

	memesCounterText.setFont(arial);
	memesCounterText.setCharacterSize(22);
	memesCounterText.setString("100");
	memesCounterText.setOrigin(sf::Vector2f(0, memesCounterText.getLocalBounds().height / 2));
	memesCounterText.setPosition(sf::Vector2f(memesCounterBackground.getPosition().x + 4, memesCounterBackground.getPosition().y - 6));

	// unit counter
	unitCounterBackground.setFillColor(sf::Color(70, 70, 70));
	unitCounterBackground.setSize(sf::Vector2f(46 - padding, 26 - padding));
	unitCounterBackground.setOrigin(sf::Vector2f(unitCounterBackground.getSize().x / 2, unitCounterBackground.getSize().y / 2));
	unitCounterBackground.setOutlineThickness(padding);
	unitCounterBackground.setOutlineColor(sf::Color(50, 50, 50));
	unitCounterBackground.setPosition(sf::Vector2f(leftX + 125, topMiddleY));

	unitCounterIcon_tex.loadFromFile("resource//textures//silk//user.png");
	unitCounterIcon.setTexture(&unitCounterIcon_tex);
	unitCounterIcon.setSize(sf::Vector2f(16, 16));
	unitCounterIcon.setOrigin(sf::Vector2f(8, 8));
	unitCounterIcon.setPosition(sf::Vector2f(unitCounterBackground.getPosition().x - 10, unitCounterBackground.getPosition().y));

	unitCounterText.setFont(arial);
	unitCounterText.setCharacterSize(22);
	unitCounterText.setString("0");
	unitCounterText.setOrigin(sf::Vector2f(0, unitCounterText.getLocalBounds().height / 2));
	unitCounterText.setPosition(sf::Vector2f(unitCounterBackground.getPosition().x + 4, unitCounterBackground.getPosition().y - 6));

	create_ent_button.setString("create");
	create_ent_button.setPosition(sf::Vector2f(leftX + 50, bottomMiddleY - 55));

	delete_ent_button.setString("delete");
	delete_ent_button.disable();
	delete_ent_button.setPosition(sf::Vector2f(leftX + 150, bottomMiddleY - 55));

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

void Interface::HandleEvents(sf::Event& event)
{
	/*
	if (engine::logic::mouseIsOver(ui->create_ent_button.m_shape, *app->window, *ui->getViewAnchor())) // create new entity
	{
		callback for createentbutton
	}
	else if (engine::logic::mouseIsOver(ui->delete_ent_button.m_shape, *app->window, *ui->getViewAnchor()) && !entMan->selectedEnts.empty())
	{
		callback for delete ent button
	}
	*/
}

void Interface::Update()
{
	unitCounterBackground.setSize(sf::Vector2f(unitCounterText.getGlobalBounds().width + 32, unitCounterBackground.getSize().y));
	memesCounterBackground.setSize(sf::Vector2f(memesCounterText.getGlobalBounds().width + 32, unitCounterBackground.getSize().y));
}

void Interface::Draw()
{
	Update();
	
	targetWindow->draw(topBar);

	targetWindow->draw(unitCounterBackground);
	targetWindow->draw(unitCounterIcon);
	targetWindow->draw(unitCounterText);

	targetWindow->draw(memesCounterBackground);
	targetWindow->draw(memesCounterIcon);
	targetWindow->draw(memesCounterText);

	targetWindow->draw(bottomBar);

	targetWindow->draw(create_ent_button);
	targetWindow->draw(delete_ent_button);
}
