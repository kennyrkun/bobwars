#include "Util.hpp"

#include <fstream>
#include <iostream>
#include <string>

std::string write_dir = "./bobwars/game.log";

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
		std::cerr << "/n" << "unable to open log file for writing!" << std::endl;
	}
}

namespace logger
{
	void DEBUG(std::string output, bool silent)
	{
		output = util::getTimestamp() + " DEBUG: " + output;

		if (!silent)
			std::cout << output << std::endl;

		writeLog(output);
	}

	void INFO(std::string output, bool silent)
	{
		output = util::getTimestamp() + " INFO: " + output;

		if (!silent)
			std::cout << output << std::endl;

		writeLog(output);
	}

	void WARNING(std::string output, bool silent)
	{
		output = util::getTimestamp() + " WARNING: " + output;
		
		if (!silent)
			std::cout << output << std::endl;

		writeLog(output);
	}

	void ERROR(std::string output, bool silent)
	{
		output = util::getTimestamp() + " ERROR: " + output;
		
		if (!silent)
			std::cerr << output << std::endl;

		writeLog(output);
	}

	void CUSTOM(std::string type, std::string output, bool silent)
	{
		output = util::getTimestamp() + " " + type + ": " + output;
		
		if (!silent)
			std::cout << output << std::endl;

		writeLog(output);
	}

	void LINE_BREAK()
	{
		std::cout << std::endl;
		writeLog("\n");
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

	std::string logLocation = ".//bobars.log"; // default

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

		message.insert(0, util::getTimestamp() + " INFO: ");
		logToFile(message);

		if (logLevel >= 2) // info or greater
			logToConsole(message);
	}

	void WARNING(std::string message)
	{
		if (logLevel == 0)
			return;

		message.insert(0, util::getTimestamp() + " WARNING: ");
		logToFile(message);

		if (logLevel >= 2) // info or greater
			logToConsole(message);
	}

	void ERROR(std::string message)
	{
		if (logLevel == 0)
			return;

		message.insert(0, util::getTimestamp() + " ERROR: ");
		logToFile(message);

		if (logLevel >= 2) // info or greater
			logToConsole(message);
	}

	void DEBUG(std::string message)
	{
		if (logLevel == 0)
			return;

		message.insert(0, util::getTimestamp() + " DEBUG: ");
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

Logger::Logger(std::string logOutputDir_)
{
	outputfile = logOutputDir_;

	std::cout << "log output redirected to (at \"" + outputfile + "\")" << std::endl;
}

Logger::~Logger()
{
	std::cout << "Logger destroyed" << std::endl;
}

void Logger::log(LogType type, std::string message)
{
	// TODO: translate Type to String

	//message = util::getTimestamp() + " " + type + ": " + message;

	std::cout << message << std::endl;
	writeLog(message);
}

void Logger::info(std::string message)
{
	message = util::getTimestamp() + " INFO: " + message;

	std::cout << message << std::endl;
	writeLog(message);
}

void Logger::warn(std::string message)
{
	message = util::getTimestamp() + " WARNING: " + message;

	std::cout << message << std::endl;
	writeLog(message);
}

void Logger::error(std::string message)
{
	message = util::getTimestamp() + " ERROR: " + message;

	std::cout << message << std::endl;
	writeLog(message);
}

//private

void Logger::printLog(std::string log)
{
	std::cout << log << std::endl;
}

void Logger::writeLog(const std::string output)
{
	std::ofstream log((write_dir), std::ios::app);

	if (log.is_open())
	{
		log << output << std::endl;

		if (log.bad())
		{
			std::cerr << "failed to write to log!" << std::endl;
		}
	}
	else
	{
		std::cerr << "/n" << "unable to open log file for writing!" << std::endl;
	}

	log.close();
}
