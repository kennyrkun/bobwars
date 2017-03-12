#include <ENGINE\engine_main.hpp>

#include "ObjectManager.hpp"
#include "BaseEntity.hpp"
#include "Button.hpp"

#include <iostream>
#include <filesystem>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

sf::Font Arial;
sf::Text text;
sf::Text framecounter;
sf::Texture player_tex;
sf::RectangleShape world;

Button create_ent(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT), sf::Vector2f(200, 50), "create");
Button delete_ent(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT), sf::Vector2f(200, 50), "delete");

ObjectManager obMan;

float view_speed = 5.0f;
float player_speed = 2.5f;

bool clicked(sf::Shape &object, sf::RenderWindow &window, sf::View &view)
{
	if (object.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window), view)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool clicked(sf::Sprite &object, sf::RenderWindow &window, sf::View &view)
{
	if (object.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window), view)))
	{
		return true;
	}
	else
	{
		return false;
	}
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

	for (size_t i = 0; i < obMan.entities.size(); i++)
		window.draw(obMan.entities[i]->m_sprite);

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

		for (size_t i = 0; i < obMan.entities.size(); i++)
		{
			if (obMan.entities[i] != obMan.entities[0])
			{
				show_coords(window, obMan.entities[i]->m_sprite);
				engine::graphics::outline(window, obMan.entities[i]->m_sprite, 2, sf::Color::Red);
				engine::text::draw(window, text, std::to_string(obMan.entities[i]->m_id) + "/" + std::to_string(obMan.entities.size()), sf::Vector2f(obMan.entities[i]->m_sprite.getPosition().x, obMan.entities[i]->m_sprite.getPosition().y));
			}
		}
	}

	if (obMan.selected != obMan.entities[0])
		engine::graphics::outline(window, obMan.selected->m_sprite, 2, sf::Color::Yellow);

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
	if (!player_tex.loadFromFile("resource\\textures\\bob.png"))
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

	sf::RenderWindow gameWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), ("bobwars 0.3.0-" + engine::version), sf::Style::Titlebar | sf::Style::Close);
	sf::View main_view(sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));

	gameWindow.setFramerateLimit(120); // everything goes REALLY WICKED FAST without this. do not remove.

	gui_load();

	sf::Clock frame_time; // for fps
	sf::Clock clock;

	// game loop
	while (gameWindow.isOpen())
	{
		sf::Event event;

		bool should_screenshot(false);

		while (gameWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				gameWindow.close();

			//---------------KEYBOARD
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
					logger::DEBUG("pause function not yet implemented.");

				if (event.key.code == sf::Keyboard::LShift)
					view_speed = 2.5f;

				if (event.key.code == sf::Keyboard::F12)
					should_screenshot = true;
			}

			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::LShift)
					view_speed = 5.0f;
			}

			//---------------MOUSE
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.key.code == sf::Mouse::Left)
				{
					if ( clicked(create_ent.m_shape, gameWindow, main_view) )
					{
						obMan.createObject();
						obMan.entities.back()->m_sprite.setTexture(player_tex); // TODO: load textures in ResourceManager, which ObjectManager will have access to.
					}
					else if ( clicked(delete_ent.m_shape, gameWindow, main_view) && obMan.selected != obMan.entities[0] )
					{
						obMan.deleteObject(obMan.selected);
					}
					else // didn't click a button so let's see if they clicked an entity
					{
						for (size_t i = 0; i < obMan.entities.size(); i++)
						{
							bool entity_was_selected(false);

							if (clicked(obMan.entities[i]->m_sprite, gameWindow, main_view))
							{
								if (obMan.entities[i] == obMan.selected)
								{
									logger::DEBUG("this entity is already selected");
									break;
								}
								else
								{
									obMan.selectObject(obMan.entities[i]);
									logger::DEBUG("selected an entity");
									break;
								}
							}

							if (!entity_was_selected && (obMan.selected != obMan.entities[0]))
							{
								obMan.selected = obMan.entities[0];
								logger::DEBUG("entity deselected");
							}
						} // what entity did we click
					} // what did we click
				} // left mouse button
			} // mouseButtonPressedgit
		} // pollevent

		if (gameWindow.hasFocus())
		{
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
					main_view.move(0, -view_speed);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
					main_view.move(-view_speed, 0);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
					main_view.move(0, view_speed);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
					main_view.move(view_speed, 0);

				if (main_view.getCenter().x > 800)
					main_view.setCenter(800, main_view.getCenter().y);
				if (main_view.getCenter().y > 600)
					main_view.setCenter(main_view.getCenter().x, 600);
				if (main_view.getCenter().x < 0)
					main_view.setCenter(0, main_view.getCenter().y);
				if (main_view.getCenter().y < 0)
					main_view.setCenter(main_view.getCenter().x, 0);
			}

			if (obMan.selected != obMan.entities[0])
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
					obMan.selected->m_sprite.move(0, -player_speed);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
					obMan.selected->m_sprite.move(-player_speed, 0);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
					obMan.selected->m_sprite.move(0, player_speed);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
					obMan.selected->m_sprite.move(player_speed, 0);
			}

			{ //FRAMES PER SECOND
				float frames_per_second = frame_time.getElapsedTime().asSeconds();

				framecounter.setPosition(main_view.getCenter().x - 199, main_view.getCenter().y - 140);
				framecounter.setString("FPS: " + std::to_string((int)(1.0f / frames_per_second)));

				frame_time.restart().asSeconds();
			}
		} // gameWindow.hasFocus()

		draw(gameWindow, main_view);

		if (should_screenshot)
		{
			engine::screenshot(gameWindow);

			should_screenshot = false;
		}
	}

	logger::INFO("exiting...");

	return 0;
}
