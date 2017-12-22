#ifndef GAME_CREATION_STATE
#define GAME_CREATION_STATE

#include "AppState.hpp"

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

	static GameCreationState* Instance()
	{
		return &GameCreationState_dontfuckwithme;
	}

protected:
	GameCreationState() { }

private:
	static GameCreationState GameCreationState_dontfuckwithme;
	AppEngine* app;
};

#endif // !GAME_CREATION_STATE
