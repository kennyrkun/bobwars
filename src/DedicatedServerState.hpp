#ifndef DEDICATED_SERVER_STATE_HPP
#define DEDICATED_SERVER_STATE_HPP

#include "AppState.hpp"

#include "AppEngine.hpp"

#include "Server.hpp"

class DedicatedServerState : public AppState
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
    bool gui = false;

	Server* server;
};

#endif // !DEDICATED_SERVER_STATE_HPP