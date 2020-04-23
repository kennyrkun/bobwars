#include "AppEngine.hpp"
#include "StartupState.hpp"

#include "Util/Logger.hpp"
#include "Util/DiscordRPC.hpp"

#include <SFUI/Theme.hpp>

int main(int argc, char *argv[])
{
	std::cout << "Launched with " << argc << " arguments: " << std::endl;

	for (int i = 0; i < argc; i++)
		std::cout << i << ": " << argv[i] << std::endl;

	AppSettings options;
	options.launchOptions.argc = argc;
	options.launchOptions.argv = argv;

	AppEngine app;
	app.Init(options);

	app.PushState(new StartupState);

	while (app.isRunning())
	{
		app.HandleEvents();
		app.Update();
		app.Draw();
	}

	logger::INFO("Exiting...");

	return EXIT_SUCCESS;
}
