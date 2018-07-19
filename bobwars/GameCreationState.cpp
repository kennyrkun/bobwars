#include "GameCreationState.hpp"
#include "MainMenuState.hpp"

#include "AppEngine.hpp"
#include "AppState.hpp"

#include "Util/Logger.hpp"
#include "GamePlayState.hpp"

enum Callback
{
	CLIENT_READY,
	START_GAME,
	RETURN_TO_MAIN_MENU,
};

void GameCreationState::Init(AppEngine* app_)
{
	logger::INFO("GameCreationState Init");
	app = app_;

	// create a menu 
	menu = new SFUI::Menu(*app->window);
	// set the origin of the menu to 10, 10
	menu->setPosition(sf::Vector2f(10, 10));

	SFUI::FormLayout* form = menu->addFormLayout();

	// Slider for scale
	SFUI::CheckBox* clientReadyToStartGame = new SFUI::CheckBox();

	form->addRow("ClientReady:", clientReadyToStartGame, CLIENT_READY);
	form->addButton("Start Game", START_GAME);
	menu->addButton("back to Main Menu", RETURN_TO_MAIN_MENU);

	app->dRPC.clearPresence();
	app->dRPC.setState("creating a game");
	app->dRPC.updatePresence();

	logger::INFO("GameCreationState ready.");
}

void GameCreationState::Cleanup()
{
	logger::INFO("GameCreationState Cleaningup");

	logger::INFO("GameCreationState Cleanedup");
}

void GameCreationState::Pause()
{
	logger::INFO("GameCreationState Paused");
}

void GameCreationState::Resume()
{
	logger::INFO("GameCreationState Resumed");
}

void GameCreationState::HandleEvents()
{
	sf::Event event;
	while (app->window->pollEvent(event))
	{
		int id = menu->onEvent(event);
		switch (id)
		{
		case Callback::CLIENT_READY:
		{
			logger::INFO("Client toggled ready.");

			isClientReady = !isClientReady;

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

void GameCreationState::Update()
{
}

void GameCreationState::Draw()
{
	app->window->clear(SFUI::Theme::windowBgColor);

	app->window->draw(*menu);

	app->window->display();
}
