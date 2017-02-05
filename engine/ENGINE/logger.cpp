#include <fstream>
#include <iostream>
#include <string>

#include "engine_main.hpp"

namespace logger
{
	void write(std::string output)
	{
		std::cout << output << std::endl;

		std::ofstream log("bobwars.log", std::ios::app);

		if (log.is_open())
		{
			log << output << std::endl;

			log.close();

			if (log.bad())
			{
				std::cerr << "failed to write to log!" << std::endl;
			}
		}
		else
		{
			std::cerr << "\n" << "unable to open log file for writing!" << std::endl;
		}
	}

	void INFO(std::string output)
	{
		output = engine::getTime() + " INFO: " + output;

		write(output);
	}

	void WARNING(std::string output)
	{
		output = engine::getTime() + " WARNING: " + output;

		write(output);
	}

	void ERROR(int type, std::string output)
	{
		if (type == 0)
		{
			output = engine::getTime() + " ERROR: " + output;
		}
		else if (type == 1)
		{
			output = engine::getTime() + " ERROR#FATAL: " + output;
		}
		else
		{
			output = engine::getTime() + " ERROR#UNKOWN: " + output;
		}

		write(output);
	}

	void DEBUG(std::string output)
	{
		output = engine::getTime() + " DEBUG#INFO: " + output;
		write(output);
	}
}