#include "AppEngine.hpp"
#include "AppState.hpp"

#include <iostream>
#include <filesystem>

namespace fs = std::experimental::filesystem;

void AppEngine::Init(std::string title_, AppSettings settings_)
{
	std::cout << "AppEngine Init" << std::endl;

	settings = settings_;
	title = title_;

	if (!fs::exists("./bobwars"))
		fs::create_directory("./bobwars");

	window = new sf::RenderWindow(sf::VideoMode(settings.width, settings.height), settings.title);
	window->setVerticalSyncEnabled(true);

	resMan = new ResourceManager;

	running = true;

	std::cout << "AppEngine ready" << std::endl;
}

void AppEngine::Cleanup()
{
	std::cout << "Cleaning up AppEngine." << std::endl;
	
	// cleanup the all states
	while (!states.empty())
	{
		states.back()->Cleanup();
		states.pop_back();
	}

	window->close();
	delete window;

	resMan->freeAllTextures();

	std::cout << "AppEngine cleaned up." << std::endl;
}

void AppEngine::ChangeState(AppState* state)
{
	if (!states.empty()) 
	{
		states.back()->Cleanup();
		states.pop_back();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->Init(this);
}

void AppEngine::PushState(AppState* state)
{
	// pause current state
	if (!states.empty())
		states.back()->Pause();

	// store and init the new state
	states.push_back(state);
	states.back()->Init(this);
}

void AppEngine::PopState()
{
	// cleanup the current state
	if (!states.empty())
	{
		states.back()->Cleanup();
		states.pop_back();
	}

	// resume previous state
	if (!states.empty())
		states.back()->Resume();
	else
		Quit();
}

void AppEngine::PopState(int amount)
{
	for (size_t i = 0; i < amount; i++)
	{
		// cleanup the current state
		if (!states.empty())
		{
			states.back()->Cleanup();
			states.pop_back();
		}

		// resume previous state
		if (!states.empty())
			states.back()->Resume();
		else
			Quit();
	}
}

void AppEngine::HandleEvents()
{
	if (running)
		states.back()->HandleEvents();
}

void AppEngine::Update()
{
	// let the state update the game
	if (running)
		states.back()->Update();
}

void AppEngine::Draw()
{
	// let the state draw the screen
	if (running)
		states.back()->Draw();
}
