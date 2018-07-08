#include "GamePlayState.hpp"
#include "GamePauseState.hpp"

#include "Interface.hpp"
#include "EntityManager.hpp"

#include "Util/Util.hpp"
#include "Util/Logger.hpp"
#include "Util/Graphics/Line.hpp"
#include "Util/Graphics/Graphics.hpp"
#include "Util/Graphics/Text.hpp"

GamePlayState GamePlayState::GamePlayState_dontfuckwithme;

void GamePlayState::Init(AppEngine* app_)
{
	app = app_;

	logger::INFO("GamePlayState Init...");

	logger::INFO("Pre-game setup.");

	entMan = new EntityManager;

	if (!Arial.loadFromFile("C:/Windows/Fonts/Arial.ttf"))
	{
		logger::ERROR("Failed to load font Arial!", true);
	}
	else
	{
		debugText.setFont(Arial);
		debugText.setCharacterSize(14);
	}

	logger::INFO("Loading world texture...");

	worldTexture = new sf::Texture; // fix not running in debug

	if (!worldTexture->loadFromFile("bobwars/resource/textures/world.png"))
		logger::ERROR("Failed to load world textures!", true);

	//TODO: make camera align with world center on game start
	world.setSize(sf::Vector2f(800, 600));
	world.setTexture(*&worldTexture);

	logger::INFO("Preparing user interface elements...");
	debugFrameCounter.setFont(Arial);
	debugFrameCounter.setPosition(4, 40);
	debugFrameCounter.setCharacterSize(14);

	sf::Vector2f screendimensions;
	screendimensions.x = app->window->getSize().x / 2;
	screendimensions.y = app->window->getSize().y / 2;

	mainView2 = new Camera;
	{
		sf::View tempViewBecuaseIDonTKnowTheCorrectFunctionCallsToAchieveWhatTheConstructorDoes(screendimensions, screendimensions);
		mainView2->view = tempViewBecuaseIDonTKnowTheCorrectFunctionCallsToAchieveWhatTheConstructorDoes;
	}

//	viewAnchor = new sf::View(screendimensions, sf::Vector2f(app->window->getSize().x, app->window->getSize().y));

	ui = new Interface(app->window, &mainView2->view);

	baseViewSpeed = 500;

	entMan->newCommentSection()->setPosition(sf::Vector2f(app->window->getSize().x / 2, app->window->getSize().y / 2));

	logger::INFO("GamePlayState ready!");
}

void GamePlayState::Cleanup()
{
	logger::INFO("GamePlayState cleaning up");

	app->window->setView(app->window->getDefaultView());

//	delete viewAnchor;
	delete mainView2;
	delete entMan;
	delete ui;
	delete worldTexture;

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

//	if (timeSinceLastUpdate >= timePerFrame)
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
					if (!entMan->selectedEnts.empty())
					{
						logger::INFO("centering mainview on selected entity");

						mainView2->setCenter(entMan->selectedEnts[0]->sprite.getPosition());
//						viewAnchor->setCenter(entMan->selectedEnts[0]->sprite.getPosition());

						mainView2->view.setRotation(0);
					}
				}
				else if (event.key.code == sf::Keyboard::Key::LShift)
				{
					baseViewSpeed = 250;
				}
				else if (event.key.code == sf::Keyboard::Key::F12)
				{
					util::screenshot(*app->window);

					app->window->clear(sf::Color::White);
					app->window->display();
				}
				else if (event.key.code == sf::Keyboard::Key::Tilde)
				{
					app->debugModeActive = !app->debugModeActive;

					logger::INFO("cl_debug set to " + std::to_string(app->debugModeActive));
				}
				else if (event.key.code == sf::Keyboard::Key::Delete || event.key.code == sf::Keyboard::Key::End)
				{
					deleteButton();
				}
				else if (event.key.code == sf::Keyboard::Key::LControl || event.key.code == sf::Keyboard::Key::A || event.key.code == sf::Keyboard::Key::Equal || sf::Keyboard::Key::Dash)
				{
					//TODO: this probably isn't the best way to handle key combinations
					//TODO: really find a better way to do this.

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl))
					{
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && !entMan->entities.empty())
						{
							entMan->deselectAllEnts();
							entMan->selectedEnts = entMan->entities;

							for (size_t i = 0; i < entMan->entities.size(); i++)
								entMan->entities[i]->isSelected = true;

							ui->deleteEnabled = true;
//							ui->delete_ent_button.enable();

							logger::INFO("selected " + std::to_string(entMan->selectedEnts.size()) + " entities (of " + std::to_string(entMan->entities.size()) + ")");
						}
						else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Equal))
						{
							mainView2->zoom(0.5f);
							// zoom camera in
						}
						else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Dash))
						{
							mainView2->zoom(2.0f);
							// zoom camera out
						}
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
					if (ui->delete_ent_button->containsPoint(sf::Vector2f(sf::Mouse::getPosition(*app->window).x, sf::Mouse::getPosition(*app->window).y)))
					{
						if (entMan->entities.size() >= entMan->maxEnts)
						{
							ui->createEnabled = false;
//							ui->create_ent_button.disable();
							ui->unitCounterText.setFillColor(sf::Color::Red);
							logger::INFO("Unit cap reached.");
						}
						else
						{
							entMan->deselectAllEnts();
							entMan->newBob();

							ui->deleteEnabled = true;
//							ui->delete_ent_button.enable();

							if (entMan->entities.size() >= entMan->maxEnts)
							{
								ui->createEnabled = false;
//								ui->create_ent_button.disable();
								ui->unitCounterText.setFillColor(sf::Color::Red);
								logger::INFO("Unit cap reached.");
							}
						}

						break;
					}
					else if (ui->delete_ent_button->containsPoint(sf::Vector2f(sf::Mouse::getPosition(*app->window).x, sf::Mouse::getPosition(*app->window).y)) && !entMan->selectedEnts.empty())
					{

						deleteButton();
					}

					// if we haven't broken the loop already, it means we either clicked an entity or clicked nothing
					bool selectedNothing(true);
					for (size_t i = 0; i < entMan->entities.size(); i++)
					{
						if (util::logic::mouseIsOver(entMan->entities[i]->sprite, *app->window, mainView2->view))
						{
							if (entMan->entities[i]->isSelected)
							{
								if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl))
								{
									entMan->deselectEnt(entMan->entities[i]);
									selectedNothing = false;
								}
								else
								{
									if (!entMan->selectedEnts.empty())
									{
										entMan->deselectAllEnts();
										entMan->selectEnt(entMan->entities[i]);

										logger::INFO("selected entity" + std::to_string(entMan->entities[i]->entityID));

										selectedNothing = false;
									}
									else
									{
										logger::INFO("entity" + std::to_string(entMan->entities[i]->entityID) + " is already selected");
										selectedNothing = false;
									}
								}
							}
							else
							{
								if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl))
									entMan->deselectAllEnts();

								entMan->selectEnt(entMan->entities[i]);

								logger::INFO("selected entity" + std::to_string(entMan->entities[i]->entityID));

								ui->deleteEnabled = true;
//								ui->delete_ent_button.enable();

								selectedNothing = false;
							}

							break;
						}
					} // what entity did we click

					if (selectedNothing && !entMan->selectedEnts.empty()) // selected nothing and didn't already have nothing
					{
						entMan->deselectAllEnts();

						ui->deleteEnabled = false;
//						ui->delete_ent_button.disable();

						logger::INFO("All entities deselected");
						break;
					}
				} // left mouse button
				else if (event.key.code == sf::Mouse::Button::Right)
				{
					if (!entMan->selectedEnts.empty())
					{
						sf::Vector2f movePos(app->window->mapPixelToCoords(sf::Mouse::getPosition(*app->window), mainView2->view));

						if (!world.getGlobalBounds().contains(movePos))
						{
							logger::INFO("Cannot move target out of bounds!");
						}
						else
						{
							for (size_t i = 0; i < entMan->selectedEnts.size(); i++)
							{
								entMan->selectedEnts[i]->moveTo(movePos);
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
						mainView2->zoom(2.0f);
						zoomlevel *= 2.0f;
					}
				}
				else if (event.mouseWheel.delta > 0) // down (out)
				{
					if (zoomlevel > 0.5f)
					{
						mainView2->zoom(0.5f);
						zoomlevel *= 0.5f;
					}
				}
			}

			ui->HandleEvents(event);
		} // pollevent

		if (app->window->hasFocus())
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
				mainView2->move(sf::Vector2f(0, -baseViewSpeed * timePerFrame.asSeconds()));
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
				mainView2->move(sf::Vector2f(-baseViewSpeed * timePerFrame.asSeconds(), 0));
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
				mainView2->move(sf::Vector2f(0, baseViewSpeed * timePerFrame.asSeconds()));
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
				mainView2->move(sf::Vector2f(baseViewSpeed * timePerFrame.asSeconds(), 0));

			//TODO: this is probably really bad
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
			{
				mainView2->setRotation(0);
			}
			else
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
					mainView2->rotate(1);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
					mainView2->rotate(-1);
			}

			if (mainView2->getCenter().x > 800)
				mainView2->setCenter(sf::Vector2f(800, mainView2->getCenter().y));
			if (mainView2->getCenter().y > 600)
				mainView2->setCenter(sf::Vector2f(mainView2->getCenter().x, 600));
			if (mainView2->getCenter().x < 0)
				mainView2->setCenter(sf::Vector2f(0, mainView2->getCenter().y));
			if (mainView2->getCenter().y < 0)
				mainView2->setCenter(sf::Vector2f(mainView2->getCenter().x, 0));

			{ //FRAMES PER SECOND
				float frames_per_second = framesClock.restart().asSeconds();

				debugFrameCounter.setString("FPS: " + std::to_string(static_cast<int>(1.0f / frames_per_second)));
			}
		} // app->window.hasFocus()

		sf::Time deltaTime = deltaClock.restart();  // get elapsed time and reset clock
		timeSinceLastUpdate += deltaTime;
	}
}

void GamePlayState::Update()
{
	for (size_t i = 0; i < entMan->entities.size(); i++)
		entMan->entities[i]->Update();
}

void GamePlayState::Draw()
{
	app->window->clear();
	
	// ------------ MAIN VIEW
	app->window->setView(mainView2->view);
	app->window->draw(world);

	for (size_t i = 0; i < entMan->entities.size(); i++)
		app->window->draw(entMan->entities[i]->sprite);

	if (app->debugModeActive)
	{
		for (size_t i = 0; i < entMan->entities.size(); i++) // outline entities
		{
			if (!entMan->entities.empty())
			{
				util::graphics::outline(*app->window, entMan->entities[i]->sprite, 2, sf::Color::Red);
				showObjectCoords(entMan->entities[i]->sprite);
				util::text::draw(*app->window, debugText, std::to_string(entMan->entities[i]->entityID) + "/" + std::to_string(entMan->entities.size()), sf::Vector2f(entMan->entities[i]->sprite.getPosition().x, entMan->entities[i]->sprite.getPosition().y - entMan->entities[i]->sprite.getLocalBounds().height / 2), sf::Vector2f(.2f, .2f));
				util::text::draw(*app->window, debugText, entMan->entities[i]->type, sf::Vector2f(entMan->entities[i]->sprite.getPosition().x, entMan->entities[i]->sprite.getPosition().y), sf::Vector2f(.2f, .2f));

				if (entMan->entities[i]->moving)
				{
					static Line line;

					if (entMan->entities[i]->isSelected)
					{
						entMan->entities[i]->moveDest.setFillColor(sf::Color::Yellow);
						line.setColor(sf::Color::Yellow);
					}
					else
					{
						line.setColor(sf::Color::Red);
						entMan->entities[i]->moveDest.setFillColor(sf::Color::Red);
					}

					app->window->draw(entMan->entities[i]->moveDest);
					line.setPoints(entMan->entities[i]->sprite.getPosition(), entMan->entities[i]->moveDest.getPosition());

					app->window->draw(line.vertices, 4, sf::Quads);
				}
			}
		}
	}

	if (!entMan->selectedEnts.empty())
		for (size_t i = 0; i < entMan->selectedEnts.size(); i++)
			util::graphics::outline(*app->window, entMan->selectedEnts[i]->sprite, 2, sf::Color::Yellow);

	// ------------- ANCHOR
	app->window->setView(*ui->getViewAnchor());

	ui->unitCounterText.setString(std::to_string(entMan->entities.size()) + "/" + std::to_string(entMan->maxEntsPerTeam));

	ui->Draw();

	// debug info like coordinates and stuff
	if (app->debugModeActive)
	{
		app->window->draw(debugFrameCounter);

		// view coordinates
		std::string x = "X: " + std::to_string(static_cast<int>(mainView2->getCenter().x));
		std::string y = "Y: " + std::to_string(static_cast<int>(mainView2->getCenter().y));
		util::text::draw(*app->window, debugText, x + " " + y, sf::Vector2f(debugFrameCounter.getPosition().x, debugFrameCounter.getPosition().y + 12));
		util::text::draw(*app->window, debugText, "selectedEntities: " + std::to_string(entMan->selectedEnts.size()), sf::Vector2f(debugFrameCounter.getPosition().x, debugFrameCounter.getPosition().y + 24));
		util::text::draw(*app->window, debugText, "totalEntities: " + std::to_string(entMan->entities.size()), sf::Vector2f(debugFrameCounter.getPosition().x, debugFrameCounter.getPosition().y + 36));
		util::text::draw(*app->window, debugText, "maxEntities: " + std::to_string(entMan->maxEnts), sf::Vector2f(debugFrameCounter.getPosition().x, debugFrameCounter.getPosition().y + 48));
		util::text::draw(*app->window, debugText, "physicalMaxEntities: " + std::to_string(entMan->physicalMaxEnts), sf::Vector2f(debugFrameCounter.getPosition().x, debugFrameCounter.getPosition().y + 60));
		util::text::draw(*app->window, debugText, "maxEntitiesPerTeam: " + std::to_string(entMan->maxEntsPerTeam), sf::Vector2f(debugFrameCounter.getPosition().x, debugFrameCounter.getPosition().y + 72));
		util::text::draw(*app->window, debugText, "delta: " + std::to_string(deltaClock.getElapsedTime().asMilliseconds()), sf::Vector2f(debugFrameCounter.getPosition().x, debugFrameCounter.getPosition().y + 84));

		if (!entMan->selectedEnts.empty() && entMan->selectedEnts.size() == 1)
		{
			util::text::draw(*app->window, debugText, "entityID: " + std::to_string(entMan->selectedEnts[0]->entityID), sf::Vector2f(debugFrameCounter.getPosition().x, debugFrameCounter.getPosition().y + 108));
			util::text::draw(*app->window, debugText, "team: " + std::to_string(entMan->selectedEnts[0]->team), sf::Vector2f(debugFrameCounter.getPosition().x, debugFrameCounter.getPosition().y + 120));
			util::text::draw(*app->window, debugText, "type: " + entMan->selectedEnts[0]->type, sf::Vector2f(debugFrameCounter.getPosition().x, debugFrameCounter.getPosition().y + 132));
			util::text::draw(*app->window, debugText, "health: " + std::to_string(entMan->selectedEnts[0]->health), sf::Vector2f(debugFrameCounter.getPosition().x, debugFrameCounter.getPosition().y + 144));
			util::text::draw(*app->window, debugText, "hp: " + std::to_string(entMan->selectedEnts[0]->hitpoints), sf::Vector2f(debugFrameCounter.getPosition().x, debugFrameCounter.getPosition().y + 156));
			util::text::draw(*app->window, debugText, "armor: " + std::to_string(entMan->selectedEnts[0]->armor), sf::Vector2f(debugFrameCounter.getPosition().x, debugFrameCounter.getPosition().y + 168));
			util::text::draw(*app->window, debugText, "movable: " + std::to_string(entMan->selectedEnts[0]->movable), sf::Vector2f(debugFrameCounter.getPosition().x, debugFrameCounter.getPosition().y + 180));
			util::text::draw(*app->window, debugText, "moving: " + std::to_string(entMan->selectedEnts[0]->moving), sf::Vector2f(debugFrameCounter.getPosition().x, debugFrameCounter.getPosition().y + 192));
		}

		sf::RectangleShape top;
		sf::RectangleShape left;
		sf::RectangleShape right;
		sf::RectangleShape bottom;

		float width = 20;

		top.setFillColor(sf::Color(255, 200, 0, 50));
		left.setFillColor(sf::Color(255, 200, 0, 50));
		right.setFillColor(sf::Color(255, 200, 0, 50));
		bottom.setFillColor(sf::Color(255, 200, 0, 50));

		top.setSize(sf::Vector2f(app->window->getSize().x, width)); // -
		left.setSize(sf::Vector2f(width, app->window->getSize().y)); // |
		right.setSize(sf::Vector2f(width, app->window->getSize().y)); // |
		bottom.setSize(sf::Vector2f(app->window->getSize().x, width)); // -

		top.setPosition(sf::Vector2f(0, 0));
//		left.setPosition(sf::Vector2f(0, 0));
		right.setPosition(sf::Vector2f(app->window->getSize().x - width, 0));
		bottom.setPosition(sf::Vector2f(0, app->window->getSize().y - width));

		app->window->draw(top);
		app->window->draw(left);
		app->window->draw(right);
		app->window->draw(bottom);
	}

	app->window->display();
}

// Private

void GamePlayState::deleteButton()
{
	int deleteAmount = entMan->selectedEnts.size();

	if (!entMan->selectedEnts.empty())
	{
		for (size_t i = 0; i < entMan->selectedEnts.size(); i++)
		{
			logger::INFO("deleting entity " + std::to_string(entMan->selectedEnts[i]->entityID));

			entMan->deleteEnt(entMan->selectedEnts[i]);

			// this is here because deleteEnt will delete the object from both entities and selectedEntities,
			// when it does this, those vectors resize themselven. this resize causes the deletion to skip numbers
			// instead of deleting 1 2 3 4, like we expect, it deletes 2 4 6 8.
			// keep this
			i--;
		}

		ui->deleteEnabled = false;
		//ui->delete_ent_button.disable();

		if (entMan->entities.size() < entMan->maxEnts)
			ui->createEnabled = true;
			//ui->create_ent_button.enable();
	}
	else
	{
		logger::INFO("nothing to delete");
	}
}

void GamePlayState::showObjectCoords(sf::Sprite &object)
{
	// TODO: this can be put into draw call, yes?

	std::string coords =
		"X: " +
		std::to_string(static_cast<int>(object.getPosition().x)) +
		" Y: " +
		std::to_string(static_cast<int>(object.getPosition().y));

	float x = object.getPosition().x - object.getLocalBounds().width;
	float y = object.getPosition().y - object.getLocalBounds().height / 1.5;
	sf::Vector2f position(x, y);

	util::text::draw(*app->window, debugText, coords, position, sf::Vector2f(.2f, .2f));
}

void GamePlayState::saveGame()
{
	
}
