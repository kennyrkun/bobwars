#ifndef GAME_PAUSE_STATE
#define GAME_PAUSE_STATE

#include "AppState.hpp"

#include "Menu.hpp"
#include <SFUI/TextButton.hpp>

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
		return &GameCreationState_dontfuckwithme;
	}

protected:
	GamePauseState() { }

private:
	static GamePauseState GameCreationState_dontfuckwithme;
	AppEngine* app;

	sf::RectangleShape background;
	sf::Texture backgroundTexture;

	Menu* pauseMenu;
};

#endif // !GAME_PAUSE_STATE
