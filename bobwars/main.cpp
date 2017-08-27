#include "GamePlayState.hpp"
#include "MainMenuState.hpp"
#include "StateMachine.hpp"

#include <iostream>

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

	AppEngine2* appEngine = new AppEngine2("bobwars", options);

	MainMenuState* mainMenuState = new MainMenuState(appEngine);
	appEngine->ChangeState(mainMenuState);

	while (appEngine->Running())
	{
		appEngine->HandleEvents();
		appEngine->Update();
		appEngine->Draw();
	}

	delete appEngine;

	logger::INFO("Exiting...");
	return EXIT_SUCCESS;
}
