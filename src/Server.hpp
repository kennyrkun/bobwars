#ifndef SERVER_HPP
#define SERVER_HPP

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
    void Update();

    sf::TcpSocket* socket;
	sf::SocketSelector selector;
};

class DedicatedServer
{
public:
    bool Start();
    void Stop();

    void Update();

    void disconnectClient(sf::TcpSocket* socket, std::string reason = "Generic Disconnect");
    bool detailedClientConnectionTest(sf::TcpSocket* socket);
    void broadcastMessage(sf::Packet packet, sf::TcpSocket* socketToIgnore = nullptr);

private:
    sf::TcpListener listener;
    sf::SocketSelector selector;
    std::list<sf::TcpSocket*> sockets;
};

#endif // !SERVER_HPP