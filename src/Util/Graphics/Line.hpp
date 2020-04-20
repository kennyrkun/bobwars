#ifndef LINE_HPP
#define LINE_HPP

#include <SFML/Graphics.hpp>

class Line
{
public:
	Line();
	~Line();

	void setPoints(const sf::Vector2f& point1, const sf::Vector2f& point2);
	void setColor(const sf::Color& color);
	void setThickness(const float thickness);

	sf::Vertex vertices[4];

private:
	float thickness = 2.0f;
};

#endif /* LINE_HPP */
