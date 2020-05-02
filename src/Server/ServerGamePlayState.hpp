#ifndef SERVER_GAME_PLAY_STATE_HPP
#define SERVER_GAME_PLAY_STATE_HPP

#include "ServerState.hpp"
#include "DedicatedServer.hpp"

#include <vector>

class ServerGamePlayState : public ServerState
{
public:
    void HandleClientConnect();
    void HandleClientDisconnect(DedicatedServer::Client* client);

    void Update() override;

    enum UnitCommands
    {
        CreateUnit,
        DeleteUnit,
        MoveUnit,
        UnitAction,

        FailNotEnoughResource,
        FailTooManyUnits,
    };

    struct Player
    {
        int playerID;
        const std::string name;
        float ping;
    };

    int maxPlayers = 8;

    std::vector<Player*> players;
};

#endif // !SERVER_GAME_PLAY_STATE_HPP
