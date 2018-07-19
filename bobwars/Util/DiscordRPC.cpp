#include "DiscordRPC.hpp"
#include "Logger.hpp"

#include <DISCORD/discord_rpc.h> // discord rpc functions

void DiscordRPC::Init()
{
	logger::INFO("[DISCORD] Initialising DiscordRPC");

	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));

	handlers.errored = &this->handleDiscordError;
	handlers.disconnected = &this->handleDiscordDisconnected;
//	handlers.joinGame = handleDiscordJoinGame;
//	handlers.spectateGame = handleDiscordSpectateGame;
//	handlers.joinRequest = handleDiscordJoinRequest;

	// intialise rich presence
	memset(&presence, 0, sizeof(presence));

	// Discord_Initialize(const char* applicationId, DiscordEventHandlers* handlers, int autoRegister, const char* optionalSteamId)
	Discord_Initialize("468957546359685121", &handlers, false, nullptr);

	update();
}
void DiscordRPC::Shutdown()
{
	Discord_Shutdown();
}

void DiscordRPC::update()
{
#ifdef DISCORD_DISABLE_IO_THREAD
	Discord_UpdateConnection();
#endif
	Discord_RunCallbacks();
}

void DiscordRPC::setState(std::string state)
{
	presence.state = state.c_str();

	logger::DEBUG("[DISCORD] State: " + state);
}

void DiscordRPC::setDetails(std::string details)
{
	presence.details = details.c_str();

	logger::DEBUG("[DISCORD] Details: " + details);
}

void DiscordRPC::setStartTime(int64_t startTime)
{
	presence.startTimestamp = startTime;

	logger::DEBUG("[DISCORD] StartTimestamp: " + std::to_string(startTime));
}

void DiscordRPC::setEndTime(int64_t endTime)
{
	presence.endTimestamp = endTime;
	logger::DEBUG("[DISCORD] EndTimestamp: " + std::to_string(endTime));
}

void DiscordRPC::setLargeImage(std::string imageKey, std::string imageText)
{
	presence.largeImageKey = imageKey.c_str();
	logger::DEBUG("[DISCORD] LargeImageKey: " + imageKey);

	if (!imageText.empty())
	{
		presence.largeImageText = imageText.c_str();
		logger::DEBUG("[DISCORD] LargeImageText: " + imageText);
	}
}

void DiscordRPC::setSmallImage(std::string imageKey, std::string imageText)
{
	presence.smallImageKey = imageKey.c_str();
	logger::DEBUG("[DISCORD] SmallImageKey: " + imageKey);

	if (!imageText.empty())
	{
		presence.smallImageText = imageText.c_str();
		logger::DEBUG("[DISCORD] SmallImageText: " + imageText);
	}
}

void DiscordRPC::setPartyID(std::string partyID)
{
	presence.partyId = partyID.c_str();
	logger::DEBUG("[DISCORD] PartyID: " + partyID);
}

void DiscordRPC::setPartySize(int partySize)
{
	presence.partySize = partySize;
	logger::DEBUG("[DISCORD] PartySize: " + std::to_string(partySize));
}

void DiscordRPC::setPartyMaxPlayers(int maxPlayers)
{
	presence.partyMax = maxPlayers;
	logger::DEBUG("[DISCORD] PartyMaxSize: " + std::to_string(maxPlayers));
}

void DiscordRPC::setMatchSecret(std::string matchSecret)
{
	presence.matchSecret = matchSecret.c_str();
	logger::DEBUG("[DISCORD] MatchSecret: " + matchSecret);
}

void DiscordRPC::setJoinSecrete(std::string joinSecret)
{
	presence.joinSecret = joinSecret.c_str();
	logger::DEBUG("[DISCORD] JoinSecret: " + joinSecret);
}

void DiscordRPC::setSpectateSecrete(std::string spectateSecret)
{
	presence.spectateSecret = spectateSecret.c_str();
	logger::DEBUG("[DISCORD] SpectateSecret: " + spectateSecret);
}

void DiscordRPC::updatePresence()
{

	Discord_UpdatePresence(&presence);
}

void DiscordRPC::clearPresence()
{
	DiscordRichPresence pres;
	presence = pres;

	Discord_ClearPresence();
}

void DiscordRPC::handleDiscordReady(const DiscordUser* connectedUser)
{
	logger::INFO("[DISCORD] Connected!");
	logger::INFO("[DISCORD] User: " + std::string(connectedUser->username) + std::string(connectedUser->discriminator) + " (" + std::string(connectedUser->userId) + ")");
}

void DiscordRPC::handleDiscordError(int errorCode, const char* message)
{
	logger::ERROR("[DISCORD] Disconnected: " + std::string(message) + " (" + std::to_string(errorCode) + ")");
}

void DiscordRPC::handleDiscordDisconnected(int errorCode, const char* message)
{
	logger::ERROR("[DISCORD] " + std::string(message) + " (" + std::to_string(errorCode) + ")");
}
