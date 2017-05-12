#include <ENGINE\Engine.hpp>

#include <fstream>
#include <iostream>
#include <string>

std::string write_dir = "bobwars.log";

void write_log(const std::string output)
{
	std::ofstream log((write_dir), std::ios::app);

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
		output = engine::getTimestamp() + " INFO: " + output;

		std::cout << output << std::endl;
		write_log(output);
	}

	void WARNING(std::string output)
	{
		output = engine::getTimestamp() + " WARNING: " + output;
		std::cout << output << std::endl;

		write_log(output);
	}

	void ERROR(std::string output)
	{
		output = engine::getTimestamp() + " ERROR: " + output;
		std::cout << output << std::endl;

		write_log(output);
	}

	void SILENT(std::string type, std::string output)
	{
		output = engine::getTimestamp() + " " + type + ": " + output;

		write_log(output);
	}

	void CUSTOM(std::string type, std::string output)
	{
		output = engine::getTimestamp() + " " + type + ": " + output;
		std::cout << output << std::endl;

		write_log(output);
	}

	void BREAK()
	{
		std::cout << std::endl;
	}

	void setOutputDir(const std::string dir)
	{
		write_dir = dir;
	}

	void setOutputDir(const std::string dir, const std::string filename)
	{
		write_dir = dir + "//" + filename + ".log";
	}
}
