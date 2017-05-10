#include <ENGINE\Engine.hpp>
#include <ENGINE\Text.hpp>
#include <ENGINE\Graphics.hpp>
#include <SFML\Audio.hpp>

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

Button create_ent_button(sf::Vector2f(50, 25), "create");
Button delete_ent_button(sf::Vector2f(50, 25), "delete");

ObjectManager obMan;

float view_speed = 5.0f;
float player_speed = 2.5f;

void showObjectCoords(sf::RenderWindow &window, sf::Sprite &object)
{
	std::string coords = "X: " +
		std::to_string( static_cast<int>(object.getPosition().x) ) +
		" Y: " +
		std::to_string( static_cast<int>(object.getPosition().y) );

	float x = object.getPosition().x + object.getLocalBounds().width / 4;
	float y = object.getPosition().y - 5;

	sf::Vector2f position(x, y);

	engine::text::draw(window, text, coords, position, 34);
}

void draw(sf::RenderWindow &window, sf::View &anchor, sf::View &view)
{
	window.clear();

	window.setView(view);
	window.draw(world);

	for (size_t i = 0; i < obMan.entities.size(); i++)
		window.draw(obMan.entities[i]->m_sprite);

	create_ent_button.draw(window);
	delete_ent_button.draw(window);

	// debug info like coordinates and stuff
	if (engine::cl_debug)
	{
		{ // main view coordinates
			window.setView(anchor);

			std::string x = "X: " + std::to_string( static_cast<int>(view.getCenter().x) );
			std::string y = "Y: " + std::to_string( static_cast<int>(view.getCenter().y) );

			engine::text::draw(window, text, x + " " + y, sf::Vector2f(view.getCenter().x - 199, view.getCenter().y - 150));

			window.draw(framecounter);

			window.setView(view);
		}

		for (size_t i = 0; i < obMan.entities.size(); i++)
		{
			if (obMan.entities[i] != obMan.entities[0])
			{
				showObjectCoords(window, obMan.entities[i]->m_sprite);
				engine::graphics::outline(window, obMan.entities[i]->m_sprite, 2, sf::Color::Red);
				engine::text::draw(window, text, std::to_string(obMan.entities[i]->m_id) + "/" + std::to_string(obMan.entities.size()), sf::Vector2f(obMan.entities[i]->m_sprite.getPosition().x, obMan.entities[i]->m_sprite.getPosition().y));
			}
		}
	}

	if (obMan.selected != obMan.entities[0])
		engine::graphics::outline(window, obMan.selected->m_sprite, 2, sf::Color::Yellow);

	window.display();
}

void prepareInterface()
{
	logger::INFO("Pre-game setup.");

	if (!Arial.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf"))
		logger::ERROR("Failed to load font \"Arial\"!");
	else
		text.setFont(Arial);

	text.setScale(sf::Vector2f(.2f, .2f));

	logger::INFO("Loading world texture...");

	static sf::Texture world_tex;
	if (!world_tex.loadFromFile("resource\\textures\\world.png"))
		logger::ERROR("Failed to load world textures!");

	logger::INFO("Loading player texture...");
	if (!player_tex.loadFromFile("resource\\textures\\bob.png"))
		logger::ERROR("Failed to load player textures!");

	world.setSize(sf::Vector2f(800, 600));
	world.setTexture(&world_tex);

	logger::INFO("Preparing user interface elements...");
	framecounter.setFont(Arial);
	framecounter.setScale(sf::Vector2f(.2f, .2f));

	create_ent_button.setPosition(sf::Vector2f(30, -30));
	delete_ent_button.setPosition(sf::Vector2f(80, -30));
	create_ent_button.setScale(sf::Vector2f(.6, .6));
	delete_ent_button.setScale(sf::Vector2f(.6, .6));

	logger::INFO("Ready!");
}

int main(int argc, char *argv[])
{
	{
		std::cout << "Launched with " << argc << " arguments: " << std::endl;

		for (int i = 0; i < argc; i++) // TODO: parse arguments
		{
			std::cout << i << ": " << argv[i] << std::endl;
		}

		std::cout << std::endl;
	}

	logger::INFO("Initialising...");

	sf::RenderWindow gameWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), ("bobwars 0.4.0-" + engine::version), sf::Style::Titlebar | sf::Style::Close);
	sf::View main_view(sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
	sf::View anchor(sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2)); // HACK?: to keep text in place

	gameWindow.setFramerateLimit(120); // everything goes REALLY WICKED FAST without this. do not remove until delta time is implemented

	prepareInterface();

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
					logger::SILENT("DEBUG", "Pause function not yet implemented.");

				if (event.key.code == sf::Keyboard::LShift)
					view_speed = 2.5f;

				if (event.key.code == sf::Keyboard::F12)
					should_screenshot = true;

				if (event.key.code == sf::Keyboard::Delete && obMan.selected != obMan.entities[0])
					obMan.deleteObject(obMan.selected);
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
					if (engine::logic::didClick(create_ent_button.m_shape, gameWindow, main_view))
					{
						obMan.createObject();
						obMan.entities.back()->m_sprite.setTexture(player_tex); // TODO: load textures in ResourceManager, which ObjectManager will have access to.
					}
					else if (engine::logic::didClick(delete_ent_button.m_shape, gameWindow, main_view) && obMan.selected != obMan.entities[0])
					{
						obMan.deleteObject(obMan.selected);
					}
					else // didn't click a button so let's see if they clicked an entity
					{
						bool entity_was_selected(false);
						
						for (size_t i = 0; i < obMan.entities.size(); i++)
						{
							if (engine::logic::didClick(obMan.entities[i]->m_sprite, gameWindow, main_view))
							{
								if (obMan.entities[i] == obMan.selected)
								{
									logger::INFO("This entity is already selected.");
									entity_was_selected = true;
									break;
								}
								else
								{
									obMan.selectObject(obMan.entities[i]);
									logger::INFO("Selected an entity.");
									entity_was_selected = true;
									break;
								}
							}
						} // what entity did we click

						if (!entity_was_selected && (obMan.selected != obMan.entities[0]))
						{
							obMan.selected = obMan.entities[0];
							logger::INFO("Entity deselected.");
							break;
						}
					} // what did we click
				} // left mouse button
			} // mouseButtonPressed

			if (event.type == sf::Event::MouseWheelMoved)
			{
				static float zoomlevel = 2.0f;

				if (event.mouseWheel.delta < 0)
				{
					if (zoomlevel <= 4)
					{
						main_view.zoom(2.0f);
						zoomlevel = zoomlevel * 2.0f;
					}
				}
				else
				{
					if (zoomlevel > 0.5f)
					{
						main_view.zoom(0.5f);
						zoomlevel *= 0.5f;
					}
				}
			}
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

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
				{
					main_view.setCenter(obMan.selected->m_sprite.getPosition());
				}
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
				float frames_per_second = frame_time.restart().asSeconds();

				framecounter.setPosition(main_view.getCenter().x - 199, main_view.getCenter().y - 140);
				framecounter.setString("FPS: " + std::to_string(static_cast<int>(1.0f / frames_per_second)));
			}

			if (obMan.selected == obMan.entities[0])
			{
				delete_ent_button.setButtonColor(sf::Color(150, 150, 150));
			}
			else
			{
				delete_ent_button.setButtonColor(sf::Color(255, 255, 255));
			}
		} // gameWindow.hasFocus()

		anchor.setCenter(main_view.getCenter());
		draw(gameWindow, anchor, main_view);

		if (should_screenshot)
		{
			engine::screenshot(gameWindow);
			should_screenshot = false;
		}
	}

	logger::INFO("exiting...");

	return EXIT_SUCCESS;
}
