#ifndef MAIN_MENU_STATE
#define MAIN_MENU_STATE

#include "AppEngine.hpp"
#include "AppState.hpp"

#include <SFUI\SFUI.hpp>

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

	static MainMenuState* Instance()
	{
		return &MainMenuState_dontfuckwithme;
	}

private:
	static MainMenuState MainMenuState_dontfuckwithme;
	AppEngine* app;

	bool mouseIsOver(sf::Shape &object);

	int dr, dg , db;
	sf::Uint8 r = 255, g = 0, b = 0;
	sf::RectangleShape logoShape;

	SFUI::Menu *menu;

	enum MENU_CALLBACKS
	{
		PLAY_BUTTON,
		LOAD_BUTTON,
		SETTINGS_BUTTON,
		EXIT_BUTTON
	};
};

#endif // !MAIN_MENU_STATE
