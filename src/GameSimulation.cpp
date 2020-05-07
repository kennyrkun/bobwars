#include "GameSimulation.hpp"

#include "Util/Logger.hpp"

GameSimulation::GameSimulation()
{
    logger::DEBUG("Constructed GameSimulation");
}

void GameSimulation::create(AppEngine* app)
{
    this->app = app;

    logger::DEBUG("Creating game simulation.");
}

void GameSimulation::loadFromFile(const std::string& file)
{
    logger::DEBUG("Loading game simulation.");
    world.loadFromFile(file);
}

void GameSimulation::saveToFile(const std::string& file)
{
    logger::DEBUG("Saving game simulatiodsadsn.");
    world.saveToFile(file);
}

void GameSimulation::createEntity(EntityType type, const sf::Vector2f& position)
{
	if (entMan->entities.size() >= entMan->maxEntsPerTeam)
	{

	}
	else
	{
		logger::ERROR("Cannot create any more entities.");
	}
}

void GameSimulation::deleteEntities(const std::vector<BaseEntity*>& entities)
{

}

void GameSimulation::deleteButton()
{
	app->network.sendCommand("DeleteSelection");

	if (!entMan->selectedEnts.empty())
	{
		for (size_t i = 0; i < entMan->selectedEnts.size(); i++)
		{
			logger::DEBUG("Deleting entity " + std::to_string(entMan->selectedEnts[i]->entityID));

			entMan->deleteEnt(entMan->selectedEnts[i]);

			// this is here because deleteEnt will delete the object from both entities and selectedEntities,
			// when it does this, those vectors resize themselves. this resize causes the deletion to skip numbers
			// instead of deleting 1 2 3 4, like we expect, it deletes 2 4 6 8.
			// keep this
			// TODO: i think this is bullshit
			i--;
		}

		ui->deleteEnabled = false;

		if (entMan->entities.size() < entMan->maxEnts)
			ui->createEnabled = true;
	}
	else
	{
		logger::INFO("nothing to delete");
	}

	ui->updateSelectionInfo(entMan->selectedEnts);
}

void GameSimulation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(world);
}