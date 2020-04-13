#include "GamePauseState.hpp"
#include "GamePlayState.hpp"
#include "MainMenuState.hpp"

#include "AppEngine.hpp"
#include "AppState.hpp"

#include "Util/Logger.hpp"
#include "Util/Graphics/Text.hpp"
#include "Util/Graphics/DisabledButton.hpp"
#include "Util/Util.hpp"

enum Callbacks
{
	Continue,
	Settings,
	ExitToMainMenu,
};

void GamePauseState::Init(AppEngine* app_)
{
	logger::INFO("Initialising GamePauseState.");

	app = app_;

	backgroundTexture = new sf::Texture;

	sf::Vector2u windowSize = app->window->getSize();
	backgroundTexture->create(windowSize.x, windowSize.y);
	backgroundTexture->update(*app->window); // screenshot, basically

	background.setSize(sf::Vector2f(windowSize.x, windowSize.y));
	background.setTexture(*&backgroundTexture);
	background.setFillColor(sf::Color(255, 255, 255, 155));

	menu = new SFUI::Menu(*app->window);

	menu->addButton("Continue", Callbacks::Continue);
	menu->add(new DisabledButton("Settings"));
	menu->addButton("Exit to Menu", Callbacks::ExitToMainMenu);

	menuShape.setSize(sf::Vector2f(menu->getSize().x + 10, menu->getSize().y + 10));
	menuShape.setPosition(sf::Vector2f((app->window->getSize().x / 2) - (menuShape.getSize().x / 2), (app->window->getSize().y / 2) - (menuShape.getSize().x / 2)));
	menuShape.setFillColor(sf::Color(50, 50, 50));
	menuShape.setOutlineColor(sf::Color(70, 70, 70));
	menuShape.setOutlineThickness(4);

	menu->setPosition(sf::Vector2f(menuShape.getPosition().x + 5, menuShape.getPosition().y + 5));

	logger::INFO("GamePauseState ready.");
}

void GamePauseState::Cleanup()
{
	logger::INFO("Cleaning up GamePauseState.");

	delete backgroundTexture;

	logger::INFO("Cleaned up GamePauseState.");
}

void GamePauseState::Pause()
{
	logger::INFO("GamePauseState paused.");
}

void GamePauseState::Resume()
{
	logger::INFO("GamePauseState resumed.");
}

void GamePauseState::HandleEvents()
{
	sf::Event event;
	while (app->window->pollEvent(event))
	{
		if (event.type == sf::Event::EventType::Closed)
		{
			app->Quit();
		}
		else if (event.type == sf::Event::EventType::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Key::Escape)
			{
				app->PopState();
				return;
			}
		}

		int id = menu->onEvent(event);

		switch (id)
		{
			case Callbacks::Continue:
				app->PopState();
				return;
			case Callbacks::Settings:
				break;
			case Callbacks::ExitToMainMenu:
				app->PopState();
				app->ChangeState(new MainMenuState);
				return;
		}
	}
}

void GamePauseState::Update()
{
}

void GamePauseState::Draw()
{
	app->window->clear();

	app->window->draw(background);

	app->window->draw(menuShape);
	app->window->draw(*menu);

	app->window->display();
}
