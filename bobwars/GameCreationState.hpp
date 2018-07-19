#ifndef GAME_CREATION_STATE
#define GAME_CREATION_STATE

#include "AppState.hpp"

#include <SFUI\SFUi.hpp>

class GameCreationState : public AppState
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
	AppEngine* app;

	bool isClientReady = false;
};

#endif // !GAME_CREATION_STATE
