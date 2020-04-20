#include "Interface.hpp"

#include "Util/Logger.hpp"
#include "Util/Graphics/ProgressBar.hpp"
#include "Util/Graphics/SmallUnitIcon.hpp"

#include <SFUI/Image.hpp>
#include <SFUI/SpriteButton.hpp>
#include <SFUI/Theme.hpp>

enum MENU_CALLBACKS
{
	CREATE_BOB,
	CREATE_BOOMER,
	CREATE_COMMENT_SECTION,
	DELETE_SELECTION,
};

Interface::Interface(sf::RenderWindow *_targetWindow, sf::View *_mainView) : targetWindow(_targetWindow), mainView(_mainView)
{
	viewAnchor = new sf::View(mainView->getCenter(), sf::Vector2f(targetWindow->getSize().x, targetWindow->getSize().y));

	topBar.setSize(sf::Vector2f(targetWindow->getSize().x, 40));
	bottomBar.setSize(sf::Vector2f(targetWindow->getSize().x, 170));

	float leftX = viewAnchor->getCenter().x - targetWindow->getSize().x / 2;
	float rightX = 0;
	float topMiddleY = viewAnchor->getCenter().y - targetWindow->getSize().y / 2 + topBar.getSize().y / 2;
	float bottomMiddleY = viewAnchor->getCenter().y + targetWindow->getSize().y / 2 - bottomBar.getSize().y / 2;

	arial = SFUI::Theme::getFont();

	topBar.setFillColor(sf::Color(100, 100, 100));
	topBar.setPosition(sf::Vector2f(0, 0));

	bottomBar.setFillColor(sf::Color(100, 100, 100));
	bottomBar.setPosition(sf::Vector2f(0, viewAnchor->getSize().y - bottomBar.getSize().y));

	memesCounter = new ResourceCounter("heart.png", { leftX + 8, 8}, 100);
	unitCounter = new ResourceCounter("user.png", { leftX + memesCounter->getGlobalBounds().width + 16, 8}, 100);

	// TODO: use this stuff uwu
	float totalWidth = targetWindow->getSize().x;
	float leftWidth = totalWidth * 20 / 100;
	float middleWidth = totalWidth * 60 / 100;
	float rightWidth = totalWidth * 20 / 100;

	unitActionMenu = new SFUI::Menu(*targetWindow);
	unitActionMenu->setPosition(sf::Vector2f(bottomBar.getPosition().x + SFUI::Theme::MARGIN, bottomBar.getPosition().y + SFUI::Theme::MARGIN));

	unitInformationMenu = new SFUI::Menu(*targetWindow);
	unitInformationMenu->setPosition(sf::Vector2f(bottomBar.getPosition().x + SFUI::Theme::MARGIN, bottomBar.getPosition().y + SFUI::Theme::MARGIN));

	gameInformationMenu = new SFUI::Menu(*targetWindow);
	gameInformationMenu->setPosition(sf::Vector2f(bottomBar.getPosition().x + SFUI::Theme::MARGIN, bottomBar.getPosition().y + SFUI::Theme::MARGIN));

	logger::DEBUG("New interface created.");
}

Interface::~Interface()
{
	delete memesCounter;
	delete unitCounter;

	delete unitActionMenu;
	delete unitInformationMenu;
	delete gameInformationMenu;

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

	targetWindow->draw(*unitActionMenu);
	targetWindow->draw(*unitInformationMenu);
	targetWindow->draw(*gameInformationMenu);

//	targetWindow->draw(*create_ent_button);
//	targetWindow->draw(*delete_ent_button);
}

void Interface::updateSelectionInfo(const std::vector<BaseEntity*>& entities)
{
	logger::DEBUG("[INTERFACE] Updating unit information for " + std::to_string(entities.size()) + " entities.");

	sf::Vector2f pos = unitActionMenu->getPosition();
	delete unitActionMenu;
	unitActionMenu = new SFUI::Menu(*targetWindow);
	unitActionMenu->setPosition(pos);

	pos = unitInformationMenu->getPosition();
	delete unitInformationMenu;
	unitInformationMenu = new SFUI::Menu(*targetWindow);
	unitInformationMenu->setPosition(pos);

	if (entities.empty() || entities.size() <= 0)
		return;

	SFUI::VerticalBoxLayout* actionContainer = unitActionMenu->addVerticalBoxLayout();
	SFUI::HorizontalBoxLayout* topRow = actionContainer->addHorizontalBoxLayout();
	SFUI::HorizontalBoxLayout* middleRow = actionContainer->addHorizontalBoxLayout();
	SFUI::HorizontalBoxLayout* bottomRow = actionContainer->addHorizontalBoxLayout();

	if (entities.size() == 1)
	{
		BaseEntity* entity = entities[0];

		SFUI::HorizontalBoxLayout* mainContainer = unitInformationMenu->addHorizontalBoxLayout();
		SFUI::VerticalBoxLayout* iconContainer = mainContainer->addVerticalBoxLayout();
		SFUI::VerticalBoxLayout* informationContainer = mainContainer->addVerticalBoxLayout();
		SFUI::HorizontalBoxLayout* healthContainer = informationContainer->addHorizontalBoxLayout();
		SFUI::HorizontalBoxLayout* wandContainer = informationContainer->addHorizontalBoxLayout();
		SFUI::HorizontalBoxLayout* shieldContainer = informationContainer->addHorizontalBoxLayout();

		sf::Texture* iconTexture = new sf::Texture;
		iconTexture->loadFromFile("./bobwars/resource/textures/" + entity->type + ".png");
		SFUI::Image* iconImage = new SFUI::Image(*iconTexture);
		iconContainer->add(iconImage);

		sf::Texture* heartTexture = new sf::Texture;
		heartTexture->loadFromFile("./bobwars/resource/textures/silk/heart.png");
		SFUI::Image* heartImage = new SFUI::Image(*heartTexture);
		healthContainer->add(heartImage);
		healthContainer->addLabel(std::to_string(entity->health) + "/" + std::to_string(entity->maxHealth));

		if (entity->hitpoints > 0)
		{
			sf::Texture* wandTexture = new sf::Texture;
			wandTexture->loadFromFile("./bobwars/resource/textures/silk/wand.png");
			SFUI::Image* wandImage = new SFUI::Image(*wandTexture);
			wandContainer->add(wandImage);
			wandContainer->addLabel(std::to_string(entity->hitpoints));
		}

		if (entity->armor > 0)
		{
			sf::Texture* shieldTexture = new sf::Texture;
			shieldTexture->loadFromFile("./bobwars/resource/textures/silk/shield.png");
			SFUI::Image* shieldImage = new SFUI::Image(*shieldTexture);
			shieldContainer->add(shieldImage);
			shieldContainer->addLabel(std::to_string(entity->armor));
		}

		if (entity->type == "bob")
		{
			sf::Texture* buttonTexture = new sf::Texture;
			buttonTexture->loadFromFile("./bobwars/resource/textures/spritebuttons/createcommentsection.png");

			SFUI::SpriteButton* createCommentSectionButton = new SFUI::SpriteButton(*buttonTexture);
			topRow->add(createCommentSectionButton, MENU_CALLBACKS::CREATE_COMMENT_SECTION);
		}
		else if (entity->type == "commentsection")
		{
			sf::Texture* buttonTexture = new sf::Texture;
			buttonTexture->loadFromFile("./bobwars/resource/textures/spritebuttons/createbob.png");
			SFUI::SpriteButton* createBobButton = new SFUI::SpriteButton(*buttonTexture);
			topRow->add(createBobButton, MENU_CALLBACKS::CREATE_BOB);

			sf::Texture* buttonTexture2 = new sf::Texture;
			buttonTexture2->loadFromFile("./bobwars/resource/textures/spritebuttons/createboomer.png");
			SFUI::SpriteButton* createBoomerButton = new SFUI::SpriteButton(*buttonTexture2);
			topRow->add(createBoomerButton, MENU_CALLBACKS::CREATE_BOOMER);
		}
		else
			logger::WARNING("trying to update entity info for unknown entity");

		sf::Texture* emptyTexture = new sf::Texture;
		emptyTexture->loadFromFile("./bobwars/resource/textures/spritebuttons/empty.png");

		for (int i = 0; i < 1; i++)
		{
			SFUI::SpriteButton* emptyButton = new SFUI::SpriteButton(*emptyTexture);
			topRow->add(emptyButton);
		}

		sf::Texture* skullTexture = new sf::Texture;
		skullTexture->loadFromFile("./bobwars/resource/textures/spritebuttons/skull.png");

		SFUI::SpriteButton* deleteSelectionButton = new SFUI::SpriteButton(*skullTexture);
		topRow->add(deleteSelectionButton, MENU_CALLBACKS::DELETE_SELECTION);

		for (int i = 0; i < 4; i++)
		{
			SFUI::SpriteButton* emptyButton = new SFUI::SpriteButton(*emptyTexture);
			middleRow->add(emptyButton);
		}

		for (int i = 0; i < 4; i++)
		{
			SFUI::SpriteButton* emptyButton = new SFUI::SpriteButton(*emptyTexture);
			bottomRow->add(emptyButton);
		}

		sf::Vector2f newPosition;
		newPosition.x = unitActionMenu->getPosition().x + unitActionMenu->getSize().x + SFUI::Theme::MARGIN;
		newPosition.y = unitActionMenu->getPosition().y;
		unitInformationMenu->setPosition(newPosition);
	}
	else // multiple entities
	{
		// TODO: split the bottom interface into three sections
		// Left: shows actions available for the entity
		// Middle: show information about the entity
		// Right: game information, not related the entities

		SFUI::HorizontalBoxLayout* hbox = unitInformationMenu->addHorizontalBoxLayout();

		for (size_t i = 0; i < entities.size(); i++)
		{
			SmallUnitIcon* unitIcon = new SmallUnitIcon(entities[i]);

			// TODO: make this the amount that we can fit on screen without breaking things
			if (i % 28 == 0)
				hbox = unitInformationMenu->addHorizontalBoxLayout();

			hbox->add(unitIcon);
		}

		sf::Texture* emptyTexture = new sf::Texture;
		emptyTexture->loadFromFile("./bobwars/resource/textures/spritebuttons/empty.png");

		for (int i = 0; i < 3; i++)
			topRow->add(new SFUI::SpriteButton(*emptyTexture));

		sf::Texture* skullTexture = new sf::Texture;
		skullTexture->loadFromFile("./bobwars/resource/textures/spritebuttons/skull.png");

		SFUI::SpriteButton* deleteSelectionButton = new SFUI::SpriteButton(*skullTexture);
		topRow->add(deleteSelectionButton, MENU_CALLBACKS::DELETE_SELECTION);

		for (int i = 0; i < 4; i++)
			middleRow->add(new SFUI::SpriteButton(*emptyTexture));

		for (int i = 0; i < 4; i++)
			bottomRow->add(new SFUI::SpriteButton(*emptyTexture));

		sf::Vector2f newPosition;
		newPosition.x = unitActionMenu->getPosition().x + unitActionMenu->getSize().x + SFUI::Theme::MARGIN;
		newPosition.y = unitActionMenu->getPosition().y;
		unitInformationMenu->setPosition(newPosition);
	}

	logger::DEBUG("[INTERFACE] Updated Unit Information.");
}
