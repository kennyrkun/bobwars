#include "Interface.hpp"

#include "Util/Logger.hpp"
#include "Util/Graphics/ProgressBar.hpp"
#include "Util/Graphics/SmallUnitIcon.hpp"

#include <SFUI/Image.hpp>
#include <SFUI/Theme.hpp>

enum MENU_CALLBACKS
{
	CREATE_BOB,
	CREATE_COMMENT_SECTION,
	DELETE_ENTITY,
	DELETE_ALL,
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

	arial = SFUI::Theme::getFont();

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

	logger::DEBUG("New interface created.");
}

Interface::~Interface()
{
	delete memesCounter;
	delete unitCounter;
	delete menu;

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

void Interface::updateSelectionInfo(const std::vector<BaseEntity*>& entities)
{
	logger::DEBUG("[INTERFACE] Updating unit information for " + std::to_string(entities.size()) + " entities.");

	sf::Vector2f pos = menu->getPosition();
	delete menu;
	menu = new SFUI::Menu(*targetWindow);
	menu->setPosition(pos);

	if (entities.empty() || entities.size() <= 0)
		return;

	if (entities.size() == 1)
	{
		BaseEntity* entity = entities[0];

		SFUI::HorizontalBoxLayout *mainContainer = menu->addHorizontalBoxLayout();
		SFUI::VerticalBoxLayout *iconContainer = mainContainer->addVerticalBoxLayout();
		SFUI::VerticalBoxLayout *textContainer = mainContainer->addVerticalBoxLayout();

		// TODO: make this entity-agnostic, so we don't have to write so much identical code
		if (entity->type == "bob")
		{
			// TODO: get this from the resource mananger
			bobIcon.loadFromFile("./bobwars/resource/textures/bob.png");
			SFUI::Image* iconImage = new SFUI::Image(bobIcon);
			iconContainer->add(iconImage);

			mainContainer->addButton("create commentsection", MENU_CALLBACKS::CREATE_COMMENT_SECTION);
		}
		else if (entity->type == "commentsection")
		{
			// TODO: get this from the resource manager
			bobIcon.loadFromFile("./bobwars/resource/textures/commentsection.png");
			SFUI::Image* iconImage = new SFUI::Image(bobIcon);
			iconContainer->add(iconImage);

			mainContainer->addButton("create bob", MENU_CALLBACKS::CREATE_BOB);
		}
		else
		{
			logger::WARNING("trying to update entity info for unknown entity");
		}

		textContainer->addLabel("health: " + std::to_string(entity->health));
		textContainer->addLabel("hitpoints: " + std::to_string(entity->hitpoints));
		textContainer->addLabel("team: " + std::to_string(entity->team));
	}
	else // multiple entities
	{
		// TODO: split the bottom interface into three sections
		// Left: shows actions available for the entity
		// Middle: show information about the entity
		// Right: game information, not related the entities

		SFUI::HorizontalBoxLayout* hbox = menu->addHorizontalBoxLayout();

		for (size_t i = 0; i < entities.size(); i++)
		{
			SmallUnitIcon* unitIcon = new SmallUnitIcon(entities[i]);

			// TODO: make this the amount that we can fit on screen without breaking things
			if (i % 28 == 0)
				hbox = menu->addHorizontalBoxLayout();

			hbox->add(unitIcon);
		}

		// TODO: make this an icon button
		menu->addButton("Delete All", MENU_CALLBACKS::DELETE_ALL);
	}

	logger::DEBUG("[INTERFACE] Updated Unit Information.");
}

