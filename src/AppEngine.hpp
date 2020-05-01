#ifndef APP_ENGINE_HPP
#define APP_ENGINE_HPP

#include "ResourceManager.hpp"
#include "Util/DiscordRPC.hpp"
#include "Server.hpp"

#include <SFML/Graphics.hpp>

#include <vector>
#include <string>

class AppState;

struct AppSettings
{
	std::string version = "0.17-20w18a-multiplayer";
	std::string title = "bobwars " + version;

	// TODO: make these const
	int clientProtocol = 1;
	int serverProtocol = 1;

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

	sf::Keyboard::Key multipleSelectionModifier = sf::Keyboard::Key::LControl;
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

	std::vector<AppState*> states;
	std::vector<sf::Event> events;

	sf::RenderWindow* window;
	AppSettings settings;

	ResourceManager* resMan = nullptr;

	DiscordRPC dRPC;

	Keybindings keys;

	time_t delta;

	bool dedicatedServer = false;

	DedicatedServer* server = nullptr;

	ClientServerInterface network;

private:
	bool running;

	sf::Clock deltaClock;
};

#endif // !APP_ENGINE_HPP
