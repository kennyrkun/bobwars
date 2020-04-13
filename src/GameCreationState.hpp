#ifndef GAME_CREATION_STATE
#define GAME_CREATION_STATE

#include "AppState.hpp"
#include "Util/Graphics/DisabledButton.hpp"

#include <SFUI/SFUI.hpp>

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

	DisabledButton* startGameButton;
};

#endif // !GAME_CREATION_STATE
