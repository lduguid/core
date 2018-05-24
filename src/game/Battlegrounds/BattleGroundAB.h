/*
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
 * Copyright (C) 2009-2011 MaNGOSZero <https://github.com/mangos/zero>
 * Copyright (C) 2011-2016 Nostalrius <https://nostalrius.org>
 * Copyright (C) 2016-2017 Elysium Project <https://github.com/elysium-project>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __BATTLEGROUNDAB_H
#define __BATTLEGROUNDAB_H

#include "Common.h"
#include "BattleGround.h"

enum BG_AB_WorldStates
{
    BG_AB_OP_OCCUPIED_BASES_HORDE       = 1778,
    BG_AB_OP_OCCUPIED_BASES_ALLY        = 1779,
    BG_AB_OP_RESOURCES_ALLY             = 1776,
    BG_AB_OP_RESOURCES_HORDE            = 1777,
    BG_AB_OP_RESOURCES_MAX              = 1780,
    BG_AB_OP_RESOURCES_WARNING          = 1955
/*
    BG_AB_OP_STABLE_ICON                = 1842,             //Stable map icon (NONE)
    BG_AB_OP_STABLE_STATE_ALIENCE       = 1767,             //Stable map state (ALIENCE)
    BG_AB_OP_STABLE_STATE_HORDE         = 1768,             //Stable map state (HORDE)
    BG_AB_OP_STABLE_STATE_CON_ALI       = 1769,             //Stable map state (CON ALIENCE)
    BG_AB_OP_STABLE_STATE_CON_HOR       = 1770,             //Stable map state (CON HORDE)
    BG_AB_OP_FARM_ICON                  = 1845,             //Farm map icon (NONE)
    BG_AB_OP_FARM_STATE_ALIENCE         = 1772,             //Farm state (ALIENCE)
    BG_AB_OP_FARM_STATE_HORDE           = 1773,             //Farm state (HORDE)
    BG_AB_OP_FARM_STATE_CON_ALI         = 1774,             //Farm state (CON ALIENCE)
    BG_AB_OP_FARM_STATE_CON_HOR         = 1775,             //Farm state (CON HORDE)

    BG_AB_OP_BLACKSMITH_ICON            = 1846,             //Blacksmith map icon (NONE)
    BG_AB_OP_BLACKSMITH_STATE_ALIENCE   = 1782,             //Blacksmith map state (ALIENCE)
    BG_AB_OP_BLACKSMITH_STATE_HORDE     = 1783,             //Blacksmith map state (HORDE)
    BG_AB_OP_BLACKSMITH_STATE_CON_ALI   = 1784,             //Blacksmith map state (CON ALIENCE)
    BG_AB_OP_BLACKSMITH_STATE_CON_HOR   = 1785,             //Blacksmith map state (CON HORDE)
    BG_AB_OP_LUMBERMILL_ICON            = 1844,             //Lumber Mill map icon (NONE)
    BG_AB_OP_LUMBERMILL_STATE_ALIENCE   = 1792,             //Lumber Mill map state (ALIENCE)
    BG_AB_OP_LUMBERMILL_STATE_HORDE     = 1793,             //Lumber Mill map state (HORDE)
    BG_AB_OP_LUMBERMILL_STATE_CON_ALI   = 1794,             //Lumber Mill map state (CON ALIENCE)
    BG_AB_OP_LUMBERMILL_STATE_CON_HOR   = 1795,             //Lumber Mill map state (CON HORDE)
    BG_AB_OP_GOLDMINE_ICON              = 1843,             //Gold Mine map icon (NONE)
    BG_AB_OP_GOLDMINE_STATE_ALIENCE     = 1787,             //Gold Mine map state (ALIENCE)
    BG_AB_OP_GOLDMINE_STATE_HORDE       = 1788,             //Gold Mine map state (HORDE)
    BG_AB_OP_GOLDMINE_STATE_CON_ALI     = 1789,             //Gold Mine map state (CON ALIENCE
    BG_AB_OP_GOLDMINE_STATE_CON_HOR     = 1790,             //Gold Mine map state (CON HORDE)
*/
};

const uint32 BG_AB_OP_NODESTATES[5] =    {1767, 1782, 1772, 1792, 1787};

const uint32 BG_AB_OP_NODEICONS[5]  =    {1842, 1846, 1845, 1844, 1843};

enum BG_AB_ObjectType
{
    // TODO drop them (pool-system should be used for this)
    //buffs
    BG_AB_OBJECT_SPEEDBUFF_STABLES       = 1,
    BG_AB_OBJECT_REGENBUFF_STABLES       = 2,
    BG_AB_OBJECT_BERSERKBUFF_STABLES     = 3,
    BG_AB_OBJECT_SPEEDBUFF_BLACKSMITH    = 4,
    BG_AB_OBJECT_REGENBUFF_BLACKSMITH    = 5,
    BG_AB_OBJECT_BERSERKBUFF_BLACKSMITH  = 6,
    BG_AB_OBJECT_SPEEDBUFF_FARM          = 7,
    BG_AB_OBJECT_REGENBUFF_FARM          = 8,
    BG_AB_OBJECT_BERSERKBUFF_FARM        = 9,
    BG_AB_OBJECT_SPEEDBUFF_LUMBER_MILL   = 10,
    BG_AB_OBJECT_REGENBUFF_LUMBER_MILL   = 11,
    BG_AB_OBJECT_BERSERKBUFF_LUMBER_MILL = 12,
    BG_AB_OBJECT_SPEEDBUFF_GOLD_MINE     = 13,
    BG_AB_OBJECT_REGENBUFF_GOLD_MINE     = 14,
    BG_AB_OBJECT_BERSERKBUFF_GOLD_MINE   = 15,
    BG_AB_OBJECT_MAX                     = 16,
};


/* node events */
// node-events are just event1=BG_AB_Nodes, event2=BG_AB_NodeStatus
// so we don't need to define the constants here :)

enum BG_AB_Timers
{
    BG_AB_FLAG_CAPTURING_TIME           = 60000,
};

enum BG_AB_Score
{
    BG_AB_WARNING_NEAR_VICTORY_SCORE    = 1800,
    BG_AB_MAX_TEAM_SCORE                = 2000
};

/* do NOT change the order, else wrong behaviour */
enum BG_AB_Nodes
{
    BG_AB_NODE_STABLES          = 0,
    BG_AB_NODE_BLACKSMITH       = 1,
    BG_AB_NODE_FARM             = 2,
    BG_AB_NODE_LUMBER_MILL      = 3,
    BG_AB_NODE_GOLD_MINE        = 4,
    BG_AB_NODES_ERROR           = 255
};

#define BG_AB_NODES_MAX   5

enum BG_AB_NodeStatus
{
    BG_AB_NODE_TYPE_NEUTRAL             = 0,
    BG_AB_NODE_TYPE_CONTESTED           = 1,
    BG_AB_NODE_STATUS_ALLY_CONTESTED    = 1,
    BG_AB_NODE_STATUS_HORDE_CONTESTED   = 2,
    BG_AB_NODE_TYPE_OCCUPIED            = 3,
    BG_AB_NODE_STATUS_ALLY_OCCUPIED     = 3,
    BG_AB_NODE_STATUS_HORDE_OCCUPIED    = 4
};

enum BG_AB_Sounds
{
    BG_AB_SOUND_NODE_CLAIMED            = 8192,
    BG_AB_SOUND_NODE_CAPTURED_ALLIANCE  = 8173,
    BG_AB_SOUND_NODE_CAPTURED_HORDE     = 8213,
    BG_AB_SOUND_NODE_ASSAULTED_ALLIANCE = 8212,
    BG_AB_SOUND_NODE_ASSAULTED_HORDE    = 8174,
    BG_AB_SOUND_NEAR_VICTORY            = 8456
};

enum BG_AB_NodeOccupiedCredit
{
    BG_AB_NODE_STABLES_CREDIT       = 15001,
    BG_AB_NODE_BLACKSMITH_CREDIT    = 15002,
    BG_AB_NODE_FARM_CREDIT          = 15003,
    BG_AB_NODE_LUMBER_MILL_CREDIT   = 15004,
    BG_AB_NODE_GOLD_MINE_CREDIT     = 15005,
};

// [-PROGRESSIVE] Patch 1.9 and less code:
#define BG_AB_NotABBGWeekendHonorTicks      330
#define BG_AB_ABBGWeekendHonorTicks         200
#define BG_AB_NotABBGWeekendReputationTicks 330
#define BG_AB_ABBGWeekendReputationTicks    200

// Tick intervals and given points: case 0,1,2,3,4,5 captured nodes
const uint32 BG_AB_TickIntervals[6] = {0, 12000, 9000, 6000, 3000, 1000};
const uint32 BG_AB_TickPoints[6] = {0, 10, 10, 10, 10, 30};

// Honor granted depending on player's level
const uint32 BG_AB_PerTickHonor[MAX_BATTLEGROUND_BRACKETS] = {41, 68, 113, 189, 198};
const uint32 BG_AB_WinMatchHonor[MAX_BATTLEGROUND_BRACKETS] = {41, 68, 113, 189, 198};

// WorldSafeLocs ids for 5 nodes, and for ally, and horde starting location
const uint32 BG_AB_GraveyardIds[7] = {895, 894, 893, 897, 896, 898, 899};

// x, y, z, o
// Nostalrius: Modified to retail 5.4.8 positions.
const float BG_AB_BuffPositions[BG_AB_NODES_MAX][4] = {
    {1185.566f, 1184.629f, -56.36329f, 2.303831f},         // stables
    {989.939026f, 1008.75f, -42.60327f, 0.8203033f},       // blacksmith
    {818.0089f, 842.3543f, -56.54062f, 3.176533f},         // farm
    {808.8463f, 1185.417f, 11.92161f, 5.619962f},          // lumber mill
    {1147.091f, 816.8362f, -98.39896f, 6.056293f}          // gold mine
};

class BattleGroundABScore : public BattleGroundScore
{
    public:
        BattleGroundABScore(): BasesAssaulted(0), BasesDefended(0) {};
        virtual ~BattleGroundABScore() {};
        uint32 BasesAssaulted;
        uint32 BasesDefended;
};

class BattleGroundAB : public BattleGround
{
    friend class BattleGroundMgr;

    public:
        BattleGroundAB();
        ~BattleGroundAB();

        void Update(uint32 diff);
        void AddPlayer(Player *plr);
        virtual void StartingEventCloseDoors();
        virtual void StartingEventOpenDoors();
        void RemovePlayer(Player *plr, ObjectGuid guid);
        void HandleAreaTrigger(Player *Source, uint32 Trigger);
        virtual bool SetupBattleGround();
        virtual void Reset();
        void EndBattleGround(Team winner);
        virtual WorldSafeLocsEntry const* GetClosestGraveYard(Player* player);

        /* Scorekeeping */
        virtual void UpdatePlayerScore(Player *Source, uint32 type, uint32 value);

        virtual void FillInitialWorldStates(WorldPacket& data, uint32& count);

        /* Nodes occupying */
        virtual void EventPlayerClickedOnFlag(Player *source, GameObject* target_obj);

    private:
        /* Gameobject spawning/despawning */
        void _CreateBanner(uint8 node, uint8 type, uint8 teamIndex);
        void _DelBanner(uint8 node, uint8 type, uint8 teamIndex);
        void _SendNodeUpdate(uint8 node);

        /* Creature spawning/despawning */
        // TODO: working, scripted peons spawning
        void _NodeOccupied(uint8 node,Team team);

        int32 _GetNodeNameId(uint8 node);

        /* Nodes info:
            0: neutral
            1: ally contested
            2: horde contested
            3: ally occupied
            4: horde occupied     */
        uint8               m_Nodes[BG_AB_NODES_MAX];
        uint8               m_prevNodes[BG_AB_NODES_MAX];   // used for performant wordlstate-updating
        uint32              m_NodeTimers[BG_AB_NODES_MAX];
        uint32              m_lastTick[BG_TEAMS_COUNT];
        uint32              m_HonorScoreTics[BG_TEAMS_COUNT];
        uint32              m_ReputationScoreTics[BG_TEAMS_COUNT];
        bool                m_IsInformedNearVictory;
        uint32              m_HonorTics;
        uint32              m_ReputationTics;
};
#endif
