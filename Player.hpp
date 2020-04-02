#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Network.hpp>

class Player
{
public:
	int team;
	sf::IpAddress address;
	short port;
};

#endif // !PLAYER_HPP
