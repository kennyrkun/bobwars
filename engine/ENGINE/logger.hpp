#pragma once
#include <string>

namespace logger
{
	void INFO(std::string output);

	void WARNING(std::string output);

	void ERROR(int type, std::string output);
}