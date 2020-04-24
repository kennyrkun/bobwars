#include "LobbyState.hpp"
#include "MainMenuState.hpp"

#include "AppEngine.hpp"
#include "AppState.hpp"

#include "Util/Logger.hpp"
#include "Util/Graphics/DisabledButton.hpp"
#include "GamePlayState.hpp"

enum Callback
{
	CLIENT_READY_TICK_BOX,
	START_GAME,
	RETURN_TO_MAIN_MENU,
};

void LobbyState::Init(AppEngine* app_)
{
	logger::INFO("LobbyState Init");
	app = app_;

	// create a menu 
	menu = new SFUI::Menu(*app->window);
	// set the origin of the menu to 10, 10
	menu->setPosition(sf::Vector2f(10, 10));

	SFUI::VerticalBoxLayout* mainContainer = menu->addVerticalBoxLayout();
	mainContainer->addLabel("Multiplayer Game");

	SFUI::VerticalBoxLayout* playerInformationContainer = mainContainer->addVerticalBoxLayout();
	SFUI::HorizontalBoxLayout* columnsContainer = playerInformationContainer->addHorizontalBoxLayout();
	SFUI::VerticalBoxLayout* nameContainer = columnsContainer->addVerticalBoxLayout();
	SFUI::VerticalBoxLayout* pingContainer = columnsContainer->addVerticalBoxLayout();
	SFUI::VerticalBoxLayout* teamContainer = columnsContainer->addVerticalBoxLayout();
	SFUI::VerticalBoxLayout* colorContainer = columnsContainer->addVerticalBoxLayout();
	nameContainer->addLabel("Name");

	enum class SlotStatus
	{
		Open,
		Closed,
		AI
	};

	const int maxPlayers = 7;
	nameContainer->addLabel("You!");
	for (size_t i = 0; i < maxPlayers - 1; i++)
	{
		SFUI::OptionsBox<SlotStatus>* opt = new SFUI::OptionsBox<SlotStatus>();
		opt->addItem("Open", SlotStatus::Open);
		opt->addItem("Closed", SlotStatus::Closed);
		opt->addItem("AI", SlotStatus::AI);
		nameContainer->add(opt);
	}
	
	pingContainer->addLabel("Ping");
	teamContainer->addLabel("Team");
	colorContainer->addLabel("Color");

	playerInformationContainer->add(new SFUI::MultilineInputBox);
	playerInformationContainer->add(new SFUI::InputBox);

	SFUI::HorizontalBoxLayout* readyContainer = playerInformationContainer->addHorizontalBoxLayout();
	readyContainer->addLabel("Ready");
	readyContainer->add(new SFUI::CheckBox);

	SFUI::HorizontalBoxLayout* gameControlsContainer = playerInformationContainer->addHorizontalBoxLayout();
	gameControlsContainer->addButton("Start");
	gameControlsContainer->addButton("Cancel");

/*
	SFUI::VerticalBoxLayout* gameSettingsContainer = mainContainer->addVerticalBoxLayout();
	SFUI::HorizontalBoxLayout* settingsTicksContainer = settingsTicksContainer->addHorizontalBoxLayout();
	SFUI::VerticalBoxLayout* firstTickContainer = settingsTicksContainer->addVerticalBoxLayout();
	SFUI::VerticalBoxLayout* secondTickContainer = settingsTicksContainer->addVerticalBoxLayout();
*/

	logger::INFO("LobbyState ready.");
}

void LobbyState::Cleanup()
{
	logger::INFO("LobbyState Cleaningup");

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
		case Callback::CLIENT_READY_TICK_BOX:
		{
			logger::INFO("Ready state for Client 1 switched.");

			isClientReady = !isClientReady;

			if (isClientReady)
				startGameButton->enable();
			else
				startGameButton->disable();

			break;
		}
		case Callback::START_GAME:
		{
			if (isClientReady)
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
		case Callback::RETURN_TO_MAIN_MENU:
		{
			app->ChangeState(new MainMenuState);
			return;
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
