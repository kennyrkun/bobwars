#include "DedicatedServer.hpp"

#include "../LobbyInformation.hpp"
#include "../Util/Logger.hpp"

bool DedicatedServer::Start()
{
    logger::INFO("SERVER: Starting bobwars server...");

    // TODO: allow the port to be selected
    if (listener.listen(sf::TcpListener::AnyPort) != sf::Socket::Status::Done)
	{
        logger::ERROR("SERVER: Failed to bind to port.");
        return false;
	}
	else
        logger::INFO("SERVER: Successfully bound to port " + std::to_string(listener.getLocalPort()));

	selector.add(listener);

    logger::INFO("SERVER: local ipv4: " + sf::IpAddress::getLocalAddress().toString());
    logger::INFO("SERVER: public ipv4: " + sf::IpAddress::getPublicAddress().toString());

    logger::INFO("SERVER: Server is ready.");

    return true;
}

void DedicatedServer::Stop()
{
    logger::INFO("SERVER: Shutting down server...");

    listener.close();

    sf::Packet shutdownNotification;
    shutdownNotification << "Disconnected";
    shutdownNotification << "ServerShuttingDown";

    for (Client* client : clients)
    {
        if (client->send(shutdownNotification) != sf::Socket::Status::Done)
            logger::WARNING("SERVER: Failed to send shutdown notification to " + client->getRemoteAddress().toString() + ":" + std::to_string(client->getRemotePort()));

        delete client;
        client = nullptr;
    }

    clients.clear();
    selector.clear();

    logger::INFO("SERVER: Goodbye.");
}

void DedicatedServer::Update()
{
    state->Update();
}

void DedicatedServer::disconnectClient(Client* client, std::string reason)
{
    logger::INFO("SERVER: Disconnecting " + client->getRemoteAddress().toString());

	sf::Packet disconnect;
	disconnect << "Disconnected";
	disconnect << reason;

	// don't bother checking for errors, this is just a courtesy
	client->send(disconnect);

	client->disconnect();
	selector.remove(*client);
	clients.remove(client);
	delete client;
    client = nullptr;

	if (clients.size() <= 0)
	{
        logger::INFO("SERVER: Client list empty, cleaning...");

		clients.clear();
		selector.clear();
		selector.add(listener);
	}
}

bool DedicatedServer::detailedClientConnectionTest(Client* client)
{
    logger::INFO("SERVER: Testing connection to " + client->getRemoteAddress().toString());

	sf::Packet connectionTest;

	if (client->send(connectionTest) == sf::Socket::Status::Disconnected)
		return false;
	else if (client->getRemoteAddress() == sf::IpAddress::None)
		return false;

	return true;
}

void DedicatedServer::broadcastMessage(sf::Packet packet, Client* ignoreClient)
{
	for (auto& client : clients)
		if (client != ignoreClient)
			client->send(packet);
}