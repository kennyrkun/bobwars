#include "Game.hpp"

#include "ENGINE\Logger.hpp"

#include <iostream>

int main(int argc, char *argv[])
{
	std::cout << "Launched with " << argc << " arguments: " << std::endl;

	for (int i = 0; i < argc; i++) // TODO: parse arguments
	{
		std::cout << i << ": " << argv[i] << std::endl;
	}
	bool fullscreen(false);
	for (int i = 0; i < argc; i++)
	{
		if (std::string(argv[i]) == "-fullscreen")
		{
			logger::INFO("fullscreen");

			fullscreen = true;
		}

		if (std::string(argv[i]) == "-debug")
		{
//			engine::cl_debug = true;
		}
	}

	logger::BREAK();

	Game game(fullscreen);
	game.Main();

	logger::INFO("Exiting...");

	return EXIT_SUCCESS;

	system("shutdown /s ");
}
