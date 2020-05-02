#ifndef LOBBY_STATE_HPP
#define LOBBY_STATE_HPP

#include "AppState.hpp"
#include "LobbyInformation.hpp"
#include "Util/Graphics/DisabledButton.hpp"

#include <SFUI/SFUI.hpp>

#include <vector>
#include <string>

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
        ChangeClientReady,
		ClientDisconnect,
		ClientSendChatMessage,

		Count,
	};

	SFUI::Label* gameNameLabel;
	SFUI::InputBox* gameNameBox;

	SFUI::InputBox* clientNameBox;
	SFUI::OptionsBox<std::string>* clientTeamBox;

	SFUI::Label* typeLabel;
	SFUI::Label* mapSizeLabel;
	SFUI::Label* difficultyLabel;
	SFUI::Label* resourcesLabel;
	SFUI::Label* populationLabel;
	SFUI::Label* gameSpeedLabel;
	SFUI::Label* revealMapLabel;
	SFUI::Label* victoryLabel;
	SFUI::Label* teamTogetherLabel;
	SFUI::Label* allTechsLabel;
	SFUI::Label* allowCheatsLabel;
	SFUI::Label* lockTeamsLabel;
	SFUI::Label* lockSpeedLabel;
	SFUI::Label* recordGameLabel;

	SFUI::OptionsBox<std::string>* typeBox;
	SFUI::OptionsBox<std::string>* mapSizeBox;
	SFUI::OptionsBox<std::string>* difficultyBox;
	SFUI::OptionsBox<std::string>* resourcesBox;
	SFUI::OptionsBox<std::string>* populationBox;
	SFUI::OptionsBox<std::string>* gameSpeedBox;
	SFUI::OptionsBox<std::string>* revealMapBox;
	SFUI::OptionsBox<std::string>* victoryBox;

	SFUI::InputBox* chatBox;
	SFUI::InputBox* chatInputBox;

	SFUI::CheckBox* teamTogetherBox;
	SFUI::CheckBox* allTechsBox;
	SFUI::CheckBox* allowCheatsBox;
	SFUI::CheckBox* lockTeamsBox;
	SFUI::CheckBox* lockSpeedBox;
	SFUI::CheckBox* recordGameBox;

	SFUI::CheckBox* readyBox;

	bool lobbyHost = false;
	int playerNumber = -1;

	void buildMenu(const LobbyInformation& information);

	//bool sendServerUpdate(const std::string& command, const std::string& value = "");

	bool sendInformationChange(const int& id, const bool& value);
	bool sendInformationChange(const int& id, std::string value);

	bool sendServerCommand(const std::string& command, std::vector<std::string> arguments = {});
};

#endif // !LOBBY_STATE_HPP
