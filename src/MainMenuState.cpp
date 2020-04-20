#include "GameCreationState.hpp"
#include "MainMenuState.hpp"

#include "Util/Logger.hpp"
#include "Util/Graphics/Text.hpp"

#include "Util/Graphics/DisabledButton.hpp"

// public:

void MainMenuState::Init(AppEngine* app_)
{
	logger::INFO("Initialising MainMenuState");

	app = app_;

	font.loadFromFile("./bobwars/resource/interface/tahoma.ttf");

	bobwars.setFont(font);
	// TODO: 1/100000 chance the title screen will say bobo instead
	bobwars.setString("bobwars");
	bobwars.setCharacterSize(56);

	sf::Texture* backgroundMediaTexture = app->resMan->loadTexture("background_media", "./bobwars/resource/textures/background.png");

	if (backgroundMediaTexture)
	{
		backgroundMediaTexture->setSmooth(true);
		backgroundmedia.setSize(sf::Vector2f(app->window->getSize().y, app->window->getSize().y));
		backgroundmedia.setTexture(backgroundMediaTexture);

		//logoTexture.setSmooth(true);
		backgroundmedia.setOrigin(backgroundmedia.getLocalBounds().width, 0);
		backgroundmedia.setPosition(sf::Vector2f(app->window->getSize().x, 0));
	}
	else
	{
		logger::ERROR("Failed to load title screen logo.");
	}

	menu = new SFUI::Menu(*app->window);
	menu->addButton("New Game", MAIN_MENU_CALLBACKS::PLAY_BUTTON);
	menu->add(new DisabledButton("Load"), MAIN_MENU_CALLBACKS::LOAD_BUTTON);
	menu->add(new DisabledButton("Settings"), MAIN_MENU_CALLBACKS::SETTINGS_BUTTON);
	menu->addButton("Exit", MAIN_MENU_CALLBACKS::EXIT_BUTTON);

	menu->setPosition(sf::Vector2f((app->window->getSize().x / 2 - (menu->getSize().x / 2)) / 4, app->window->getSize().y / 2 - (menu->getSize().y / 2)));

	bobwars.setPosition(menu->getPosition() - sf::Vector2f(50, bobwars.getLocalBounds().width / 2));

	app->dRPC.clearPresence();
	app->dRPC.setState("at Main Menu");
	app->dRPC.updatePresence();

	logger::INFO("MainMenuState ready.");
}

void MainMenuState::Cleanup()
{
	logger::INFO("Cleaning up MainMenuState.");

	app->resMan->freeAllTextures();

	delete menu;

	logger::INFO("Cleaned up MainMenuState.");
}

void MainMenuState::Pause()
{
	logger::INFO("MainMenuState paused.");
}

void MainMenuState::Resume()
{
	logger::INFO("MainMenuState resumed.");
}

void MainMenuState::HandleEvents()
{
	sf::Event event;
	while (app->window->pollEvent(event))
	{
		int id = menu->onEvent(event);
		switch (id)
		{
		case MAIN_MENU_CALLBACKS::PLAY_BUTTON:
			logger::INFO("Starting a new game...");
			// TODO: we should ChangeState and rebuild the menu everytime, to save memory.
			app->ChangeState(new GameCreationState);
			return;
		case MAIN_MENU_CALLBACKS::LOAD_BUTTON:
			logger::WARNING("Saving/Loading system not yet implemented.");
			break;
		case MAIN_MENU_CALLBACKS::SETTINGS_BUTTON:
			logger::WARNING("Settings functions not yet implemented.");
			break;
		case MAIN_MENU_CALLBACKS::EXIT_BUTTON:
			logger::INFO("Exiting game...");
			app->Quit();
			return;
		default:
			break;
		}

		if (event.type == sf::Event::EventType::Closed)
		{
			app->Quit();
			return;
		}
		else if (event.type == sf::Event::EventType::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Key::Tilde)
			{
				app->settings.debug = !app->settings.debug;

				logger::INFO("cl_debug set to " + std::to_string(app->settings.debug));
			}
		}
	}
}

void MainMenuState::Update()
{
	r += dr;
	g += dg;
	b += db;

	if (r == 255 && g == 0 && b == 0)
		dr = 0; dg = 1; db = 0;

	if (r == 255 && g == 255 && b == 0)
		dr = -1; dg = 0; db = 0;

	if (r == 0 && g == 255 && b == 0)
		dr = 0; dg = 0; db = 1;

	if (r == 0 && g == 255 && b == 255)
		dr = 0; dg = -1; db = 0;

	if (r == 0 && g == 0 && b == 255)
		dr = 1; dg = 0; db = 0;

	if (r == 255 && g == 0 && b == 255)
		dr = 0; dg = 0; db = -1;
}

void MainMenuState::Draw()
{
	app->window->clear(sf::Color(64, 64, 64));

	app->window->draw(backgroundmedia);

	app->window->draw(bobwars);

	app->window->draw(*menu);

	if (app->settings.debug)
		util::text::draw(*app->window, "states: " + std::to_string(app->states.size()), sf::Vector2f(0, 0));

	app->window->display();
}

// public:

bool MainMenuState::mouseIsOver(sf::Shape &object)
{
	if (object.getGlobalBounds().contains(app->window->mapPixelToCoords(sf::Mouse::getPosition(*app->window))))
		return true;
	else
		return false;
}
