#ifndef GAME_PAUSE_STATE
#define GAME_PAUSE_STATE

#include "AppState.hpp"

#include "Menu.hpp"
#include <SFUI/Button.hpp>

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

	static GamePauseState* Instance()
	{
		return &GamePauseState_dontfuckwithme;
	}

protected:
	GamePauseState() { }

private:
	static GamePauseState GamePauseState_dontfuckwithme;
	AppEngine* app;

	sf::RectangleShape background;
	sf::Texture* backgroundTexture; //HACK: don't crash on startup while in debug

	Menu* pauseMenu;
};

#endif // !GAME_PAUSE_STATE
