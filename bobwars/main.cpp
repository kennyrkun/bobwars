#include "AppEngine.hpp"
#include "MainMenuState.hpp"

#include "Util/Logger.hpp"

#include <iostream>

#include <SFUI/SFUI.hpp>

int main(int argc, char *argv[])
{
	std::cout << "Launched with " << argc << " arguments: " << std::endl;

	for (int i = 0; i < argc; i++)
		std::cout << i << ": " << argv[i] << std::endl;

	SFUI::Theme::loadFont("bobwars/resource/interface/tahoma.ttf");
	SFUI::Theme::loadTexture("bobwars/resource/interface/texture_square.png");
	SFUI::Theme::fontSize = 11;
	SFUI::Theme::click.textColor = SFUI::Theme::hexToRgb("#191B18");
	SFUI::Theme::click.textColorHover = SFUI::Theme::hexToRgb("#191B18");
	SFUI::Theme::click.textColorFocus = SFUI::Theme::hexToRgb("#000000");
	SFUI::Theme::input.textColor = SFUI::Theme::hexToRgb("#000000");
	SFUI::Theme::input.textColorHover = SFUI::Theme::hexToRgb("#CC7A00");
	SFUI::Theme::input.textColorFocus = SFUI::Theme::hexToRgb("#000000");
	SFUI::Theme::windowBgColor = SFUI::Theme::hexToRgb("#dddbde");
	SFUI::Theme::PADDING = 2.f;

	AppSettings options;

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

	logger::LINE_BREAK();

	AppEngine app;
	app.Init("bobwars", options);

	app.PushState(MainMenuState::Instance());

//	AppEngine2* appEngine = new AppEngine2("bobwars", options);

//	MainMenuState* mainMenuState = new MainMenuState(appEngine);
//	appEngine->ChangeState(mainMenuState);

	while (app.Running())
	{
		app.HandleEvents();
		app.Update();
		app.Draw();
	}

	logger::INFO("Exiting...");
	return EXIT_SUCCESS;
}
