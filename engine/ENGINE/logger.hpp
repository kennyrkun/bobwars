#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

namespace logger
{
	void INFO(std::string output);

	void INFO(std::string output, int);

	void WARNING(std::string output);

	void ERROR(std::string output);

	void SILENT(std::string type, std::string output);

	void CUSTOM(std::string type, std::string output);

	void BREAK();

	void setOutputDir(const std::string dir);
	void setOutputDir(const std::string dir, const std::string filename);
}

#endif /* LOGGER_HPP */
