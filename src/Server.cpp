#include "Server.hpp"

#include "LobbyInformation.hpp"

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

    state = ServerState::Lobby;

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
    if (selector.wait(updateDelay))
    {
        if (selector.isReady(listener))
        {
            logger::INFO("SERVER: Client is attempting to connect.");

            if (state != ServerState::Lobby)
            {
                logger::INFO("SERVER: rejected client connection while game is in progress");
                return;
            }

            sf::TcpSocket* newSocket = new sf::TcpSocket;

            if (listener.accept(*newSocket) == sf::Socket::Status::Done)
            {
                selector.add(*newSocket);
                sockets.push_back(newSocket);

                if (information.totalPlayers + 1 > information.maxPlayers + 1)
                {
                    logger::INFO("rejecting client because the server is full");

                    sf::Packet packet;
                    packet << "serverFull";

                    newSocket->send(packet);

                    delete newSocket;
                    newSocket = nullptr;
                    selector.remove(*newSocket);

                    return;
                }
                else
                {
                    information.totalPlayers++;

                    LobbyInformation::SlotInformation newSlot;
                    newSlot.state = LobbyInformation::SlotInformation::SlotState::OccupiedByPlayer;
                    newSlot.name = newSocket->getRemoteAddress().toString();
                    newSlot.ping = 0;
                    newSlot.team = "unassigned";
                    newSlot.color = "unassigned";

                    information.slots.push_back(newSlot);

                    sf::Packet packet;
                    packet << "LobbyUpdate";
                    packet << information;
                    broadcastMessage(packet);
                }

                logger::INFO("SERVER: listener accepted client");
            }
            else
                logger::ERROR("SERVER: listener failed to accept client");
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
                        logger::INFO("SERVER: client has disconnected");
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
    logger::INFO("SERVER: Disconnecting " + socket->getRemoteAddress().toString());

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
        logger::INFO("SERVER: Client list empty, cleaning up and resetting...");

		sockets.clear();
		selector.clear();
		selector.add(listener);
	}
}

bool DedicatedServer::detailedClientConnectionTest(sf::TcpSocket* socket)
{
    logger::INFO("SERVER: Testing connection to " + socket->getRemoteAddress().toString());

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