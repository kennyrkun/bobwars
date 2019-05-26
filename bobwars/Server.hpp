#ifndef SERVER_HPP
#define SERVER_HPP

#include <SFML/Network.hpp>

class Server
{
	void init(const sf::IpAddress& address, const int port);
	void cleanup();

	void start();
	void stop();

	void Update();
	void HandleEvents();

private:
	const sf::IpAddress address;
	const int port;
};

#endif // !SERVER_HPP