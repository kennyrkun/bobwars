#include "Game.hpp"

#include "Interface.hpp"
#include "ObjectManager.hpp"
#include "Button.hpp"

#include <ENGINE\Engine.hpp>
#include <ENGINE\Logger.hpp>
#include <ENGINE\Graphics\Graphics.hpp>
#include <ENGINE\Graphics\Text.hpp>

Game::Game(bool fullscreen, bool vsync)
{
	logger::INFO("Initialising...");

	{
		sf::VideoMode gameWindowDimensions(800, 600);
		std::string gameWindowTitle("bobwars " + gameVersion + "-" + engine::version);

		gameWindow = new sf::RenderWindow;

		if (fullscreen)
			gameWindow->create(gameWindowDimensions, gameWindowTitle, sf::Style::Fullscreen);
		else
			gameWindow->create(gameWindowDimensions, gameWindowTitle, sf::Style::Titlebar | sf::Style::Close);

		if (vsync)
			gameWindow->setVerticalSyncEnabled(true);
	}

	logger::INFO("Pre-game setup.");

	obMan = new ObjectManager;

	if (!Arial.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf"))
	{
		logger::SILENT("ERROR", "Failed to load font \"Arial\"!");
	}
	else
	{
		text.setFont(Arial);
		text.setScale(sf::Vector2f(.2f, .2f));
	}

	logger::INFO("Loading world texture...");

	static sf::Texture world_tex;
	if (!world_tex.loadFromFile("resource\\textures\\world.png"))
		logger::SILENT("ERROR", "Failed to load world textures!");

	world.setSize(sf::Vector2f(800, 600));
	world.setTexture(&world_tex);

	logger::INFO("Preparing user interface elements...");
	frameCounter.setFont(Arial);
	frameCounter.setScale(sf::Vector2f(.2f, .2f));

	sf::Vector2f screendimensions;
	screendimensions.x = std::size_t(gameWindow->getSize().x) / 2.0f;
	screendimensions.y = std::size_t(gameWindow->getSize().y) / 2.0f;
	mainView = new sf::View(screendimensions, screendimensions);
	viewAnchor = new sf::View(screendimensions, screendimensions);
	ui = new Interface(this->gameWindow, this->viewAnchor, this->mainView);

	logger::INFO("New Game created. (Ready!)");
}

Game::~Game()
{
	logger::INFO("Game destroyed.");
}

// Public

void Game::Main()
{
	timePerFrame = sf::seconds(1.0f / 60.0f); // 60 frames per second
	timeSinceLastUpdate = sf::Time::Zero;
	sf::Clock deltaClock;  // tracks how much time has past since the last frame

	sf::Clock framesClock; // for fps
	sf::Clock clock;

	// game loop
	while(gameWindow->isOpen())
	{
		sf::Time deltaTime = deltaClock.restart();  // get elapsed time and reset clock
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

					if (event.key.code == sf::Keyboard::Key::Delete)
					{
						if (!obMan->selectedEnts.empty())
						{
							// delete the thing
							if (obMan->selectedEnts.size() > 1)
							{
								for (size_t i = 0; i < obMan->selectedEnts.size(); i++)
								{
									obMan->deleteObject(obMan->selectedEnts[i]);
								}

								obMan->clearSelected();
							}
							else
							{
								obMan->deleteObject(obMan->selectedEnts.front());
								obMan->clearSelected();
							}

							ui->delete_ent_button.disable();

							if (ui->create_ent_button.disabled && obMan->entities.size() < 100)
								ui->create_ent_button.enable();

							// TODO: tell us which ones were deleted (e.g. Deleted entities 1-6. or Deleted entities 1, 4, 5, 6, and 7.)
						}
					}
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
						if (engine::logic::mouseIsOver(ui->create_ent_button.m_shape, *gameWindow, *viewAnchor))
						{
							if (obMan->entities.size() >= 100)
							{
								logger::INFO("Cannot create anymore units; you have too many.");
								break;
							}
							else
							{
								obMan->createObject();
								ui->delete_ent_button.enable();

								if (obMan->entities.size() >= 100)
								{
									ui->create_ent_button.disable();
								}

								break;
							}
						}
						else if (engine::logic::mouseIsOver(ui->delete_ent_button.m_shape, *gameWindow, *viewAnchor) && !obMan->selectedEnts.empty())
						{
							// delete the thing
							if (obMan->selectedEnts.size() > 1)
							{
								for (size_t i = 0; i < obMan->selectedEnts.size(); i++)
								{
									obMan->deleteObject(obMan->selectedEnts[i]);
								}

								obMan->clearSelected();
							}
							else
							{
								obMan->deleteObject(obMan->selectedEnts.front());
								obMan->clearSelected();
							}

							ui->delete_ent_button.disable();

							break;

							// TODO: tell us which ones were deleted (e.g. Deleted entities 1-6. or Deleted entities 1, 4, 5, 6, and 7.)
						}

						// if we haven't broken the loop already, it means we either clicked an entity or clicked nothing
						bool entity_was_selected(false);
						for (size_t i = 0; i < obMan->entities.size(); i++)
						{
							if (engine::logic::mouseIsOver(obMan->entities[i]->sprite, *gameWindow, *mainView))
							{
								if (obMan->selectObject(obMan->entities[i]))
								{
									logger::INFO("Selected an entity. (" + std::to_string(obMan->entities[i]->id) + ")");
									entity_was_selected = true;
									ui->delete_ent_button.enable();

									break;
								}
								else // returned 0, meaning it was already seleceted
								{
									entity_was_selected = true;
								}
							}
						} // what entity did we click

						if (entity_was_selected)
							break;

						if (!entity_was_selected && (!obMan->selectedEnts.empty())) // selected nothing and didn't already have nothing
						{
							logger::INFO("Entity(s) deselected.");

							obMan->selectedEnts.clear();

							ui->delete_ent_button.disable();
							break;
						}
					} // left mouse button

					if (event.key.code == sf::Mouse::Button::Right)
					{
						if (!obMan->selectedEnts.empty())
						{
							// if we haven't broken the loop already, it means we've clicked nothing.
							sf::Vector2f movePos(gameWindow->mapPixelToCoords(sf::Mouse::getPosition(*gameWindow), *mainView));

							for (size_t i = 0; i < obMan->selectedEnts.size(); i++)
							{
								obMan->selectedEnts[i]->moveTo(movePos);
							}
						}
					}
				} // mouseButtonPressed

				else if (event.type == sf::Event::EventType::MouseWheelMoved)
				{
					static float zoomlevel = 2.0f;

					if (event.mouseWheel.delta < 0) // up (in)
					{
						if (zoomlevel <= 4)
						{
							mainView->zoom(2.0f);
							zoomlevel = zoomlevel * 2.0f;
						}
					}
					else if (event.mouseWheel.delta > 0) // down (out)
					{
						if (zoomlevel > 0.5f)
						{
							mainView->zoom(0.5f);
							zoomlevel *= 0.5f;
						}
					}
				}
			} // pollevent

			if (gameWindow->hasFocus())
			{
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
						mainView->move(0, -view_speed * timePerFrame.asSeconds());
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
						mainView->move(-view_speed * timePerFrame.asSeconds(), 0);
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
						mainView->move(0, view_speed * timePerFrame.asSeconds());
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
						mainView->move(view_speed * timePerFrame.asSeconds(), 0);

					if (mainView->getCenter().x > 800)
						mainView->setCenter(800, mainView->getCenter().y);
					if (mainView->getCenter().y > 600)
						mainView->setCenter(mainView->getCenter().x, 600);
					if (mainView->getCenter().x < 0)
						mainView->setCenter(0, mainView->getCenter().y);
					if (mainView->getCenter().y < 0)
						mainView->setCenter(mainView->getCenter().x, 0);
				}

				if (engine::cl_debug && !obMan->selectedEnts.size() == 1)
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
						obMan->selectedEnts[0]->sprite.move(0, -player_speed * timePerFrame.asSeconds());
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
						obMan->selectedEnts[0]->sprite.move(-player_speed * timePerFrame.asSeconds(), 0);
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
						obMan->selectedEnts[0]->sprite.move(0, player_speed * timePerFrame.asSeconds());
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
						obMan->selectedEnts[0]->sprite.move(player_speed * timePerFrame.asSeconds(), 0);
				}

				{ //FRAMES PER SECOND
					float frames_per_second = framesClock.restart().asSeconds();

					frameCounter.setPosition(mainView->getCenter().x - 199, mainView->getCenter().y - 130);
					frameCounter.setString("FPS: " + std::to_string(static_cast<int>(1.0f / frames_per_second)));
				}
			} // gameWindow.hasFocus()

			viewAnchor->setCenter(mainView->getCenter());

			Update();
			Render();

			if (should_screenshot)
			{
				engine::screenshot(*gameWindow);
				should_screenshot = false;
			}
		}
	}
}

void Game::Update()
{
	for (size_t i = 0; i < obMan->entities.size(); i++)
	{
		if (obMan->entities[i]->moving)
		{
			obMan->entities[i]->Update();
		}
	}
}

void Game::Render()
{
	gameWindow->clear();

	// ------------ MAIN VIEW
	gameWindow->setView(*mainView);
	gameWindow->draw(world);

	for (size_t i = 0; i < obMan->entities.size(); i++)
		gameWindow->draw(obMan->entities[i]->sprite);

	if (engine::cl_debug)
	{
		for (size_t i = 0; i < obMan->entities.size(); i++) // outline entities
		{
			if (!obMan->entities.empty())
			{
				showObjectCoords(*gameWindow, obMan->entities[i]->sprite);
				engine::graphics::outline(*gameWindow, obMan->entities[i]->sprite, 2, sf::Color::Red);
				engine::text::draw(*gameWindow, text, std::to_string(obMan->entities[i]->id) + "/" + std::to_string(obMan->entities.size()), sf::Vector2f(obMan->entities[i]->sprite.getPosition().x, obMan->entities[i]->sprite.getPosition().y));

				if (obMan->entities[i]->moving)
				{
					gameWindow->draw(obMan->entities[i]->moveDest);
					gameWindow->draw(obMan->entities[i]->line.vertices, 4, sf::Quads);
				}
			}
		}
	}

	if (!obMan->selectedEnts.empty())
	{
		for (size_t i = 0; i < obMan->selectedEnts.size(); i++)
		{
			engine::graphics::outline(*gameWindow, obMan->selectedEnts[i]->sprite, 2, sf::Color::Yellow);
		}
	}

	// ------------- ANCHOR
	gameWindow->setView(*viewAnchor);

	ui->Render();
	sf::Vector2f pos;
	pos.x = (mainView->getCenter().x - 98.5f);
	pos.y = (mainView->getCenter().y - 144.5f);
//	engine::text::draw(*gameWindow, text, std::to_string(obMan->entities.size()), pos);
	ui->unitCounterText.setString(std::to_string(obMan->entities.size()));

	// debug info like coordinates and stuff
	if (engine::cl_debug)
	{
		// view coordinates
		std::string x = "X: " + std::to_string(static_cast<int>(mainView->getCenter().x));
		std::string y = "Y: " + std::to_string(static_cast<int>(mainView->getCenter().y));
		engine::text::draw(*gameWindow, text, x + " " + y, sf::Vector2f(mainView->getCenter().x - 199, mainView->getCenter().y - 150));

		gameWindow->draw(frameCounter);
	}

	gameWindow->display();
}

// Private

void Game::showObjectCoords(sf::RenderWindow &window, sf::Sprite &object)
{
	std::string coords =
		"X: " +
		std::to_string(static_cast<int>(object.getPosition().x)) +
		" Y: " +
		std::to_string(static_cast<int>(object.getPosition().y));

	float x = object.getPosition().x + object.getLocalBounds().width / 4;
	float y = object.getPosition().y - 5;
	sf::Vector2f position(x, y);

	engine::text::draw(window, text, coords, position, 34);
}
