/*
 *
 */

#pragma once

#include "GameEventMgr.h"
#include "ObjectMgr.h"
#include "PlayerBotAI.h"
#include "AdvancedPlayerBotAI.h"

/*
 * Elemental Invasion
 */

enum
{
    EVENT_INVASION = 13,
    EVENT_FIRE = 68,
    EVENT_AIR = 69,
    EVENT_EARTH = 70,
    EVENT_WATER = 71,
    EVENT_BOSS_F = 72,
    EVENT_BOSS_A = 73,
    EVENT_BOSS_E = 74,
    EVENT_BOSS_W = 75,

    BOSS_A_GUID = 58000,
    BOSS_W_GUID = 1184054,
    BOSS_F_GUID = 58300,
    BOSS_E_GUID = 58100,
};

struct InvasionDataStruct
{
    uint32 eventRift;
    uint32 eventBoss;
    uint32 varDelay;
    uint32 varKills;
    uint32 varStage;
    uint32 bossGuid;
};

const static InvasionDataStruct InvasionData[] =
{
    { EVENT_FIRE,   EVENT_BOSS_F, VAR_DELAY_FIRE,   VAR_FIRE_KILLS,     VAR_FIRE,   BOSS_F_GUID },
    { EVENT_AIR,    EVENT_BOSS_A, VAR_DELAY_AIR,    VAR_AIR_KILLS,      VAR_AIR,    BOSS_A_GUID },
    { EVENT_EARTH,  EVENT_BOSS_E, VAR_DELAY_EARTH,  VAR_EARTH_KILLS,    VAR_EARTH,  BOSS_E_GUID },
    { EVENT_WATER,  EVENT_BOSS_W, VAR_DELAY_WATER,  VAR_WATER_KILLS,    VAR_WATER,  BOSS_W_GUID }
};

struct ElementalInvasion : WorldEvent
{
    ElementalInvasion() : WorldEvent(EVENT_INVASION) {}

    void Update() override;
    void Enable() override;
    void Disable() override;

private:
    void StartLocalInvasion(uint8 index, uint32 stage);
    void StartLocalBoss(uint8 index, uint32 stage, uint8 delay);
    void StopLocalInvasion(uint8 index, uint32 stage, uint8 delay);
    void ResetThings();
};

/*
 * Leprithus (rare) & Rotten Ghouls spawn at night
 */

enum LeprithusEventState
{
    LEPRITHUS_EVENT_NONE = 0,
    LEPRITHUS_EVENT_ONGOING = 150
};

struct Leprithus : WorldEvent
{
    Leprithus() : WorldEvent(LEPRITHUS_EVENT_ONGOING) {}

    void Update() override;
    void Enable() override;
    void Disable() override;

private:
    LeprithusEventState GetLeprithusState();
};

/*
 * Moonbrook graveyard vultures(Fleshrippers) spawn at daylight
 */

enum MoonbrookEventState
{
    MOONBROOK_EVENT_NONE = 0,
    MOONBROOK_EVENT_ONGOING = 151
};

struct Moonbrook : WorldEvent
{
    Moonbrook() : WorldEvent(MOONBROOK_EVENT_ONGOING) {}

    void Update() override;
    void Enable() override;
    void Disable() override;

private:
    MoonbrookEventState GetMoonbrookState();
};

/*
 * Dragons of Nightmare
 */

enum
{
    EVENT_NIGHTMARE = 66,       // spawn of all 4 Dragons of Nightmare
};

static uint32 NightmareDragons[] =
{
    NPC_YSONDRE, NPC_LETHON, NPC_EMERISS, NPC_TAERAR
};

struct DragonsOfNightmare : WorldEvent
{
    DragonsOfNightmare() : WorldEvent(EVENT_NIGHTMARE) {}

    void Update() override;
    void Enable() override;
    void Disable() override;

    static void CheckSingleVariable(uint32 idx, uint32& value);

private:
    void GetAliveCountAndUpdateRespawnTime(std::vector<ObjectGuid> &dragons, uint32 &alive, time_t respawnTime);
    bool LoadDragons(std::vector<ObjectGuid> &dragonGUIDs);
    //void GetExistingDragons(std::vector<ObjectGuid> &dragonGUIDs, std::vector<Creature*> &existingDragons);
    void PermutateDragons();
};

/*
 * Darkmoon Faire
 */

enum DarkmoonState
{
    DARKMOON_NONE = 0,
    DARKMOON_A2_INSTALLATION = 100, // TODO (spawns, game_event)
    DARKMOON_A2 = 4,
    DARKMOON_H2_INSTALLATION = 101, // TODO (spawns, game_event)
    DARKMOON_H2 = 5,
};

static const uint16 DMFValidEvent[] =
{
    DARKMOON_A2_INSTALLATION, DARKMOON_A2,
    DARKMOON_H2_INSTALLATION, DARKMOON_H2
};

struct DarkmoonFaire : WorldEvent
{
    DarkmoonFaire() : WorldEvent(DARKMOON_A2) {} // TODO - should not be used that way

    void Update() override;
    void Enable() override;
    void Disable() override;

private:
    uint32 FindMonthFirstMonday(bool &foireAlly, struct tm *timeinfo);
    DarkmoonState GetDarkmoonState();
};

/*
 * Lunar Festival Firework
 */

enum
{
    EVENT_FIREWORKS         = 6,
    EVENT_LUNAR_FIREWORKS   = 76,

    FIREWORKS_DURATION      = 5
};

struct LunarFestivalFirework : WorldEvent
{
    LunarFestivalFirework() : WorldEvent(EVENT_FIREWORKS) {}

    void Update() override;
    void Enable() override;
    void Disable() override;

private:
    bool IsHourBeginning(uint8 minutes = FIREWORKS_DURATION) const;
};

enum EventSilithusWarEffortState
{
    EVENT_SILITHUS_WE_START = 98
};


struct RaceClassCombo
{
    int Race;
    int Class;

    RaceClassCombo(int InRace, int InClass)
        : Race(InRace), Class(InClass)
    {}
};

class BattlePlayerAI : public AdvancedPlayerBotAI
{
public:
    explicit BattlePlayerAI(Player* pPlayer, uint8 _race_, uint8 _class_, uint32 mapId, uint32 instanceId, float x, float y, float z, float o) :
        AdvancedPlayerBotAI(pPlayer, _race_, _class_, mapId, instanceId, x, y, z, o)
    {
    }

    virtual ~BattlePlayerAI ()
    {
    }

    virtual void OnPlayerLogin();

};

struct BotEventInfo
{
    BattlePlayerAI* pBot;

    BotEventInfo()
        : BotEventInfo(nullptr)
    {}

    BotEventInfo(BattlePlayerAI* InBot)
        : pBot(InBot)
    {}
};

struct SilithusWarEffortBattle : WorldEvent
{
    SilithusWarEffortBattle();

    void Update() override;
    void Enable() override;
    void Disable() override;

private:

    std::vector <RaceClassCombo> AvaliableCombos;
    std::vector <BotEventInfo> Bots;

    std::vector <Creature*> SummonedMobs;
    const WorldLocation EventPos = WorldLocation(1, -8065.42f, 1527.93f, 2.61001f);
};