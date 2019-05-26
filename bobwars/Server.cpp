#include "Server.hpp"

#include "Util/Logger.hpp"

void Server::init(const sf::IpAddress& address, const int port)
{
	logger::INFO("initialising server at " + address.toString() + " on port " + std::to_string(port));
}

void Server::cleanup()
{
	logger::INFO("cleaning up server");
}

void Server::start()
{
	logger::INFO("starting server");
}

void Server::stop()
{
	logger::INFO("stopping server");
}

void Server::Update()
{
}

void Server::HandleEvents()
{
}
