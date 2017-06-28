#include "Game.hpp"

#include "ENGINE\Logger.hpp"

#include <iostream>

int main(int argc, char *argv[])
{
	std::cout << "Launched with " << argc << " arguments: " << std::endl;

	for (int i = 0; i < argc; i++)
		std::cout << i << ": " << argv[i] << std::endl;

	bool fullscreen(false);
	bool vsync(false);
	int fpsLimit(60); // default 60

	for (int i = 0; i < argc; i++)
	{
		if (std::string(argv[i]) == "-fullscreen")
			fullscreen = true;

		if (std::string(argv[i]) == "-vsync")
			vsync = true;

//		if (std::string(argv[i]) == "-fps")
//		{
			// next arg should be an int (the fps)
//			fpsLimit == std::stoi(argv[i + 1]);
//		}
	}

	logger::BREAK();

	Game game(fullscreen, vsync);
	game.Main();

	logger::INFO("Exiting...");
	logger::BREAK();

	return EXIT_SUCCESS;
}
