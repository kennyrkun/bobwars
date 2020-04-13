#ifndef GAME_PAUSE_STATE
#define GAME_PAUSE_STATE

#include "AppState.hpp"

#include <SFUI/Layouts/Menu.hpp>

class GamePauseState : public AppState
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

	sf::RectangleShape background;
	sf::Texture* backgroundTexture; //HACK: don't crash on startup while in debug

	sf::RectangleShape menuShape;

	SFUI::Menu* menu;
};

#endif // !GAME_PAUSE_STATE
