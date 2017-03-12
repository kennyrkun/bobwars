#include <ENGINE\engine_main.hpp>

#include <string>
#include <time.h>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace engine
{
	std::string const version("1.0.8");
	CVar_B cl_debug(1);
	CVar_I cl_debugoutput(2);

	namespace text
	{
		void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const int characterSize, const sf::Color &c)
		{
			text.setPosition(pos);
			text.setCharacterSize(characterSize);
			text.setFillColor(c);
			text.setString(string);

			window.draw(text);

			text.setFillColor(sf::Color::White);
		}

		void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const int characterSize)
		{
			text.setPosition(pos);
			text.setCharacterSize(characterSize);
			text.setString(string);

			window.draw(text);
		}

		void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos)
		{
			text.setPosition(pos);
			text.setCharacterSize(34);
			text.setString(string);

			window.draw(text);
		}
	}

	namespace graphics
	{
		void outline(sf::RenderWindow &window, sf::Shape &object, float thickness, sf::Color color)
		{
			sf::RectangleShape line;

			line.setPosition(object.getPosition());
			line.setSize(sf::Vector2f(object.getLocalBounds().width, object.getLocalBounds().height));
			line.setFillColor(sf::Color(0, 0, 0, 0));

			line.setOutlineColor(color);
			line.setOutlineThickness(thickness);

			window.draw(line);
		}

		void outline(sf::RenderWindow &window, sf::Sprite &object, float thickness, sf::Color color)
		{
			sf::RectangleShape line;

			line.setPosition(object.getPosition());
			line.setSize(sf::Vector2f(object.getLocalBounds().width, object.getLocalBounds().height));
			line.setFillColor(sf::Color(0, 0, 0, 0));

			line.setOutlineColor(color);
			line.setOutlineThickness(thickness);

			window.draw(line);
		}

		void fade(sf::Shape &object, int opacity) // TODO: run this in another thread
		{
			logger::DEBUG("fading to " + std::to_string(opacity));

			if (opacity < object.getFillColor().a) // 255 0
			{
				object.setFillColor(sf::Color(object.getFillColor().r, object.getFillColor().g, object.getFillColor().b, object.getFillColor().a - 1));
			}
			else if (opacity > object.getFillColor().a)
			{
				object.setFillColor(sf::Color(object.getFillColor().r, object.getFillColor().g, object.getFillColor().b, object.getFillColor().a + 1));
			}
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
				int screenshots = 0;

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


	std::string getTime()
	{
		time_t $time = time(0);

		int seconds = $time % 60;
		$time /= 60;

		int minutes = $time % 60;
		$time /= 60;

		int hours = $time % 24;
		$time /= 24;

		std::string seconds_s;
		std::string minutes_s;
		std::string hours_s(std::to_string(hours));

		if (seconds < 10)
			seconds_s = "0" + std::to_string(seconds);
		else
			seconds_s = std::to_string(seconds);

		if (minutes < 10)
			minutes_s = "0" + std::to_string(minutes);
		else
			minutes_s = std::to_string(minutes);

		if (hours < 10)
				hours_s = "0" + std::to_string(hours);
		else
			hours_s = std::to_string(hours);

		std::string time = hours_s + ":" + minutes_s + ":" + seconds_s;

		return time;
	}
}
