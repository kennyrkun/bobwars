#include "ServerLobbyState.hpp"

#include "DedicatedServer.hpp"
#include "ServerGamePlayState.hpp"
#include "../Util/Logger.hpp"

void ServerLobbyState::NewConnectionEvent()
{
    logger::INFO("SERVER: Client is attempting to connect.");

    sf::TcpSocket* newSocket = new sf::TcpSocket;

    if (server->listener.accept(*newSocket) == sf::Socket::Status::Done)
    {
        logger::INFO("SERVER: New client is connecting...");

        server->selector.add(*newSocket);

        if (information.slots.size() + 1 > information.maxPlayers)
        {
            sf::Packet notifyServerFull;
            notifyServerFull << "ConnectionRejected";
            notifyServerFull << "ServerFull";

            newSocket->send(notifyServerFull);

            delete newSocket;
            newSocket = nullptr;
            server->selector.remove(*newSocket);

            logger::INFO("SEVER: Connection Rejected: Server is full.");

            return;
        }
        else
        {
            DedicatedServer::Client* newClient = static_cast<DedicatedServer::Client*>(newSocket);
            newClient->playerID = ++server->nextClientID;
            server->clients.push_back(newClient);

            LobbyInformation::SlotInformation newSlot;
            newSlot.state = LobbyInformation::SlotInformation::SlotState::OccupiedByPlayer;
            newSlot.name = newSocket->getRemoteAddress().toString() + "(" + std::to_string(newClient->playerID) +")";
            newSlot.playerID = newClient->playerID;
            newSlot.ping = -1;
            newSlot.team = "Bob";

            information.slots.push_back(newSlot);

            sf::Packet acceptConnection;
            acceptConnection << "ConnectionAccepted";
            acceptConnection << newClient->playerID;
            newClient->send(acceptConnection);

            updateLobby();

            logger::INFO("SERVER: now has " + std::to_string(information.slots.size()) + " players. newest client is ID: " + std::to_string(newClient->playerID));
        }
    }
    else
        logger::ERROR("SERVER: listener failed to accept client");
}

void ServerLobbyState::Update()
{
    if (server->selector.wait(server->updateDelay))
    {
        if (server->selector.isReady(server->listener))
        {
            NewConnectionEvent();
        }
        else
        {
            for (auto& client : server->clients)
            {
                if (server->selector.isReady(*client))
                {
                    sf::Packet packet;

                    sf::Socket::Status socketStatus = client->receive(packet);
                    if (socketStatus == sf::Socket::Status::Error)
                    {
                        logger::ERROR("FATAL: SERVER: socket status was error");
                        abort();
                    }
                    else if (socketStatus == sf::Socket::Status::Disconnected)
                    {
                        logger::INFO("SERVER: client has disconnected");
                        server->disconnectClient(client, "Timed out");

                        updateLobby();

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

                        server->disconnectClient(client, "DisconnectAcknowledged");

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
                            sf::Packet packet;
                            packet << "StartingGame";
                            server->broadcastMessage(packet);

                            server->changeState(new ServerGamePlayState);
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
                        case LobbyCallbacks::ChangeGameSpeed:
                            packet >> information.gameSpeed;
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
                        {                            
                            for (int i = 0; i < information.slots.size(); i++)
                                if (information.slots[i].playerID == client->playerID)
                                {
                                    packet >> information.slots[i].name;
                                    break;
                                }
                            
                            break;
                        }
                        case LobbyCallbacks::ChangeClientTeam:
                        {
                            for (int i = 0; i < information.slots.size(); i++)
                                if (information.slots[i].playerID == client->playerID)
                                {
                                    packet >> information.slots[i].team;
                                    break;
                                }

                            break;
                        }
                        case LobbyCallbacks::ChangeClientReady:
                        {
                            for (int i = 0; i < information.slots.size(); i++)
                                if (information.slots[i].playerID == client->playerID)
                                {
                                    packet >> information.slots[i].ready;
                                    break;
                                }

                            break;
                        }
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
                        server->broadcastMessage(packet);
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

void ServerLobbyState::updateLobby()
{
    sf::Packet packet;
    packet << "LobbyUpdate";
    packet << information;
    server->broadcastMessage(packet);
}