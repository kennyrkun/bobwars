#include <string>
#include <time.h>
#include <iostream>
#include <fstream>

#include <SFML\Graphics.hpp>

sf::Text text_default;

namespace engine
{
	std::string build("04");

	void draw_text(sf::RenderWindow &window, sf::Text &text, const std::string string, sf::Vector2f &pos, const sf::Vector2f &scale)
	{
		text.setPosition(pos);
		text.setScale(scale);
		text.setString(string);

		window.draw(text);
	}

	void draw_text(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos)
	{
		text.setPosition(pos);
		text.setCharacterSize(34);
		text.setString(string);

		window.draw(text);
	}

	std::string getTime()
	{
		time_t $time = time(0);

		int seconds = $time % 60;
		$time /= 60;

		int minutes = $time % 60;
		$time /= 60;

		int hours = $time % 24; // Oklahoma timezone.
		$time /= 24;

		std::string seconds_s;
		std::string minutes_s;
		std::string hours_s(std::to_string(hours));

		if (seconds < 10)
		{
			for (int i = 0; i < 10; i++)
			{
				seconds_s = "0" + std::to_string(seconds);
			}
		}
		else
		{
			seconds_s = std::to_string(seconds);
		}

		if (minutes < 10)
		{
			for (int i = 0; i < 10; i++)
			{
				minutes_s = "0" + std::to_string(minutes);
			}
		}
		else
		{
			minutes_s = std::to_string(minutes);
		}

		if (hours < 10)
		{
			for (int i = 0; i < 10; i++)
			{
				hours_s = "0" + std::to_string(hours);
			}
		}
		else
		{
			hours_s = std::to_string(hours);
		}

		std::string time = hours_s + ":" + minutes_s + ":" + seconds_s;

		return time;
	}

	std::string getObjectCoords(sf::Sprite &object)
	{
		std::string coords = "X: " + std::to_string(static_cast<int>(object.getPosition().x)) + " Y: " + std::to_string(static_cast<int>(object.getPosition().y));

		return coords;
	}
}
