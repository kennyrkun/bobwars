#include "DedicatedServerState.hpp"
#include "MainMenuState.hpp"

#include "Util/Logger.hpp"
#include "Util/Graphics/Text.hpp"
#include "Server/DedicatedServer.hpp"
#include "Server/ServerLobbyState.hpp"

#include <SFML/Graphics.hpp>

void DedicatedServerState::Init(AppEngine* app_)
{
	logger::INFO("Initialising DedicatedServerState");

	app = app_;

    app->server = new DedicatedServer;
    if (!app->server->Start())
	{
		logger::ERROR("Failed to start server, aborting.");
		abort();
	}

	app->server->changeState(new ServerLobbyState);

	logger::INFO("DedicatedServerState ready.");
}

void DedicatedServerState::Cleanup()
{
	logger::INFO("Cleaning up DedicatedServerState.");

    app->server->Stop();
    delete app->server;
	app->server = nullptr;

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
}

void DedicatedServerState::Update()
{
	// TODO: if server is running
    app->server->Update();
}

void DedicatedServerState::Draw()
{
}
