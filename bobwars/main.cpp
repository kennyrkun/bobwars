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

void fade(sf::Shape &object, int opacity) // TODO: run this in another thread
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
			show_coords(window, obMan.entities[i]->m_sprite);

			if (obMan.entities[i] != obMan.selected)
				engine::graphics::outline(window, obMan.entities[i]->m_sprite, 2, sf::Color::Red);

			engine::text::draw(window, text, std::to_string(obMan.entities[i]->m_id) + "/" + std::to_string(obMan.entities.size()), sf::Vector2f(obMan.entities[i]->m_sprite.getPosition().x, obMan.entities[i]->m_sprite.getPosition().y));
		}
	}

	if (obMan.selected != &obMan.null)
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

void screenshot(sf::RenderWindow &window)
{
	sf::Vector2u windowSize = window.getSize();
	sf::Texture texture;

	texture.create(windowSize.x, windowSize.y);
	texture.update(window); // give texture window

	{
		namespace fs = std::experimental::filesystem;

		if (fs::exists("screenshots"))
		{
			int screenshots = 0;

			for (auto& p : fs::recursive_directory_iterator("screenshots")) // gets us the amount of files in there
			{
				screenshots += 1;
			}

			std::string savePath = "screenshots\\screenshot_" + std::to_string(screenshots) + ".png";

			if (!texture.copyToImage().saveToFile(savePath))
			{
				logger::ERROR("failed to save screenshot.");
			}
			else
			{
				logger::INFO("saved screenshot #" + std::to_string(screenshots));
			}
		}
		else // screenshots folder is a non
		{
			logger::WARNING("screenshots folder does not exist, attempting to create one.");

			if (fs::create_directory("screenshots"))
			{
				logger::INFO("attempting to save screenshot again...");

				screenshot(window);
			}
			else
			{
				logger::ERROR("failed to create screenshots folder, make it yourself.");
			}
		}
	}
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

	sf::RenderWindow gameWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), ("bobwars 0.2.0-" + engine::version), sf::Style::Titlebar | sf::Style::Close);
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
				{
					should_screenshot = true;
				}
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
					bool entity_was_selected(false);

					for (size_t i = 0; i < obMan.entities.size(); i++)
					{
						if (obMan.entities[i]->m_sprite.getGlobalBounds().contains(gameWindow.mapPixelToCoords(sf::Mouse::getPosition(gameWindow), main_view)))
						{
							if (obMan.entities[i] != obMan.selected) // if this entity isn't already selected
							{
								obMan.selected = obMan.entities[i];

								entity_was_selected = true;

								logger::DEBUG("selected an entity");
							}
							else
							{
								logger::DEBUG("this entity is already selected");
							}
						}

						if (!entity_was_selected && (obMan.selected != &obMan.null))
						{
							logger::DEBUG("entity deselected");

							obMan.selected = &obMan.null;
						}
					}

					// check if one of the entity buttons were pressed
					if (create_ent.m_shape.getGlobalBounds().contains(gameWindow.mapPixelToCoords(sf::Mouse::getPosition(gameWindow), main_view)))
					{
						logger::INFO("creating new entity");

						BaseEntity* newEnt = new BaseEntity();
						newEnt->m_sprite.setTexture(player_tex);
						newEnt->m_id = obMan.entities.size() + 1;

						obMan.entities.push_back(newEnt);
						obMan.selected = newEnt;
					}
					else if (delete_ent.m_shape.getGlobalBounds().contains(gameWindow.mapPixelToCoords(sf::Mouse::getPosition(gameWindow), main_view)) && obMan.entities.size() != 0)
					{
						logger::INFO("deleting last entity");
						obMan.entities.pop_back();

						if (obMan.entities.size() == 0)
							obMan.selected = &obMan.null;
						else
							obMan.selected = obMan.entities.back();
					} // entity button
				} // left button
			} // mouseButtonPressedgit
		} // pollevent

		if (gameWindow.hasFocus())
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
				obMan.selected->m_sprite.move(0, -player_speed);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				obMan.selected->m_sprite.move(-player_speed, 0);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				obMan.selected->m_sprite.move(0, player_speed);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				obMan.selected->m_sprite.move(player_speed, 0);

			if (!obMan.selected->m_sprite.getGlobalBounds().intersects(world.getGlobalBounds()) &&( obMan.selected != &obMan.null))
			{
				obMan.selected->m_sprite.getOrigin();

				if ((int)clock.getElapsedTime().asSeconds() == 5)
				{
					logger::WARNING("player is out of bounds");

					clock.restart();
				}

				if (clock.getElapsedTime().asSeconds() > 5)
					clock.restart();
			}
		} // gameWindow.hasFocus()

		{ //FRAMES PER SECOND
			float frames_per_second = frame_time.getElapsedTime().asSeconds();

			framecounter.setPosition(main_view.getCenter().x - 199, main_view.getCenter().y -  140);
			framecounter.setString( "FPS: " + std::to_string( (int)(1.0f / frames_per_second) ) );

			frame_time.restart();
		}

		draw(gameWindow, main_view);

		if (should_screenshot)
		{
			screenshot(gameWindow);

			should_screenshot = false;
		}
	}

	logger::INFO("exiting...");

	return 0;
}
