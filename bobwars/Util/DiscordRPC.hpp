#ifndef DISCORD_RPC_HPP
#define DISCORD_RPC_HPP

#include <string>

// TODO: find a way to not include this everywhere.
#include <DISCORD\discord_rpc.h>

class DiscordRPC
{
public:
	void Init();
	void Shutdown();

	void update();

	void setState(std::string state);
	void setDetails(std::string details);
	void setStartTime(int64_t startTime);
	void setEndTime(int64_t endTime);
	void setLargeImage(std::string imageKey, std::string imageText = "");
	void setSmallImage(std::string imageKey, std::string imageText = "");
	void setPartyID(std::string partyID);
	void setPartySize(int partySize);
	void setPartyMaxPlayers(int maxPlayers);
	void setMatchSecret(std::string matchSecret);
	void setJoinSecrete(std::string joinSecret);
	void setSpectateSecrete(std::string spectateSecret);

	void updatePresence();
	void clearPresence();

private:
	DiscordRichPresence presence;

	static void handleDiscordReady(const DiscordUser* connectedUser);
	static void handleDiscordError(int errorCode, const char* message);
	static void handleDiscordDisconnected(int errorCode, const char* message);
};

#endif // !DISCORD_RPC_HPP
