#ifndef SERVER_INTERFACE_HPP
#define SERVER_INTERFACE_HPP

#include <SFML/Network.hpp>

class ServerInterface
{
public:
    // TODO: pass information by reference
    bool connect(sf::IpAddress address, unsigned short port, sf::Time timeout = sf::Time::Zero);
    void close();

    sf::Socket::Status sendCommand(const std::string& command);

    // TODO: pass packet by reference
    sf::Socket::Status send(sf::Packet packet);
    sf::Socket::Status receive(sf::Packet& packet);

    bool ready();

private:
    sf::TcpSocket* socket;
	sf::SocketSelector selector;
};

#endif // !SERVER_INTERFACE_HPP