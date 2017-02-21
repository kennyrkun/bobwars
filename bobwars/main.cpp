#include <ENGINE\engine_main.hpp>

#include "GameObjectManager.hpp"

#include <iostream>

BaseEntity null_ent;
BaseEntity *selectedObject = &null_ent;
std::vector<BaseEntity*> entities;

sf::Font font;
sf::Text text;
sf::Text framecounter;
sf::Texture player_tex;
sf::RectangleShape world;
sf::RectangleShape create_ent_button;
sf::RectangleShape delete_ent_button;

static float view_speed = .1f;
static float player_speed = .05f;

bool clickedEntity(sf::RenderWindow &window, sf::View &view)
{
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->m_sprite.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window), view)))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

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

	engine::text::drawText(window, text, coords, position, 34);
}

void move_sprite(sf::Sprite &sprite, int x, int y) //TODO:move sprite function
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
	
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		window.draw(entities[i]->m_sprite);
	}

	//gui
	if (engine::cl_debug) // debug things like coordinantes
	{
		window.draw(framecounter);
		engine::text::drawText(window, text, ("X: " + std::to_string(static_cast<int>(view.getCenter().x)) + " Y: " + std::to_string(static_cast<int>(view.getCenter().y))), sf::Vector2f(view.getCenter().x - 199, view.getCenter().y - 150)); // should I get the coordinates somewhere else and only draw here? -Kenny

		if (selectedObject != &null_ent)
		{
			show_coords(window, selectedObject->m_sprite); // coords of sprite
			engine::text::drawText(window, text, std::to_string(selectedObject->m_health), sf::Vector2f(selectedObject->m_sprite.getPosition().x, selectedObject->m_sprite.getPosition().y)); // unit health
		}
	}

	window.draw(create_ent_button);
	window.draw(delete_ent_button);

	engine::text::drawText(window, text, "+", sf::Vector2f(10.4, -38.4), 128, sf::Color::Green);
	engine::text::drawText(window, text, "X", sf::Vector2f(32, -32.5), 86, sf::Color::Red);

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

	static sf::Texture world_tex;

	if (!world_tex.loadFromFile("resource\\textures\\world.png"))
		std::cout << "unable to load world textures!" << std::endl;

	logger::INFO("player texture...");
	if (!player_tex.loadFromFile("resource\\textures\\player.png"))
		std::cout << "unable to load player textures!" << std::endl;

	world.setSize(sf::Vector2f(800, 600));
	world.setTexture(&world_tex);

	framecounter.setFont(font);
	framecounter.setScale(sf::Vector2f(.2f, .2f));

	create_ent_button.setSize(sf::Vector2f(16, 16));
	delete_ent_button.setSize(sf::Vector2f(16, 16));

	create_ent_button.setPosition(sf::Vector2f(10, -30));
	delete_ent_button.setPosition(sf::Vector2f(30, -30));

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

	sf::RenderWindow window(sf::VideoMode(800, 600), ("bobwars 0.0.6:" + engine::build_number), sf::Style::Titlebar | sf::Style::Close);
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

		//---------------KEYBOARD
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

		//---------------MOUSE
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.key.code == sf::Mouse::Left)
				{
					if (clickedEntity)
					{
						for (unsigned int i = 0; i < entities.size(); i++)
						{
							if (entities[i]->m_sprite.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window), main_view)))
							{
								if (selectedObject != entities[i])
								{
									selectedObject = entities[i];

									logger::DEBUG("selected an entity");
								}
							}
						}
					}
					else
					{
						selectedObject = &null_ent;
					}

					if (create_ent_button.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window), main_view)))
					{
						logger::INFO("creating new entity");

						BaseEntity* newEnt = new BaseEntity();
						newEnt->m_sprite.setTexture(player_tex);
						entities.push_back(newEnt);
						selectedObject = newEnt;
					}
					else if (delete_ent_button.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window), main_view)) && entities.size() != 0)
					{
						logger::INFO("deleting last entity");
						entities.pop_back();

						if (entities.size() == 0)
							selectedObject = &null_ent;
						else
							selectedObject = entities.back();
					}
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
				selectedObject->m_sprite.move(0, -player_speed);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				selectedObject->m_sprite.move(-player_speed, 0);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				selectedObject->m_sprite.move(0, player_speed);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				selectedObject->m_sprite.move(player_speed, 0);

			if (!selectedObject->m_sprite.getGlobalBounds().intersects(world.getGlobalBounds()) && selectedObject != &null_ent)
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
