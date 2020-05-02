#include "ServerGamePlayState.hpp"

#include "DedicatedServer.hpp"
#include "../Util/Logger.hpp"

void ServerGamePlayState::HandleClientConnect()
{
    logger::INFO("SERVER: Client is attempting to connect.");

    sf::TcpSocket* newSocket = new sf::TcpSocket;

    if (server->listener.accept(*newSocket) == sf::Socket::Status::Done)
    {
        logger::INFO("SERVER: Turning around new client");

        server->selector.add(*newSocket);

        sf::Packet notifyGameInProgress;
        notifyGameInProgress << "ConnectionRejected";
        notifyGameInProgress << "GameAlreadyInProgress_AdHocConnectionDisallowed";

        newSocket->send(notifyGameInProgress);

        newSocket->disconnect();
        delete newSocket;
        newSocket = nullptr;
        server->selector.remove(*newSocket);

        logger::INFO("SEVER: Connection Rejected: Server is full.");
    }
    else
        logger::ERROR("SERVER: listener failed to accept client");
}

void ServerGamePlayState::HandleClientDisconnect(DedicatedServer::Client* client)
{
    logger::INFO("SERVER: disconnecting client");

    for (int i = 0; i < players.size(); i++)
    {
        if (players[i]->playerID == client->playerID)
        {
            delete players[i];
            players[i] = nullptr;

            players.erase(players.begin() + i);
            break;
        }
    }

    server->disconnectClient(client, "DisconnectAcknowledged");

    // TODO: inform the other clients this client has left the session
}

void ServerGamePlayState::Update()
{
    if (server->selector.wait(server->updateDelay))
    {
        if (server->selector.isReady(server->listener))
        {
            HandleClientConnect();
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
                        HandleClientDisconnect(client);
                        break;
                    }

                    std::string command;
                    packet >> command;

                    if (command == "ClientDisconnecting")
                    {
                        HandleClientDisconnect(client);
                    }
                    else if (command == "PauseGame")
                    {
                    }
                    else if (command == "UnpauseGame")
                    {
                    }
                    else if (command == "UnitCommand")
                    {
                        int id = 0;
                        packet >> id;

                        switch (id)
                        {
                        default:
                        {
                            if (id != -1)
                                logger::INFO("SERVER: unhandled case for lobby callback: " + std::to_string(id));
                            break;
                        }
                        } // switch
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