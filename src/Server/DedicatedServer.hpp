#ifndef DEDICATED_SERVER_HPP
#define DEDICATED_SERVER_HPP

#include "ServerState.hpp"

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

    inline void changeState(ServerState* state)
    {
        if (this->state != nullptr)
        {
            delete this->state;
            this->state = nullptr;
        }

        this->state = state;
        state->Init(this);
    }

    class Client : public sf::TcpSocket
    {
    public:
        std::string name;
        int playerID = -1;
    };

    void disconnectClient(Client* client, std::string reason = "Generic Disconnect");
    bool detailedClientConnectionTest(Client* client);
    void broadcastMessage(sf::Packet packet, Client* ignoreClient = nullptr);

    sf::Time updateDelay = sf::seconds(10.0f);

    int nextClientID = 0;

    sf::TcpListener listener;
    sf::SocketSelector selector;
    std::list<Client*> clients;

private:
    ServerState* state = nullptr;
};

#endif // !DEDICATED_SERVER_HPP