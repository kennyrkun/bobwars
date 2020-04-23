#ifndef MAIN_MENU_STATE
#define MAIN_MENU_STATE

#include "AppEngine.hpp"
#include "AppState.hpp"

#include <SFUI/SFUI.hpp>

class MainMenuState : public AppState
{
public:
	void Init(AppEngine* app_);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents();
	void Update();
	void Draw();

private:
	sf::RectangleShape backgroundmedia;

	sf::Text bobwars;

	SFUI::Menu *menu;

	enum class MenuState
	{
		Main,
		Multiplayer,
		MultiplayerServer,
		Singleplayer,
		Settings
	};

	enum MenuCallbacks
	{
		Multiplayer,
		MultieplayerServerJoin,
		Singleplayer,
		Settings,
		Exit,

		MultiplayerServerConnect,
		MultiplayerServerBack,

		MultiplayerNewGame,
		MultiplayerJoinGame,
		MultiplayerLoadGame,
		MultiplayerBack,

		SingleplayerNewGame,
		SingleplayerLoadGame,
		SingleplayerBack,
		
		Debug,
		VSync,
		MaxFPS,
		Fullscreen,
		ResolutionX,
		ResolutionY,
		DarkMode,
		ApplySettings,
		SettingsBack
	};

	void build(const MenuState& state);
};

#endif // !MAIN_MENU_STATE
