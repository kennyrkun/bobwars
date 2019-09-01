#ifndef SMALL_UNIT_ICON
#define SMALL_UNIT_ICON

#include "../../BaseEntity.hpp"
#include "ProgressBar.hpp"

#include <SFUI/Widget.hpp>
#include <SFML/Graphics.hpp>

class SmallUnitIcon : public SFUI::Widget
{
public:
	SmallUnitIcon(BaseEntity* entity);
	~SmallUnitIcon();

	void update();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	const BaseEntity* entity;

	ProgressBar healthBar;

	sf::RectangleShape red;
	sf::RectangleShape box;
};

#endif // !SMALL_UNIT_ICON
