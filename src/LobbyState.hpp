#ifndef LOBBY_STATE_HPP
#define LOBBY_STATE_HPP

#include "AppState.hpp"
#include "LobbyInformation.hpp"
#include "Util/Graphics/DisabledButton.hpp"

#include <SFUI/SFUI.hpp>

#include <map>

class LobbyState : public AppState
{
public:
	LobbyState(bool host = false) : lobbyHost(host) {}

	void Init(AppEngine* app_);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents();
	void Update();
	void Draw();

private:
	SFUI::Menu* menu = nullptr;

	enum MenuCallbacks
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

		Count,
	};

	SFUI::InputBox* gameNameBox = nullptr;

	SFUI::OptionsBox<std::string>* typeBox;
	SFUI::OptionsBox<std::string>* mapSizeBox;
	SFUI::OptionsBox<std::string>* difficultyBox;
	SFUI::OptionsBox<std::string>* resourcesBox;
	SFUI::OptionsBox<std::string>* populationBox;
	SFUI::OptionsBox<std::string>* gameSpeedBox;
	SFUI::OptionsBox<std::string>* revealMapBox;
	SFUI::OptionsBox<std::string>* victoryBox;

	SFUI::InputBox* chatBox = nullptr;
	SFUI::InputBox* chatInputBox = nullptr;

	SFUI::CheckBox* teamTogetherBox = nullptr;
	SFUI::CheckBox* allTechsBox = nullptr;
	SFUI::CheckBox* allowCheatsBox = nullptr;
	SFUI::CheckBox* lockTeamsBox = nullptr;
	SFUI::CheckBox* lockSpeedBox = nullptr;
	SFUI::CheckBox* recordGameBox = nullptr;

	bool lobbyHost = false;
	bool allPlayersReady = false;
	int playerNumber = 2;

	void buildMenu(const LobbyInformation& information);
};

#endif // !LOBBY_STATE_HPP
