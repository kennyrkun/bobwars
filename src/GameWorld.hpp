#ifndef GAME_WORLD_HPP
#define GAME_WORLD_HPP

#include <SFML/Graphics.hpp>

class GameWorld : public sf::Drawable
{
public:
    void generate();

	void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename);

    const sf::Vector2f& getCenter() const;
	const sf::Vector2f& getSize() const;

	const sf::FloatRect getGlobalBounds() const;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Texture mapTexture;
	sf::RectangleShape map;

	size_t mapSizeX;
	size_t mapSizeY;
};

#endif // !GAME_WORLD_HPP