#include <fstream>
#include <iostream>
#include <string>

#include "engine_main.hpp"

void write_log(const std::string output)
{
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

namespace logger
{
	void INFO(std::string output)
	{
		output = engine::getTime() + " INFO: " + output;
		std::cout << output << std::endl;

		write_log(output);
	}

	void WARNING(std::string output)
	{
		output = engine::getTime() + " WARNING: " + output;
		std::cout << output << std::endl;

		write_log(output);
	}

	void ERROR(std::string output)
	{
		output = engine::getTime() + " ERROR: " + output;
		std::cout << output << std::endl;

		write_log(output);
	}

	void SILENT(std::string output)
	{
		output = engine::getTime() + " SILENT: " + output;

		write_log(output);
	}

	void DEBUG(std::string output)
	{
		if (engine::cl_debug)
		{
			output = engine::getTime() + " DEBUG: " + output;
			std::cout << output << std::endl;

			write_log(output);
		}
	}
}
