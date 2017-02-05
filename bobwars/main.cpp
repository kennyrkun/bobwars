#include <SFML\Graphics.hpp>
#include <ENGINE\engine_main.hpp>

#include <iostream>

sf::Font font;
sf::Text coordinates;
sf::Text player_coordinates;
sf::Text coords;
sf::Text fps;
sf::Sprite player;
sf::Texture player_tex;
sf::RectangleShape world;
sf::RectangleShape selector;
sf::Texture world_tex;

void showcoords(sf::RenderWindow &window, sf::Sprite &object)
{
	{
		coords.setFont(font);
		coords.setString("coordinates");
		coords.setScale(sf::Vector2f(.2, .2));
	}

	coords.setPosition(object.getPosition().x + 15, object.getPosition().y - 5);
	coords.setString("X: " + std::to_string(static_cast<int>(object.getPosition().x)) + " Y: " + std::to_string(static_cast<int>(object.getPosition().y)));

	window.draw(coords);
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
	window.draw(player);
	//gui
	window.draw(coordinates);
	showcoords(window, player);
	window.draw(fps);

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
		coordinates.setFont(font);
		coordinates.setString("coordinates");
		coordinates.setScale(sf::Vector2f(.2, .2));

		player_coordinates.setFont(font);
		player_coordinates.setScale(sf::Vector2f(.2, .2));
		player_coordinates.setString("coordinates");
	}

	if (!world_tex.loadFromFile("resource\\textures\\world.png"))
		std::cout << "unable to load world textures!" << std::endl;

	if (!player_tex.loadFromFile("resource\\textures\\player.png"))
		std::cout << "unable to load player textures!" << std::endl;

	world.setSize(sf::Vector2f(800, 600));
	world.setTexture(&world_tex);
	player.setTexture(player_tex);

	fps.setFont(font);
	fps.setScale(.2, .2);

	logger::INFO("done!");
}

int main(int argc, char *argv[])
{	
	{
		std::string args;

		if (argc > 1)
			args = "arguments";
		else
			args = "argument";

		std::cout << "launched with " << argc << " " + args + ": " << std::endl;

		for (int i = 0; i < argc; i++)
		{
			std::cout << i << ": " << argv[i] << std::endl;
		}

		std::cout << std::endl;
	}

	logger::INFO("initializing");

	sf::RenderWindow window(sf::VideoMode(800, 600), "bobwars 0.0.1", sf::Style::Titlebar | sf::Style::Close);
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
					std::cout << "pause function not yet implemented." << std::endl;
			}
		}

		if (window.hasFocus())
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				main_view.move(0, -.1f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				main_view.move(-.1f, -0);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				main_view.move(0, .1f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				main_view.move(.1f, 0);

			{
				if (main_view.getCenter().x > 800)
				{
					main_view.setCenter(800, main_view.getCenter().y);
				}

				if (main_view.getCenter().y > 600)
				{
					main_view.setCenter(main_view.getCenter().x, 600);
				}

				if (main_view.getCenter().x < 0)
				{
					main_view.setCenter(0, main_view.getCenter().y);
				}

				if (main_view.getCenter().y < 0)
				{
					main_view.setCenter(main_view.getCenter().x, 0);
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				player.move(0, -.1f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				player.move(-.1f, -0);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				player.move(0, .1f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				player.move(.1f, 0);
		}

		coordinates.setPosition(main_view.getCenter().x - 199, main_view.getCenter().y - 150);
		coordinates.setString("X: " + std::to_string(static_cast<int>(main_view.getCenter().x)) + " Y: " + std::to_string(static_cast<int>(main_view.getCenter().y)));

		fps.setPosition(main_view.getCenter().x + 170, main_view.getCenter().y - 150);

		fps.setString(std::to_string(lastTime));

		float currentTime = clock.restart().asSeconds();
		float fps = 1.f / (currentTime - lastTime);
		lastTime = currentTime;

		draw(window, main_view);
	}

	logger::INFO("exiting...");
}