#ifndef APP_ENGINE_HPP
#define APP_ENGINE_HPP

#include "ResourceManager.hpp"
#include "Util/DiscordRPC.hpp"

#include <SFML/Graphics.hpp>

#include <vector>
#include <string>

class AppState;

struct AppSettings
{
	std::string version = "0.15-20w14a";
	std::string title = "bobwars " + version;
	bool fullscreen = false;
	bool vsync = true;
	bool debug = false;
	bool console = false;
	int maxfps = 60;
	int width = 1080;
	int height = 720;
	float mouseMoveBorderWidth = 20.0f;
	bool mouseMoveEnabled = true;

	struct CommandLineArguments
	{
		int argc;
		char** argv;
	} launchOptions;
};

struct Keybindings
{
	sf::Keyboard::Key moveCameraUp = sf::Keyboard::Key::W;
	sf::Keyboard::Key moveCameraLeft = sf::Keyboard::Key::A;
	sf::Keyboard::Key moveCameraDown = sf::Keyboard::Key::S;
	sf::Keyboard::Key moveCameraRight = sf::Keyboard::Key::D;
	sf::Keyboard::Key deleteSelected = sf::Keyboard::Key::Delete;

	sf::Keyboard::Key groupSelect = sf::Keyboard::Key::LControl;
};

class AppEngine
{
public:
	void Init(AppSettings settings_);
	void Cleanup();

	// Note: code after these functions is still executed.
	void ChangeState(AppState* state);
	void PushState(AppState* state);
	void PopState();
	void PopState(int amount);

	void HandleEvents();
	void Update();
	void Draw();

	bool isRunning() { return running; }
	void Quit();

	sf::RenderWindow* window;
	AppSettings settings;

	std::vector<AppState*> states;
	std::vector<sf::Event> events;

	ResourceManager* resMan;

	DiscordRPC dRPC;

	Keybindings keys;

	time_t delta;

private:
	bool running;

	time_t lastUpdate;
};

#endif // !APP_ENGINE_HPP
