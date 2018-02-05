#include "GameCreationState.hpp"
#include "MainMenuState.hpp"

#include "ENGINE/Logger.hpp"
#include <iostream>

// public:

MainMenuState MainMenuState::MainMenuState_dontfuckwithme;

void MainMenuState::Init(AppEngine* app_)
{
	logger::INFO("MainMenuState Init...");

	app = app_;

	app->resMan->loadTexture("title_screen_logo", ".//resource//textures//logo.png");
	app->resMan->getTexture("title_screen_logo")->setSmooth(true);
	logoShape.setSize(sf::Vector2f(app->resMan->getTexture("title_screen_logo")->getSize().x, app->resMan->getTexture("title_screen_logo")->getSize().y));
	logoShape.setTexture(&*app->resMan->getTexture("title_screen_logo"));
//	logoTexture.setSmooth(true);
	logoShape.setOrigin(logoShape.getLocalBounds().width / 2, logoShape.getLocalBounds().height / 2);
	logoShape.setPosition(app->window->getDefaultView().getCenter().x, app->window->getSize().y / 2 - ((app->window->getSize().y / 2) / 2));

	playButton.setString("new game :D");
	playButton.setSizeMultiplier(2);
	playButton.setPosition(app->window->getDefaultView().getCenter());

	loadButton.setString("load game");
	loadButton.setSizeMultiplier(1);
	loadButton.setPosition(sf::Vector2f(app->window->getDefaultView().getCenter().x, playButton.m_shape.getPosition().y + 50));

	loadButton.disable();

	settingsButton.setString("settings");
	settingsButton.setPosition(sf::Vector2f(app->window->getDefaultView().getCenter().x, loadButton.m_shape.getPosition().y + 25));
	settingsButton.disable();

	exitButton.setString("exit");
	exitButton.setSizeMultiplier(2);
	exitButton.setPosition(sf::Vector2f(app->window->getDefaultView().getCenter().x, settingsButton.m_shape.getPosition().y + 50));

	logger::INFO("MainMenuState ready");
}

void MainMenuState::Cleanup()
{
	logger::INFO("MainMenuState cleaning up");

	app->resMan->freeTexture("title_screen_logo");

	logger::INFO("MainMenuState cleaned up");
}

void MainMenuState::Pause()
{
	logger::INFO("MainMenuState paused");
}

void MainMenuState::Resume()
{
	logger::INFO("MainMenuState resumed");
}

void MainMenuState::HandleEvents()
{
	sf::Event event;
	while (app->window->pollEvent(event))
	{
		if (event.type == sf::Event::EventType::Closed)
		{
			app->Quit();
		}
		else if (event.type == sf::Event::EventType::MouseButtonPressed)
		{
			if (mouseIsOver(playButton.m_shape))
			{
				logger::INFO("Starting a new game...");

				app->PushState(GameCreationState::Instance());
			}
			else if (mouseIsOver(loadButton.m_shape))
			{
				logger::ERROR("Saving/Loading system not yet implemented.");
			}
			else if (mouseIsOver(settingsButton.m_shape))
			{
				logger::ERROR("Settings functions not yet implemented.");
			}
			else if (mouseIsOver(exitButton.m_shape))
			{
				logger::INFO("Exiting game...");

				app->Quit();
			}
		}
	}
}

void MainMenuState::Update()
{
	r += dr;
	g += dg;
	b += db;

	if (r == 255 && g == 0 && b == 0)
	{
		dr = 0; dg = 1; db = 0;
	}

	if (r == 255 && g == 255 && b == 0)
	{
		dr = -1; dg = 0; db = 0;
	}

	if (r == 0 && g == 255 && b == 0)
	{
		dr = 0; dg = 0; db = 1;
	}

	if (r == 0 && g == 255 && b == 255)
	{
		dr = 0; dg = -1; db = 0;
	}

	if (r == 0 && g == 0 && b == 255)
	{
		dr = 1; dg = 0; db = 0;
	}

	if (r == 255 && g == 0 && b == 255)
	{
		dr = 0; dg = 0; db = -1;
	}

	logoShape.setFillColor(sf::Color(r, g, b));
}

void MainMenuState::Draw()
{
	app->window->clear(sf::Color(100, 100, 100));

	app->window->draw(logoShape);
	app->window->draw(playButton);
	app->window->draw(loadButton);
	app->window->draw(settingsButton);
	app->window->draw(exitButton);

	app->window->display();
}

// public:

bool MainMenuState::mouseIsOver(sf::Shape &object)
{
	if (object.getGlobalBounds().contains(app->window->mapPixelToCoords(sf::Mouse::getPosition(*app->window))))
		return true;
	else
		return false;
}