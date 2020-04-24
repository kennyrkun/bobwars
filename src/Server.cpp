#include "Server.hpp"

#include "Util/Logger.hpp"

bool DedicatedServer::Start()
{
    logger::INFO("Starting bobwars server...");

	if (listener.listen(12345) != sf::Socket::Status::Done)
	{
        logger::ERROR("Failed to bind to port.");
        return false;
	}
	else
        logger::INFO("Successfully bound to port " + std::to_string(listener.getLocalPort()));

	selector.add(listener);

    logger::INFO("local ipv4: " + sf::IpAddress::getLocalAddress().toString());
    logger::INFO("public ipv4: " + sf::IpAddress::getPublicAddress().toString());

    logger::INFO("Server is ready.");

    return true;
}

void DedicatedServer::Stop()
{
    logger::INFO("Shutting down server...");

    logger::INFO("Goodbye.");
}

void DedicatedServer::Update()
{
    if (selector.wait(sf::seconds(10.0f)))
    {
        if (selector.isReady(listener))
        {
            logger::INFO("listener is ready to accept a new client");

            sf::TcpSocket* newSocket = new sf::TcpSocket;

            if (listener.accept(*newSocket) == sf::Socket::Status::Done)
            {
                selector.add(*newSocket);
                sockets.push_back(newSocket);
                logger::INFO("listener accepted client");
            }
            else
                logger::ERROR("listener failed to accept client");
        }
        else
        {
            for (auto& socket : sockets)
            {
                if (selector.isReady(*socket))
                {
                    sf::Packet packet;

                    if (socket->receive(packet) == sf::Socket::Disconnected)
                    {
                        logger::INFO("client has disconnected");
                        disconnectClient(socket, "Timed out");

                        sf::Packet notify;
                        notify << "userLeft";
                        notify << "UNKNOWN";
                        notify << "timedOut";

                        broadcastMessage(notify);

                        break;
                    }

                    std::string command;
                    packet >> command;

                    if (command == "userJoining")
                    {
                    }
                    break;
                }
            }
        }
    }
}

void DedicatedServer::disconnectClient(sf::TcpSocket* socket, std::string reason)
{
    logger::INFO("Disconnecting " + socket->getRemoteAddress().toString());

	sf::Packet disconnect;
	disconnect << "youGotDisconnected";
	disconnect << reason;

	// don't bother checking for errors, this is just a courtesy
	socket->send(disconnect);

	selector.remove(*socket);
	socket->disconnect();
	sockets.remove(socket);
	delete socket;

	if (sockets.size() <= 0)
	{
        logger::INFO("Client list empty, cleaning up and resetting...");

		sockets.clear();
		selector.clear();
		selector.add(listener);
	}
}

bool DedicatedServer::detailedClientConnectionTest(sf::TcpSocket* socket)
{
    logger::INFO("Testing connection to " + socket->getRemoteAddress().toString());

	sf::Packet connectionTest;

	if (socket->send(connectionTest) == sf::Socket::Status::Disconnected)
		return false;
	else if (socket->getRemoteAddress() == sf::IpAddress::None)
		return false;

	return true;
}

void DedicatedServer::broadcastMessage(sf::Packet packet, sf::TcpSocket* socketToIgnore)
{
	for (auto& socket : sockets)
		if (socket != socketToIgnore)
			socket->send(packet);
}