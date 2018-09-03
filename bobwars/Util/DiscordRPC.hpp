#ifndef DISCORD_RPC_HPP
#define DISCORD_RPC_HPP

#include <string>

// TODO: find a way to not include this everywhere.
#include <DISCORD\discord_rpc.h>

// TODO: test std::string for null terminator

class DiscordRPC
{
public:
	void Init();
	void Shutdown();

	void update();

	void setState(const char* state);
	void setDetails(const char* details);
	void setStartTime(int64_t startTime);
	void setEndTime(int64_t endTime);
	void setLargeImage(const char* imageKey, const char* imageText = "");
	void setSmallImage(const char* imageKey, const char* imageText = "");
	void setPartyID(const char* partyID);
	void setPartySize(int partySize);
	void setPartyMaxPlayers(int maxPlayers);
	void setMatchSecret(const char* matchSecret);
	void setJoinSecrete(const char* joinSecret);
	void setSpectateSecrete(const char* spectateSecret);

	void updatePresence();
	void clearPresence();

private:
	DiscordRichPresence presence;

	static void handleDiscordReady(const DiscordUser* connectedUser);
	static void handleDiscordError(int errorCode, const char* message);
	static void handleDiscordDisconnected(int errorCode, const char* message);
};

#endif // !DISCORD_RPC_HPP
