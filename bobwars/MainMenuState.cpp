#include "MainMenuState.hpp"

#include <iostream>

// public:

MainMenuState::MainMenuState(AppEngine2* app_)
{
	app = app_;

	app->resMan->loadTexture("title_screen_logo", ".\\resource\\textures\\logo.png");
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

	logger::INFO("title screen state created");
}

MainMenuState::~MainMenuState()
{
	app->resMan->freeTexture("title_screen_logo");
	delete app;

	std::cout << "menu gone" << std::endl;
}

void MainMenuState::Pause()
{

}

void MainMenuState::Resume()
{

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

				GamePlayState* game = new GamePlayState(this->app, false, false);
				app->ChangeState(game);
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

}

void MainMenuState::Draw()
{
	app->window->clear(sf::Color(100, 100, 100));

	playButton.draw(*app->window);
	loadButton.draw(*app->window);
	settingsButton.draw(*app->window);
	exitButton.draw(*app->window);
	app->window->draw(logoShape);

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