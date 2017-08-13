#include <ENGINE\Engine.hpp>

#include <fstream>
#include <iostream>
#include <string>

std::string write_dir = "bobwars.log";

void writeLog(const std::string output)
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
		writeLog(output);
	}

	void WARNING(std::string output)
	{
		output = engine::getTimestamp() + " WARNING: " + output;
		std::cout << output << std::endl;

		writeLog(output);
	}

	void ERROR(std::string output)
	{
		output = engine::getTimestamp() + " ERROR: " + output;
		std::cerr << output << std::endl;

		writeLog(output);
	}

	void SILENT(std::string type, std::string output)
	{
		output = engine::getTimestamp() + " " + type + ": " + output;

		writeLog(output);
	}

	void CUSTOM(std::string type, std::string output)
	{
		output = engine::getTimestamp() + " " + type + ": " + output;
		std::cout << output << std::endl;

		writeLog(output);
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

namespace loggerv2
{
	int logLevel = 2;
	/* 0 = no logging
	// 1 = log to files only
	// 2 = only print info, warnings, and errors // default
	// 3 = 1 + light debug messages
	// 4 = print all debug messages */

	std::string logLocation = ".\\bobars.log"; // default

	void logToFile(std::string message)
	{
		std::ofstream log(logLocation, std::ios::app);

		if (log.is_open())
			log << message << std::endl;
	}

	void logToConsole(std::string message)
	{
		std::cout << message << std::endl;
	}

	void INFO(std::string message)
	{
		if (logLevel == 0)
			return;

		message.insert(0, engine::getTimestamp() + " INFO: ");
		logToFile(message);

		if (logLevel >= 2) // info or greater
			logToConsole(message);
	}

	void WARNING(std::string message)
	{
		if (logLevel == 0)
			return;

		message.insert(0, engine::getTimestamp() + " WARNING: ");
		logToFile(message);

		if (logLevel >= 2) // info or greater
			logToConsole(message);
	}

	void ERROR(std::string message)
	{
		if (logLevel == 0)
			return;

		message.insert(0, engine::getTimestamp() + " ERROR: ");
		logToFile(message);

		if (logLevel >= 2) // info or greater
			logToConsole(message);
	}

	void DEBUG(std::string message)
	{
		if (logLevel == 0)
			return;

		message.insert(0, engine::getTimestamp() + " DEBUG: ");
		logToFile(message);

		if (logLevel >= 3) // debug or greater
			logToConsole(message);
	}

	// i plan to do something with that extra arg, i just don't know what yet.
	void log(std::string type, std::string message, int level)
	{
		if (logLevel == 0) // silent
		{
//			writeLog();
		}
		else if (logLevel == 1)
		{
//			writeLog();
//			printLog();
		}
	}
}

// class Loggerv3

Logger3::Logger3(std::string logOutputDir_)
{
	logOutputDir = logOutputDir_;

	std::cout << "Logger created" << std::endl;
}

Logger3::~Logger3()
{
	std::cout << "Logger destroyed" << std::endl;
}

void Logger3::log(std::string type, std::string message)
{
	message = engine::getTimestamp() + " " + type + ": " + message;

	std::cout << message << std::endl;
	writeLog(message);
}

void Logger3::info(std::string message)
{
	message = engine::getTimestamp() + " INFO: " + message;

	std::cout << message << std::endl;
	writeLog(message);
}

void Logger3::warn(std::string message)
{
	message = engine::getTimestamp() + " WARNING: " + message;

	std::cout << message << std::endl;
	writeLog(message);
}

void Logger3::error(std::string message)
{
	message = engine::getTimestamp() + " ERROR: " + message;

	std::cout << message << std::endl;
	writeLog(message);
}

//private

void Logger3::writeLog(const std::string output)
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
