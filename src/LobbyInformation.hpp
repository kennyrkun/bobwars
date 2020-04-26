#ifndef LOBBY_INFORMATION_HPP
#define LOBBY_INFORMATION_HPP

#include <SFML/Network.hpp>

#include <ctime>
#include <string>
#include <vector>

struct LobbyInformation
{
    std::string gameName = "Multiplayer Game";

    std::string type;
    std::string mapSize;
    std::string difficulty;
    std::string resources;
    std::string population;
    std::string gameSpeed;
    std::string revealMap;
    std::string victory;

    bool teamTogether;
    bool allTechs;
    bool allowCheats;
    bool lockTeams;
    bool lockSpeed;
    bool recordGame;

    bool allPlayersReady = false;
    int totalPlayers = -1;
    int maxPlayers = 7;

    time_t timeSent;

    struct SlotInformation
    {
        enum SlotState
        {
            Open,
            Closed,
            OccupiedByPlayer,
            AI,
        } state;

        std::string name;
        float ping;
        std::string team;
        std::string color;
    };

    std::vector<SlotInformation> slots;
};

inline sf::Packet& operator <<(sf::Packet& packet, const LobbyInformation& lobby)
{
    packet << lobby.gameName 
           << lobby.type 
           << lobby.mapSize
           << lobby.difficulty 
           << lobby.resources 
           << lobby.population 
           << lobby.gameSpeed 
           << lobby.revealMap 
           << lobby.teamTogether 
           << lobby.allTechs 
           << lobby.allowCheats 
           << lobby.lockTeams 
           << lobby.lockSpeed 
           << lobby.recordGame 
           << lobby.allPlayersReady 
           << lobby.totalPlayers
           << lobby.maxPlayers;

    for (size_t i = 0; i < lobby.slots.size(); i++)
    {
        packet << lobby.slots[i].state
               << lobby.slots[i].name
               << lobby.slots[i].ping
               << lobby.slots[i].team
               << lobby.slots[i].color;
    }

    return packet;
}

inline sf::Packet& operator >>(sf::Packet& packet, LobbyInformation& lobby)
{
        packet >> lobby.gameName 
           >> lobby.type 
           >> lobby.mapSize
           >> lobby.difficulty 
           >> lobby.resources 
           >> lobby.population 
           >> lobby.gameSpeed 
           >> lobby.revealMap 
           >> lobby.teamTogether 
           >> lobby.allTechs 
           >> lobby.allowCheats 
           >> lobby.lockTeams 
           >> lobby.lockSpeed 
           >> lobby.recordGame 
           >> lobby.allPlayersReady 
           >> lobby.totalPlayers
           >> lobby.maxPlayers;

    for (size_t i = 0; i < lobby.totalPlayers; i++)
    {
        LobbyInformation::SlotInformation slot;

        int state = 0;
        packet >> state;

        auto state_ = static_cast<LobbyInformation::SlotInformation::SlotState>(state);
        slot.state = state_;

        packet >> slot.name
               >> slot.ping
               >> slot.team
               >> slot.color;

        lobby.slots.push_back(slot);
    }

    return packet;
}

#endif // !LOBBY_INFORMATION_HPP