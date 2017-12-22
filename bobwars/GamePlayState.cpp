#include "GamePlayState.hpp"
#include "GamePauseState.hpp"

#include "Interface.hpp"
#include "ObjectManager.hpp"
//#include "Button.hpp"

#include <ENGINE\Engine.hpp>
#include <ENGINE\Logger.hpp>
#include <ENGINE\Graphics\Graphics.hpp>
#include <ENGINE\Graphics\Text.hpp>

GamePlayState GamePlayState::GamePlayState_dontfuckwithme;

void GamePlayState::Init(AppEngine* app_)
{
	app = app_;

	logger::INFO("GamePlayState Init...");

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
		debugText.setFont(Arial);
		debugText.setScale(sf::Vector2f(.2f, .2f));
	}

	logger::INFO("Loading world texture...");

	if (!worldTexture.loadFromFile("resource\\textures\\world.png"))
		logger::SILENT("ERROR", "Failed to load world textures!");

	world.setSize(sf::Vector2f(800, 600));
	world.setTexture(&worldTexture);

	logger::INFO("Preparing user interface elements...");
	debugFrameCounter.setFont(Arial);
	debugFrameCounter.setScale(sf::Vector2f(.2f, .2f));

	sf::Vector2f screendimensions;
	screendimensions.x = app->window->getSize().x / 2.0f;
	screendimensions.y = app->window->getSize().y / 2.0f;
	mainView = new sf::View(screendimensions, screendimensions);
	viewAnchor = new sf::View(screendimensions, screendimensions);

	ui = new Interface(app->window, viewAnchor, mainView);

	baseViewSpeed = 500;

	logger::INFO("GamePlayState ready!");
}

void GamePlayState::Cleanup()
{
	logger::INFO("GamePlayState cleaning up");

	app->window->setView(app->window->getDefaultView());

	delete viewAnchor;
	delete mainView;
	delete obMan;
	delete ui;

	logger::INFO("GamePlayState cleaned up");
}

// Public

void GamePlayState::Pause()
{
	app->window->setView(app->window->getDefaultView());

	logger::INFO("GamePlayState paused");
}

void GamePlayState::Resume()
{
	logger::INFO("GamePlayState resumed");
}

void GamePlayState::HandleEvents()
{
	timePerFrame = sf::seconds(1.0f / 60.0f); // 60 frames per second
	timeSinceLastUpdate = sf::Time::Zero;

	sf::Time deltaTime = deltaClock.restart();  // get elapsed time and reset clock
	timeSinceLastUpdate += deltaTime;

	bool should_screenshot(false);

//	while (timeSinceLastUpdate >= timePerFrame)
	{
		timeSinceLastUpdate -= timePerFrame;

		sf::Event event;
		while (app->window->pollEvent(event))
		{
			app->events.push_back(event);

			if (event.type == sf::Event::EventType::Closed)
			{
				app->Quit();
			}
			else if (event.type == sf::Event::EventType::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Key::Escape)
				{
					app->PushState(GamePauseState::Instance());
				}
				else if (event.key.code == sf::Keyboard::Key::Space)
				{
					if (!obMan->selectedEnts.empty())
					{
						logger::INFO("centering mainview on selected entity");

						mainView->setCenter(obMan->selectedEnts[0]->sprite.getPosition());
						viewAnchor->setCenter(obMan->selectedEnts[0]->sprite.getPosition());
					}
				}
				else if (event.key.code == sf::Keyboard::Key::LShift)
				{
					baseViewSpeed = 250;
				}
				else if (event.key.code == sf::Keyboard::Key::F12)
				{
					should_screenshot = true;
				}
				else if (event.key.code == sf::Keyboard::Key::Tilde)
				{
					app->debugModeActive = !app->debugModeActive;

					logger::INFO("cl_debug set to " + std::to_string(app->debugModeActive));
				}
				else if (event.key.code == sf::Keyboard::Key::Delete)
				{
					//TODO: make this all a function so that it isn't duplicated from the interface, or the keyboard, and both do the same
					if (!obMan->selectedEnts.empty())
					{
						if (obMan->selectedEnts.size() > 1)
						{
							for (size_t i = 0; i < obMan->selectedEnts.size(); i++)
							{
								obMan->deleteObject(obMan->selectedEnts[i]);

								logger::INFO("deleted entity " + std::to_string(obMan->selectedEnts[i]->id));
							}

							obMan->deselectAllObjects();
						}
						else
						{
							obMan->deleteObject(obMan->selectedEnts.front());
							obMan->deselectAllObjects();
						}

						ui->delete_ent_button.disable();

						if (ui->create_ent_button.disabled && obMan->entities.size() < 100)
							ui->create_ent_button.enable();
					}
				}
				else if (event.key.code == sf::Keyboard::Key::LControl || event.key.code == sf::Keyboard::Key::A)
				{
					//TODO: this probably isn't the best way to handle key combinations

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
					{
						for (size_t i = 0; i < obMan->entities.size(); i++)
						{
							obMan->entities[i]->isSelected = true;
							obMan->selectedEnts.push_back(obMan->entities[i]);

							logger::INFO("selected entity" + std::to_string(obMan->entities[i]->id));
						}

						logger::INFO("selected all entities");
					}
				}
			}
			else if (event.type == sf::Event::EventType::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Key::LShift)
				{
					baseViewSpeed = 500;
				}
			}
			else if (event.type == sf::Event::EventType::MouseButtonPressed)
			{
				if (event.key.code == sf::Mouse::Button::Left)
				{
					if (engine::logic::mouseIsOver(ui->create_ent_button.m_shape, *app->window, *viewAnchor)) // create new entity
					{
						if (obMan->entities.size() >= 100)
						{
							logger::INFO("Cannot create anymore units; you have too many.");
						}
						else
						{
							obMan->deselectAllObjects();
							obMan->createNewObject();
							ui->delete_ent_button.enable();

							if (obMan->entities.size() >= 100)
							{
								ui->create_ent_button.disable();
							}
						}

						break; //TODO: do we need this?
					}
					else if (engine::logic::mouseIsOver(ui->delete_ent_button.m_shape, *app->window, *viewAnchor) && !obMan->selectedEnts.empty())
					{
						//TODO: make this all a function so that it isn't duplicated from the interface, or the keyboard, and both do the same
						if (obMan->selectedEnts.size() > 1)
						{
							for (size_t i = 0; i < obMan->selectedEnts.size(); i++)
							{
								obMan->deleteObject(obMan->selectedEnts[i]);

								logger::INFO("deleted entity " + std::to_string(obMan->selectedEnts[i]->id));
							}

							obMan->deselectAllObjects();
						}
						else
						{
							obMan->deleteObject(obMan->selectedEnts.front());
							obMan->deselectAllObjects();
						}

						ui->delete_ent_button.disable();

						break; //TODO: do we need this?
					}

					// if we haven't broken the loop already, it means we either clicked an entity or clicked nothing
					bool selectedNothing(true);
					for (size_t i = 0; i < obMan->entities.size(); i++)
					{
						if (engine::logic::mouseIsOver(obMan->entities[i]->sprite, *app->window, *mainView))
						{
							logger::INFO("mouse is over entity" + std::to_string(obMan->entities[i]->id));

							if (obMan->entities[i]->isSelected)
							{
								if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl))
								{
									obMan->deselectObject(obMan->entities[i]);
									selectedNothing = false;
								}
								else
								{
									logger::INFO("entity" + std::to_string(obMan->entities[i]->id) + " is already selected");
								}
							}
							else
							{
								if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl))
									obMan->deselectAllObjects();

								obMan->selectObject(obMan->entities[i]);

								logger::INFO("selected entity" + std::to_string(obMan->entities[i]->id));
								ui->delete_ent_button.enable();

								selectedNothing = false;
							}

							break;
						}
					} // what entity did we click

					if (selectedNothing && !obMan->selectedEnts.empty()) // selected nothing and didn't already have nothing
					{
						obMan->deselectAllObjects();
						ui->delete_ent_button.disable();

						logger::INFO("All entities deselected");
						break;
					}
				} // left mouse button

				if (event.key.code == sf::Mouse::Button::Right)
				{
					if (!obMan->selectedEnts.empty())
					{
						sf::Vector2f movePos(app->window->mapPixelToCoords(sf::Mouse::getPosition(*app->window), *mainView));

						if (!world.getGlobalBounds().contains(movePos))
						{
							logger::ERROR("Cannot move target out of bounds!");
						}
						else
						{
							for (size_t i = 0; i < obMan->selectedEnts.size(); i++)
							{
								obMan->selectedEnts[i]->moveTo(movePos);
							}
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

			if (app->debugModeActive && !obMan->selectedEnts.size() == 1)
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

				debugFrameCounter.setPosition(mainView->getCenter().x - 199, mainView->getCenter().y - 130);
				debugFrameCounter.setString("FPS: " + std::to_string(static_cast<int>(1.0f / frames_per_second)));
			}
		} // app->window.hasFocus()

		viewAnchor->setCenter(mainView->getCenter());

		if (should_screenshot)
		{
			engine::screenshot(*app->window);

			app->window->clear(sf::Color::White);
			app->window->display();

			should_screenshot = false;
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

	if (app->debugModeActive)
	{
		for (size_t i = 0; i < obMan->entities.size(); i++) // outline entities
		{
			if (!obMan->entities.empty())
			{
				showObjectCoords(obMan->entities[i]->sprite);
				engine::graphics::outline(*app->window, obMan->entities[i]->sprite, 2, sf::Color::Red);
				engine::text::draw(*app->window, debugText, std::to_string(obMan->entities[i]->id) + "/" + std::to_string(obMan->entities.size()), sf::Vector2f(obMan->entities[i]->sprite.getPosition().x, obMan->entities[i]->sprite.getPosition().y));

				if (obMan->entities[i]->moving)
				{
					static Line line;

					if (obMan->entities[i]->isSelected)
					{
						obMan->entities[i]->moveDest.setFillColor(sf::Color::Yellow);
						line.setColor(sf::Color::Yellow);
					}
					else
					{
						line.setColor(sf::Color::Red);
						obMan->entities[i]->moveDest.setFillColor(sf::Color::Red);
					}

					app->window->draw(obMan->entities[i]->moveDest);
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
	if (app->debugModeActive)
	{
		// view coordinates
		std::string x = "X: " + std::to_string(static_cast<int>(mainView->getCenter().x));
		std::string y = "Y: " + std::to_string(static_cast<int>(mainView->getCenter().y));
		engine::text::draw(*app->window, debugText, x + " " + y, sf::Vector2f(debugFrameCounter.getPosition().x, debugFrameCounter.getPosition().y + 6));
		engine::text::draw(*app->window, debugText, "selectedEntities: " + std::to_string(obMan->selectedEnts.size()), sf::Vector2f(debugFrameCounter.getPosition().x, debugFrameCounter.getPosition().y + 12));
		engine::text::draw(*app->window, debugText, "totalEntities: " + std::to_string(obMan->entities.size()), sf::Vector2f(debugFrameCounter.getPosition().x, debugFrameCounter.getPosition().y + 18));

		app->window->draw(debugFrameCounter);
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

	engine::text::draw(*app->window, debugText, coords, position, 34);
}
