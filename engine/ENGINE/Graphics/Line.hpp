#ifndef LINE_HPP
#define LINE_HPP

#include <SFML\Graphics.hpp>

class Line
{
public:
	Line();
	~Line();

	void create(const sf::Vector2f& point1, const sf::Vector2f& point2);
	void setPoints(const sf::Vector2f& point1, const sf::Vector2f& point2);
	void setColor(const sf::Color $color);
	void setThickness(const float $thickness);
	void draw(sf::RenderTarget &target);

	sf::Vertex vertices[4];

private:
	sf::Color color;
	float thickness;
};

#endif /* LINE_HPP */
