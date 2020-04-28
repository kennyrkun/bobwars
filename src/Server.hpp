#ifndef SERVER_HPP
#define SERVER_HPP

#include "LobbyInformation.hpp"

#include "Util/Logger.hpp"

#include <SFML/Network.hpp>

#include <string>
#include <list>

// TODO:
/*
for now, let's just make a lobby system.
it will have three states
1. lobby setup state
    - the place where we pick teams and game settings
2. lobby play state
    - the one where you play
3. lobby finish state
    - the one that recaps all stats from the game
we will just 1 & 3 first, then 2.
*/

class ClientServerInterface
{
public:
    inline bool connect(sf::IpAddress address, unsigned short port, sf::Time timeout = sf::Time::Zero)
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

    void close()
    {
        socket->disconnect();
        delete socket;
        socket = nullptr;
        selector.clear();
    }

    // TODO: pass packet by reference
    inline sf::Socket::Status send(sf::Packet packet)
    {
        return socket->send(packet);
    }

    inline sf::Socket::Status receive(sf::Packet& packet)
    {
        return socket->receive(packet);
    }

    inline bool ready()
    {
        if (selector.wait(sf::milliseconds(10)))
		    if (selector.isReady(*socket))
                return true;

        return false;
    }

private:
    sf::TcpSocket* socket;
	sf::SocketSelector selector;
};

class Client : public sf::TcpSocket
{
public:
    std::string name;
    int playerID = -1;
};

class DedicatedServer
{
public:
    bool Start();
    void Stop();

    void Update();

    inline unsigned short getBoundPort() const
    {
        return listener.getLocalPort();
    }

    void disconnectClient(Client* client, std::string reason = "Generic Disconnect");
    bool detailedClientConnectionTest(Client* client);
    void broadcastMessage(sf::Packet packet, Client* ignoreClient = nullptr);

    sf::Time updateDelay = sf::seconds(10.0f);

    int nextClientID = 0;

    enum class ServerState
    {
        Lobby,
        Game,
        End,
    };

    inline void changeState(const ServerState& state)
    {
        this->state = state;
    }

    inline void updateLobby()
    {
        sf::Packet packet;
        packet << "LobbyUpdate";
        packet << information;
        broadcastMessage(packet);
    }

    enum LobbyCallbacks
	{
		HostStartGame,
		LeaveLobby,

		ChangeLobbyName,
		ChangeSlotStatus,
		RandomiseLobbySettings,
		ChangeGameType,
		ChangeMapSize,
		ChangeDifficulty,
		ChangeResources,
		ChangePopulation,
		ChangeGameSpeed,
		ChangeRevealMap,
		ChangeVictory,
		ChangeTeamTogether,
		ChangeLockTeams,
		ChangeAllTechs,
		ChangeLockSpeed,
		ChangeAllowCheats,
		ChangeRecordGame,

		ChangeClientName,
		ChangeClientTeam,
		ChangeClientColor,

        ClientLeave,

		Count,
	};

private:
    ServerState state;

    LobbyInformation information;

    sf::TcpListener listener;
    sf::SocketSelector selector;
    std::list<Client*> clients;
};

#endif // !SERVER_HPP