#include "AppEngine.hpp"
#include "MainMenuState.hpp"

#include "Util/Logger.hpp"

#include <iostream>

#include <SFUI/SFUI.hpp>

#include <DISCORD/discord_rpc.h>

class DiscordRPC
{
public:
	static void handleDiscordReady(const DiscordUser* connectedUser)
	{
		logger::INFO("[DISCORD] Connected!");
		logger::INFO("[DISCORD] User: " + std::string(connectedUser->username) + std::string(connectedUser->discriminator) + " (" + std::string(connectedUser->userId) + ")");
	}

	static void handleDiscordError(int errorCode, const char* message)
	{
		logger::ERROR("[DISCORD] Disconnected: " + std::string(message) + " (" + std::to_string(errorCode) + ")");
	}

	static void handleDiscordDisconnected(int errorCode, const char* message)
	{
		logger::ERROR("[DISCORD] " + std::string(message) + " (" + std::to_string(errorCode) + ")");
	}

	void updateDiscordPresence()
	{
		char buffer[256];

		DiscordRichPresence discordPresence;
		memset(&discordPresence, 0, sizeof(discordPresence));
		discordPresence.state = "in Main Menu";
		//	sprintf(buffer, "Frustration level: %d", FrustrationLevel);
		//	discordPresence.details = "probably beating off";
		//	discordPresence.endTimestamp = time(0) + 5 * 60;
		discordPresence.largeImageKey = "large-bob";
		discordPresence.smallImageKey = "side-google";
		//	discordPresence.partyId = "party1234";
		//	discordPresence.partySize = 1;
		//	discordPresence.partyMax = 6;
		//	discordPresence.instance = 0;

		Discord_UpdatePresence(&discordPresence);
	}

	void InitDiscord()
	{
		DiscordEventHandlers handlers;
		memset(&handlers, 0, sizeof(handlers));

		handlers.ready = &this->handleDiscordReady;
		handlers.errored = &this->handleDiscordError;
		handlers.disconnected = &this->handleDiscordDisconnected;
		//	handlers.joinGame = handleDiscordJoinGame;
		//	handlers.spectateGame = handleDiscordSpectateGame;
		//	handlers.joinRequest = handleDiscordJoinRequest;

		// Discord_Initialize(const char* applicationId, DiscordEventHandlers* handlers, int autoRegister, const char* optionalSteamId)
		Discord_Initialize("468957546359685121", &handlers, false, nullptr);
	}
};


int main(int argc, char *argv[])
{
	std::cout << "Launched with " << argc << " arguments: " << std::endl;

	for (int i = 0; i < argc; i++)
		std::cout << i << ": " << argv[i] << std::endl;

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
	dRPC.InitDiscord();
	dRPC.updateDiscordPresence();

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

//		Discord_UpdateConnection();
		Discord_RunCallbacks();
	}

	Discord_Shutdown();

	logger::INFO("Exiting...");
	return EXIT_SUCCESS;
}
