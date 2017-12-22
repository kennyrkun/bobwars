#ifndef MAIN_MENU_STATE
#define MAIN_MENU_STATE

#include "GamePlayState.hpp"
#include "ENGINE\Logger.hpp"
#include <SFUI\TextButton.hpp>
#include "ResourceManager.hpp"

#include "AppEngine.hpp"
#include "AppState.hpp"

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

	SFUI::TextButton playButton;
	SFUI::TextButton loadButton;
	SFUI::TextButton settingsButton;
	SFUI::TextButton exitButton;
};

#endif // !MAIN_MENU_STATE
