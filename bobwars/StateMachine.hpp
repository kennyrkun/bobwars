#ifndef PROGRAM_ENGINE_HPP
#define PROGRAM_ENGINE_HPP

#include "ResourceManager.hpp"

#include <SFML\Graphics.hpp>
#include <vector>
#include <string>

class AppState2;

struct LaunchOptions
{
	bool fullscreen = false;
	bool vsync = false;
	int fps    = 60;
	int width  = 800;
	int height = 600;
};

class AppEngine2
{
public:
	AppEngine2(std::string title, LaunchOptions launchOptions_);
	~AppEngine2();

	void ChangeState(AppState2* state);
	void PushState(AppState2* state);
	void PopState();

	void HandleEvents();
	void Update();
	void Draw();

	bool Running() { return running; }
	void Quit();

	sf::RenderWindow* window;
	LaunchOptions options;
	bool debugModeActive = false;

	std::vector<AppState2*> states;

	ResourceManager* resMan;

private:

	bool running = false;
	bool fullscreen = false;
};

#endif // !PROGRAM_ENGINE_HPP
