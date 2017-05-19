#include "Game.hpp"

#include "Interface.hpp"
#include "ObjectManager.hpp"
#include "Button.hpp"

#include <ENGINE\Engine.hpp>
#include <ENGINE\Logger.hpp>
#include <ENGINE\Graphics.hpp>
#include <ENGINE\Text.hpp>

Game::Game(bool $fullscreen)
{
	logger::INFO("Initialising...");

	{
		sf::VideoMode gameWindowDimensions(800, 600);
		std::string gameWindowTitle("bobwars " + gameVersion + "-" + engine::version);

		gameWindow = new sf::RenderWindow;

		if ($fullscreen)
			gameWindow->create(gameWindowDimensions, gameWindowTitle, sf::Style::Fullscreen);
		else
			gameWindow->create(gameWindowDimensions, gameWindowTitle, sf::Style::Titlebar | sf::Style::Close);

//		gameWindow->setVerticalSyncEnabled(true);
	}

	logger::INFO("Pre-game setup.");

	obMan = new ObjectManager;

	if (!Arial.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf"))
	{
		logger::ERROR("Failed to load font \"Arial\"!");
	}
	else
	{
		text.setFont(Arial);
		text.setScale(sf::Vector2f(.2f, .2f));
	}

	logger::INFO("Loading world texture...");

	static sf::Texture world_tex;
	if (!world_tex.loadFromFile("resource\\textures\\world.png"))
		logger::ERROR("Failed to load world textures!");

	world.setSize(sf::Vector2f(800, 600));
	world.setTexture(&world_tex);

	logger::INFO("Preparing user interface elements...");
	frameCounter.setFont(Arial);
	frameCounter.setScale(sf::Vector2f(.2f, .2f));

	main_view.setCenter(sf::Vector2f(800 / 2, 600 / 2));
	main_view.setSize(sf::Vector2f(800 / 2, 600 / 2));
	anchor.setCenter(sf::Vector2f(800 / 2, 600 / 2));
	anchor.setSize(sf::Vector2f(800 / 2, 600 / 2));

	ui = new Interface(this->gameWindow);

	ui->create_ent_button.setPosition(sf::Vector2f(30, -30));
	ui->create_ent_button.setScale(sf::Vector2f(.6f, .6f));
	ui->create_ent_button.setString("create");

	ui->delete_ent_button.setPosition(sf::Vector2f(100, -30));
	ui->delete_ent_button.setScale(sf::Vector2f(.6f, .6f));
	ui->delete_ent_button.setString("delete");

	logger::INFO("New Game created. (Ready!)");
}

Game::~Game()
{
	logger::INFO("Game destroyed.");
}

// Public

void Game::Main()
{
	sf::Time timePerFrame = sf::seconds(1.0f / 60.0f); // 60 frames per second
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Clock deltaClock;  // tracks how much time has past since the last frame

	sf::Clock framesClock; // for fps
	sf::Clock clock;

	// game loop
	while(gameWindow->isOpen())
	{
		sf::Time deltaTime = deltaClock.restart();  // get elapsed time and reset
		timeSinceLastUpdate += deltaTime;

		bool should_screenshot(false);

		while (timeSinceLastUpdate >= timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;

			sf::Event event;
			while (gameWindow->pollEvent(event))
			{
				if (event.type == sf::Event::EventType::Closed)
					gameWindow->close();

				//---------------KEYBOARD
				else if (event.type == sf::Event::EventType::KeyPressed)
				{
					if (event.key.code == sf::Keyboard::Key::Escape)
						logger::SILENT("DEBUG", "Pause has function not yet been implemented.");

					if (event.key.code == sf::Keyboard::Key::LShift)
						view_speed = 250;

					if (event.key.code == sf::Keyboard::Key::F12)
						should_screenshot = true;

					if (event.key.code == sf::Keyboard::Key::Tilde)
					{
						if (engine::cl_debug == true)
						{
							engine::cl_debug = false;

							logger::INFO("Toggled debug information OFF.");
						}
						else
						{
							engine::cl_debug = true;

							logger::INFO("Toggled debug information ON.");
						}
					}

					if (event.key.code == sf::Keyboard::Key::Delete && obMan->selected != obMan->entities[0])
						obMan->deleteObject(obMan->selected);
				}

				else if (event.type == sf::Event::EventType::KeyReleased)
				{
					if (event.key.code == sf::Keyboard::Key::LShift)
						view_speed = 500;
				}

				//---------------MOUSE
				else if (event.type == sf::Event::EventType::MouseButtonPressed)
				{
					if (event.key.code == sf::Mouse::Button::Left)
					{
						if (engine::logic::mouseIsOver(ui->create_ent_button.m_shape, *gameWindow, main_view))
						{
							if (ui->create_ent_button.disabled)
							{
								logger::INFO("Cannot create anymore units; you have too many.");
							}
							else
							{
								obMan->createObject();

								if (obMan->entities.size() >= 100)
									ui->create_ent_button.disable();
							}
						}
						else if (engine::logic::mouseIsOver(ui->delete_ent_button.m_shape, *gameWindow, main_view) && obMan->selected != obMan->entities[0])
						{
							obMan->deleteObject(obMan->selected);
						}
						else // didn't click a button so let's see if they clicked an entity
						{
							bool entity_was_selected(false);

							for (size_t i = 1; i < obMan->entities.size(); i++)
							{
								if (engine::logic::mouseIsOver(obMan->entities[i]->m_sprite, *gameWindow, main_view))
								{
									if (obMan->entities[i] == obMan->selected)
									{
										logger::INFO("This entity is already selected.");
										entity_was_selected = true;
										break;
									}
									else
									{
										obMan->selectObject(obMan->entities[i]);
										logger::INFO("Selected an entity. (" + std::to_string(obMan->entities[i]->m_id) + ")");
										entity_was_selected = true;
										break;
									}
								}
							} // what entity did we click

							if (!entity_was_selected && (obMan->selected != obMan->entities[0]))
							{
								logger::INFO("Entity deselected. (" + std::to_string(obMan->selected->m_id) + ")");
								obMan->selected = obMan->entities[0];
								break;
							}
						} // what did we click
					} // left mouse button
				} // mouseButtonPressed

				else if (event.type == sf::Event::EventType::MouseWheelMoved)
				{
					static float zoomlevel = 2.0f;

					if (event.mouseWheel.delta < 0) // up (in)
					{
						if (zoomlevel <= 4)
						{
							main_view.zoom(2.0f);
							zoomlevel = zoomlevel * 2.0f;
						}
					}
					else if (event.mouseWheel.delta > 0) // down (out)
					{
						if (zoomlevel > 0.5f)
						{
							main_view.zoom(0.5f);
							zoomlevel *= 0.5f;
						}
					}
				}
			} // pollevent

			if (gameWindow->hasFocus())
			{
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
						main_view.move(0, -view_speed * timePerFrame.asSeconds());
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
						main_view.move(-view_speed * timePerFrame.asSeconds(), 0);
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
						main_view.move(0, view_speed * timePerFrame.asSeconds());
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
						main_view.move(view_speed * timePerFrame.asSeconds(), 0);

					if (main_view.getCenter().x > 800)
						main_view.setCenter(800, main_view.getCenter().y);
					if (main_view.getCenter().y > 600)
						main_view.setCenter(main_view.getCenter().x, 600);
					if (main_view.getCenter().x < 0)
						main_view.setCenter(0, main_view.getCenter().y);
					if (main_view.getCenter().y < 0)
						main_view.setCenter(main_view.getCenter().x, 0);

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
						main_view.setCenter(obMan->selected->m_sprite.getPosition());
				}

				if (obMan->selected != obMan->entities[0])
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
						obMan->selected->m_sprite.move(0, -player_speed * timePerFrame.asSeconds());
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
						obMan->selected->m_sprite.move(-player_speed * timePerFrame.asSeconds(), 0);
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
						obMan->selected->m_sprite.move(0, player_speed * timePerFrame.asSeconds());
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
						obMan->selected->m_sprite.move(player_speed * timePerFrame.asSeconds(), 0);
				}

				{ //FRAMES PER SECOND
					float frames_per_second = framesClock.restart().asSeconds();

					frameCounter.setPosition(main_view.getCenter().x - 199, main_view.getCenter().y - 140);
					frameCounter.setString("FPS: " + std::to_string(static_cast<int>(1.0f / frames_per_second)));
				}

				if (obMan->selected == obMan->entities[0])
					ui->delete_ent_button.setButtonColor(sf::Color(150, 150, 150));
				else
					ui->delete_ent_button.setButtonColor(sf::Color(255, 255, 255));
			} // gameWindow.hasFocus()
		}

		anchor.setCenter(main_view.getCenter());
		Render();

		if (should_screenshot)
		{
			engine::screenshot(*gameWindow);
			should_screenshot = false;
		}
	}

	logger::INFO("Exiting...");
}

void Game::Update()
{

}

void Game::Render()
{
	gameWindow->clear();

	// ------------ MAIN VIEW
	gameWindow->setView(main_view);
	gameWindow->draw(world);

	for (size_t i = 1; i < obMan->entities.size(); i++)
		gameWindow->draw(obMan->entities[i]->m_sprite);

	ui->create_ent_button.draw(gameWindow);
	ui->delete_ent_button.draw(gameWindow);

	if (engine::cl_debug)
	{
		for (size_t i = 1; i < obMan->entities.size(); i++) // outline entities
		{
			if (obMan->entities[i] != obMan->entities[0])
			{
				showObjectCoords(*gameWindow, obMan->entities[i]->m_sprite);
				engine::graphics::outline(*gameWindow, obMan->entities[i]->m_sprite, 2, sf::Color::Red);
				engine::text::draw(*gameWindow, text, std::to_string(obMan->entities[i]->m_id) + "/" + std::to_string(obMan->entities.size()), sf::Vector2f(obMan->entities[i]->m_sprite.getPosition().x, obMan->entities[i]->m_sprite.getPosition().y));
			}
		}
	}

	if (obMan->selected != obMan->entities[0])
		engine::graphics::outline(*gameWindow, obMan->selected->m_sprite, 2, sf::Color::Yellow);

	// ------------- ANCHOR
	gameWindow->setView(anchor);

	// debug info like coordinates and stuff
	if (engine::cl_debug)
	{
		std::string x = "X: " + std::to_string(static_cast<int>(main_view.getCenter().x));
		std::string y = "Y: " + std::to_string(static_cast<int>(main_view.getCenter().y));
		engine::text::draw(*gameWindow, text, x + " " + y, sf::Vector2f(main_view.getCenter().x - 199, main_view.getCenter().y - 150));
		gameWindow->draw(frameCounter);
	}

	// ------------ MAIN VIEW
	gameWindow->setView(main_view);
	gameWindow->display();
}

// Private

void Game::showObjectCoords(sf::RenderWindow &window, sf::Sprite &object)
{
	std::string coords = "X: " +
		std::to_string(static_cast<int>(object.getPosition().x)) +
		" Y: " +
		std::to_string(static_cast<int>(object.getPosition().y));

	float x = object.getPosition().x + object.getLocalBounds().width / 4;
	float y = object.getPosition().y - 5;

	sf::Vector2f position(x, y);

	engine::text::draw(window, text, coords, position, 34);
}
