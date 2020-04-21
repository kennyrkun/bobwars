#include "DedicatedServerState.hpp"
#include "MainMenuState.hpp"

#include "Util/Logger.hpp"
#include "Util/Graphics/Text.hpp"

#include <SFML/Graphics.hpp>

void DedicatedServerState::Init(AppEngine* app_)
{
	logger::INFO("Initialising DedicatedServerState");

	app = app_;

    server = new Server;
    if (!server->Start())
		app->Quit();

	logger::INFO("DedicatedServerState ready.");
}

void DedicatedServerState::Cleanup()
{
	logger::INFO("Cleaning up DedicatedServerState.");

    server->Stop();
    delete server;

	logger::INFO("Cleaned up DedicatedServerState.");
}

void DedicatedServerState::Pause()
{
	logger::INFO("DedicatedServerState paused.");
}

void DedicatedServerState::Resume()
{
	logger::INFO("DedicatedServerState resumed.");
}

void DedicatedServerState::HandleEvents()
{
	sf::Event event;
	while (app->window->pollEvent(event))
	{
		if (event.type == sf::Event::EventType::Closed)
        {
			app->Quit();
            return;
        }
	}
}

void DedicatedServerState::Update()
{
    server->Update();
}

void DedicatedServerState::Draw()
{
	app->window->clear(sf::Color(100, 100, 100));

	if (app->settings.debug)
		util::text::draw(*app->window, "states: " + std::to_string(app->states.size()), sf::Vector2f(0, 0));

	app->window->display();
}
