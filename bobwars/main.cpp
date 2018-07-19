#include "AppEngine.hpp"
#include "MainMenuState.hpp"
#include "Util/Logger.hpp"
#include "Util/DiscordRPC.hpp"

#include <SFUI/Theme.hpp>

int main(int argc, char *argv[])
{
	std::cout << "Launched with " << argc << " arguments: " << std::endl;

	for (int i = 0; i < argc; i++)
		std::cout << i << ": " << argv[i] << std::endl;

	AppSettings options;

	/*
	for (int i = 0; i < argc; i++)
	{
		if (std::string(argv[i]) == "-fullscreen")
			options.fullscreen = true;

		if (std::string(argv[i]) == "-vsync")
			options.vsync = true;

		if (std::string(argv[i]) == "-maxfps")
		{
			// next arg should be an int (the maxfps)
			options.maxfps = std::stoi(argv[i += 1]);
		}
	}
	*/

	logger::INFO("Preparing SFUI");
	SFUI::Theme::loadFont("bobwars/resource/interface/tahoma.ttf");
	SFUI::Theme::loadTexture("bobwars/resource/interface/texture_square.png");
	SFUI::Theme::textCharacterSize = 11;
	SFUI::Theme::click.textColor = SFUI::Theme::hexToRgb("#191B18");
	SFUI::Theme::click.textColorHover = SFUI::Theme::hexToRgb("#191B18");
	SFUI::Theme::click.textColorFocus = SFUI::Theme::hexToRgb("#000000");
	SFUI::Theme::input.textColor = SFUI::Theme::hexToRgb("#000000");
	SFUI::Theme::input.textColorHover = SFUI::Theme::hexToRgb("#CC7A00");
	SFUI::Theme::input.textColorFocus = SFUI::Theme::hexToRgb("#000000");
	SFUI::Theme::windowBgColor = SFUI::Theme::hexToRgb("#dddbde");
	SFUI::Theme::PADDING = 2.f;

	DiscordRPC dRPC;
	dRPC.Init();

	dRPC.setState("at Main Menu");
	dRPC.updatePresence();

	AppEngine app;
	app.Init("bobwars", options);

	app.PushState(MainMenuState::Instance());

//	AppEngine2* appEngine = new AppEngine2("bobwars", options);

//	MainMenuState* mainMenuState = new MainMenuState(appEngine);
//	appEngine->ChangeState(mainMenuState);

	while (app.isRunning())
	{
		app.HandleEvents();
		app.Update();
		app.Draw();

		dRPC.update();
	}

	dRPC.Shutdown();

	logger::INFO("Exiting...");
	return EXIT_SUCCESS;
}
