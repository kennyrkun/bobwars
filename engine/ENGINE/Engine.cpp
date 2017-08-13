#include <ENGINE\Engine.hpp>

#include <string>
#include <time.h>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace engine
{
	std::string const version("1.2.1");
	CVar_B cl_debug(0);

	namespace logic
	{
		bool mouseIsOver(sf::Shape &object, sf::RenderWindow &window, sf::View &view)
		{
			if (object.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window), view)))
				return true;
			else
				return false;
		}

		bool mouseIsOver(sf::Sprite &object, sf::RenderWindow &window, sf::View &view)
		{
			if (object.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window), view)))
				return true;
			else
				return false;
		}
	}

	void screenshot(sf::RenderWindow &window)
	{
		sf::Vector2u windowSize = window.getSize();
		sf::Texture texture;

		texture.create(windowSize.x, windowSize.y);
		texture.update(window); // give texture window

		{
			namespace fs = std::experimental::filesystem;

			if (fs::exists("screenshots"))
			{
				int screenshots = 1;

				for (fs::directory_iterator it("screenshots"); it != fs::directory_iterator(); ++it)
				{
					screenshots += 1;
				}

				std::string savePath = "screenshots\\screenshot_" + std::to_string(screenshots) + ".png";

				if (!texture.copyToImage().saveToFile(savePath))
				{
					logger::ERROR("failed to save screenshot.");
				}
				else
				{
					logger::INFO("saved screenshot #" + std::to_string(screenshots));
				}
			}
			else // screenshots folder is a non
			{
				logger::WARNING("screenshots folder does not exist, attempting to create one.");

				if (fs::create_directory("screenshots"))
				{
					logger::INFO("attempting to save screenshot again...");

					screenshot(window);
				}
				else
				{
					logger::ERROR("failed to create screenshots folder, make it yourself");
				}
			}
		}
	}

	std::string getTimestamp()
	{
		time_t $time = time(0);

		int seconds = $time % 60;
		$time /= 60;

		int minutes = $time % 60;
		$time /= 60;

		int hours = $time % 24;
		$time /= 24;

		std::string seconds_s(std::to_string(seconds));
		std::string minutes_s(std::to_string(minutes));
		std::string hours_s(std::to_string(hours));

		if (seconds < 10)
			seconds_s.insert(0, "0");

		if (minutes < 10)
			minutes_s.insert(0, "0");

		if (hours < 10)
			hours_s.insert(0, "0");

		std::string timestamp = hours_s + ":" + minutes_s + ":" + seconds_s;

		return timestamp;
	}
}
