#ifndef GAME_END_STATE_HPP
#define GAME_END_STATE_HPP

#include "AppState.hpp"
#include "util/Graphics/DisabledButton.hpp"

#include <SFUI/SFUI.hpp>

class GameEndState : public AppState
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
};

#endif // !GAME_END_STATE_HPP