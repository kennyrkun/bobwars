#ifndef LOBBY_STATE_HPP
#define LOBBY_STATE_HPP

#include "AppState.hpp"
#include "Util/Graphics/DisabledButton.hpp"

#include <SFUI/Layouts/Menu.hpp>

class LobbyState : public AppState
{
public:
	void Init(AppEngine* app_);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents();
	void Update();
	void Draw();

	SFUI::Menu* menu;

private:
	enum class State
	{
		CreatingLobby,
		ConnectingToLobby,
		EditingLobby,
	};

	enum MenuCallbacks
	{
		HostStartGame,
		LeaveLobby,

		Count,
	};

	bool isClientReady = false;

	DisabledButton* startGameButton;
};

#endif // !LOBBY_STATE_HPP
