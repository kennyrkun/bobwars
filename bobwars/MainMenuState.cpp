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

	app->resMan->print();

	if (!app->resMan->textureLoaded("title_screen_logo"))
		app->resMan->loadTexture("title_screen_logo", "./bobwars/resource/textures/logo.png");

	app->resMan->getTexture("title_screen_logo")->setSmooth(true);
	logoShape.setSize(sf::Vector2f(app->resMan->getTexture("title_screen_logo")->getSize().x, app->resMan->getTexture("title_screen_logo")->getSize().y));
	logoShape.setTexture(&*app->resMan->getTexture("title_screen_logo"));

	//logoTexture.setSmooth(true);
	logoShape.setOrigin(logoShape.getLocalBounds().width / 2, logoShape.getLocalBounds().height / 2);
	logoShape.setPosition(sf::Vector2f(app->window->getDefaultView().getCenter().x, app->window->getSize().y / 2 - ((app->window->getSize().y / 2) / 2)));

	menu = new SFUI::Menu(*app->window);
	menu->addButton("New Game", MAIN_MENU_CALLBACKS::PLAY_BUTTON);
	menu->add(new DisabledButton("Load"), MAIN_MENU_CALLBACKS::LOAD_BUTTON);
	menu->add(new DisabledButton("Settings"), MAIN_MENU_CALLBACKS::SETTINGS_BUTTON);
	menu->addButton("Exit", MAIN_MENU_CALLBACKS::EXIT_BUTTON);

	menu->setPosition(app->window->getDefaultView().getCenter());
	menu->setPosition(sf::Vector2f(app->window->getSize().x / 2 - (menu->getSize().x / 2), app->window->getSize().y / 2 - (menu->getSize().y / 2)));

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
		default:
			break;
		}

		if (event.type == sf::Event::EventType::Closed)
			app->Quit();
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

	logoShape.setFillColor(sf::Color(r, g, b));
}

void MainMenuState::Draw()
{
	app->window->clear(sf::Color(100, 100, 100));

	app->window->draw(logoShape);
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
