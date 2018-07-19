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

	//update();
}
void DiscordRPC::Shutdown()
{
	logger::INFO("[DISCORD] Shutting down RPC.");

	Discord_Shutdown();
}

void DiscordRPC::update()
{
#ifdef DISCORD_DISABLE_IO_THREAD
	Discord_UpdateConnection();
#endif
	Discord_RunCallbacks();
}

void DiscordRPC::setState(const char* state)
{
	presence.state = state;

	logger::DEBUG("[DISCORD] State: " + std::string(state));
}

void DiscordRPC::setDetails(const char* details)
{
	presence.details = details;

	logger::DEBUG("[DISCORD] Details: " + std::string(details));
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

void DiscordRPC::setLargeImage(const char* imageKey, const char* imageText)
{
	presence.largeImageKey = imageKey;
	logger::DEBUG("[DISCORD] LargeImageKey: " + std::string(imageKey));

	if (strlen(imageText) > 0)
	{
		presence.largeImageText = imageText;
		logger::DEBUG("[DISCORD] LargeImageText: " + std::string(imageText));
	}
}

void DiscordRPC::setSmallImage(const char* imageKey, const char* imageText)
{
	presence.smallImageKey = imageKey;
	logger::DEBUG("[DISCORD] SmallImageKey: " + std::string(imageKey));

	if (strlen(imageText) > 0)
	{
		presence.smallImageText = imageText;
		logger::DEBUG("[DISCORD] SmallImageText: " + std::string(imageText));
	}
}

void DiscordRPC::setPartyID(const char* partyID)
{
	presence.partyId = partyID;
	logger::DEBUG("[DISCORD] PartyID: " + std::string(partyID));
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

void DiscordRPC::setMatchSecret(const char*  matchSecret)
{
	presence.matchSecret = matchSecret;
	logger::DEBUG("[DISCORD] MatchSecret: " + std::string(matchSecret));
}

void DiscordRPC::setJoinSecrete(const char*  joinSecret)
{
	presence.joinSecret = joinSecret;
	logger::DEBUG("[DISCORD] JoinSecret: " + std::string(joinSecret));
}

void DiscordRPC::setSpectateSecrete(const char*  spectateSecret)
{
	presence.spectateSecret = spectateSecret;
	logger::DEBUG("[DISCORD] SpectateSecret: " + std::string(spectateSecret));
}

void DiscordRPC::updatePresence()
{
	Discord_UpdatePresence(&presence);

	logger::INFO("[DISCORD] Updated Presence");
}

void DiscordRPC::clearPresence()
{
	Discord_ClearPresence();

	presence = {};

	logger::INFO("[DISCORD] Presence cleared.");
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
