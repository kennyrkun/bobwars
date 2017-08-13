#include "StateMachine.hpp"
#include "StateBase.hpp"

#include <ENGINE\Logger.hpp>

#include <iostream>

AppEngine2::AppEngine2(std::string title, LaunchOptions launchOptions_)
{
	logger::INFO("initalising StateMachine");

	options = launchOptions_;

	window = new sf::RenderWindow(sf::VideoMode(options.width, options.height), title);
	window->setVerticalSyncEnabled(true);

	running = true;
}

AppEngine2::~AppEngine2()
{
	logger::INFO("deleting AppEngine");

	delete window;
}

void AppEngine2::ChangeState(AppState2* state)
{
	logger::INFO("switching states");

	// cleanup the current state
	if (!states.empty())
	{
		states.pop_back();
	}

	// store and init the new state
	states.push_back(state);

	logger::INFO("there are " + std::to_string(states.size()) + " states");
}

void AppEngine2::PushState(AppState2* state)
{
	logger::INFO("pushing state to stack");

	// pause current state
	if (!states.empty())
	{
		logger::INFO("pausing last state");

		states.back()->Pause();
	}

	// store and init the new state
	states.push_back(state);

	logger::INFO("there are " + std::to_string(states.size()) + " states");
}

void AppEngine2::PopState()
{
	logger::INFO("popping back state");

	// cleanup the current state
	if (!states.empty())
	{
		delete states.back();
		states.pop_back();
	}

	// resume previous state
	if (!states.empty())
	{
		logger::INFO("resuming previous state");

		states.back()->Resume();
	}
	else // no more states
	{
		logger::INFO("no more states, exiting.");

		Quit();
	}

	logger::INFO("there are " + std::to_string(states.size()) + " states");
}

void AppEngine2::HandleEvents()
{
	if (running)
	{
		// let the state handle events
		states.back()->HandleEvents();
	}
}

void AppEngine2::Update()
{
	if (running)
	{
		// let the state update the app
		states.back()->Update();
	}
}

void AppEngine2::Draw()
{
	if (running)
	{
		// let the state draw the screen
		states.back()->Draw();
	}
}

void AppEngine2::Quit()
{
	running = false;

	logger::INFO("Quitting");
}
