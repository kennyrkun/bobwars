#include "StartupState.hpp"
#include "DedicatedServerState.hpp"
#include "MainMenuState.hpp"

#include "Util/Logger.hpp"
#include "Util/Graphics/Text.hpp"

#include <SFML/Graphics.hpp>

void StartupState::Init(AppEngine* app_)
{
	logger::INFO("Initialising StartupState");

	app = app_;

	app->dRPC.clearPresence();
	app->dRPC.setState("starting game");
	app->dRPC.updatePresence();

	ready = true;

	logger::INFO("StartupState ready.");
}

void StartupState::Cleanup()
{
	logger::INFO("Cleaning up StartupState.");

	logger::INFO("Cleaned up StartupState.");
}

void StartupState::Pause()
{
	logger::INFO("StartupState paused.");
}

void StartupState::Resume()
{
	logger::INFO("StartupState resumed.");
}

void StartupState::HandleEvents()
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

void StartupState::Update()
{
	if (ready)
	{
		logger::INFO("ready to switch states");

		for (size_t i = 0; i < app->settings.launchOptions.argc; i++)
		{
			if (std::string(app->settings.launchOptions.argv[i]) == "-server")
			{
				app->ChangeState(new DedicatedServerState);
				return;
			}
		}
		
		app->ChangeState(new MainMenuState);
		return;
	}
}

void StartupState::Draw()
{
	app->window->clear(sf::Color(100, 100, 100));

	if (app->settings.debug)
		util::text::draw(*app->window, "states: " + std::to_string(app->states.size()), sf::Vector2f(0, 0));

	app->window->display();
}
