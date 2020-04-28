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
		if (!app->server->Start())
		{
			logger::ERROR("Failed to start server.");
			abort();
		}
		app->server->updateDelay = sf::milliseconds(10);

		logger::INFO("Attempting to connect to local server...");

		if (!app->network.connect(sf::IpAddress::LocalHost, app->server->getBoundPort()))
		{
			logger::ERROR("Failed to connect to local server");
			abort();
		}
	}

	menu = new SFUI::Menu(*app->window);

	logger::INFO("LobbyState ready.");
}

void LobbyState::Cleanup()
{
	logger::INFO("Cleaning up LobbyState.");

	app->network.close();

	logger::INFO("Cleaned up LobbyState.");
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

	if (app->network.ready())
	{
		sf::Packet packet;

		if (app->network.receive(packet) == sf::Socket::Disconnected)
		{
			logger::ERROR("Connection to server unexepectedly terminated.");
			app->ChangeState(new MainMenuState);
			return;
		}

		std::string command;
		packet >> command;

		if (command == "ConnectionAccepted")
		{
			packet >> playerNumber;
			logger::INFO("server accepted connection. we are playerID " + std::to_string(playerNumber));
		}
		else if (command == "ConnectionRejected")
		{
			std::string reason;
			packet >> reason;
			
			logger::INFO("server rejected connection: " + reason);

			app->ChangeState(new MainMenuState);
			return;
		}
		else if (command == "Disconnected")
		{
			std::string reason;
			packet >> reason;
			logger::INFO("Disconnected from server: " + reason);

			app->ChangeState(new MainMenuState);
			return;
		}
		else if (command == "LobbyUpdate")
		{
			logger::INFO("Updating lobby...");

			LobbyInformation lobby;
			packet >> lobby;

			buildMenu(lobby);
		}
		else
		{
			logger::ERROR("Unknown command from server: " + command);
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
		// TODO: window resize

		int id = menu->onEvent(event);
		switch (id)
		{
		case MenuCallbacks::HostStartGame:
		{
			sendServerCommand("StartGame");
			//app->ChangeState(new GamePlayState);
			return;
		}
		case MenuCallbacks::LeaveLobby:
		{
			if (lobbyHost)
			{
				app->server->Stop();

				delete app->server;
				app->server = nullptr;
			}
			else
				sendServerCommand("ClientDisconnecting", { std::to_string(playerNumber) });

			app->ChangeState(new MainMenuState);
			return;
		}
		case MenuCallbacks::ChangeLobbyName:
			sendInformationChange(id, gameNameBox->getText().toAnsiString());
			break;
		case MenuCallbacks::ChangeSlotStatus:
			sendInformationChange(id, "");
			break;
		case MenuCallbacks::RandomiseLobbySettings:
			sendInformationChange(id, "");
			break;
		case MenuCallbacks::ChangeGameType:
			sendInformationChange(id, typeBox->getSelectedValue());
			break;
		case MenuCallbacks::ChangeMapSize:
			sendInformationChange(id, mapSizeBox->getSelectedValue());
			break;
		case MenuCallbacks::ChangeDifficulty:
			sendInformationChange(id, difficultyBox->getSelectedValue());
			break;
		case MenuCallbacks::ChangeResources:
			sendInformationChange(id, resourcesBox->getSelectedValue());
			break;
		case MenuCallbacks::ChangePopulation:
			sendInformationChange(id, populationBox->getSelectedValue());
			break;
		case MenuCallbacks::ChangeGameSpeed:
			sendInformationChange(id, gameSpeedBox->getSelectedValue());
			break;
		case MenuCallbacks::ChangeRevealMap:
			sendInformationChange(id, revealMapBox->getSelectedValue());
			break;
		case MenuCallbacks::ChangeVictory:
			sendInformationChange(id, victoryBox->getSelectedValue());
			break;
		case MenuCallbacks::ChangeTeamTogether:
			sendInformationChange(id, teamTogetherBox->isChecked());
			break;
		case MenuCallbacks::ChangeLockTeams:
			sendInformationChange(id, lockTeamsBox->isChecked());
			break;
		case MenuCallbacks::ChangeLockSpeed:
			sendInformationChange(id, lockSpeedBox->isChecked());
			break;
		case MenuCallbacks::ChangeAllTechs:
			sendInformationChange(id, allTechsBox->isChecked());
			break;
		case MenuCallbacks::ChangeAllowCheats:
			sendInformationChange(id, allowCheatsBox->isChecked());
			break;
		case MenuCallbacks::ChangeRecordGame:
			sendInformationChange(id, recordGameBox->isChecked());
			break;
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
		menu->add(gameNameBox, MenuCallbacks::ChangeLobbyName);
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

	for (int i = 0; i < information.slots.size(); i++)
	{
		if (i == playerNumber)
			nameContainer->addLabel("You!");
		else
			nameContainer->addLabel(information.slots[i].name);

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

	SFUI::VerticalBoxLayout* gameSettingsContainer = mainContainer->addVerticalBoxLayout();
	SFUI::HorizontalBoxLayout* firstRow = gameSettingsContainer->addHorizontalBoxLayout();
	firstRow->addLabel("Settings");

	if (lobbyHost)
		firstRow->add(new DisabledButton("Random"), MenuCallbacks::RandomiseLobbySettings);

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
		settingsForm->addLabel("Game Type: " + information.type);
		settingsForm->addLabel("Map Size: "+ information.mapSize);
		settingsForm->addLabel("Difficulty: "+ information.difficulty);
		settingsForm->addLabel("Resources: "+ information.resources);
		settingsForm->addLabel("Max Population: "+ information.population);
		settingsForm->addLabel("Game Speed: "+ information.gameSpeed);
		settingsForm->addLabel("Reveal Map: "+ information.revealMap);
		settingsForm->addLabel("Victory: "+ information.victory);
	}
	
	SFUI::HorizontalBoxLayout* settingsTicksContainer = gameSettingsContainer->addHorizontalBoxLayout();
	SFUI::VerticalBoxLayout* firstTickContainer = settingsTicksContainer->addVerticalBoxLayout();
	SFUI::VerticalBoxLayout* secondTickContainer = settingsTicksContainer->addVerticalBoxLayout();

	SFUI::FormLayout* firstTickForm = firstTickContainer->addFormLayout();
	if (lobbyHost)
	{
		firstTickForm->addRow("Team Together", teamTogetherBox = new SFUI::CheckBox(information.teamTogether), MenuCallbacks::ChangeTeamTogether);
		firstTickForm->addRow("All Techs", allTechsBox = new SFUI::CheckBox(information.allTechs), MenuCallbacks::ChangeAllTechs);
		firstTickForm->addRow("Allow cheats", allowCheatsBox = new SFUI::CheckBox(information.allowCheats), MenuCallbacks::ChangeAllowCheats);
	}
	else
	{
		firstTickForm->addLabel("Team Together: " + std::string(information.teamTogether ? "true" : "false"));
		firstTickForm->addLabel("All Techs: " + std::string(information.allTechs ? "true" : "false"));
		firstTickForm->addLabel("Allow cheats: " + std::string(information.allowCheats ? "true" : "false"));
	}
	
	SFUI::FormLayout* secondTickForm = secondTickContainer->addFormLayout();
	if (lobbyHost)
	{
		secondTickForm->addRow("Lock Teams", lockTeamsBox = new SFUI::CheckBox(information.lockTeams), MenuCallbacks::ChangeLockTeams);
		secondTickForm->addRow("Lock Speed", lockSpeedBox = new SFUI::CheckBox(information.lockSpeed), MenuCallbacks::ChangeLockSpeed);
		secondTickForm->addRow("Record Game", recordGameBox = new SFUI::CheckBox(information.recordGame), MenuCallbacks::ChangeRecordGame);
	}
	else
	{
		secondTickForm->add(lockTeamsLabel = new SFUI::Label("Lock Teams: " + std::string(information.lockTeams ? "true" : "false")));
		secondTickForm->add(lockSpeedLabel = new SFUI::Label("Lock Speed: " + std::string(information.lockSpeed ? "true" : "false")));
		secondTickForm->add(recordGameLabel= new SFUI::Label("Record Game: " + std::string(information.recordGame ? "true" : "false")));
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
		if (information.allPlayersReady)
			gameControlsContainer->addButton("Start Game", MenuCallbacks::HostStartGame);
		else
			gameControlsContainer->add(new DisabledButton("Start Game"), MenuCallbacks::HostStartGame);

	gameControlsContainer->addButton("Cancel", MenuCallbacks::LeaveLobby);
}

/*
bool LobbyState::sendInformationChange(const std::string& command, const std::string& value)
{
	sf::Packet packet;
	packet << "LobbyInformationChange";
	packet << command;
	packet << value;

	if (!app->network.send(packet))
		return false;

	return true;
}
*/

template <typename T>
bool LobbyState::sendInformationChange(const int& id, const T& value)
{
	sf::Packet packet;
	packet << "LobbyInformationChange";
	packet << id;
	packet << value;

	if (!app->network.send(packet))
		return false;

	return true;
}

bool LobbyState::sendServerCommand(const std::string& command, std::vector<std::string> arguments)
{
	sf::Packet packet;
	packet << command;

	for (size_t i = 0; i < arguments.size(); i++)
		packet << arguments[i];

	if (!app->network.send(packet))
		return false;

	return true;
}
