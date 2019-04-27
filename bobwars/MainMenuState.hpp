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

private:
	AppEngine* app;

	bool mouseIsOver(sf::Shape &object);

	int dr, dg , db;
	sf::Uint8 r = 255, g = 0, b = 0;

	sf::RectangleShape backgroundmedia;

	sf::Font font;
	sf::Text bobwars;

	SFUI::Menu *menu;

	enum MAIN_MENU_CALLBACKS
	{
		PLAY_BUTTON,
		LOAD_BUTTON,
		SETTINGS_BUTTON,
		EXIT_BUTTON
	};

	enum OPTIONS_MENU_CALLBACKS
	{
		FULLSCREEN,
		DEBUG,

		RESOLUTIONX,
		RESOLUTIONY,
	};
};

#endif // !MAIN_MENU_STATE
