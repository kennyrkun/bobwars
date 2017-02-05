#pragma once

#include <SFML\Graphics.hpp>
#include <ENGINE\engine_main.hpp>

#include "game.hpp"

namespace GUI
{
	/*** UI COLOUR PALETTE ***/
	// background = 58, 58, 58
	// background2 = 100, 100, 100
	// button-default = 125, 125, 125
	// button-useable = 190, 190, 190
	/*************************/

	void init()
	{
		logger::INFO("prepping graphics");

		if (world_tex.loadFromFile("resource\\textures\\world.png"))
		{
			world.setTexture(&world_tex);
		}

		if (player_tex.loadFromFile("resource\\textures\\player.png"))
		{
			player.setTexture(player_tex);
		}

		bottom.setFillColor(sf::Color(58, 58, 58));
		top.setFillColor(sf::Color(58, 58, 58));

		main_view.setViewport(sf::FloatRect(0, 0, 1, 1));

		logger::INFO("done");
	}
}
