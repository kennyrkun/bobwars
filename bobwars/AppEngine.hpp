#ifndef APP_ENGINE_HPP
#define APP_ENGINE_HPP

#include "ResourceManager.hpp"

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class AppState;

struct AppSettings
{
	std::string title = "bobwars " + version;
	std::string version = "0.9.0";
	bool fullscreen = false;
	bool vsync = true;
	bool debug = false;
	bool console = false;
	int fps = 60;
	int width = 800;
	int height = 600;
};

class AppEngine
{
public:
	void Init(std::string title, AppSettings settings_);
	void Cleanup();

	void ChangeState(AppState* state);
	void PushState(AppState* state);
	void PopState();
	void PopState(int amount);

	void HandleEvents();
	void Update();
	void Draw();

	bool Running() { return running; }
	void Quit() { running = false; }

	std::string title;
	bool debugModeActive = false;

	sf::RenderWindow* window;
	AppSettings settings;

	std::vector<AppState*> states;
	std::vector<sf::Event> events;

	ResourceManager* resMan;

private:
	bool running;
};

#endif // !APP_ENGINE_HPP
