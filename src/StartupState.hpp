#ifndef STARTUP_STATE_HPP
#define STARTUP_STATE_HPP

#include "AppEngine.hpp"
#include "AppState.hpp"

#include <SFUI/SFUI.hpp>
#include <SFML/Graphics.hpp>

class StartupState : public AppState
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
	bool ready = false;
};

#endif // !STARTUP_STATE_HPP
