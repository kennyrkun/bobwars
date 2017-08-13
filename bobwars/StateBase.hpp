#ifndef APP_STATE_HPP
#define APP_STATE_HPP

#include "StateMachine.hpp"

class AppState2
{
public:
	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void HandleEvents() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

#endif // !APP_STATE_HPP
