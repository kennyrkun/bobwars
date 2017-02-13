#include <ENGINE\engine_main.hpp>

#include "BaseEntity.hpp"
#include "BaseUnit.hpp"
#include "BaseBuilding.hpp"

#include <iostream>

sf::Font font;
sf::Text text;
sf::Text framecounter;
sf::Texture player_tex;
sf::RectangleShape world;
sf::Texture world_tex;

BaseUnit unit;

static float view_speed = .1f;
static float player_speed = .05f;

void fade(sf::Shape &object, int opacity)
{
	if (object.getFillColor().a > opacity) // 255 0
	{
		object.setFillColor(sf::Color(object.getFillColor().r, object.getFillColor().g, object.getFillColor().b, object.getFillColor().a - 1));
	}
}

void show_coords(sf::RenderWindow &window, sf::Sprite &object)
{
	std::string coords = "X: " + 
						std::to_string(static_cast<int>(object.getPosition().x)) + 
						" Y: " + 
						std::to_string(static_cast<int>(object.getPosition().y));

	sf::Vector2f position(object.getPosition().x + 15, object.getPosition().y - 5);

	engine::draw_text(window, text, coords, position);
}

void move_sprite(sf::Sprite &sprite, int x, int y)
{
	while ((sprite.getPosition().x != x) && (sprite.getPosition().y != y))
	{
		sprite.move(1, 0);
	}
}

void draw(sf::RenderWindow &window, sf::View &view)
{
	window.clear();

	// draw the main view
	window.setView(view);
	//world
	window.draw(world);
	window.draw(unit.m_sprite);

	//gui
	show_coords(window, unit.m_sprite);
	engine::draw_text(window, text, ("X: " + std::to_string(static_cast<int>(view.getCenter().x)) + " Y: " + std::to_string(static_cast<int>(view.getCenter().y))), sf::Vector2f(view.getCenter().x - 199, view.getCenter().y - 150)); // should I get the coordinates somewhere else and only draw here? -Kenny
	engine::draw_text(window, text, std::to_string(unit.m_health), sf::Vector2f(unit.m_sprite.getPosition().x, unit.m_sprite.getPosition().y));

	window.draw(framecounter);
	window.display();
}

void gui_load()
{
	logger::INFO("preparing ui elements...");

	if (!font.loadFromFile("resource\\fonts\\arial.ttf"))
	{
		std::cout << "unable to load font 'arial'." << std::endl;
	}
	else
	{
		text.setFont(font);
		text.setScale(sf::Vector2f(.2f, .2f));
	}

	logger::INFO("world texture...");
	if (!world_tex.loadFromFile("resource\\textures\\world.png"))
		std::cout << "unable to load world textures!" << std::endl;

	logger::INFO("player texture...");
	if (!player_tex.loadFromFile("resource\\textures\\player.png"))
		std::cout << "unable to load player textures!" << std::endl;

	world.setSize(sf::Vector2f(800, 600));
	world.setTexture(&world_tex);
	unit.m_sprite.setTexture(player_tex);

	framecounter.setFont(font);
	framecounter.setScale(sf::Vector2f(.2f, .2f));

	logger::INFO("done!");
}

int main(int argc, char *argv[])
{	
	{
		std::cout << "launched with " << argc << " arguments: " << std::endl;

		for (int i = 0; i < argc; i++)
		{
			std::cout << i << ": " << argv[i] << std::endl;
		}

		std::cout << std::endl;

//TODO: parseArgs(argv[]); 
	}

	logger::INFO("initializing");

	sf::RenderWindow window(sf::VideoMode(800, 600), ("bobwars 0.0.1:" + engine::build), sf::Style::Titlebar | sf::Style::Close);
	sf::View main_view(sf::Vector2f(400, 300), sf::Vector2f(400, 300));

	gui_load();

	sf::Clock clock;
	float lastTime = 0; // for fps

	// game loop
	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
					logger::INFO("pause function not yet implemented.");

				if (event.key.code == sf::Keyboard::LShift)
				{
					view_speed = .05f;
				}
			}

			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::LShift)
				{
					view_speed = .1f;
				}
			}
		}

		if ((window.hasFocus()) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LSystem) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RSystem)))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				main_view.move(0, -view_speed);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				main_view.move(-view_speed, 0);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				main_view.move(0, view_speed);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				main_view.move(view_speed, 0);

			{
				if (main_view.getCenter().x > 800)
					main_view.setCenter(800, main_view.getCenter().y);
				if (main_view.getCenter().y > 600)
					main_view.setCenter(main_view.getCenter().x, 600);
				if (main_view.getCenter().x < 0)
					main_view.setCenter(0, main_view.getCenter().y);
				if (main_view.getCenter().y < 0)
					main_view.setCenter(main_view.getCenter().x, 0);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				unit.m_sprite.move(0, -player_speed);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				unit.m_sprite.move(-player_speed, 0);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				unit.m_sprite.move(0, player_speed);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				unit.m_sprite.move(player_speed, 0);

			if (!unit.m_sprite.getGlobalBounds().intersects(world.getGlobalBounds()))
			{
				static int times_logged_out_of_bounds;
				if (times_logged_out_of_bounds == 1000)
				{
					logger::WARNING("unit is out of bounds!");
					times_logged_out_of_bounds = 0;
				}
				else
				{
					times_logged_out_of_bounds++;
				}
			}
		}

		framecounter.setPosition(main_view.getCenter().x + 172.5f, main_view.getCenter().y - 150);
		framecounter.setString(std::to_string(lastTime));

		float currentTime = clock.restart().asSeconds();
		float fps = 1.f / (currentTime - lastTime);
		lastTime = currentTime;

		draw(window, main_view);
	}

	logger::INFO("exiting...");

	return 0;
}
