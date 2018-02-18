#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Graphics/View.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	float zoomLevel = 2.0;
	int rotation = 0;

	sf::View view;

	void setPosition(sf::Vector2f position);
	void setCenter(sf::Vector2f position);

	sf::Vector2f getPosition();
	sf::Vector2f getCenter();

	void move(sf::Vector2f amount);

	void zoom(float step);

	void rotate(float degrees);
	void setRotation(float degrees);
};

#endif // !CAMERA_HPP
