#ifndef APP_ENGINE_HPP
#define APP_ENGINE_HPP

#include "ResourceManager.hpp"
#include "Util\DiscordRPC.hpp"

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class AppState;

struct AppSettings
{
	std::string version = "0.13.0";
	std::string title = "bobwars " + version;
	bool fullscreen = false;
	bool vsync = true;
	bool debug = false;
	bool console = false;
	int fps = 60;
	int width = 1080;
	int height = 720;
};

class AppEngine
{
public:
	void Init(std::string title, AppSettings settings_);
	void Cleanup();

	// Note: code after this function is still executed.
	void ChangeState(AppState* state);
	// Note: code after this function is still executed.
	void PushState(AppState* state);
	// Note: code after this function is still executed.
	void PopState();
	// Note: code after this function is still executed.
	void PopState(int amount);

	void HandleEvents();
	void Update();
	void Draw();

	bool isRunning() { return running; }
	void Quit();

	std::string title;
	bool debugModeActive = false;

	sf::RenderWindow* window;
	AppSettings settings;

	std::vector<AppState*> states;
	std::vector<sf::Event> events;

	ResourceManager* resMan;

	DiscordRPC dRPC;

private:
	bool running;
};

#endif // !APP_ENGINE_HPP
