#include "GameEndState.hpp"
#include "MainMenuState.hpp"

#include "AppEngine.hpp"

#include "Util/Logger.hpp"
#include "Util/Graphics/DisabledButton.hpp"

enum Callback
{
	RETURN_TO_MAIN_MENU,
};

void GameEndState::Init(AppEngine* app_)
{
	logger::INFO("GameEndState Init");
	app = app_;

	app->dRPC.clearPresence();
	app->dRPC.setState("finishing a game");
	app->dRPC.updatePresence();


	// create a menu 
	menu = new SFUI::Menu(*app->window);
	// set the origin of the menu to 10, 10
	menu->setPosition(sf::Vector2f(10, 10));

	SFUI::FormLayout* form = menu->addFormLayout();

	form->addButton("Exit", RETURN_TO_MAIN_MENU);

	logger::INFO("GameEndState ready.");
}

void GameEndState::Cleanup()
{
	logger::INFO("GameEndState Cleaningup");

	logger::INFO("GameEndState Cleanedup");
}

void GameEndState::Pause()
{
	logger::INFO("GameEndState Paused");
}

void GameEndState::Resume()
{
	logger::INFO("GameEndState Resumed");
}

void GameEndState::HandleEvents()
{
	sf::Event event;
	while (app->window->pollEvent(event))
	{
		if (event.type == sf::Event::EventType::Closed)
			app->Quit();

		int id = menu->onEvent(event);
		switch (id)
		{
		case Callback::RETURN_TO_MAIN_MENU:
		{
			app->ChangeState(new MainMenuState);
			return;
		}
		} // switch
	}
}

void GameEndState::Update()
{
}

void GameEndState::Draw()
{
	app->window->clear(SFUI::Theme::windowBgColor);

	app->window->draw(*menu);

	app->window->display();
}
