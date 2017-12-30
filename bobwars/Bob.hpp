#ifndef BOB_HPP
#define BOB_HPP

#include "BaseEntity.hpp"

class Bob : public BaseEntity
{
public:
	Bob();
	~Bob();

	int armor = 0;
	int hitpoints = 1;

	bool movable = true;

	const std::string type = "bob";
};

#endif // !BOB_HPP
