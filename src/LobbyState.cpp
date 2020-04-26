#include "LobbyState.hpp"
#include "MainMenuState.hpp"
#include "GamePlayState.hpp"

#include "AppEngine.hpp"
#include "AppState.hpp"
#include "BaseEntity.hpp"
#include "Server.hpp"

#include "Util/Logger.hpp"

void LobbyState::Init(AppEngine* app_)
{
	logger::INFO("LobbyState Init");
	app = app_;

	if (lobbyHost)
	{
		if (app->server != nullptr)
			delete app->server;

		app->server = new DedicatedServer;
		app->server->Start();
		app->server->updateDelay = sf::milliseconds(10);

		if (app->socket != nullptr)
		{
			delete app->socket;
			app->selector.clear();
		}

		logger::INFO("Attempting to connect to local server...");

		app->socket = new sf::TcpSocket;

		if (app->socket->connect(sf::IpAddress::LocalHost, app->server->getBoundPort()) != sf::Socket::Status::Done)
		{
			logger::ERROR("Failed to connect to server");

			delete app->socket;
			app->socket = nullptr;
			app->selector.clear();
			abort();
		}

		app->selector.add(*app->socket);
	}

	menu = new SFUI::Menu(*app->window);

	logger::INFO("LobbyState ready.");
}

void LobbyState::Cleanup()
{
	logger::INFO("LobbyState Cleaningup");

/*
	if (lobbyHost)
		// notify clients the server is shutting down
	else
		// notify server client (me) is disconnecting
*/

	logger::INFO("LobbyState Cleanedup");
}

void LobbyState::Pause()
{
	logger::INFO("LobbyState Paused");
}

void LobbyState::Resume()
{
	logger::INFO("LobbyState Resumed");
}

void LobbyState::HandleEvents()
{
	if (lobbyHost)
		app->server->Update();

	if (app->selector.wait(sf::milliseconds(10)))
	{
		if (app->selector.isReady(*app->socket))
		{
			sf::Packet packet;

			if (app->socket->receive(packet) == sf::Socket::Disconnected)
			{
				logger::ERROR("Disconnected.");
				abort();
			}

			std::string command;
			packet >> command;

			if (command == "LobbyUpdate")
			{
				logger::INFO("Updating networked lobby.");

				LobbyInformation lobby;
				packet >> lobby;

				logger::DEBUG("Recieved lobby information.");

				buildMenu(lobby);
			}
			else
			{
				logger::ERROR("unknown command received from server: " + command);
			}
		}
	}

	sf::Event event;
	while (app->window->pollEvent(event))
	{
		if (event.type == sf::Event::EventType::Closed)
		{
			app->Quit();
			return;
		}

		int id = menu->onEvent(event);
		switch (id)
		{
		case MenuCallbacks::HostStartGame:
		{
			if (allPlayersReady)
			{
				app->ChangeState(new GamePlayState);
				return;
			}
			else
			{
				logger::ERROR("cannot start game until all players are ready.");
			}

			break;
		}
		case MenuCallbacks::LeaveLobby:
		{
			app->ChangeState(new MainMenuState);
			return;
		}
		default:
		{
			if (id != -1)
				logger::INFO("unhandled case for menu callback: " + std::to_string(id));
			break;
		}
		} // switch
	}
}

void LobbyState::Update()
{
}

void LobbyState::Draw()
{
	app->window->clear(sf::Color(64, 64, 64));

	app->window->draw(*menu);

	app->window->display();
}

void LobbyState::buildMenu(const LobbyInformation& information)
{
	if (menu != nullptr)
	{
		delete menu;
		menu = nullptr;
	}

	// create a menu 
	menu = new SFUI::Menu(*app->window);
	// set the origin of the menu to 10, 10
	menu->setPosition(sf::Vector2f(10, 10));

	if (lobbyHost)
	{
		gameNameBox = new SFUI::InputBox;
		gameNameBox->setText(information.gameName);
		menu->add(gameNameBox);
	}
	else
		menu->addLabel(information.gameName);

	SFUI::HorizontalBoxLayout* mainContainer = menu->addHorizontalBoxLayout();

	SFUI::VerticalBoxLayout* playerInformationContainer = mainContainer->addVerticalBoxLayout();
	SFUI::HorizontalBoxLayout* columnsContainer = playerInformationContainer->addHorizontalBoxLayout();
	SFUI::VerticalBoxLayout* nameContainer = columnsContainer->addVerticalBoxLayout();
	SFUI::VerticalBoxLayout* pingContainer = columnsContainer->addVerticalBoxLayout();
	SFUI::VerticalBoxLayout* teamContainer = columnsContainer->addVerticalBoxLayout();
	SFUI::VerticalBoxLayout* colorContainer = columnsContainer->addVerticalBoxLayout();

	nameContainer->addLabel("Name");
	pingContainer->addLabel("Ping");
	teamContainer->addLabel("Team");
	colorContainer->addLabel("Color");

	std::vector<std::pair<std::string, sf::Color>> availableColors = { { "Red", sf::Color::Red }, 
																	   { "Blue", sf::Color::Blue }, 
																	   { "Green", sf::Color::Green }, 
																	   { "Yellow", sf::Color::Yellow },
																	   { "Magenta", sf::Color::Magenta },
																	   { "Black", sf::Color::Black },
																	   { "Cyan", sf::Color::Cyan } };

	logger::INFO("doing spoopy shit");

	for (int i = 0; i < information.totalPlayers; i++)
	{
		logger::DEBUG("loopieloop");

		if (i == playerNumber)
			nameContainer->addLabel("You!");
		else
			nameContainer->addLabel(information.slots[i].name);

		logger::DEBUG("playerNumber");

		if (i == playerNumber)
		{
			SFUI::OptionsBox<Team>* team = new SFUI::OptionsBox<Team>();
			team->addItem("Bob", Team::GoodGuys);
			team->addItem("Google", Team::BadGuys);
			teamContainer->add(team);
		}
		else
		{
			teamContainer->addLabel(information.slots[i].team);
		}

		/*
		SFUI::ItemBox<sf::Text>* ib = new SFUI::ItemBox<sf::Text>();
		ib->item().setFont(SFUI::Theme::getFont());
		ib->item().setCharacterSize(SFUI::Theme::textCharacterSize);
		ib->item().setString("0");
		ib->setSize(SFUI::Theme::minWidgetWidth, SFUI::Theme::getBoxHeight());
		pingContainer->add(ib);
		*/

		pingContainer->addLabel(std::to_string(information.slots[i].ping));

		if (i == playerNumber)
		{
			SFUI::OptionsBox<sf::Color>* opt = new SFUI::OptionsBox<sf::Color>();
			for (size_t i = 0; i < availableColors.size(); i++)
				opt->addItem(availableColors[i].first, availableColors[i].second);
			
			colorContainer->add(opt);
		}
		else
			colorContainer->addLabel(information.slots[i].color);
	}

	logger::DEBUG("Building settings panel");

	SFUI::VerticalBoxLayout* gameSettingsContainer = mainContainer->addVerticalBoxLayout();
	SFUI::HorizontalBoxLayout* firstRow = gameSettingsContainer->addHorizontalBoxLayout();
	firstRow->addLabel("Settings");

	/*
	if (lobbyHost)
		firstRow->addButton("Random", MenuCallbacks::RandomiseLobbySettings);
	*/

	SFUI::FormLayout* settingsForm = gameSettingsContainer->addFormLayout();

	if (lobbyHost)
	{
		typeBox = new SFUI::OptionsBox<std::string>();
		typeBox->addItem("Regular", "regular");
		settingsForm->addRow("Game Type", typeBox, MenuCallbacks::ChangeGameType);

		mapSizeBox = new SFUI::OptionsBox<std::string>();
		mapSizeBox->addItem("Tiny", "tiny");
		mapSizeBox->addItem("Small", "small");
		mapSizeBox->addItem("Regular", "regular");
		mapSizeBox->addItem("Big", "big");
		mapSizeBox->addItem("Huge", "huge");
		mapSizeBox->addItem("Memory Maximum", "memoryMaximum");
		settingsForm->addRow("Map Size", mapSizeBox, MenuCallbacks::ChangeMapSize);

		difficultyBox = new SFUI::OptionsBox<std::string>();
		difficultyBox->addItem("Easy", "easy");
		difficultyBox->addItem("Normal", "normal");
		difficultyBox->addItem("Hard", "hard");
		difficultyBox->addItem("Cheaters", "cheaters");
		settingsForm->addRow("Difficulty", difficultyBox, MenuCallbacks::ChangeDifficulty);

		resourcesBox = new SFUI::OptionsBox<std::string>();
		resourcesBox->addItem("Few", "few");
		resourcesBox->addItem("Normal", "normal");
		resourcesBox->addItem("Lots", "lots");
		settingsForm->addRow("Resources", resourcesBox, MenuCallbacks::ChangeResources);

		populationBox = new SFUI::OptionsBox<std::string>();
		populationBox->addItem("Less", "less");
		populationBox->addItem("Normal", "normal");
		populationBox->addItem("More", "more");
		populationBox->addItem("Memory Maximum", "memoryMaximum");
		settingsForm->addRow("Population", populationBox, MenuCallbacks::ChangePopulation);

		gameSpeedBox = new SFUI::OptionsBox<std::string>();
		gameSpeedBox->addItem("Slow", "slow");
		gameSpeedBox->addItem("Normal", "normal");
		gameSpeedBox->addItem("Fast", "fast");
		settingsForm->addRow("Game Speed", gameSpeedBox, MenuCallbacks::ChangeGameSpeed);

		revealMapBox = new SFUI::OptionsBox<std::string>();
		revealMapBox->addItem("Fog", "fog");
		revealMapBox->addItem("All Revealed", "allRevealed");
		settingsForm->addRow("Reveal Map", revealMapBox, MenuCallbacks::ChangeRevealMap);

		victoryBox = new SFUI::OptionsBox<std::string>();
		victoryBox->addItem("Regular", "regular");
		settingsForm->addRow("Victory", victoryBox, MenuCallbacks::ChangeVictory);
	}
	else
	{
		settingsForm->addLabel("Game Type: ");
		settingsForm->addLabel("Map Size: ");
		settingsForm->addLabel("Difficulty: ");
		settingsForm->addLabel("Resources: ");
		settingsForm->addLabel("Max Population: ");
		settingsForm->addLabel("Game Speed: ");
		settingsForm->addLabel("Reveal Map: ");
		settingsForm->addLabel("Victory: ");
	}
	
	SFUI::HorizontalBoxLayout* settingsTicksContainer = gameSettingsContainer->addHorizontalBoxLayout();
	SFUI::VerticalBoxLayout* firstTickContainer = settingsTicksContainer->addVerticalBoxLayout();
	SFUI::VerticalBoxLayout* secondTickContainer = settingsTicksContainer->addVerticalBoxLayout();

	SFUI::FormLayout* firstTickForm = firstTickContainer->addFormLayout();
	if (lobbyHost)
	{
		firstTickForm->addRow("Team Together", new SFUI::CheckBox(information.teamTogether));
		firstTickForm->addRow("All Techs", new SFUI::CheckBox(information.allTechs));
		firstTickForm->addRow("Allow cheats", new SFUI::CheckBox(information.allowCheats));
	}
	else
	{
		firstTickForm->addLabel("Team Together: false");
		firstTickForm->addLabel("All Techs: false");
		firstTickForm->addLabel("Allow cheats: false");
	}
	
	SFUI::FormLayout* secondTickForm = secondTickContainer->addFormLayout();
	if (lobbyHost)
	{
		secondTickForm->addRow("Lock Teams", new SFUI::CheckBox(information.lockTeams));
		secondTickForm->addRow("Lock Speed", new SFUI::CheckBox(information.lockSpeed));
		secondTickForm->addRow("Record Game", new SFUI::CheckBox(information.recordGame));
	}
	else
	{
		secondTickForm->addLabel("Lock Teams: false");
		secondTickForm->addLabel("Lock Speed: false");
		secondTickForm->addLabel("Record Game: false");
	}

	SFUI::VerticalBoxLayout* chatAndControlsContainer = menu->addVerticalBoxLayout();

	float size = mainContainer->getSize().x;

	chatAndControlsContainer->add(new SFUI::MultilineInputBox(size));
	chatAndControlsContainer->add(new SFUI::InputBox(size));

	SFUI::HorizontalBoxLayout* readyContainer = chatAndControlsContainer->addHorizontalBoxLayout();
	readyContainer->addLabel("Ready");
	readyContainer->add(new SFUI::CheckBox);

	SFUI::HorizontalBoxLayout* gameControlsContainer = chatAndControlsContainer->addHorizontalBoxLayout();

	if (lobbyHost)
		gameControlsContainer->addButton("Start Game", MenuCallbacks::HostStartGame);

	gameControlsContainer->addButton("Cancel", MenuCallbacks::LeaveLobby);
}