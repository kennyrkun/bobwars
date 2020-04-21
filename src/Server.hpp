#ifndef SERVER_HPP
#define SERVER_HPP

#include <SFML/Network.hpp>

#include <string>
#include <list>

class Client
{
public:
    int ID;
    int team;
    
    sf::TcpSocket* socket = nullptr;
};

class Server
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