#ifndef SERVER_GAME_PLAY_STATE_HPP
#define SERVER_GAME_PLAY_STATE_HPP

#include "ServerState.hpp"

class ServerGamePlayState : public ServerState
{
public:
    void Update() override;

    enum GamePlayCallbacks
    {
        PlayerDisconnect,

        CreateUnit,
        DeleteUnit,
        MoveUnit,
        UnitAction,

        FailNotEnoughResource,
        FailTooManyUnits,

        GamePlayCallbacksCount,
    };
};

#endif // !SERVER_GAME_PLAY_STATE_HPP
