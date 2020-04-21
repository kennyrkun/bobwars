#ifndef GAME_STARTUP_STATE_HPP
#define GAME_STARTUP_STATE_HPP

#include "AppEngine.hpp"
#include "AppState.hpp"

#include <SFUI/SFUI.hpp>
#include <SFML/Graphics.hpp>

class GameStartupState : public AppState
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
	bool mouseIsOver(sf::Shape &object);

	bool ready = false;
};

#endif // !GAME_STARTUP_STATE_HPP
