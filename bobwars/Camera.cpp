#include "Camera.hpp"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::setPosition(sf::Vector2f position)
{
	view.setCenter(position);
}

void Camera::setCenter(sf::Vector2f position)
{
	view.setCenter(position);
}

sf::Vector2f Camera::getPosition()
{
	return view.getCenter();
}

sf::Vector2f Camera::getCenter()
{
	return view.getCenter();
}

void Camera::move(sf::Vector2f offset)
{
	view.move(offset);
}

void Camera::zoom(float step)
{
	view.zoom(step);
	zoomLevel *= step;
}

void Camera::rotate(float degrees)
{
	view.rotate(degrees);
}

void Camera::setRotation(float degrees)
{
	view.setRotation(degrees);
}

// zoom
// rotate
//
