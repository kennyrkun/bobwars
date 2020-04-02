#include "GamePauseState.hpp"
#include "MainMenuState.hpp"

#include "AppEngine.hpp"
#include "AppState.hpp"

#include "Util/Logger.hpp"
#include "Util/Graphics/Text.hpp"
#include "Util/Util.hpp"
#include "GamePlayState.hpp"

void GamePauseState::Init(AppEngine* app_)
{
	logger::INFO("Initialising GamePauseState.");

	app = app_;

	backgroundTexture = new sf::Texture;

	sf::Vector2u windowSize = app->window->getSize();
	backgroundTexture->create(windowSize.x, windowSize.y);
	backgroundTexture->update(*app->window); // give texture window

	background.setSize(sf::Vector2f(windowSize.x, windowSize.y));
	background.setTexture(*&backgroundTexture);
	background.setFillColor(sf::Color(255, 255, 255, 155));

	std::vector<std::pair<std::string, int>> options = { {"Continue", 0},
//															{"Save Game", 1},
//															{"Options", 2},
															{"Exit to Menu", 3} };

	pauseMenu = new Menu(app->window, "Pause", options);

	logger::INFO("GamePauseState ready.");
}

void GamePauseState::Cleanup()
{
	logger::INFO("Cleaning up GamePauseState.");

	delete pauseMenu;
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

		pauseMenu->HandleEvents(event);
	}
}

void GamePauseState::Update()
{
	if (pauseMenu->done)
	{
		// continue
		// save
		// options
		// exit

		if (pauseMenu->selectedOption == 0)
		{
			// FIXME: should we return after this to avoid access violations?
			app->PopState();
			return;
		}
		else if (pauseMenu->selectedOption == 1)
		{
			// nothing

			pauseMenu->done = false; // not done, because this option is not real.
		}
		else if (pauseMenu->selectedOption == 2)
		{
			// nothing 

			pauseMenu->done = false; // not done, because this option is not real.
		}
		else if (pauseMenu->selectedOption == 3)
		{
			// FIXME: should we return after this to avoid access violations?
			app->PopState();
			app->ChangeState(new MainMenuState);
			return;
		}
		else
		{
			logger::ERROR("Wrong option ID was returned (" + std::to_string(pauseMenu->selectedOption) + ")");
		}
	}
}

void GamePauseState::Draw()
{
	app->window->clear();

	app->window->draw(background);

	app->window->draw(*pauseMenu);

	app->window->display();
}
