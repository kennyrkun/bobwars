#include "Graphics.hpp"
#include "..\Logger.hpp"

namespace engine {
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
		logger::SILENT("DEBUG", "fading to " + std::to_string(opacity));

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
}