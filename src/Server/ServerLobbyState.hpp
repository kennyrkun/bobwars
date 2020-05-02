#ifndef SERVER_LOBBY_STATE_HPP
#define SERVER_LOBBY_STATE_HPP

#include "ServerState.hpp"
#include "../LobbyInformation.hpp"

class ServerLobbyState : public ServerState
{
public:
    void Update() override;

    void NewConnectionEvent();

    void updateLobby();

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
        ChangeClientReady,
		ClientDisconnect,
		ClientSendChatMessage,

		LobbyCallbacksCount,
	};

    LobbyInformation information;
};

#endif // !SERVER_LOBBY_STATE_HPP