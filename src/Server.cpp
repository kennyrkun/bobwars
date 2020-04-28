#include "Server.hpp"

#include "LobbyInformation.hpp"

#include "Util/Logger.hpp"

bool DedicatedServer::Start()
{
    logger::INFO("SERVER: Starting bobwars server...");

	if (listener.listen(12345) != sf::Socket::Status::Done)
	{
        logger::ERROR("SERVER: Failed to bind to port.");
        return false;
	}
	else
        logger::INFO("SERVER: Successfully bound to port " + std::to_string(listener.getLocalPort()));

	selector.add(listener);

    logger::INFO("SERVER: local ipv4: " + sf::IpAddress::getLocalAddress().toString());
    logger::INFO("SERVER: public ipv4: " + sf::IpAddress::getPublicAddress().toString());

    state = ServerState::Lobby;

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
                logger::INFO("SERVER: New client is connecting...");

                selector.add(*newSocket);

                if (information.slots.size() + 1 > information.maxPlayers)
                {
                    sf::Packet notifyServerFull;
                    notifyServerFull << "ConnectionRejected";
                    notifyServerFull << "ServerFull";

                    newSocket->send(notifyServerFull);

                    delete newSocket;
                    newSocket = nullptr;
                    selector.remove(*newSocket);

                    logger::INFO("SEVER: Connection Rejected: Server is full.");

                    return;
                }
                else
                {
                    Client* newClient = static_cast<Client*>(newSocket);
                    newClient->playerID = ++nextClientID;
                    clients.push_back(newClient);

                    LobbyInformation::SlotInformation newSlot;
                    newSlot.state = LobbyInformation::SlotInformation::SlotState::OccupiedByPlayer;
                    newSlot.name = newSocket->getRemoteAddress().toString();
                    newSlot.playerID = newClient->playerID;
                    newSlot.ping = 0;
                    newSlot.team = "unassigned";
                    newSlot.color = "unassigned";

                    information.slots.push_back(newSlot);

                    sf::Packet acceptConnection;
                    acceptConnection << "ConnectionAccepted";
                    broadcastMessage(acceptConnection);

                    updateLobby();

                    logger::INFO("SERVER: now has " + std::to_string(information.slots.size()) + " players");
                }
            }
            else
                logger::ERROR("SERVER: listener failed to accept client");
        }
        else
        {
            for (auto& client : clients)
            {
                if (selector.isReady(*client))
                {
                    sf::Packet packet;

                    if (client->receive(packet) == sf::Socket::Disconnected)
                    {
                        logger::INFO("SERVER: client has disconnected");
                        disconnectClient(client, "Timed out");

                        sf::Packet notify;
                        notify << "userLeft";
                        notify << "UNKNOWN";
                        notify << "timedOut";

                        broadcastMessage(notify);

                        break;
                    }

                    std::string command;
                    packet >> command;

                    if (command == "ClientDisconnecting")
                    {
                        logger::INFO("SERVER: disconnecting client");

                        for (int i = 0; i < information.slots.size(); i++)
                        {
                            if (information.slots[i].playerID == client->playerID)
                            {
                                information.slots.erase(information.slots.begin() + i);
                                break;
                            }
                        }

                        disconnectClient(client, "DisconnectAcknowledged");

                        updateLobby();
                    }
                    else if (command == "ChangeLobbyName")
                    {
                        std::string newName;
                        packet >> newName;

                        information.gameName = newName;

                        updateLobby();
                    }
                    else if (command == "StartGame")
                    {
                        // TODO: make sure the person this is coming from is the host
                        if (information.allPlayersReady)
                        {
                            changeState(ServerState::Game);
                            // TODO: play game;
                            return;
                        }
                        else
                        {
                            logger::ERROR("SERVER: cannot start game until all players are ready.");
                        } 
                    }
                    else if (command == "LobbyInformationChange")
                    {
                        int id = 0;
                        packet >> id;

                        logger::INFO("id: " + std::to_string(id));

                        switch (id)
                        {
                        case LobbyCallbacks::ChangeLobbyName:
                        {
                            packet >> information.gameName;
                            break;
                        }
                        case LobbyCallbacks::ChangeSlotStatus:
                            break;
                        case LobbyCallbacks::RandomiseLobbySettings:
                            break;
                        case LobbyCallbacks::ChangeGameType:
                            packet >> information.type;
                            break;
                        case LobbyCallbacks::ChangeMapSize:
                            packet >> information.mapSize;
                            break;
                        case LobbyCallbacks::ChangeDifficulty:
                            packet >> information.difficulty;
                            break;
                        case LobbyCallbacks::ChangeResources:
                            packet >> information.resources;
                            break;
                        case LobbyCallbacks::ChangePopulation:
                            packet >> information.population;
                            break;
                        case LobbyCallbacks::ChangeRevealMap:
                            packet >> information.revealMap;
                            break;
                        case LobbyCallbacks::ChangeVictory:
                            packet >> information.victory;
                            break;
                        case LobbyCallbacks::ChangeTeamTogether:
                            packet >> information.teamTogether;
                            break;
                        case LobbyCallbacks::ChangeLockTeams:
                            packet >> information.lockTeams;
                            break;
                        case LobbyCallbacks::ChangeAllTechs:
                            packet >> information.allTechs;
                            break;
                        case LobbyCallbacks::ChangeLockSpeed:
                            packet >> information.lockSpeed;
                            break;
                        case LobbyCallbacks::ChangeAllowCheats:
                            packet >> information.allowCheats;
                            break;
                        case LobbyCallbacks::ChangeRecordGame:
                            packet >> information.recordGame;
                            break;
                        case LobbyCallbacks::ChangeClientName:
                            break;
                        case LobbyCallbacks::ChangeClientTeam:
                            break;
                        case LobbyCallbacks::ChangeClientColor:
                            break;
                        default:
                        {
                            if (id != -1)
                                logger::INFO("SERVER: unhandled case for lobby callback: " + std::to_string(id));
                            break;
                        }
                        } // switch

                        sf::Packet packet;
                        packet << "LobbyUpdate";
                        packet << information;
                        broadcastMessage(packet);
                    }
                    else
                    {  
                        logger::INFO("SERVER: received unknown command from client: " + command);
                    }
                    
                    break;
                }
            }
        }
    }
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