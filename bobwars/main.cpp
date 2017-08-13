#include "GamePlayState.hpp"
#include "MainMenuState.hpp"

#include "StateMachine.hpp"

#include <iostream>

/*
class MainMenuState_ : public AppState2
{
public:
	MainMenuState_(AppEngine2* app_)
	{
		app = app_;

		logoTexture.loadFromFile(".\\resource\\textures\\logo.png");
		logoShape.setSize(sf::Vector2f(logoTexture.getSize().x, logoTexture.getSize().y));
		logoShape.setTexture(&logoTexture);
		logoTexture.setSmooth(true);
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

		std::cout << "title screen created" << std::endl;
	}

	~MainMenuState_()
	{
		delete app;
	}

	void Pause() 
	{

	}

	void Resume() 
	{

	}

	void HandleEvents()
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
					std::cout << "starting new game" << std::endl;

					GamePlayState* game = new GamePlayState(this->app, false, false);
					app->PushState(game);
				}
				else if (mouseIsOver(loadButton.m_shape))
				{
					std::cout << "load button" << std::endl;
				}
				else if (mouseIsOver(settingsButton.m_shape))
				{
					std::cout << "settings button" << std::endl;
				}
				else if (mouseIsOver(exitButton.m_shape))
				{
					std::cout << "exiting" << std::endl;

					app->Quit();
				}
			}
		}
	}

	void Update()
	{

	}

	void Draw()
	{
		app->window->clear(sf::Color(100, 100, 100));

		playButton.draw(*app->window);
		loadButton.draw(*app->window);
		settingsButton.draw(*app->window);
		exitButton.draw(*app->window);
		app->window->draw(logoShape);

		app->window->display();
	}

private:
	bool mouseIsOver(sf::Shape &object)
	{
		if (object.getGlobalBounds().contains(app->window->mapPixelToCoords(sf::Mouse::getPosition(*app->window))))
			return true;
		else
			return false;
	}

	AppEngine2* app;

	sf::RectangleShape logoShape;
	sf::Texture		   logoTexture;

	SFUI::Button playButton;
	SFUI::Button loadButton;
	SFUI::Button settingsButton;
	SFUI::Button exitButton;
};
*/

int main(int argc, char *argv[])
{
	std::cout << "Launched with " << argc << " arguments: " << std::endl;

	for (int i = 0; i < argc; i++)
		std::cout << i << ": " << argv[i] << std::endl;

	LaunchOptions options;

	int fpsLimit(60); // default 60

	for (int i = 0; i < argc; i++)
	{
		if (std::string(argv[i]) == "-fullscreen")
			options.fullscreen = true;

		if (std::string(argv[i]) == "-vsync")
			options.vsync = true;

//		if (std::string(argv[i]) == "-fps")
//		{
			// next arg should be an int (the fps)
//			fpsLimit == std::stoi(argv[i + 1]);
//		}
	}

	logger::BREAK();

	AppEngine2* stateMachine = new AppEngine2("bobwars", options);

	MainMenuState* mainMenu = new MainMenuState(stateMachine);
	stateMachine->ChangeState(mainMenu);

	while (stateMachine->Running())
	{
		stateMachine->HandleEvents();
		stateMachine->Update();
		stateMachine->Draw();
	}

	delete stateMachine;

//	Game bobwars(fullscreen, vsync);
//	bobwars.Main();

	logger::INFO("Exiting...");
	return EXIT_SUCCESS;
}
