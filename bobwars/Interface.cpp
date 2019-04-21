#include "Interface.hpp"

#include "Util/Logger.hpp"

#include <SFUI\Image.hpp>

enum MENU_CALLBACKS
{
	CREATE_BOB,
	CREATE_COMMENT_SECTION,
	DELETE_ENTITY
};

Interface::Interface(sf::RenderWindow *_targetWindow, sf::View *_mainView) : targetWindow(_targetWindow), mainView(_mainView)
{
	viewAnchor = new sf::View(mainView->getCenter(), sf::Vector2f(targetWindow->getSize().x, targetWindow->getSize().y));

	topBar.setSize(sf::Vector2f(targetWindow->getSize().x, 40));
	bottomBar.setSize(sf::Vector2f(targetWindow->getSize().x, 150));

	float leftX = viewAnchor->getCenter().x - targetWindow->getSize().x / 2;
	float rightX = 0;
	float topMiddleY = viewAnchor->getCenter().y - targetWindow->getSize().y / 2 + topBar.getSize().y / 2;
	float bottomMiddleY = viewAnchor->getCenter().y + targetWindow->getSize().y / 2 - bottomBar.getSize().y / 2;

	arial.loadFromFile("C:/Windows/Fonts/Arial.ttf");

	topBar.setFillColor(sf::Color(100, 100, 100));
	topBar.setOrigin(topBar.getLocalBounds().width / 2, topBar.getLocalBounds().height / 2);
	topBar.setPosition(sf::Vector2f(viewAnchor->getCenter().x, topMiddleY));

	bottomBar.setFillColor(sf::Color(100, 100, 100));
	bottomBar.setOrigin(bottomBar.getLocalBounds().width / 2, bottomBar.getLocalBounds().height / 2);
	bottomBar.setPosition(sf::Vector2f(viewAnchor->getCenter().x, bottomMiddleY));

	memesCounter = new ResourceCounter("heart.png", { leftX + 8, 8}, 100);
	unitCounter = new ResourceCounter("user.png", { leftX + memesCounter->getGlobalBounds().width + 16, 8}, 100);

	menu = new SFUI::Menu(*targetWindow);
	menu->setPosition(sf::Vector2f(leftX + 25, bottomMiddleY - 55));
	menu->addButton("test", 1);

	//create_ent_button = new SFUI::Button("create");
//	create_ent_button.setString("create");
	//create_ent_button->setPosition(sf::Vector2f(leftX + 50, bottomMiddleY - 55));

	//delete_ent_button = new SFUI::Button("delete");
//	delete_ent_button.setString("delete");
//	delete_ent_button.disable();
	//delete_ent_button->setPosition(sf::Vector2f(leftX + 150, bottomMiddleY - 55));

	logger::DEBUG("New interface created.");
}

Interface::~Interface()
{
	delete memesCounter;
	delete unitCounter;

	logger::DEBUG("Interface destroyed.");
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
	if (util::logic::mouseIsOver(ui->create_ent_button.m_shape, *app->window, *ui->getViewAnchor())) // create new entity
	{
		callback for createentbutton
	}
	else if (util::logic::mouseIsOver(ui->delete_ent_button.m_shape, *app->window, *ui->getViewAnchor()) && !entMan->selectedEnts.empty())
	{
		callback for delete ent button
	}
	*/
}

void Interface::Update()
{
}

void Interface::Draw()
{
	Update();
	
	targetWindow->draw(topBar);

	targetWindow->draw(*memesCounter);
	targetWindow->draw(*unitCounter);

	targetWindow->draw(bottomBar);

	targetWindow->draw(*menu);

//	targetWindow->draw(*create_ent_button);
//	targetWindow->draw(*delete_ent_button);
}

void Interface::updateUnitInfo(State state, BaseEntity *entity)
{
	sf::Vector2f pos = menu->getPosition();
	delete menu;
	menu = new SFUI::Menu(*targetWindow);
	menu->setPosition(pos);

	// TODO: this does not work with comment sections
	if (state == State::SingleEntitySelected)
	{
		SFUI::HorizontalBoxLayout *mainContainer = menu->addHorizontalBoxLayout();
		SFUI::VerticalBoxLayout *iconContainer = mainContainer->addVerticalBoxLayout();
		SFUI::VerticalBoxLayout *textContainer = mainContainer->addVerticalBoxLayout();

		// TODO: cache this
		bobIcon.loadFromFile("bobwars/resource/textures/bob.png");
		SFUI::Image* iconImage = new SFUI::Image(bobIcon);
		iconContainer->add(iconImage);

		textContainer->addLabel("health: " + std::to_string(entity->health));
		textContainer->addLabel("hitpoints: " + std::to_string(entity->hitpoints));
		textContainer->addLabel("team: " + std::to_string(entity->team));
	}
	else if (state == State::MultipleEntitiesSelected)
	{
		menu->addButton("delete all");
	}
	else if (state == State::NoEntitiesSelected)
	{
		menu->addButton("create bob", MENU_CALLBACKS::CREATE_BOB);
		menu->addButton("create commentsection", MENU_CALLBACKS::CREATE_COMMENT_SECTION);
	}

	logger::DEBUG("[INTERFACE] Updated Unit Information.");
}
