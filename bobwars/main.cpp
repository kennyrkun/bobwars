#include <ENGINE\engine_main.hpp>
//#include "GameObjectManager.hpp"
#include "BaseEntity.hpp"
#include "Button.hpp"
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

Button create_ent(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT), sf::Vector2f(200, 50), "create");
Button delete_ent(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT), sf::Vector2f(200, 50), "delete");

BaseEntity null_ent;
BaseEntity *selectedObject = &null_ent;
std::vector<BaseEntity*> entities;

sf::Font Arial;
sf::Text text;
sf::Text framecounter;
sf::Texture player_tex;
sf::RectangleShape world;

float view_speed = .1f;
float player_speed = .05f;

void fade(sf::Shape &object, int opacity)
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

void show_coords(sf::RenderWindow &window, sf::Sprite &object)
{
	std::string coords = "X: " +
		std::to_string((int)(object.getPosition().x)) +
		" Y: " +
		std::to_string((int)(object.getPosition().y));

	float x = object.getPosition().x + object.getLocalBounds().width / 4;
	float y = object.getPosition().y - 5;

	sf::Vector2f position(x, y);

	engine::text::draw(window, text, coords, position, 34);
}

void draw(sf::RenderWindow &window, sf::View &view)
{
	window.clear();

	window.setView(view);
	window.draw(world);

	for (size_t i = 0; i < entities.size(); i++)
		window.draw(entities[i]->m_sprite);

	create_ent.draw(window);
	delete_ent.draw(window);

	// debug info like coordinates and stuff
	if (engine::cl_debug)
	{
		window.draw(framecounter);

		{ // main view coordinates
			std::string x = "X: " + std::to_string((int)view.getCenter().x);
			std::string y = "Y: " + std::to_string((int)view.getCenter().y);

			engine::text::draw(window, text, x + " " + y, sf::Vector2f(view.getCenter().x - 199, view.getCenter().y - 150));
		}

		for (size_t i = 0; i < entities.size(); i++)
		{
			show_coords(window, entities[i]->m_sprite);

			if (entities[i] != selectedObject)
				outline(window, entities[i]->m_sprite, 2, sf::Color::Red);

			engine::text::draw(window, text, std::to_string(entities[i]->m_id) + "/" + std::to_string(entities.size()), sf::Vector2f(entities[i]->m_sprite.getPosition().x, entities[i]->m_sprite.getPosition().y));
		}
	}

	if (selectedObject != &null_ent)
		outline(window, selectedObject->m_sprite, 2, sf::Color::Yellow);

	window.display();
}

void gui_load()
{
	logger::INFO("preparing graphics");

	if (!Arial.loadFromFile("resource\\fonts\\arial.ttf"))
	{
		std::cout << "unable to load font 'arial'." << std::endl;
	}
	else
	{
		text.setFont(Arial);
		text.setScale(sf::Vector2f(.2f, .2f));
	}

	logger::INFO("world texture...");

	static sf::Texture world_tex;
	if (!world_tex.loadFromFile("resource\\textures\\world.png"))
		logger::ERROR("unable to load world textures!");

	logger::INFO("player texture...");
	if (!player_tex.loadFromFile("resource\\textures\\player.png"))
		logger::ERROR("unable to load player textures!");

	world.setSize(sf::Vector2f(800, 600));
	world.setTexture(&world_tex);

	logger::INFO("ui elements");
	framecounter.setFont(Arial);
	framecounter.setScale(sf::Vector2f(.2f, .2f));

	create_ent.setPosition(sf::Vector2f(30, -30));
	delete_ent.setPosition(sf::Vector2f(80, -30));

	logger::INFO("done!");
}

int main(int argc, char *argv[])
{
	{
		std::cout << "launched with " << argc << " arguments: " << std::endl;

		for (int i = 0; i < argc; i++) // TODO: parse arguments
		{
			std::cout << i << ": " << argv[i] << std::endl;
		}

		std::cout << std::endl;
	}

	logger::INFO("initializing");

	sf::RenderWindow rWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), ("bobwars 0.1.0-" + engine::version), sf::Style::Titlebar | sf::Style::Close);
	sf::View main_view(sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));

	rWindow.setFramerateLimit(60);

	gui_load();

	sf::Clock clock;    // for fps
	float lastTime = 0; // also for fps

	// game loop
	while (rWindow.isOpen())
	{
		sf::Event event;

		while (rWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				rWindow.close();

			//---------------KEYBOARD
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
					logger::INFO("pause function not yet implemented.");

				if (event.key.code == sf::Keyboard::LShift)
					view_speed = .05f;
			}

			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::LShift)
					view_speed = .1f;
			}

			//---------------MOUSE
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.key.code == sf::Mouse::Left)
				{
					bool entity_was_selected(false);

					for (size_t i = 0; i < entities.size(); i++)
					{
						if (entities[i]->m_sprite.getGlobalBounds().contains(rWindow.mapPixelToCoords(sf::Mouse::getPosition(rWindow), main_view)))
						{
							if (entities[i] != selectedObject) // if this entity isn't already selected
							{
								selectedObject = entities[i];

								entity_was_selected = true;

								logger::DEBUG("selected an entity");
							}
							else
							{
								logger::DEBUG("this entity is already selected");
							}
						}

						if (!entity_was_selected && selectedObject != &null_ent)
						{
							logger::DEBUG("entity deselected");

							selectedObject = &null_ent;
						}
					}

					// check if one of the entity buttons were pressed
					if (create_ent.m_shape.getGlobalBounds().contains(rWindow.mapPixelToCoords(sf::Mouse::getPosition(rWindow), main_view)))
					{
						logger::INFO("creating new entity");

						BaseEntity* newEnt = new BaseEntity();
						newEnt->m_sprite.setTexture(player_tex);
						newEnt->m_id = entities.size() + 1;
						entities.push_back(newEnt);
						selectedObject = newEnt;
					}
					else if (delete_ent.m_shape.getGlobalBounds().contains(rWindow.mapPixelToCoords(sf::Mouse::getPosition(rWindow), main_view)) && entities.size() != 0)
					{
						logger::INFO("deleting last entity");
						entities.pop_back();

						if (entities.size() == 0)
							selectedObject = &null_ent;
						else
							selectedObject = entities.back();
					} // entity button
				} // left button
			} // mouseButtonPressedgit
		} // pollevent

		if (rWindow.hasFocus())
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				main_view.move(0, -view_speed);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				main_view.move(-view_speed, 0);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				main_view.move(0, view_speed);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				main_view.move(view_speed, 0);

			{ // keeps the camera from going out of bounds
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
				selectedObject->m_sprite.move(0, -player_speed);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				selectedObject->m_sprite.move(-player_speed, 0);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				selectedObject->m_sprite.move(0, player_speed);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				selectedObject->m_sprite.move(player_speed, 0);

			if (!selectedObject->m_sprite.getGlobalBounds().intersects(world.getGlobalBounds()) && selectedObject != &null_ent)
			{
				static int times_logged; // this so that we don't print thousands of messages and lag the game
				if (times_logged == 1000)
				{
					logger::WARNING("unit is out of bounds!");
					times_logged = 0;
				}
				else
				{
					times_logged++;
				}
			}
		}

		{ //FRAMES PER SECOND
			sf::Time frames_per_second = clock.getElapsedTime();
			framecounter.setPosition(main_view.getCenter().x + 170, main_view.getCenter().y - 150); // top right
			framecounter.setString( "FPS: " + std::to_string( (1.0f / frames_per_second.asSeconds()) ) );

			if (frames_per_second.asSeconds() < 60)
			{
				logger::WARNING("less than 60 fps");
			}

			clock.restart();
		}

		draw(rWindow, main_view);
	}

	logger::INFO("exiting...");

	return 0;
}
