#include "GamePlayState.hpp"

#include "Interface.hpp"
#include "ObjectManager.hpp"
//#include "Button.hpp"

#include <ENGINE\Engine.hpp>
#include <ENGINE\Logger.hpp>
#include <ENGINE\Graphics\Graphics.hpp>
#include <ENGINE\Graphics\Text.hpp>

GamePlayState::GamePlayState(AppEngine2* app_, bool fullscreen, bool vsync)
{
	engine::cl_debug = true;

	app = app_;

	logger::INFO("Initialising...");

	{
		app->window->setTitle("bobwars " + gameVersion + "-" + engine::version);

//		if (fullscreen)
//			app->window->create(app->windowDimensions, app->windowTitle, sf::Style::Fullscreen);

//		else
//			app->window->create(app->windowDimensions, app->windowTitle, sf::Style::Titlebar | sf::Style::Close);

//		if (vsync)
//			app->window->setVerticalSyncEnabled(true);
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

	if (!worldTexture.loadFromFile("resource\\textures\\world.png"))
		logger::SILENT("ERROR", "Failed to load world textures!");

	world.setSize(sf::Vector2f(800, 600));
	world.setTexture(&worldTexture);

	logger::INFO("Preparing user interface elements...");
	frameCounter.setFont(Arial);
	frameCounter.setScale(sf::Vector2f(.2f, .2f));

	sf::Vector2f screendimensions;
	screendimensions.x = app->window->getSize().x / 2.0f;
	screendimensions.y = app->window->getSize().y / 2.0f;
	mainView = new sf::View(screendimensions, screendimensions);
	viewAnchor = new sf::View(screendimensions, screendimensions);

	ui = new Interface(app->window, viewAnchor, mainView);

	logger::INFO("New Game created. (Ready!)");
}

GamePlayState::~GamePlayState()
{
	delete viewAnchor;
	delete mainView;
	delete obMan;
	delete ui;

	delete app;

	logger::INFO("Game destroyed.");
}

// Public

void GamePlayState::Pause()
{
	logger::INFO("game paused");
}

void GamePlayState::Resume()
{
	logger::INFO("game resumed");
}

void GamePlayState::HandleEvents()
{
	timePerFrame = sf::seconds(1.0f / 60.0f); // 60 frames per second
	timeSinceLastUpdate = sf::Time::Zero;

	sf::Time deltaTime = deltaClock.restart();  // get elapsed time and reset clock
	timeSinceLastUpdate += deltaTime;

	bool should_screenshot(false);

	while (timeSinceLastUpdate >= timePerFrame)
	{
		timeSinceLastUpdate -= timePerFrame;

		sf::Event event;
		while (app->window->pollEvent(event))
		{
			if (event.type == sf::Event::EventType::Closed)
			{
				app->Quit();
			}

			//---------------KEYBOARD
			else if (event.type == sf::Event::EventType::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Key::Escape)
				{
					logger::INFO("Pause has function not yet been implemented.");

//					app->Quit();
				}

				if (event.key.code == sf::Keyboard::Key::Space)
				{
					if (!obMan->selectedEnts.empty())
					{
						logger::INFO("centering mainview on selected entity");

						mainView->setCenter(obMan->selectedEnts[0]->sprite.getPosition());
						viewAnchor->setCenter(obMan->selectedEnts[0]->sprite.getPosition());
					}
				}

				if (event.key.code == sf::Keyboard::Key::LShift)
					baseViewSpeed = 250;

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
					baseViewSpeed = 500;
			}

			//---------------MOUSE
			else if (event.type == sf::Event::EventType::MouseButtonPressed)
			{
				if (event.key.code == sf::Mouse::Button::Left)
				{
					if (engine::logic::mouseIsOver(ui->create_ent_button.m_shape, *app->window, *viewAnchor))
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
					else if (engine::logic::mouseIsOver(ui->delete_ent_button.m_shape, *app->window, *viewAnchor) && !obMan->selectedEnts.empty())
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
						if (engine::logic::mouseIsOver(obMan->entities[i]->sprite, *app->window, *mainView))
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
						sf::Vector2f movePos(app->window->mapPixelToCoords(sf::Mouse::getPosition(*app->window), *mainView));

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
						zoomlevel *= 2.0f;
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

		if (app->window->hasFocus())
		{
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
					mainView->move(0, -baseViewSpeed * timePerFrame.asSeconds());
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
					mainView->move(-baseViewSpeed * timePerFrame.asSeconds(), 0);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
					mainView->move(0, baseViewSpeed * timePerFrame.asSeconds());
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
					mainView->move(baseViewSpeed * timePerFrame.asSeconds(), 0);

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
				#define PLAYER_SPEED 250
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
					obMan->selectedEnts[0]->sprite.move(0, -PLAYER_SPEED * timePerFrame.asSeconds());
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
					obMan->selectedEnts[0]->sprite.move(-PLAYER_SPEED * timePerFrame.asSeconds(), 0);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
					obMan->selectedEnts[0]->sprite.move(0, PLAYER_SPEED * timePerFrame.asSeconds());
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
					obMan->selectedEnts[0]->sprite.move(PLAYER_SPEED * timePerFrame.asSeconds(), 0);
				#undef PLAYER_SPEED	
			}

			{ //FRAMES PER SECOND
				float frames_per_second = framesClock.restart().asSeconds();

				frameCounter.setPosition(mainView->getCenter().x - 199, mainView->getCenter().y - 130);
				frameCounter.setString("FPS: " + std::to_string(static_cast<int>(1.0f / frames_per_second)));
			}
		} // app->window.hasFocus()

		viewAnchor->setCenter(mainView->getCenter());

		Update();

		if (should_screenshot)
		{
			engine::screenshot(*app->window);

			app->window->clear(sf::Color::White);
			app->window->display();

			should_screenshot = false;
		}
		else
		{
			Draw();
		}
	}
}

void GamePlayState::Update()
{
	for (size_t i = 0; i < obMan->entities.size(); i++)
	{
		if (obMan->entities[i]->moving)
		{
			obMan->entities[i]->Update();
		}
	}
}

void GamePlayState::Draw()
{
	app->window->clear();

	// ------------ MAIN VIEW
	app->window->setView(*mainView);
	app->window->draw(world);

	for (size_t i = 0; i < obMan->entities.size(); i++)
		app->window->draw(obMan->entities[i]->sprite);

	if (engine::cl_debug)
	{
		for (size_t i = 0; i < obMan->entities.size(); i++) // outline entities
		{
			if (!obMan->entities.empty())
			{
				showObjectCoords(obMan->entities[i]->sprite);
				engine::graphics::outline(*app->window, obMan->entities[i]->sprite, 2, sf::Color::Red);
				engine::text::draw(*app->window, text, std::to_string(obMan->entities[i]->id) + "/" + std::to_string(obMan->entities.size()), sf::Vector2f(obMan->entities[i]->sprite.getPosition().x, obMan->entities[i]->sprite.getPosition().y));

				if (obMan->entities[i]->moving)
				{
					app->window->draw(obMan->entities[i]->moveDest);

					static Line line;
					line.setPoints(obMan->entities[i]->sprite.getPosition(), obMan->entities[i]->moveDest.getPosition());
					app->window->draw(line.vertices, 4, sf::Quads);
				}
			}
		}
	}

	if (!obMan->selectedEnts.empty())
	{
		for (size_t i = 0; i < obMan->selectedEnts.size(); i++)
		{
			engine::graphics::outline(*app->window, obMan->selectedEnts[i]->sprite, 2, sf::Color::Yellow);
		}
	}

	// ------------- ANCHOR
	app->window->setView(*viewAnchor);

	ui->Draw();
//	sf::Vector2f pos;
//	pos.x = (mainView->getCenter().x - 98.5f);
//	pos.y = (mainView->getCenter().y - 144.5f);
//	engine::text::draw(*app->window, text, std::to_string(obMan->entities.size()), pos);
	ui->unitCounterText.setString(std::to_string(obMan->entities.size()));

	// debug info like coordinates and stuff
	if (engine::cl_debug)
	{
		// view coordinates
		std::string x = "X: " + std::to_string(static_cast<int>(mainView->getCenter().x));
		std::string y = "Y: " + std::to_string(static_cast<int>(mainView->getCenter().y));
		engine::text::draw(*app->window, text, x + " " + y, sf::Vector2f(frameCounter.getPosition().x, frameCounter.getPosition().y + 6));

		app->window->draw(frameCounter);
	}

	app->window->display();
}

// Private

void GamePlayState::showObjectCoords(sf::Sprite &object)
{
	std::string coords =
		"X: " +
		std::to_string(static_cast<int>(object.getPosition().x)) +
		" Y: " +
		std::to_string(static_cast<int>(object.getPosition().y));

	float x = object.getPosition().x + object.getLocalBounds().width / 4;
	float y = object.getPosition().y - 5;
	sf::Vector2f position(x, y);

	engine::text::draw(*app->window, text, coords, position, 34);
}
