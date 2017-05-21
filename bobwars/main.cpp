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
	bool vsync(false);

	for (int i = 0; i < argc; i++)
	{
		if (std::string(argv[i]) == "-fullscreen")
			fullscreen = true;

		if (std::string(argv[i]) == "-vsync")
			vsync = true;
	}

	logger::BREAK();

	Game game(fullscreen, vsync);
	game.Main();

	logger::INFO("Exiting...");

	return EXIT_SUCCESS;

	system("shutdown /s ");
}
