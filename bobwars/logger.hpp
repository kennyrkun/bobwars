#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

namespace logger
{
	void INFO(std::string output);

	void WARNING(std::string output);

	void ERROR(int type, std::string output);

	void DEBUG(std::string output);
}

#endif
