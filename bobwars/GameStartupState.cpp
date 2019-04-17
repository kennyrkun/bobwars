#include "GameStartupState.hpp"
#include "MainmenuState.hpp"

#include "Util/Logger.hpp"
#include "Util/Graphics/Text.hpp"

#include <SFML/Graphics.hpp>

void GameStartupState::Init(AppEngine* app_)
{
	logger::INFO("Initialising GameStartupState");

	app = app_;

	app->dRPC.clearPresence();
	app->dRPC.setState("starting game");
	app->dRPC.updatePresence();

	ready = true;

	logger::INFO("GameStartupState ready.");
}

void GameStartupState::Cleanup()
{
	logger::INFO("Cleaning up GameStartupState.");

	logger::INFO("Cleaned up GameStartupState.");
}

void GameStartupState::Pause()
{
	logger::INFO("GameStartupState paused.");
}

void GameStartupState::Resume()
{
	logger::INFO("GameStartupState resumed.");
}

void GameStartupState::HandleEvents()
{
	sf::Event event;
	while (app->window->pollEvent(event))
	{
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

void GameStartupState::Update()
{
	if (ready)
	{
		logger::INFO("ready to switch states");

		app->ChangeState(new MainMenuState);
	}
}

void GameStartupState::Draw()
{
	app->window->clear(sf::Color(100, 100, 100));

	if (app->settings.debug)
		util::text::draw(*app->window, "states: " + std::to_string(app->states.size()), sf::Vector2f(0, 0));

	app->window->display();
}

bool GameStartupState::mouseIsOver(sf::Shape &object)
{
	if (object.getGlobalBounds().contains(app->window->mapPixelToCoords(sf::Mouse::getPosition(*app->window))))
		return true;
	else
		return false;
}
