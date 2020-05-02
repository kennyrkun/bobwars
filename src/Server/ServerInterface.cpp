#include "ServerInterface.hpp"

#include "../Util/Logger.hpp"

bool ServerInterface::connect(sf::IpAddress address, unsigned short port, sf::Time timeout)
{
    selector.clear();
    socket = new sf::TcpSocket;

    if (socket->connect(address, port, timeout) != sf::Socket::Status::Done)
    {
        logger::ERROR("Failed to connect to server at " + address.toString() + ":" + std::to_string(port));

        close();

        return false;
    }

    selector.add(*socket);

    return true;
}

void ServerInterface::close()
{
    socket->disconnect();
    delete socket;
    socket = nullptr;
    selector.clear();
}

sf::Socket::Status ServerInterface::sendCommand(const std::string& command)
{
    sf::Packet packet;
    packet << command;
    return send(packet);
}

// TODO: pass packet by reference
sf::Socket::Status ServerInterface::send(sf::Packet packet)
{
    return socket->send(packet);
}

sf::Socket::Status ServerInterface::receive(sf::Packet& packet)
{
    return socket->receive(packet);
}

bool ServerInterface::ready()
{
    if (selector.wait(sf::milliseconds(10)))
        if (selector.isReady(*socket))
            return true;

    return false;
}