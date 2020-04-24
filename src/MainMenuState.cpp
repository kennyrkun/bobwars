#include "MainMenuState.hpp"
#include "LobbyState.hpp"

#include "Util/Logger.hpp"
#include "Util/Graphics/Text.hpp"

#include "Util/Graphics/DisabledButton.hpp"

// public:

void MainMenuState::Init(AppEngine* app_)
{
	logger::INFO("Initialising MainMenuState");

	app = app_;

	build(MenuState::Main);

	bobwars.setFont(SFUI::Theme::getFont());
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
		case MenuCallbacks::Multiplayer:
			build(MenuState::Multiplayer);
			break;
		case MenuCallbacks::Settings:
			logger::WARNING("Settings functions not yet implemented.");
			build(MenuState::Settings);
			break;

		case MenuCallbacks::MultiplayerServerConnect:
		{
			if (!tryServerConnect())
				build(MenuState::MultiplayerConnectFailed);
			else
			{
				app->ChangeState(new LobbyState);
				return;
			}
		}
		case MenuCallbacks::MultiplayerServerBack:
			build(MenuState::Multiplayer);
			return;

		case MenuCallbacks::MultiplayerJoinGame:
			build(MenuState::MultiplayerServer);
			return;
		case MenuCallbacks::MultiplayerNewGame:
			app->ChangeState(new LobbyState);
			return;
		case MenuCallbacks::MultiplayerLoadGame:
			//app->ChangeState(new SaveListState(true));
			return;
		case MenuCallbacks::MultiplayerBack:
			build(MenuState::Main);
			return;

		case MenuCallbacks::SettingsBack:
			build(MenuState::Main);
			return;

		case MenuCallbacks::Exit:
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

				build(MenuState::Main);
			}
		}
	}
}

void MainMenuState::Update()
{
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

void MainMenuState::build(const MenuState& state)
{
	menu = new SFUI::Menu(*app->window);

	sf::Vector2u windowSize = app->window->getSize();

	switch (state)
	{
	case MenuState::Main:
	{
		menu->addButton("Multiplayer", MenuCallbacks::Multiplayer);

		if (!app->settings.debug)
			menu->add(new DisabledButton("Settings"), MenuCallbacks::Settings);
		else
			menu->addButton("Settings", MenuCallbacks::Settings);
		
		menu->addButton("Exit", MenuCallbacks::Exit);

		menu->setPosition(sf::Vector2f((windowSize.x / 2 - (menu->getSize().x / 2)) / 4, windowSize.y / 2 - (menu->getSize().y / 2)));
		break;
	}
	case MenuState::MultiplayerServer:
	{
		SFUI::InputBox* addressBox = new SFUI::InputBox;
		SFUI::InputBox* portBox = new SFUI::InputBox;

		SFUI::FormLayout* form = menu->addFormLayout();

		form->addRow("Address: ", addressBox);
		form->addRow("Port: ", portBox);

		SFUI::HorizontalBoxLayout* hbox = form->addHorizontalBoxLayout();

		hbox->addButton("Connect", MenuCallbacks::MultiplayerServerConnect);
		hbox->addButton("Back", MenuCallbacks::MultiplayerServerBack);

		menu->setPosition(sf::Vector2f((windowSize.x / 2 - (menu->getSize().x / 2)) / 8, windowSize.y / 2 - (menu->getSize().y / 2)));
		break;
	}
	case MenuState::Multiplayer:
	{
		menu->addButton("Join Game", MenuCallbacks::MultiplayerJoinGame);
		menu->addButton("Host New Game", MenuCallbacks::MultiplayerNewGame);

		if (!app->settings.debug)
			menu->add(new DisabledButton("Host Saved Game"), MenuCallbacks::MultiplayerLoadGame);
		else
			menu->addButton("Host Saved Game", MenuCallbacks::MultiplayerLoadGame);

		menu->addButton("Back", MenuCallbacks::MultiplayerBack);

		menu->setPosition(sf::Vector2f((windowSize.x / 2 - (menu->getSize().x / 2)) / 4, windowSize.y / 2 - (menu->getSize().y / 2)));
		break;
	}
	case MenuState::Settings:
	{
		menu->addButton("Uwu");
		menu->addButton("Back", MenuCallbacks::SettingsBack);

		menu->setPosition(sf::Vector2f((windowSize.x / 2 - (menu->getSize().x / 2)) / 4, windowSize.y / 2 - (menu->getSize().y / 2)));
		break;
	}
	default:
		break;
	}
}

bool MainMenuState::tryServerConnect()
{
	if (app->server)
		delete app->server;

	sf::IpAddress address = "192.168.0.1";
	unsigned short port = 25565;

	logger::INFO("Attempting to connect to server at " + address.toString() + ":" + std::to_string(port));

//	if (app->server->socket->connect())
//		return true;

	return false;
}