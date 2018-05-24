/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

/* ScriptData
 SDName: Silithus
 SD%Complete: 100
 SDComment: Quest support: 7785, 8304, 8534, 8519.
 SDCategory: Silithus
 EndScriptData */

/* ContentData
 npc_highlord_demitrian
 npcs_rutgar_and_frankal
 go_pierre_vents
 npc_cenarion_scout_azenel
 EndContentData */

#include "scriptPCH.h"
#include "Database/DatabaseEnv.h"

/*###
 ## npc_highlord_demitrian
 ###*/

#define GOSSIP_ITEM_DEMITRIAN1 "What do you know of it?"
#define GOSSIP_ITEM_DEMITRIAN2 "I am listening , Demitrian."
#define GOSSIP_ITEM_DEMITRIAN3 "Continue, please."
#define GOSSIP_ITEM_DEMITRIAN4 "A battle?"
#define GOSSIP_ITEM_DEMITRIAN5 "<Nod>"
#define GOSSIP_ITEM_DEMITRIAN6 "Caught unaware? How?"
#define GOSSIP_ITEM_DEMITRIAN7 "So what did Ragnaros do next?"

enum
{
    QUEST_EXAMINE_THE_VESSEL        =   7785,
    ITEM_BINDINGS_WINDSEEKER_LEFT   =   18563,
    ITEM_BINDINGS_WINDSEEKER_RIGHT  =   18564,
    ITEM_VESSEL_OF_REBIRTH          =   19016,
    GOSSIP_TEXTID_DEMITRIAN1        =   6842,
    GOSSIP_TEXTID_DEMITRIAN2        =   6843,
    GOSSIP_TEXTID_DEMITRIAN3        =   6844,
    GOSSIP_TEXTID_DEMITRIAN4        =   6867,
    GOSSIP_TEXTID_DEMITRIAN5        =   6868,
    GOSSIP_TEXTID_DEMITRIAN6        =   6869,
    GOSSIP_TEXTID_DEMITRIAN7        =   6870
};

bool GossipHello_npc_highlord_demitrian(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetQuestStatus(QUEST_EXAMINE_THE_VESSEL) == QUEST_STATUS_NONE &&
            (pPlayer->HasItemCount(ITEM_BINDINGS_WINDSEEKER_LEFT, 1, false) || pPlayer->HasItemCount(ITEM_BINDINGS_WINDSEEKER_RIGHT, 1, false)))
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_DEMITRIAN1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_highlord_demitrian(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch (uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_DEMITRIAN2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_DEMITRIAN1, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_DEMITRIAN3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_DEMITRIAN2, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_DEMITRIAN4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_DEMITRIAN3, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_DEMITRIAN5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_DEMITRIAN4, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_DEMITRIAN6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_DEMITRIAN5, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_DEMITRIAN7, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_DEMITRIAN6, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+6:
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_DEMITRIAN7, pCreature->GetGUID());

            ItemPosCountVec dest;
            uint8 msg = pPlayer->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, ITEM_VESSEL_OF_REBIRTH, 1);
            if (msg == EQUIP_ERR_OK)
                pPlayer->StoreNewItem(dest, ITEM_VESSEL_OF_REBIRTH, true);
            break;
    }
    return true;
}

/*###
 ## npcs_rutgar_and_frankal
 ###*/

//gossip item text best guess
#define GOSSIP_ITEM_SEEK1 "I seek information about Natalia"

#define GOSSIP_ITEM_RUTGAR2 "That sounds dangerous!"
#define GOSSIP_ITEM_RUTGAR3 "What did you do?"
#define GOSSIP_ITEM_RUTGAR4 "Who?"
#define GOSSIP_ITEM_RUTGAR5 "Women do that. What did she demand?"
#define GOSSIP_ITEM_RUTGAR6 "What do you mean?"
#define GOSSIP_ITEM_RUTGAR7 "What happened next?"

#define GOSSIP_ITEM_FRANKAL11 "Yes, please continue"
#define GOSSIP_ITEM_FRANKAL12 "What language?"
#define GOSSIP_ITEM_FRANKAL13 "The Priestess attacked you?!"
#define GOSSIP_ITEM_FRANKAL14 "I should ask the monkey about this"
#define GOSSIP_ITEM_FRANKAL15 "Then what..."

enum
{
    QUEST_DEAREST_NATALIA       =   8304,
    NPC_RUTGAR                  =   15170,
    NPC_FRANKAL                 =   15171,
    TRIGGER_RUTGAR              =   15222,
    TRIGGER_FRANKAL             =   15221,
    GOSSIP_TEXTID_RF            =   7754,
    GOSSIP_TEXTID_RUTGAR1       =   7755,
    GOSSIP_TEXTID_RUTGAR2       =   7756,
    GOSSIP_TEXTID_RUTGAR3       =   7757,
    GOSSIP_TEXTID_RUTGAR4       =   7758,
    GOSSIP_TEXTID_RUTGAR5       =   7759,
    GOSSIP_TEXTID_RUTGAR6       =   7760,
    GOSSIP_TEXTID_RUTGAR7       =   7761,
    GOSSIP_TEXTID_FRANKAL1      =   7762,
    GOSSIP_TEXTID_FRANKAL2      =   7763,
    GOSSIP_TEXTID_FRANKAL3      =   7764,
    GOSSIP_TEXTID_FRANKAL4      =   7765,
    GOSSIP_TEXTID_FRANKAL5      =   7766,
    GOSSIP_TEXTID_FRANKAL6      =   7767
};

bool GossipHello_npcs_rutgar_and_frankal(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetQuestStatus(QUEST_DEAREST_NATALIA) == QUEST_STATUS_INCOMPLETE &&
            pCreature->GetEntry() == NPC_RUTGAR &&
            !pPlayer->GetReqKillOrCastCurrentCount(QUEST_DEAREST_NATALIA, TRIGGER_RUTGAR))
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_SEEK1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    if (pPlayer->GetQuestStatus(QUEST_DEAREST_NATALIA) == QUEST_STATUS_INCOMPLETE &&
            pCreature->GetEntry() == NPC_FRANKAL &&
            pPlayer->GetReqKillOrCastCurrentCount(QUEST_DEAREST_NATALIA, TRIGGER_RUTGAR))
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_SEEK1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);

    pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_RF, pCreature->GetGUID());

    return true;
}

bool GossipSelect_npcs_rutgar_and_frankal(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch (uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_RUTGAR2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_RUTGAR1, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_RUTGAR3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_RUTGAR2, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_RUTGAR4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_RUTGAR3, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_RUTGAR5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_RUTGAR4, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_RUTGAR6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_RUTGAR5, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_RUTGAR7, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_RUTGAR6, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_RUTGAR7, pCreature->GetGUID());
            //'kill' our trigger to update quest status
            pPlayer->KilledMonsterCredit(TRIGGER_RUTGAR, pCreature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 9:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FRANKAL11, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_FRANKAL1, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 10:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FRANKAL12, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_FRANKAL2, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 11:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FRANKAL13, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_FRANKAL3, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 12:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FRANKAL14, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_FRANKAL4, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 13:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FRANKAL15, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_FRANKAL5, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 14:
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_FRANKAL6, pCreature->GetGUID());
            //'kill' our trigger to update quest status
            pPlayer->KilledMonsterCredit(TRIGGER_FRANKAL, pCreature->GetGUID());
            break;
    }
    return true;
}

enum
{
    SPELL_SET_AURA            = 24746,
    SPELL_RED_LIGHTNING       = 24240,

    ITEM_SET_ENTRY            = 492,

    ITEM_SET_SHOULDERS        = 20406,
    ITEM_SET_CHEST            = 20407,
    ITEM_SET_HEAD             = 20408,

    SPELL_APPARITION          = 25035,

    GO_TYPE_PIERRE_ERR        = 0,
    GO_TYPE_PIERRE_INF        = 1,
    GO_TYPE_PIERRE_MOYENNE    = 2,
    GO_TYPE_PIERRE_SUP        = 3,

    // Pierre moyenne
    ITEM_ACCES_PIERRE_MOYENNE = 20422,
    AURA_ACCES_PIERRE_MOYENNE = 24748,

    // Pierre superieure
    ITEM_ACCES_PIERRE_SUP     = 20451,
    AURA_ACCES_PIERRE_SUP     = 24782,

    GOSSIP_STONE_FIRST_HELLO    = 69,
    GOSSIP_STONE_FIRST_OPTION   = NOST_TEXT(220)
};

struct Silithus_WindStonesBossData
{
    int stoneType;
    int action;
    int summonEntry;
    int reqItem;
    int gossipOption;
};
static Silithus_WindStonesBossData const windStonesBosses[] =
{
    {GO_TYPE_PIERRE_INF,    1,  15209,  20416, NOST_TEXT(223) },
    {GO_TYPE_PIERRE_INF,    2,  15307,  20419, NOST_TEXT(224) },
    {GO_TYPE_PIERRE_INF,    3,  15212,  20418, NOST_TEXT(225) },
    {GO_TYPE_PIERRE_INF,    4,  15211,  20420, NOST_TEXT(226) },

    {GO_TYPE_PIERRE_MOYENNE,1,  15206,  20432, NOST_TEXT(227) },
    {GO_TYPE_PIERRE_MOYENNE,2,  15208,  20435, NOST_TEXT(228) },
    {GO_TYPE_PIERRE_MOYENNE,3,  15220,  20433, NOST_TEXT(229) },
    {GO_TYPE_PIERRE_MOYENNE,4,  15207,  20436, NOST_TEXT(230) },

    {GO_TYPE_PIERRE_SUP,    1,  15203,  20447, NOST_TEXT(231) },
    {GO_TYPE_PIERRE_SUP,    2,  15205,  20449, NOST_TEXT(232) },
    {GO_TYPE_PIERRE_SUP,    3,  15204,  20448, NOST_TEXT(233) },
    {GO_TYPE_PIERRE_SUP,    4,  15305,  20450, NOST_TEXT(234) },
};

struct go_pierre_ventsAI: public GameObjectAI
{
    go_pierre_ventsAI(GameObject* pGo) : GameObjectAI(pGo) {}

    uint32 GetStoneType()
    {
        switch (me->GetEntry())
        {
            // Pierre SUP
            case 180466:
            case 180539:
            case 180559:
                return GO_TYPE_PIERRE_SUP;
            // Pierre MOYENNE
            case 180554:
            case 180534:
            case 180502:
            case 180461:
                return GO_TYPE_PIERRE_MOYENNE;
            // Pierre INF
            case 180456:
            case 180518:
            case 180529:
            case 180544:
            case 180549:
            case 180564:
                return GO_TYPE_PIERRE_INF;
            default:
                return GO_TYPE_PIERRE_ERR;
        }
    }
    uint32 SelectRandomBoss(uint32 stoneType)
    {
        std::vector<uint32> possibleBosses;
        for (int i = 0; i < sizeof(windStonesBosses) / sizeof(windStonesBosses[0]); ++i)
            if (windStonesBosses[i].stoneType == stoneType)
                possibleBosses.push_back(windStonesBosses[i].summonEntry);
        ASSERT(!possibleBosses.empty());
        return possibleBosses[urand(0, possibleBosses.size() - 1)];
    }

    bool CheckPlayerHasAura(uint32 uiReqAura, Player *pUser, uint32 itemToDelete = 0)
    {
        if (uiReqAura && !pUser->HasAura(uiReqAura))
            if (!pUser->isGameMaster())
                return false;
        return true;
    }

    void UseFailed(Unit* user)
    {
        if (user->isAlive())
        {
            user->CastSpell(user, SPELL_RED_LIGHTNING, true);
            user->DealDamage(user, user->GetHealth() > 1000 ? 1000 : user->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
    }
    bool CanUse(Player* user)
    {
        if (!user || !me->isSpawned())
            return false;

        bool playerHasAura = true;

        ///- Check if allowed to use the stone ?
        switch (GetStoneType())
        {
            // Pierre SUP
            case GO_TYPE_PIERRE_SUP:
                if (!user->HasItemWithIdEquipped(ITEM_ACCES_PIERRE_SUP))
                    playerHasAura = false;
            // Pierre MOYENNE
            case GO_TYPE_PIERRE_MOYENNE:
                if (!user->HasItemWithIdEquipped(ITEM_ACCES_PIERRE_MOYENNE))
                    playerHasAura = false;
            // Pierre INF
            case GO_TYPE_PIERRE_INF:
                if (!user->HasItemWithIdEquipped(ITEM_SET_HEAD) ||
                    !user->HasItemWithIdEquipped(ITEM_SET_SHOULDERS) ||
                    !user->HasItemWithIdEquipped(ITEM_SET_CHEST))
                    playerHasAura = false;
                break;
        }

        if (!playerHasAura)
        {
            UseFailed(user);
            return false;
        }
        return true;
    }

    bool OnUse(Unit* user)
    {
        Player* player = user->ToPlayer();
        if (!CanUse(player))
            return true;

        uint32 stoneType = GetStoneType();
        player->PlayerTalkClass->ClearMenus();
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_STONE_FIRST_OPTION + stoneType - 1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        for (int i = 0; i < sizeof(windStonesBosses) / sizeof(windStonesBosses[0]); ++i)
            if (windStonesBosses[i].stoneType == stoneType)
                if (player->HasItemCount(windStonesBosses[i].reqItem, 1))
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, windStonesBosses[i].gossipOption, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + windStonesBosses[i].action);

        player->SEND_GOSSIP_MENU(GOSSIP_STONE_FIRST_HELLO + stoneType - 1, me->GetGUID());
        return true;
    }

    bool GossipSelect(Player* player, uint32 action)
    {
        if (!CanUse(player))
            return true;

        uint32 stoneType = GetStoneType();
        if (!stoneType)
            return true;

        uint32 summonEntry = 0;
        uint32 textId = 0;

        ///- Let's find out which mob we have to summon.
        switch (stoneType)
        {
            case GO_TYPE_PIERRE_SUP:
                textId = 116;
                break;
            case GO_TYPE_PIERRE_MOYENNE:
                textId = 115;
                break;
            case GO_TYPE_PIERRE_INF:
                textId = 105 + (player->getRace());
                break;
        }
        for (int i = 0; i < sizeof(windStonesBosses) / sizeof(windStonesBosses[0]); ++i)
            if (windStonesBosses[i].stoneType == stoneType && action == GOSSIP_ACTION_INFO_DEF + windStonesBosses[i].action)
                if (player->HasItemCount(windStonesBosses[i].reqItem, 1))
                {
                    summonEntry = windStonesBosses[i].summonEntry;
                    player->DestroyItemCount(windStonesBosses[i].reqItem, 1, true, false);
                }

        if (!summonEntry && action != GOSSIP_ACTION_INFO_DEF)
        {
            UseFailed(player);
            return true;
        }

        if (!summonEntry)
            summonEntry = SelectRandomBoss(stoneType);

        if (!summonEntry)
            return true;

        ///- Destroy required items.
        if (!player->ToPlayer()->isGameMaster())
        {
            switch (stoneType)
            {
                case GO_TYPE_PIERRE_SUP:
                    player->DestroyEquippedItem(ITEM_ACCES_PIERRE_SUP);
                // no break
                case GO_TYPE_PIERRE_MOYENNE:
                    player->DestroyEquippedItem(ITEM_ACCES_PIERRE_MOYENNE);
                // no break
                case GO_TYPE_PIERRE_INF:
                    player->DestroyEquippedItem(ITEM_SET_SHOULDERS);
                    player->DestroyEquippedItem(ITEM_SET_CHEST);
                    player->DestroyEquippedItem(ITEM_SET_HEAD);
                    break;
            }
        }

        ///- Summon the creature
        if (Creature* pInvoc = me->SummonCreature(summonEntry, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), me->GetAngle(player), TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 3600000))
        {
            pInvoc->SetTempPacified(5000);
            pInvoc->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
            player->CastSpell(player, SPELL_RED_LIGHTNING, true);
            pInvoc->CastSpell(pInvoc, SPELL_APPARITION, true);
            pInvoc->SetLootRecipient(player); // Force tag for summoner
            if (textId)
                pInvoc->MonsterSay(NOST_TEXT(textId));
        }

        ///- Mark stone as used.
        me->UseDoorOrButton();
        if (stoneType == GO_TYPE_PIERRE_SUP)
            me->SetRespawnTime(3600);
        else if (stoneType == GO_TYPE_PIERRE_MOYENNE)
            me->SetRespawnTime(300);
        else if (stoneType == GO_TYPE_PIERRE_INF)
            me->SetRespawnTime(90);
        else
            me->SetRespawnTime(me->ComputeRespawnDelay());
        return true;
    }
};

GameObjectAI* GetAIgo_pierre_vents(GameObject *go)
{
    return new go_pierre_ventsAI(go);
}

bool GossipSelect_go_pierre_vents(Player* user, GameObject* gobj, uint32 sender, uint32 action)
{
    user->CLOSE_GOSSIP_MENU();
    if (go_pierre_ventsAI* ai = dynamic_cast<go_pierre_ventsAI*>(gobj->AI()))
        ai->GossipSelect(user, action);
    return true;
}

/*###
 ## npc_cenarion_scout_azenel
 ###*/

#define GOSSIP_ITEM_REPORT "Your report, please!"

#define SAY_COMPLETE "Quickly, bring my report to Cenarion Hold!"

#define SPELL_CREATE_HIVE_ZORA_SCOUT_REPORT    25843

bool GossipHello_npc_cenarion_scout_azenel(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(8534) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_REPORT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

    pPlayer->SEND_GOSSIP_MENU(pCreature->GetEntry(), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_cenarion_scout_azenel(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF + 1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->CastSpell(pPlayer, SPELL_CREATE_HIVE_ZORA_SCOUT_REPORT, false);
        pCreature->MonsterWhisper(SAY_COMPLETE, pPlayer);
    }
    return true;
}

enum
{
    SPELL_FOOLS_PLIGHT              = 23504,

    SPELL_SOUL_FLAME                = 23272,
    SPELL_DREADFUL_FRIGHT           = 23275,
    SPELL_CREEPING_DOOM             = 23589,
    SPELL_CRIPPLING_CLIP            = 23279,

    EMOTE_IMMOBILIZED               = -1000650,

    SPELL_FROST_TRAP                = 13810,

    NPC_NELSON_THE_NICE             = 14536,
    NPC_SOLENOR_THE_SLAYER          = 14530,
    NPC_CREEPING_DOOM               = 14761,
    NPC_THE_CLEANER                 = 14503,

    QUEST_STAVE_OF_THE_ANCIENTS     = 7636
};

#define GOSSIP_ITEM                 "Show me your real face, demon."

/*#####
 ## npc_nelson_the_nice
 ######*/

/*#####
 ## npc_solenor_the_slayer
 ######*/

struct npc_solenorAI : public ScriptedAI
{
    npc_solenorAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bTransform      = false;
        m_uiDespawn_Timer = 0;
        Reset();
    }

    uint32 m_uiTransform_Timer;
    uint32 m_uiTransformEmote_Timer;
    bool m_bTransform;

    ObjectGuid m_hunterGuid;
    uint32 m_uiDreadfulFright_Timer;
    uint32 m_uiCreepingDoom_Timer;
    uint32 m_uiCastSoulFlame_Timer;
    uint32 m_uiDespawn_Timer;

    void Reset()
    {
        switch (m_creature->GetEntry())
        {
            case NPC_NELSON_THE_NICE:
                m_creature->SetRespawnDelay(35*MINUTE);
                m_creature->SetRespawnTime(35*MINUTE);
                m_creature->SetHomePosition(-7724.21f, 1676.43f, 7.0571f, 4.80044f);
                m_creature->NearTeleportTo(-7724.21f, 1676.43f, 7.0571f, 4.80044f);
                if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() != WAYPOINT_MOTION_TYPE)
                {
                    m_creature->SetDefaultMovementType(WAYPOINT_MOTION_TYPE);
                    m_creature->GetMotionMaster()->Initialize();
                }

                m_creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                m_uiTransform_Timer      = 10000;
                m_uiTransformEmote_Timer = 5000;
                m_bTransform             = false;
                m_uiDespawn_Timer        = 0;
                m_uiCastSoulFlame_Timer  = 0;
                break;
            case NPC_SOLENOR_THE_SLAYER:
                if (!m_uiDespawn_Timer)
                {
                    m_uiDespawn_Timer = 20*MINUTE*IN_MILLISECONDS;
                    m_uiCastSoulFlame_Timer  = 150;
                    m_creature->AddAura(SPELL_SOUL_FLAME); // apply on spawn in case of instant Freezing Trap
                }

                m_hunterGuid.Clear();
                m_uiDreadfulFright_Timer = urand(10000, 15000);
                m_uiCreepingDoom_Timer   = urand(3000, 6000);
                break;
        }
    }

    /** Nelson the Nice */
    void Transform()
    {
        m_creature->UpdateEntry(NPC_SOLENOR_THE_SLAYER);
        m_creature->SetHomePosition(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), m_creature->GetOrientation());
        m_creature->SetDefaultMovementType(IDLE_MOTION_TYPE);
        m_creature->GetMotionMaster()->Initialize();
        Reset();
    }

    void BeginEvent(ObjectGuid playerGuid)
    {
        m_hunterGuid = playerGuid;
        m_creature->GetMotionMaster()->Clear(false);
        m_creature->GetMotionMaster()->MoveIdle();
        m_creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
        m_bTransform = true;
    }

    /** Solenor the Slayer */
    void Aggro(Unit* pWho)
    {
        if (pWho->getClass() == CLASS_HUNTER && (m_hunterGuid.IsEmpty() || m_hunterGuid == pWho->GetObjectGuid())/*&& pWho->GetQuestStatus(QUEST_STAVE_OF_THE_ANCIENTS) == QUEST_STATUS_INCOMPLETE*/)
        {
            m_hunterGuid = pWho->GetObjectGuid();
        }
        else
            DemonDespawn();
    }

    void EnterEvadeMode() override
    {
        m_creature->RemoveGuardians();

        ScriptedAI::EnterEvadeMode();
    }

    void JustSummoned(Creature* pSummoned) override
    {
        if (m_creature->getVictim())
            pSummoned->AI()->AttackStart(m_creature->getVictim());
    }

    void JustDied(Unit* /*pKiller*/)
    {
        m_creature->SetHomePosition(-7724.21f, 1676.43f, 7.0571f, 4.80044f);
        // DRSS
        uint32 m_respawn_delay_Timer = 3*HOUR;
        if (sWorld.GetActiveSessionCount() > BLIZZLIKE_REALM_POPULATION)
            m_respawn_delay_Timer *= float(BLIZZLIKE_REALM_POPULATION) / float(sWorld.GetActiveSessionCount());

        m_creature->SetRespawnDelay(m_respawn_delay_Timer);
        m_creature->SetRespawnTime(m_respawn_delay_Timer);
        m_creature->SaveRespawnTime();
    }

    void DemonDespawn(bool triggered = true)
    {
        m_creature->RemoveGuardians();
        m_creature->SetHomePosition(-7724.21f, 1676.43f, 7.0571f, 4.80044f);
        m_creature->SetRespawnDelay(15*MINUTE);
        m_creature->SetRespawnTime(15*MINUTE);
        m_creature->SaveRespawnTime();

        if (triggered)
        {
            Creature* pCleaner = m_creature->SummonCreature(NPC_THE_CLEANER, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), m_creature->GetAngle(m_creature), TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 20*MINUTE*IN_MILLISECONDS);
            if (pCleaner)
            {
                ThreatList const& tList = m_creature->getThreatManager().getThreatList();

                for (ThreatList::const_iterator itr = tList.begin();itr != tList.end(); ++itr)
                {
                    if (Unit* pUnit = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid()))
                    {
                        if (pUnit->isAlive())
                        {
                            pCleaner->SetInCombatWith(pUnit);
                            pCleaner->AddThreat(pUnit);
                            pCleaner->AI()->AttackStart(pUnit);
                        }
                    }
                }
            }
        }

        m_creature->ForcedDespawn();
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {

        if (pSpell && pSpell->Id == 14268)   // Wing Clip (Rank 3)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_CRIPPLING_CLIP, CAST_TRIGGERED) == CAST_OK)
                DoScriptText(EMOTE_IMMOBILIZED, m_creature);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        /** Nelson the Nice */
        if (m_bTransform)
        {
            if (m_uiTransformEmote_Timer)
            {
                if (m_uiTransformEmote_Timer <= uiDiff)
                {
                    m_creature->HandleEmote(EMOTE_ONESHOT_LAUGH);
                    m_uiTransformEmote_Timer = 0;
                }
                else
                    m_uiTransformEmote_Timer -= uiDiff;
            }

            if (m_uiTransform_Timer < uiDiff)
            {
                m_bTransform = false;
                Transform();
            }
            else
                m_uiTransform_Timer -= uiDiff;
        }

        /** Solenor the Slayer */
        if (m_uiDespawn_Timer)
        {
            if (m_uiDespawn_Timer <= uiDiff)
            {
                if (m_creature->isAlive() && !m_creature->isInCombat())
                    DemonDespawn(false);
            }
            else
                m_uiDespawn_Timer -= uiDiff;
        }

        if (m_uiCastSoulFlame_Timer)
        {
            if (m_uiCastSoulFlame_Timer <= uiDiff)
            {
                // delay this cast so spell animation is visible to the player
                m_creature->CastSpell(m_creature, SPELL_SOUL_FLAME, false);
                m_uiCastSoulFlame_Timer = 0;
            }
            else
                m_uiCastSoulFlame_Timer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->HasAura(SPELL_SOUL_FLAME) && m_creature->HasAura(SPELL_FROST_TRAP))
                m_creature->RemoveAurasDueToSpell(SPELL_SOUL_FLAME);

        if (m_creature->getThreatManager().getThreatList().size() > 1 /*|| pHunter->isDead()*/)
            DemonDespawn();

        if (m_uiCreepingDoom_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_CREEPING_DOOM);
            m_uiCreepingDoom_Timer = 15000;
        }
        else
            m_uiCreepingDoom_Timer -= uiDiff;

        if (m_uiDreadfulFright_Timer < uiDiff)
        {
            if (Unit* pUnit = m_creature->getVictim())
            {
                if (m_creature->GetDistance2d(pUnit) > 5.0f)
                {
                    if (DoCastSpellIfCan(pUnit, SPELL_DREADFUL_FRIGHT) == CAST_OK)
                        m_uiDreadfulFright_Timer = urand(15000, 20000);
                }
            }
        }
        else
            m_uiDreadfulFright_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

bool GossipHello_npc_solenor(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(QUEST_STAVE_OF_THE_ANCIENTS) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());
    return true;
}

bool GossipSelect_npc_solenor(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction )
{
    pPlayer->CLOSE_GOSSIP_MENU();
    ((npc_solenorAI*)pCreature->AI())->BeginEvent(pPlayer->GetObjectGuid());
    return true;
}

CreatureAI* GetAI_npc_solenor(Creature* pCreature)
{
    return new npc_solenorAI(pCreature);
}

/*#####
 ## npc_creeping_doom
 ######*/

struct npc_creeping_doomAI : public ScriptedAI
{
    npc_creeping_doomAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    void Reset() {};

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage) override
    {
        Unit* pOwner = m_creature->GetCharmerOrOwner();
        if (pDoneBy && pOwner)
        {
            pOwner->AddThreat(pDoneBy);
            pOwner->SetInCombatWith(pDoneBy);
        }
        ScriptedAI::DamageTaken(pDoneBy, uiDamage);
    }
};

CreatureAI* GetAI_npc_creeping_doom(Creature* pCreature)
{
    return new npc_creeping_doomAI(pCreature);
}

/*#####
 ## npc_prince_thunderaan
 ######*/

enum
{
    SPELL_TENDRILS_OF_AIR           = 23009, // KB
    SPELL_TEARS_OF_THE_WIND_SEEKER    = 23011
};

struct npc_prince_thunderaanAI : public ScriptedAI
{
    npc_prince_thunderaanAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        engaged = false;
        emerged = false;
        Reset();
    }

    uint32 m_uiTendrilsTimer;
    uint32 m_uiTearsTimer;
    bool engaged;
    bool emerged;

    void Reset()
    {
        m_uiTendrilsTimer   = 8000;
        m_uiTearsTimer      = 15000;
    }

    void SpellHitTarget(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pCaster->GetTypeId() != TYPEID_PLAYER)
            return;

        if (pSpell->Id == SPELL_TENDRILS_OF_AIR)
            m_creature->getThreatManager().modifyThreatPercent(pCaster, -100);
    }

    void Aggro(Unit* pWho)
    {
        if (!engaged)
        {
            m_creature->MonsterYell("My power is discombobulatingly devastating! It is ludicrous that these mortals even attempt to enter my realm!", 0);
            engaged = true;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!emerged)
        {
            m_creature->CastSpell(m_creature, 20568, false);     // Ragnaros Emerge
            emerged = true;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiTendrilsTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_TENDRILS_OF_AIR) == CAST_OK) // KB
                m_uiTendrilsTimer = urand(12000, 20000);
        }
        else
            m_uiTendrilsTimer -= uiDiff;

        if (m_uiTearsTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_TEARS_OF_THE_WIND_SEEKER) == CAST_OK)
                m_uiTearsTimer = urand(8000, 11000);
        }
        else
            m_uiTearsTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_prince_thunderaan(Creature* pCreature)
{
    return new npc_prince_thunderaanAI(pCreature);
}


/*#####
 ## npc_colossus
 ######*/

enum
{
    SPELL_COLOSSAL_SMASH            = 26167 // Maxi KB
};

struct npc_colossusAI : public ScriptedAI
{
    npc_colossusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiColossalSmashTimer;
    uint32 m_uiColossalSmashEmoteTimer;
    bool firstSmash;

    void Reset()
    {
        firstSmash = true;
        m_uiColossalSmashTimer = 60000;
        m_uiColossalSmashEmoteTimer = 0;
    }

    void SpellHitTarget(Unit* pCaster, const SpellEntry* pSpell)
    {
    }

    void Aggro(Unit* pWho)
    {
    }

    void EnterEvadeMode()
    {
        // Ustaag <Nostalrius> : Must neither resume life if Evade, nor return to its starting point
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        // To avoid jerky movements after aggro end
        m_creature->GetMotionMaster()->MoveIdle();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiColossalSmashTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_COLOSSAL_SMASH) == CAST_OK) // Maxi KB
            {
                m_creature->MonsterTextEmote("Colossus begins to cast Colossus Smash", NULL);
                m_uiColossalSmashTimer = firstSmash ? 10000 : 60000;
                m_uiColossalSmashEmoteTimer = 5000;

                if (firstSmash)
                    firstSmash = false;
                else
                    firstSmash = true;
            }
        }
        else
            m_uiColossalSmashTimer -= uiDiff;

        if (m_uiColossalSmashEmoteTimer && m_uiColossalSmashEmoteTimer < uiDiff)
        {
            m_creature->MonsterTextEmote("Colossus lets loose a massive attack", NULL);
            m_uiColossalSmashEmoteTimer = 0;
        }
        else
            m_uiColossalSmashEmoteTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_colossus(Creature* pCreature)
{
    return new npc_colossusAI(pCreature);
}


/*#####
 ## npc_resonating_Crystal
 ######*/

// UPDATE `creature_template` SET `ScriptName` = 'npc_resonating_Crystal' WHERE `entry` = 15769;

enum
{
    GO_RESONATING_CRYSTAL_FORMATION     = 180810,

    MAX_SIGHT_DISTANCE                  = 55,

    // C'Thun's Mind Control has varying strengths based on location
    WHISPERINGS_OF_CTHUN_0              = 26259, // (10-21) Barrens
    WHISPERINGS_OF_CTHUN_1              = 26258, // (21-41) 1K Needles
    WHISPERINGS_OF_CTHUN_2              = 26195, // (30-41) Desolace?
    WHISPERINGS_OF_CTHUN_3              = 26197, // (41-51) Feralas/Tanaris/Un'Goro
    WHISPERINGS_OF_CTHUN_4              = 26198, // (51-71) Silithus

    ZONE_BARRENS                        = 17,
    ZONE_THOUSAND_NEEDLES               = 400,
    ZONE_DESOLACE                       = 405,
    ZONE_FERALAS                        = 357,
    ZONE_TANARIS                        = 440,
    ZONE_UN_GORO                        = 490,
    ZONE_SILITHUS                       = 1377,
};

struct npc_resonating_CrystalAI : public ScriptedAI
{
    npc_resonating_CrystalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
        SPELL_WHISPERINGS = GetMCSpellForZone();
    }

    bool playerDetected;
    uint32 m_uiCheckTimer;
    uint32 m_uiWisperingsTimer;
    uint32 SPELL_WHISPERINGS;

    void Reset()
    {
        SetCombatMovement(false);
        playerDetected      = false;
        m_uiCheckTimer      = 2000;
        m_uiWisperingsTimer = 1000;
    }

    uint32 GetMCSpellForZone()
    {
        switch (m_creature->GetZoneId())
        {
            case ZONE_BARRENS:
                return WHISPERINGS_OF_CTHUN_0;

            case ZONE_THOUSAND_NEEDLES:
                return WHISPERINGS_OF_CTHUN_1;

            case ZONE_DESOLACE:
                return WHISPERINGS_OF_CTHUN_2;

            case ZONE_FERALAS:
            case ZONE_TANARIS:
            case ZONE_UN_GORO:
                return WHISPERINGS_OF_CTHUN_3;

            default:
                // ZONE_SILITHUS
                return WHISPERINGS_OF_CTHUN_4;
        }
    }

    void MoveInLineOfSight(Unit* who)
    {
        if (who->GetTypeId() != TYPEID_PLAYER || who->ToPlayer()->isGameMaster())
            return;

        if (!who->isAlive())
            return;

        playerDetected = m_creature->IsWithinDistInMap(who, MAX_SIGHT_DISTANCE) ? true : false;
    }

    bool MoreThanOnePlayerNear()
    {
        Map::PlayerList const& players = m_creature->GetMap()->GetPlayers();
        int var = 0;
        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            Player* pPlayer = itr->getSource();
            if (pPlayer && pPlayer->isAlive() && m_creature->IsWithinDistInMap(pPlayer, MAX_SIGHT_DISTANCE) && !pPlayer->isGameMaster())
                ++var;

            if (var > 1)
                return true;
        }
        return false;
    }

    void AggroAllPlayerNear()
    {
        Map::PlayerList const& players = m_creature->GetMap()->GetPlayers();
        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            Player* pPlayer = itr->getSource();
            if (pPlayer && pPlayer->isAlive() && m_creature->IsWithinDistInMap(pPlayer, MAX_SIGHT_DISTANCE) && !pPlayer->isGameMaster())
            {
                m_creature->AddThreat(pPlayer);
                m_creature->SetInCombatWith(pPlayer);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (playerDetected)
        {
            if (m_uiCheckTimer < uiDiff)
            {
                if (MoreThanOnePlayerNear())
                    AggroAllPlayerNear();
                else
                    EnterEvadeMode();
                m_uiCheckTimer = 2000;
            }
            else
                m_uiCheckTimer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Whisperings of C'Thun (MC)
        if (MoreThanOnePlayerNear())
        {
            if (m_uiWisperingsTimer < uiDiff || (m_creature->GetCharm() && m_creature->GetCharm()->IsPolymorphed()))
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    if (DoCastSpellIfCan(pTarget, SPELL_WHISPERINGS, CAST_AURA_NOT_PRESENT) == CAST_OK)
                        m_uiWisperingsTimer = 20000;
            }
            else
                m_uiWisperingsTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_npc_resonating_Crystal(Creature* pCreature)
{
    return new npc_resonating_CrystalAI(pCreature);
}


/*###
 ## npc_AQwar_effort
 ###*/

enum WarEffortItemType
{
    WAREFFORT_BAR,
    WAREFFORT_HERBS,
    WAREFFORT_SKINS,
    WAREFFORT_COOKING,
    WAREFFORT_BANDAGES,
};

struct WarEffort
{
    uint32 itemID, reqCount;
    uint32 saveVarID;           // = itemID / This variable is left in case the ID of the item is not an appropriate choice.
    char itemName[50];
    WarEffortItemType type;
};

/*
 -- SQL a executer pour RESET l'event.
 -- Le champ 'comment' sert pour le site.
 REPLACE INTO variables (`index`, value, `comment`) VALUES
 -- Neutre
 (2840, 0, "AQ|N|Barre de cuivre|90000"),
 (8831, 0, "AQ|N|Lotus pourpre|26000"),
 (4304, 0, "AQ|N|Cuir epais|80000"),
 (6887, 0, "AQ|N|Jaune-queue tachete|17000"),
 (14529, 0, "AQ|N|Bandage en etoffe runique|400000"),
 -- Alliance
 (3575, 0, "AQ|A|Barre de fer|28000"),
 (12359, 0, "AQ|A|Barre de thorium|24000"),
 (8836, 0, "AQ|A|Larmes d'Arthas|20000"),
 (3820, 0, "AQ|A|Etouffante|33000"),
 (2318, 0, "AQ|A|Cuir leger|180000"),
 (2319, 0, "AQ|A|Cuir moyen|110000"),
 (12210, 0, "AQ|A|Roti de raptor|20000"),
 (5095, 0, "AQ|A|Thon arc-en-ciel|14000"),
 (1251, 0, "AQ|A|Bandage en lin|800000"),
 (6450, 0, "AQ|A|Bandage en soie|600000"),
 -- Horde
 (3576, 0, "AQ|H|Barre d'etain|22000"),
 (3860, 0, "AQ|H|Barre de mithril|18000"),
 (2447, 0, "AQ|H|Pacifique|96000"),
 (4625, 0, "AQ|H|Fleur de feu|19000"),
 (4234, 0, "AQ|H|Cuir lourd|60000"),
 (8170, 0, "AQ|H|Cuir robuste|60000"),
 (12209, 0, "AQ|H|Steak de loup|10000"),
 (13935, 0, "AQ|H|Pain de saumon|10000"),
 (3530, 0, "AQ|H|Bandage en laine|250000"),
 (8544, 0, "AQ|H|Bandage en tisse-mage|250000");
 */

static const WarEffort SharedObjectives[5] =
{
    /* Blizzlile
    {2840,  90000,  2840,  "Copper Bar", WAREFFORT_BAR},
    {8831,  26000,  8831,  "Purple Lotus", WAREFFORT_HERBS},
    {4304,  80000,  4304,  "Thick Leather", WAREFFORT_SKINS},
    {6887,  17000,  6887,  "Spotted Yellowtail", WAREFFORT_COOKING},
    {14529, 400000, 14529, "Runecloth Bandage", WAREFFORT_BANDAGES}
    */

    {2840,  1250000,  2840,  "Copper Bar",          WAREFFORT_BAR},
    {8831,   700000,  8831,  "Purple Lotus",        WAREFFORT_HERBS},
    {4304,  1300000,  4304,  "Thick Leather",       WAREFFORT_SKINS},
    {6887,   600000,  6887,  "Spotted Yellowtail",  WAREFFORT_COOKING},
    {14529, 2000000, 14529,  "Runecloth Bandage",   WAREFFORT_BANDAGES}
};

static const WarEffort AllianceObjectives[10] =
{
    /* Blizzlile
    {3575,  28000,  3575,   "Iron Bar", WAREFFORT_BAR},
    {12359, 24000,  12359,  "Thorium Bar", WAREFFORT_BAR},
    {8836,  20000,  8836,   "Arthas' Tears", WAREFFORT_HERBS},
    {3820,  33000,  3820,   "Stranglekelp", WAREFFORT_HERBS},
    {2318,  180000, 2318,   "Light Leather", WAREFFORT_SKINS},
    {2319,  110000, 2319,   "Medium Leather", WAREFFORT_SKINS},
    {12210, 20000,  12210,  "Roast Raptor", WAREFFORT_COOKING},
    {5095,  14000,  5095,   "Rainbow Fin Albacore", WAREFFORT_COOKING},
    {1251,  800000, 1251,   "Linen Bandage", WAREFFORT_BANDAGES},
    {6450,  600000, 6450,   "Silk Bandage", WAREFFORT_BANDAGES}
    */

    {3575,   600000,  3575,   "Iron Bar",               WAREFFORT_BAR},
    {12359,  850000,  12359,  "Thorium Bar",            WAREFFORT_BAR},
    {8836,   425000,  8836,   "Arthas' Tears",          WAREFFORT_HERBS},
    {3820,   450000,  3820,   "Stranglekelp",           WAREFFORT_HERBS},
    {2318,   850000,  2318,   "Light Leather",          WAREFFORT_SKINS},
    {2319,   575000,  2319,   "Medium Leather",         WAREFFORT_SKINS},
    {12210,  450000,  12210,  "Roast Raptor",           WAREFFORT_COOKING},
    {5095,   450000,  5095,   "Rainbow Fin Albacore",   WAREFFORT_COOKING},
    {1251,  2000000,  1251,   "Linen Bandage",          WAREFFORT_BANDAGES},
    {6450,  1600000,  6450,   "Silk Bandage",           WAREFFORT_BANDAGES}
};

static const WarEffort HordeObjectives[10] =
{
    /* Blizzlile
    {3576,  22000,  3576,   "Tin Bar", WAREFFORT_BAR},
    {3860,  18000,  3860,   "Mithril Bar", WAREFFORT_BAR},
    {2447,  96000,  2447,   "Peacebloom", WAREFFORT_HERBS},
    {4625,  19000,  4625,   "Firebloom", WAREFFORT_HERBS},
    {4234,  60000,  4234,   "Heavy Leather", WAREFFORT_SKINS},
    {8170,  60000,  8170,   "Rugged Leather", WAREFFORT_SKINS},
    {12209, 10000,  12209,  "Lean Wolf Steak", WAREFFORT_COOKING},
    {13935, 10000,  13935,  "Baked Salmon", WAREFFORT_COOKING},
    {3530,  250000, 3530,   "Wool Bandage", WAREFFORT_BANDAGES},
    {8544,  250000, 8544,   "Mageweave Bandage", WAREFFORT_BANDAGES}
    */

    {3576,   425000, 3576,   "Tin Bar",           WAREFFORT_BAR},
    {3860,   525000, 3860,   "Mithril Bar",       WAREFFORT_BAR},
    {2447,   825000, 2447,   "Peacebloom",        WAREFFORT_HERBS},
    {4625,   425000, 4625,   "Firebloom",         WAREFFORT_HERBS},
    {4234,   700000, 4234,   "Heavy Leather",     WAREFFORT_SKINS},
    {8170,   850000, 8170,   "Rugged Leather",    WAREFFORT_SKINS},
    {12209,  275000, 12209,  "Lean Wolf Steak",   WAREFFORT_COOKING},
    {13935,  375000, 13935,  "Baked Salmon",      WAREFFORT_COOKING},
    {3530,  1250000, 3530,   "Wool Bandage",      WAREFFORT_BANDAGES},
    {8544,  1000000, 8544,   "Mageweave Bandage", WAREFFORT_BANDAGES}
};

enum
{
    // Alliance
    NPC_FIELD_MARSHAL_SNOWFALL  = 15701,
    NPC_COMMANDER_STRONGHAMMER  = 15458,

    // Horde
    NPC_WARLORD_GORCHUK         = 15700,
    NPC_GENERAL_ZOG             = 15539
};

// UPDATE `creature_template` SET `ScriptName` = 'npc_AQwar_effort' WHERE `entry` IN (15701, 15458, 15700, 15539);

bool GossipHello_npc_AQwar_effort(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    switch (pCreature->GetEntry())
    {
        case NPC_FIELD_MARSHAL_SNOWFALL:    // Shared + Alliance
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Shared War Effort", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        // no break
        case NPC_COMMANDER_STRONGHAMMER:    // Alliance only
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Alliance War Effort", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            break;
        case NPC_WARLORD_GORCHUK:           // Shared + Horde
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Shared War Effort", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        // no break
        case NPC_GENERAL_ZOG:               // Horde only
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Horde War Effort", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            break;
        default:
            break;
    }

    pPlayer->SEND_GOSSIP_MENU(1, pCreature->GetObjectGuid()); // Text to put here the state of the resources
    return true;
}

bool GossipSelect_npc_AQwar_effort(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    if (sender != GOSSIP_SENDER_MAIN)
        return false;

    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF:
        {
            // Shared War Effort
            for (int i = 0; i < 5; i++)
            {
                uint32 stock = sObjectMgr.GetSavedVariable(SharedObjectives[i].saveVarID, 0);
                uint32 objectif = SharedObjectives[i].reqCount;
                if (stock > objectif)
                    stock = objectif;

                char sMessage[200] = "";
                sprintf(sMessage, "%s: %u / %u", SharedObjectives[i].itemName, stock, objectif);

                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, sMessage, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            }
            break;
        }
        case GOSSIP_ACTION_INFO_DEF+1:
        {
            // Alliance War Effort
            for (int i = 0; i < 10; i++)
            {
                uint32 stock = sObjectMgr.GetSavedVariable(AllianceObjectives[i].saveVarID, 0);
                uint32 objectif = AllianceObjectives[i].reqCount;

                if (stock > objectif)
                    stock = objectif;

                char sMessage[200] = "";
                sprintf(sMessage, "%s: %u / %u", AllianceObjectives[i].itemName, stock, objectif);

                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, sMessage, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            }
            break;
        }
        case GOSSIP_ACTION_INFO_DEF+2:
        {
            // Horde War Effort
            for (int i = 0; i < 10; i++)
            {
                uint32 stock = sObjectMgr.GetSavedVariable(HordeObjectives[i].saveVarID, 0);
                uint32 objectif = HordeObjectives[i].reqCount;

                if (stock > objectif)
                    stock = objectif;

                char sMessage[200] = "";
                sprintf(sMessage, "%s: %u / %u", HordeObjectives[i].itemName, stock, objectif);

                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, sMessage, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            }
            break;
        }
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->CLOSE_GOSSIP_MENU();
            return true;
        default:
            break;
    }

    pPlayer->SEND_GOSSIP_MENU(1, pCreature->GetObjectGuid());
    return true;
}

/*###
 ## npc_AQwar_collector
 ###*/

// UPDATE `creature_template` SET `ScriptName` = 'npc_AQwar_collector' WHERE `subname` like '%collector%' and subname not like '%ancestry%';

bool GossipHello_npc_AQwar_collector(Player* pPlayer, Creature* pCreature)
{
    bool objectiveReached = false;
    char itemNameReqReached[51] = "";

    QuestRelationsMapBounds bounds = sObjectMgr.GetCreatureQuestRelationsMapBounds(pCreature->GetEntry());
    for (QuestRelationsMap::const_iterator itr = bounds.first; itr != bounds.second; ++itr)
    {
        uint32 quest_id = itr->second;
        if (!quest_id)
            continue;
        Quest const *pQuest = sObjectMgr.GetQuestTemplate(quest_id);
        if (!pQuest)
            continue;

        for (uint8 i = 0; i < 5; i++)
        {
            if (pQuest->ReqItemId[0] == SharedObjectives[i].itemID)
            {
                uint32 stock = sObjectMgr.GetSavedVariable(SharedObjectives[i].saveVarID, 0);
                uint32 objectif = SharedObjectives[i].reqCount;
                if (stock >= objectif)
                {
                    objectiveReached = true;
                    strcat(itemNameReqReached, SharedObjectives[i].itemName);
                    break;
                }
            }
        }
        for (uint8 i = 0; i < 10; i++)
        {
            if (pQuest->ReqItemId[0] == AllianceObjectives[i].itemID)
            {
                uint32 stock = sObjectMgr.GetSavedVariable(AllianceObjectives[i].saveVarID, 0);
                uint32 objectif = AllianceObjectives[i].reqCount;
                if (stock >= objectif)
                {
                    objectiveReached = true;
                    strcat(itemNameReqReached, AllianceObjectives[i].itemName);
                    break;
                }
            }
        }
        for (uint8 i = 0; i < 10; i++)
        {
            if (pQuest->ReqItemId[0] == HordeObjectives[i].itemID)
            {
                uint32 stock = sObjectMgr.GetSavedVariable(HordeObjectives[i].saveVarID, 0);
                uint32 objectif = HordeObjectives[i].reqCount;
                if (stock >= objectif)
                {
                    objectiveReached = true;
                    strcat(itemNameReqReached, HordeObjectives[i].itemName);
                    break;
                }
            }
        }

        if (objectiveReached)
            break;
    }


    if (objectiveReached)
    {
        if (pCreature->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER))
            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);

        if (pCreature->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP))
            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

        char sMessage[200] = "";
        sprintf(sMessage, "The collection of %s is no longer necessary.", itemNameReqReached);

        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->MonsterSay(sMessage, 0, 0);
        pCreature->HandleEmote(EMOTE_ONESHOT_BOW);
        return true;
    }

    if (pCreature->isQuestGiver())
    {
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());
        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());
    }
    return true;
}

struct GOWarEffort
{
    int Tiers;      // A titre informatif uniquement
    uint32 GOEntry;
    int faction;    // = GetTeamId() // Alliance = 0 Horde = 1
};

static const GOWarEffort GOBars[10] =
{
    // Alliance
    {1, 180780, 0},
    {2, 180781, 0},
    {3, 180782, 0},
    {4, 180783, 0},
    {5, 180784, 0},
    // Horde
    {1, 180839, 1},
    {2, 180840, 1},
    {3, 180841, 1},
    {4, 180842, 1},
    {5, 180843, 1}
};

static const GOWarEffort GOCooking[10] =
{
    // Alliance
    {1, 180800, 0},
    {2, 180806, 0},
    {3, 180807, 0},
    {4, 180808, 0},
    {5, 180809, 0},
    // Horde
    {1, 180833, 1},
    {2, 180834, 1},
    {3, 180835, 1},
    {4, 180836, 1},
    {5, 180837, 1}
};

static const GOWarEffort GOHerbs[10] =
{
    // Alliance
    {1, 180801, 0},
    {2, 180802, 0},
    {3, 180803, 0},
    {4, 180804, 0},
    {5, 180805, 0},
    // Horde
    {1, 180819, 1},
    {2, 180820, 1},
    {3, 180821, 1},
    {4, 180822, 1},
    {5, 180823, 1}
};

static const GOWarEffort GOSkins[10] =
{
    // Alliance
    {1, 180692, 0},
    {2, 180693, 0},
    {3, 180694, 0},
    {4, 180695, 0},
    {5, 180696, 0},
    // Horde
    {1, 180813, 1},
    {2, 180814, 1},
    {3, 180815, 1},
    {4, 180816, 1},
    {5, 180817, 1}
};

static const GOWarEffort GOBandages[10] =
{
    // Alliance
    {1, 180674, 0},
    {2, 180675, 0},
    {3, 180676, 0},
    {4, 180677, 0},
    {5, 180678, 0},
    // Horde
    {1, 180827, 1},
    {2, 180828, 1},
    {3, 180829, 1},
    {4, 180830, 1},
    {5, 180831, 1}
};

void HandleWarEffortGameObject(GOWarEffort const* gameobjects, Creature* questGiver, TeamId team, WarEffortItemType type)
{
    uint32 Objective    = 0;
    uint32 current      = 0;

    // Counting
    for (int i = 0; i < 5; ++i)
    {
        if (SharedObjectives[i].type == type)
        {
            Objective += SharedObjectives[i].reqCount;
            current   += sObjectMgr.GetSavedVariable(SharedObjectives[i].saveVarID, 0);
        }
    }
    for (int i = 0; i < 10; ++i)
    {
        if (team == TEAM_HORDE && HordeObjectives[i].type == type)
        {
            Objective += HordeObjectives[i].reqCount;
            current   += sObjectMgr.GetSavedVariable(HordeObjectives[i].saveVarID, 0);
        }
        if (team == TEAM_ALLIANCE && AllianceObjectives[i].type == type)
        {
            Objective += AllianceObjectives[i].reqCount;
            current   += sObjectMgr.GetSavedVariable(AllianceObjectives[i].saveVarID, 0);
        }
    }

    // Showing gameobjects
    uint32 tempCount = Objective;
    for (int j = 9; j >= 0; j--)
    {
        if (gameobjects[j].faction != team)
            continue;
        if (current >= tempCount)
        {
            if (GameObject* go = questGiver->FindNearestGameObject(gameobjects[j].GOEntry, 50.0f))
            {
                if (!go->isSpawned())
                {
                    go->SetRespawnTime(1209600); // 14 days
                    go->SaveToDB();
                }
            }
        }
        tempCount -= Objective / 5;
    }
}

bool QuestComplete_npc_AQwar_collector(Player* pPlayer, Creature* pQuestGiver, Quest const* pQuest)
{
    if (!pQuest->ReqItemId[0] || !pQuest->ReqItemCount[0])
        return false;

    uint32 var = sObjectMgr.GetSavedVariable(pQuest->ReqItemId[0], 0);
    var += pQuest->ReqItemCount[0];
    sObjectMgr.SetSavedVariable(pQuest->ReqItemId[0], var, true);

    // Gestion des UNIT_NPC_FLAG_QUESTGIVER and UNIT_NPC_FLAG_GOSSIP des PNJ "collector"
    char itemName[51] = "";
    bool objectiveReached = false;

    for (int i = 0; i < 5; i++)
    {
        if (pQuest->ReqItemId[0] == SharedObjectives[i].itemID)
        {
            if (var >= SharedObjectives[i].reqCount)
            {
                strcat(itemName, SharedObjectives[i].itemName);
                objectiveReached = true;
                break;
            }
        }
    }

    // Gestion apparition des GO
    // GO Bars
    HandleWarEffortGameObject(GOBars, pQuestGiver, pPlayer->GetTeamId(), WAREFFORT_BAR);
    // GO Herbs
    HandleWarEffortGameObject(GOHerbs, pQuestGiver, pPlayer->GetTeamId(), WAREFFORT_HERBS);
    // GO Skins
    HandleWarEffortGameObject(GOSkins, pQuestGiver, pPlayer->GetTeamId(), WAREFFORT_SKINS);
    // GO Cooking
    HandleWarEffortGameObject(GOCooking, pQuestGiver, pPlayer->GetTeamId(), WAREFFORT_COOKING);
    // GO Bandages
    HandleWarEffortGameObject(GOBandages, pQuestGiver, pPlayer->GetTeamId(), WAREFFORT_BANDAGES);

    switch (pPlayer->GetTeam())
    {
        case HORDE:
        {
            // Gestion des UNIT_NPC_FLAG_QUESTGIVER et UNIT_NPC_FLAG_GOSSIP des PNJ "collector"
            if (objectiveReached)
                break;

            for (int i = 0; i < 10; i++)
            {
                if (pQuest->ReqItemId[0] == HordeObjectives[i].itemID)
                {
                    if (var >= HordeObjectives[i].reqCount)
                    {
                        strcat(itemName, HordeObjectives[i].itemName);
                        objectiveReached = true;
                        break;
                    }
                }
            }
            break;
        }
        case ALLIANCE:
        {
            // Gestion des UNIT_NPC_FLAG_QUESTGIVER et UNIT_NPC_FLAG_GOSSIP des PNJ "collector"
            if (objectiveReached)
                break;

            for (int i = 0; i < 10; i++)
            {
                if (pQuest->ReqItemId[0] == AllianceObjectives[i].itemID)
                {
                    if (var >= AllianceObjectives[i].reqCount)
                    {
                        strcat(itemName, AllianceObjectives[i].itemName);
                        objectiveReached = true;
                        break;
                    }
                }
            }
            break;
        }
        default:
            break;
    }

    if (objectiveReached)
    {
        if (pQuestGiver->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER))
            pQuestGiver->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);

        if (pQuestGiver->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP))
            pQuestGiver->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

        char sMessage[200] = "";
        sprintf(sMessage, "Congratulations %s! The collection of %s is finally finished!", pPlayer->GetName(), itemName);
        pQuestGiver->MonsterSay(sMessage, 0, 0);
        pQuestGiver->HandleEmote(EMOTE_ONESHOT_CHEER);
    }
    return true;
}

const WarEffort* InternalGetResource(char* ResourceName, const WarEffort* InArray, int ArrSize)
{
    for (int i = 0; i < ArrSize; ++i)
    {
        const WarEffort* SharedRes = &InArray[i];
        if (strcmp(ResourceName, SharedRes->itemName) == 0)
        {
            return SharedRes;
        }
    }

    return nullptr;
}

const WarEffort* GetResourceIDFromString(char* ResourceName)
{
    if (ResourceName == nullptr) return nullptr;

    const WarEffort* SharedWarEffortSup = InternalGetResource(ResourceName, SharedObjectives, sizeof(SharedObjectives) / sizeof(WarEffort));
    if (SharedWarEffortSup != nullptr) return SharedWarEffortSup;

    const WarEffort* AlliaceWarEffortSup = InternalGetResource(ResourceName, AllianceObjectives, sizeof(AllianceObjectives) / sizeof(WarEffort));
    if (AlliaceWarEffortSup != nullptr) return AlliaceWarEffortSup;

    const WarEffort* HordeWarEffortSup = InternalGetResource(ResourceName, HordeObjectives, sizeof(HordeObjectives) / sizeof(WarEffort));
    if (HordeWarEffortSup != nullptr) return HordeWarEffortSup;

    return nullptr;
}


bool ChatHandler::HandleGetWarEffortResource(char* args)
{
    char* pResourceName = ExtractQuotedArg(&args);

    auto PrintResources = [this] (const WarEffort* Resource)
    {
        uint32 CurrentResourceCount = sObjectMgr.GetSavedVariable(Resource->saveVarID);
        double Progress = (double)CurrentResourceCount / (double)Resource->reqCount;
        PSendSysMessage("\"%s\"[%u] Current [%u] Required [%u] Completed: %.03f", Resource->itemName, Resource->itemID, CurrentResourceCount, Resource->reqCount, Progress);
    };

    if (const WarEffort* pResource = GetResourceIDFromString(pResourceName))
    {
        PrintResources(pResource);
        return true;
    }
    else
    {
        PSendSysMessage("Error: resource with name \"%s\" not found", pResourceName);
    }

    return false;
}

bool ChatHandler::HandleSetWarEffortResource(char* args)
{
    char* pResourceName = ExtractQuotedArg(&args);
    if (pResourceName == nullptr)
    {
        PSendSysMessage("Usage example .wareffortset \"Iron Bar\" 1245");
        return false;
    }
    uint32 NewResourceCount = 0;
    if (!ExtractUInt32(&args, NewResourceCount))
    {
        PSendSysMessage("Usage example .wareffortset \"Iron Bar\" 1245");
        return false;
    }

    if (const WarEffort* pResource = GetResourceIDFromString(pResourceName))
    {
        uint32 PreviousResourceCount = sObjectMgr.GetSavedVariable(pResource->saveVarID);
        sObjectMgr.SetSavedVariable(pResource->saveVarID, NewResourceCount, true);
        double Progress = (double)NewResourceCount / (double)pResource->reqCount;
        PSendSysMessage("\"%s\" Previous count [%u] New count [%u] Completed: %.03f", pResourceName, PreviousResourceCount, NewResourceCount, Progress);
        return true;
    }
    else
    {
        PSendSysMessage("Error: resource with name \"%s\" not found", pResourceName);
    }


    return false;
}


/*###
 ## npc_Geologist_Larksbane
 ###*/

/*UPDATE `gameobject_template` SET `size` = 1.5, `data2` = 39321600, `type` = 1 WHERE `entry` = 210342;
 UPDATE `gameobject_template` SET data2 = 39321600 WHERE `entry` = 180514;
 DELETE FROM `gameobject` WHERE `id` IN (180514, 210342);
 UPDATE `creature_template` SET `ScriptName` = 'npc_Geologist_Larksbane' WHERE `entry` = 15183;*/

enum
{
    GO_GLYPHED_CRYSTAL      = 180514,
    GO_GLYPHED_CRYSTAL_BIG  = 210342
};

struct npc_Geologist_LarksbaneAI : public ScriptedAI
{
    npc_Geologist_LarksbaneAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    std::list<uint64> lCrystalGUIDs;
    uint32 uiNextActionTimer;
    uint32 uiCurrAction;

    void Reset()
    {
        m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
        m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        lCrystalGUIDs.clear();
        uiCurrAction = 0;
        uiNextActionTimer = 0;
    }

    void QuestCompleted(Player* pPlayer, Quest const* pQuest)
    {
        m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
        m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

        if (GameObject* pGo = m_creature->SummonGameObject(GO_GLYPHED_CRYSTAL, -6826.51f, 809.082f, 51.8577f, 0.259445f))
            lCrystalGUIDs.push_back(pGo->GetGUID());
        if (GameObject* pGo = m_creature->SummonGameObject(GO_GLYPHED_CRYSTAL, -6827.54f, 806.711f, 51.9809f, 2.2241f))
            lCrystalGUIDs.push_back(pGo->GetGUID());
        if (GameObject* pGo = m_creature->SummonGameObject(GO_GLYPHED_CRYSTAL_BIG, -6825.31f, 805.146f, 51.9435f, -1.255528f))
            lCrystalGUIDs.push_back(pGo->GetGUID());

        uiCurrAction = 1;
        uiNextActionTimer = 4000;
    }

    void Larksbane_DoAction()
    {
        switch (uiCurrAction)
        {
            case 1:
            {
                m_creature->HandleEmote(EMOTE_ONESHOT_TALK);
                m_creature->MonsterSay(NOST_TEXT(77));
                uiNextActionTimer = 4000;
                break;
            }
            case 2:
            {
                for (std::list<uint64>::iterator it = lCrystalGUIDs.begin(); it != lCrystalGUIDs.end(); ++it)
                {
                    if (GameObject* pCrystal = m_creature->GetMap()->GetGameObject(*it))
                        pCrystal->Use(m_creature);
                }
                uiNextActionTimer = 5000;
                break;
            }
            case 3:
            {
                m_creature->HandleEmote(EMOTE_ONESHOT_TALK);
                m_creature->MonsterSay(NOST_TEXT(78));
                uiNextActionTimer = 7000;
                break;
            }
            case 4:
            {
                m_creature->HandleEmote(EMOTE_ONESHOT_TALK);
                m_creature->MonsterSay(NOST_TEXT(79));
                uiNextActionTimer = 11000;
                break;
            }
            case 5:
            {
                m_creature->HandleEmote(EMOTE_ONESHOT_TALK);
                m_creature->MonsterSay(NOST_TEXT(80));
                uiNextActionTimer = 9000;
                break;
            }
            case 6:
            {
                m_creature->HandleEmote(EMOTE_ONESHOT_TALK);
                m_creature->MonsterSay(NOST_TEXT(81));
                uiNextActionTimer = 11000;
                break;
            }
            case 7:
            {
                m_creature->HandleEmote(EMOTE_ONESHOT_TALK);
                m_creature->MonsterSay(NOST_TEXT(82));
                uiNextActionTimer = 11000;
                break;
            }
            case 8:
            {
                m_creature->HandleEmote(EMOTE_ONESHOT_EXCLAMATION);
                m_creature->MonsterSay(NOST_TEXT(83));
                uiNextActionTimer = 9000;
                break;
            }
            case 9:
            {
                m_creature->MonsterTextEmote(NOST_TEXT(84), NULL, false);
                uiNextActionTimer = 3000;
                break;
            }
            case 10:
            {
                m_creature->HandleEmote(EMOTE_ONESHOT_TALK);
                m_creature->MonsterSay(NOST_TEXT(85));
                uiNextActionTimer = 4000;
                break;
            }
            case 11:
            {
                m_creature->HandleEmote(EMOTE_ONESHOT_TALK);
                m_creature->MonsterSay(NOST_TEXT(86));
                uiNextActionTimer = 12000;
                break;
            }
            case 12:
            {
                m_creature->HandleEmote(EMOTE_ONESHOT_EXCLAMATION);
                m_creature->MonsterSay(NOST_TEXT(87));
                uiNextActionTimer = 9000;
                break;
            }
            case 13:
            {
                m_creature->HandleEmote(EMOTE_ONESHOT_TALK);
                m_creature->MonsterSay(NOST_TEXT(88));
                uiNextActionTimer = 9000;
                break;
            }
            case 14:
            {
                m_creature->HandleEmote(EMOTE_ONESHOT_TALK);
                m_creature->MonsterSay(NOST_TEXT(89));
                uiNextActionTimer = 12000;
                break;
            }
            case 15:
            {
                m_creature->HandleEmote(EMOTE_ONESHOT_TALK);
                m_creature->MonsterSay(NOST_TEXT(90));
                uiNextActionTimer = 12000;
                break;
            }
            case 16:
            {
                m_creature->HandleEmote(EMOTE_ONESHOT_TALK);
                m_creature->MonsterSay(NOST_TEXT(91));
                uiNextActionTimer = 12000;
                break;
            }
            case 17:
            {
                m_creature->HandleEmote(EMOTE_ONESHOT_TALK);
                m_creature->MonsterSay(NOST_TEXT(92));
                uiNextActionTimer = 12000;
                break;
            }
            case 18:
            {
                m_creature->HandleEmote(EMOTE_ONESHOT_TALK);
                m_creature->MonsterSay(NOST_TEXT(93));
                uiNextActionTimer = 12000;
                break;
            }
            case 19:
            {
                m_creature->HandleEmote(EMOTE_ONESHOT_TALK);
                m_creature->MonsterSay(NOST_TEXT(94));
                uiNextActionTimer = 12000;
                break;
            }
            case 20:
            {
                m_creature->HandleEmote(EMOTE_ONESHOT_TALK);
                m_creature->MonsterSay(NOST_TEXT(95));
                uiNextActionTimer = 9000;
                break;
            }
            case 21:
            {
                m_creature->HandleEmote(EMOTE_ONESHOT_TALK);
                m_creature->MonsterSay(NOST_TEXT(96));
                uiNextActionTimer = 3000;
                break;
            }
            case 22:
            {
                m_creature->HandleEmote(EMOTE_ONESHOT_TALK);
                m_creature->MonsterSay(NOST_TEXT(97));
                uiNextActionTimer = 12000;
                break;
            }
            case 23:
            {
                m_creature->HandleEmote(EMOTE_ONESHOT_TALK);
                m_creature->MonsterSay(NOST_TEXT(98));
                uiNextActionTimer = 9000;
                break;
            }
            case 24:
            {
                m_creature->HandleEmote(EMOTE_ONESHOT_TALK);
                m_creature->MonsterSay(NOST_TEXT(99));
                uiNextActionTimer = 3000;
                break;
            }
            case 25:
            {
                m_creature->MonsterTextEmote(NOST_TEXT(100), NULL, false);
                uiNextActionTimer = 4000;
                break;
            }
            case 26:
            {
                if (Creature* Crea = m_creature->FindNearestCreature(15180, 50.0f))    // Baristolth of the Shifting Sands
                    m_creature->MonsterSay(NOST_TEXT(101));
                uiNextActionTimer = 5000;
                break;
            }
            case 27:
            {
                for (std::list<uint64>::iterator it = lCrystalGUIDs.begin(); it != lCrystalGUIDs.end(); ++it)
                {
                    if (GameObject* pCrystal = m_creature->GetMap()->GetGameObject(*it))
                        pCrystal->Delete();
                }
                lCrystalGUIDs.clear();
                uiNextActionTimer = 5000;
                break;
            }
            case 28:
            {
                m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                uiCurrAction = 0;
                uiNextActionTimer = 0;
                return;
            }
            default:
            {
                uiNextActionTimer = 4000;
                break;
            }
        }
        ++uiCurrAction;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (uiCurrAction)
        {
            if (uiNextActionTimer < uiDiff)
                Larksbane_DoAction();
            else
                uiNextActionTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_npc_Geologist_Larksbane(Creature* pCreature)
{
    return new npc_Geologist_LarksbaneAI(pCreature);
}

bool QuestComplete_npc_Geologist_Larksbane(Player* pPlayer, Creature* pQuestGiver, Quest const* pQuest)
{
    if (pQuest->GetQuestId() != 8315) // The Calling
        return false;

    if (npc_Geologist_LarksbaneAI* pLark = dynamic_cast<npc_Geologist_LarksbaneAI*>(pQuestGiver->AI()))
        pLark->QuestCompleted(pPlayer, pQuest);
    return true;
}

/*###
 ## npc_Emissary_Romankhan
 ###*/

/*
 UPDATE `creature_template` SET `minmana` = 432240, `maxmana` = 434240, `armor` = 3769, `speed_walk` = 1.24, `speed_run` = 1.24, `baseattacktime` = 1280,
 `resistance1` = 0, `mingold` = 4031, `maxgold` = 4031, `mechanic_immune_mask` = 650854367, `MovementType` = 1,
 `ScriptName` = 'npc_Emissary_Romankhan' WHERE `entry` = 14862;
 */

enum
{
    SPELL_WILT                  = 23772,
    SPELL_SUFFERING_OF_SANITY   = 23773,
    SPELL_SYSTEM_SHOCK          = 23774
};

struct npc_Emissary_RomankhanAI : public ScriptedAI
{
    npc_Emissary_RomankhanAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
        pCreature->SetVisibility(VISIBILITY_OFF);

        OverlordCount = 0;
        if (Creature* add = pCreature->SummonCreature(15288, -7233.39f, 906.415f, -1.76649f, 1.81259f, TEMPSUMMON_DEAD_DESPAWN, 0))   // Aluntir
            add->JoinCreatureGroup(pCreature, 0, 0, OPTION_RESPAWN_TOGETHER);
        if (Creature* add = pCreature->SummonCreature(15286, -7212.16f, 911.711f, -1.76649f, 2.58543f, TEMPSUMMON_DEAD_DESPAWN, 0))   // Xil'xix
            add->JoinCreatureGroup(pCreature, 0, 0, OPTION_RESPAWN_TOGETHER);
        if (Creature* add = pCreature->SummonCreature(15290, -7210.3f, 895.014f, -1.76649f, 0.544185f, TEMPSUMMON_DEAD_DESPAWN, 0))   // Arakis
            add->JoinCreatureGroup(pCreature, 0, 0, OPTION_RESPAWN_TOGETHER);
        Reset();
    }

    int OverlordCount;
    uint32 m_uiWiltTimer;
    uint32 m_uiSchockTimer;
    uint32 m_uiSanityTimer;
    uint32 m_uiCheckPlayerTimer;
    uint64 PlayerGuids[60];

    bool m_bNeedCheck;

    void Reset()
    {
        m_uiWiltTimer = urand(3000, 5000);
        m_uiSchockTimer = urand(8000, 12000);
        m_uiSanityTimer = 1;
        m_uiCheckPlayerTimer = 1000;
        m_bNeedCheck = false;
        m_creature->AllowManaRegen(false);
        m_creature->SetPower(POWER_MANA, 0);

        for (int i = 0; i < 60; i++)
            PlayerGuids[i] = 0;
    }

    void SummonedCreatureJustDied(Creature* unit)
    {
        ++OverlordCount;
        if (OverlordCount >= 3)
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
            m_creature->SetVisibility(VISIBILITY_ON);
        }
    }

    void Aggro(Unit* pWho)
    {
        m_creature->AllowManaRegen(true);
    }

    inline float GetManaPercent()
    {
        return (((float)m_creature->GetPower(POWER_MANA) / (float)m_creature->GetMaxPower(POWER_MANA)) * 100);
    }

    void SpellHitTarget(Unit* target, const SpellEntry* pSpell)
    {
        if (!target || !target->IsPlayer())
            return;

        if (pSpell->Id == SPELL_WILT || pSpell->Id == SPELL_SUFFERING_OF_SANITY || pSpell->Id == SPELL_SYSTEM_SHOCK)
        {
            for (int i = 0; i < 60; i++)
                if (PlayerGuids[i] == target->GetGUID())
                    return;

            for (int i = 0; i < 60; i++)
            {
                if (PlayerGuids[i] == 0)
                {
                    PlayerGuids[i] = target->GetGUID();
                    break;
                }
            }
            m_bNeedCheck = true;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiSchockTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (pTarget->GetTypeId() == TYPEID_PLAYER)
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_SYSTEM_SHOCK) == CAST_OK)
                        m_uiSchockTimer = urand(6000, 10000);
                }
                else
                    m_uiSchockTimer = 1000;
            }
        }
        else
            m_uiSchockTimer -= uiDiff;

        if (m_uiWiltTimer < uiDiff)
        {
            for (int i = 0; i < 60; i++)
                PlayerGuids[i] = 0;
            if (DoCastSpellIfCan(m_creature, SPELL_WILT) == CAST_OK)
                m_uiWiltTimer = urand(3000, 5000);
        }
        else
            m_uiWiltTimer -= uiDiff;

        if (GetManaPercent() == 100.0f)
        {
            if (m_uiSanityTimer < uiDiff)
            {
                for (int i = 0; i < 60; i++)
                    PlayerGuids[i] = 0;
                if (DoCastSpellIfCan(m_creature, SPELL_SUFFERING_OF_SANITY) == CAST_OK)
                    m_uiSanityTimer = 1000;
            }
            else
                m_uiSanityTimer -= uiDiff;
        }
        else if (m_uiSanityTimer != 1)
            m_uiSanityTimer = 1;

        DoMeleeAttackIfReady();

        if (!m_bNeedCheck)
            return;

        if (m_uiCheckPlayerTimer < uiDiff)
        {
            for (int i = 0; i < 60; i++)
            {
                if (PlayerGuids[i] == 0)
                    continue;

                if (Unit* pTarget = m_creature->GetMap()->GetUnit(PlayerGuids[i]))
                {
                    pTarget->SetInCombatWith(m_creature);
                    if (pTarget->isDead())
                    {
                        // + 2% MP at each death
                        float currMana = m_creature->GetPower(POWER_MANA);
                        float MaxMana = m_creature->GetMaxPower(POWER_MANA);
                        m_creature->SetPower(POWER_MANA, (currMana + MaxMana * 0.02f) < MaxMana ? (currMana + MaxMana * 0.02f) : MaxMana);

                        PlayerGuids[i] = 0;
                    }
                }
            }
            m_bNeedCheck = false;
            m_uiCheckPlayerTimer = 1000;
        }
        else
            m_uiCheckPlayerTimer -= uiDiff;
    }
};

CreatureAI* GetAI_npc_Emissary_Romankhan(Creature* pCreature)
{
    return new npc_Emissary_RomankhanAI(pCreature);
}

/*###
 ## npc_anachronos_the_ancient
 ###*/

enum
{
    // Dragons
    NPC_MERITHRA_OF_THE_DREAM = 15378,
    NPC_CAELESTRASZ = 15379,
    NPC_ARYGOS = 15380,
    NPC_ANACHRONOS_THE_ANCIENT = 15381,
    NPC_ANACHRONOS_QUEST_TRIGGER = 15454, // marks some movement for the dragons

    // Elfs
    NPC_FANDRAL_STAGHELM = 15382,
    NPC_KALDOREI_INFANTRY = 15423,

    // Qiraji warriors
    NPC_QIRAJI_WASP = 15414,
    NPC_QIRAJI_DRONE = 15421,
    NPC_QIRAJI_TANK = 15422,
    NPC_ANUBISATH_CONQUEROR = 15424,

    QUEST_A_PAWN_ON_THE_ETERNAL_BOARD = 8519,

    // Yells -> in chronological order
    SAY_ANACHRONOS_INTRO_1 = -1000753,
    SAY_FANDRAL_INTRO_2 = -1000754,
    SAY_MERITHRA_INTRO_3 = -1000755,
    EMOTE_ARYGOS_NOD = -1000756,
    SAY_CAELESTRASZ_INTRO_4 = -1000757,
    EMOTE_MERITHRA_GLANCE = -1000758,
    SAY_MERITHRA_INTRO_5 = -1000759,

    SAY_MERITHRA_ATTACK_1 = -1000760,
    SAY_ARYGOS_ATTACK_2 = -1000761,
    SAY_ARYGOS_ATTACK_3 = -1000762,
    SAY_CAELESTRASZ_ATTACK_4 = -1000763,
    SAY_CAELESTRASZ_ATTACK_5 = -1000764,

    SAY_ANACHRONOS_SEAL_1 = -1000765,
    SAY_FANDRAL_SEAL_2 = -1000766,
    SAY_ANACHRONOS_SEAL_3 = -1000767,
    SAY_ANACHRONOS_SEAL_4 = -1000768,
    SAY_ANACHRONOS_SEAL_5 = -1000769,
    SAY_FANDRAL_SEAL_6 = -1000770,

    EMOTE_FANDRAL_EXHAUSTED = -1000771,
    SAY_ANACHRONOS_EPILOGUE_1 = -1000772,
    SAY_ANACHRONOS_EPILOGUE_2 = -1000773,
    SAY_ANACHRONOS_EPILOGUE_3 = -1000774,
    EMOTE_ANACHRONOS_SCEPTER = -1000775,
    SAY_FANDRAL_EPILOGUE_4 = -1000776,
    SAY_FANDRAL_EPILOGUE_5 = -1000777,
    EMOTE_FANDRAL_SHATTER = -1000778,
    SAY_ANACHRONOS_EPILOGUE_6 = -1000779,
    SAY_FANDRAL_EPILOGUE_7 = -1000780,
    EMOTE_ANACHRONOS_DISPPOINTED = -1000781,
    EMOTE_ANACHRONOS_PICKUP = -1000782,
    SAY_ANACHRONOS_EPILOGUE_8 = -1000783,

    // The transform spell for Anachronos was removed from DBC
    //DISPLAY_ID_BRONZE_DRAGON = 15500,

    // Spells
    SPELL_GREEN_DRAGON_TRANSFORM = 25105,
    SPELL_RED_DRAGON_TRANSFORM = 25106,
    SPELL_BLUE_DRAGON_TRANSFORM = 25107,
    SPELL_BRONZE_DRAGON_TRANSFORM = 25108, // Spell was removed - exists only before 2.0.1

    SPELL_MERITHRA_WAKE = 25145, // should trigger 25172 on targets
    SPELL_WAKE_VISUAL = 25172,
    GO_MERITHRA_WAKE =  180604,  // For green fog visual
    SPELL_ARYGOS_VENGEANCE = 25149,
    SPELL_VENGEANCE_VISUAL = 25168,
    SPELL_CAELESTRASZ_MOLTEN_RAIN = 25150,
    SPELL_MOLTEN_RAIN_VISUAL = 25169,
    SPELL_MOLTEN_RAIN_DAMAGE = 25170,

    SPELL_TIME_STOP = 25158, // Anachronos stops the battle - should trigger 25171
    SPELL_GLYPH_OF_WARDING = 25166, // Sends event 9427 - should activate Go 176148
    SPELL_PRISMATIC_BARRIER = 25159, // Sends event 9425 - should activate Go 176146
    SPELL_CALL_ANCIENTS = 25167, // Sends event 9426 - should activate Go 176147
    SPELL_SHATTER_HAMMER = 25182, // Breakes the scepter - needs DB coords

    POINT_ID_DRAGON_ATTACK = 0,
    POINT_ID_EXIT = 1,
    POINT_ID_GATE = 2,
    POINT_ID_SCEPTER = 3,
    POINT_ID_SCEPTER_1 = 4,
    POINT_ID_SCEPTER_2 = 5,
    POINT_ID_EPILOGUE = 6,

    MAX_DRAGONS = 4,
    MAX_CONQUERORS = 3,
    MAX_QIRAJI = 6,
    MAX_KALDOREI = 20,

    GO_AQ_BARRIER           = 176146,
    GO_AQ_GATE_ROOTS        = 176147,
    GO_AQ_GATE_RUNES        = 176148,
    GO_AQ_GHOST_GATE        = 180322,

    AQ_OPEN_IF_CLOSED = 0,
    AQ_PREPARE_CLOSE = 1,
    AQ_CLOSE = 2,
    AQ_RESET = 3,
    AQ_CLOSE_QUIETLY = 4,

    SCENE_BLOCK_TIME = 15 * MINUTE * IN_MILLISECONDS,
};

struct EventLocations
{
    float m_fX, m_fY, m_fZ, m_fO;
    uint32 m_uiEntry;
};

static EventLocations aEternalBoardNPCs[MAX_DRAGONS] =
{
    { -8029.301f, 1534.612f, 2.609f, 3.121f, NPC_FANDRAL_STAGHELM},
    { -8034.227f, 1536.580f, 2.609f, 6.161f, NPC_ARYGOS},
    { -8031.935f, 1532.658f, 2.609f, 1.012f, NPC_CAELESTRASZ},
    { -8034.106f, 1534.224f, 2.609f, 0.290f, NPC_MERITHRA_OF_THE_DREAM},
};

static EventLocations aQirajiWarriors[MAX_CONQUERORS] =
{
    { -8092.12f, 1508.32f, 2.94f, 0.0f, 0},
    { -8096.54f, 1525.84f, 2.83f, 0.0f, 0}, // Also used as an anchor point for the rest of the summons
    { -8097.81f, 1541.74f, 2.88f, 0.0f, 0},
};

static EventLocations aEternalBoardMovement[] =
{
    { -8250.951f, 1500.241f, 110.0f, 0.0f, 0}, // Flight position for dragons
    { -8107.867f, 1526.102f, 2.645f, 0.0f, 0}, // Anachronos gate location
    { -8103.861f, 1525.923f, 2.677f, 0.0f, 0}, // Fandral gate location
    { -8110.313f, 1522.049f, 2.618f, 0.0f, 0}, // Shattered scepter
    { -8100.921f, 1527.740f, 2.871f, 0.0f, 0}, // Fandral epilogue location
    { -8115.270f, 1515.926f, 3.305f, 0.0f, 0}, // Anachronos gather broken scepter 1
    { -8116.879f, 1530.615f, 3.762f, 0.0f, 0}, // Anachronos gather broken scepter 2
    { -7997.790f, 1548.664f, 3.738f, 0.0f, 0}, // Fandral exit location
    { -8061.933f, 1496.196f, 2.556f, 0.0f, 0}, // Anachronos launch location
    { -8008.705f, 1446.063f, 44.104f, 0.0f, 0}, // Anachronos flight location
};

struct npc_anachronos_the_ancientAI : public ScriptedAI
{
    npc_anachronos_the_ancientAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiEventTimer;

    uint8 m_uiEventStage;

    ObjectGuid m_uiFandralGUID;
    ObjectGuid m_uiMerithraGUID;
    ObjectGuid m_uiCaelestraszGUID;
    ObjectGuid m_uiArygosGUID;
    ObjectGuid m_uiPlayerGUID;
    ObjectGuid m_uiTriggerGUID;

    GameObject* go_aq_barrier;
    GameObject* go_aq_gate_runes;
    GameObject* go_aq_gate_roots;

    std::list<uint64> m_lQirajiWarriorsList;

    // Needed to restore gate to state before scene
    bool AQopen;

    void Reset()
    {
        // We summon the rest of the dragons on timer
        m_uiEventTimer = 0;
        m_uiEventStage = 0;

        m_uiFandralGUID.Clear();
        m_uiMerithraGUID.Clear();
        m_uiCaelestraszGUID.Clear();
        m_uiArygosGUID.Clear();
        m_uiPlayerGUID.Clear();
        m_uiTriggerGUID.Clear();

        go_aq_barrier = nullptr;
        go_aq_gate_runes = nullptr;
        go_aq_gate_roots = nullptr;

        AQopen = true;

        m_creature->SetRespawnDelay(DAY);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void BeginScene()
    {
        m_uiEventTimer = 100;
    }

    void AbortScene()
    {
        m_uiEventStage = 0;
        m_uiEventTimer = 0;

        DoUnsummonArmy();

        // Despawn Dragons + Fandral
        if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_uiFandralGUID))
            pFandral->ForcedDespawn();

        if (Creature* pMerithra = m_creature->GetMap()->GetCreature(m_uiMerithraGUID))
            pMerithra->ForcedDespawn();

        if (Creature* pCaelestrasz = m_creature->GetMap()->GetCreature(m_uiCaelestraszGUID))
            pCaelestrasz->ForcedDespawn();

        if (Creature* pArygos = m_creature->GetMap()->GetCreature(m_uiArygosGUID))
            pArygos->ForcedDespawn();

        // Close AQ gates quietly
        SetupAQGate(AQ_CLOSE_QUIETLY);

        // Anachronos will go invisible, blocking new attempts to
        // start the scene until the time is up
        m_creature->SetVisibility(VISIBILITY_OFF);
        m_creature->ForcedDespawn(SCENE_BLOCK_TIME);
    }

    void JustDied(Unit* killer)
    {
        AbortScene();
    }

    void SetupAQGate(uint32 phase)
    {
        if (AQ_OPEN_IF_CLOSED == phase)
        {
            go_aq_barrier = GetClosestGameObjectWithEntry(m_creature, GO_AQ_BARRIER, 200);
            go_aq_gate_runes = GetClosestGameObjectWithEntry(m_creature, GO_AQ_GATE_RUNES, 200);
            go_aq_gate_roots = GetClosestGameObjectWithEntry(m_creature, GO_AQ_GATE_ROOTS, 200);
        }

        AnimateAQGate(go_aq_barrier, phase);
        AnimateAQGate(go_aq_gate_runes, phase);
        AnimateAQGate(go_aq_gate_roots, phase);
    }

    void AnimateAQGate(GameObject* go, uint32 phase = AQ_CLOSE)
    // Phase 0: Open, make invisible
    // Phase 1: Make (open) doors visible
    // Phase 2: Close doors
    // Phase 3: Return to original state if required
    {
        if (nullptr == go)
            return;

        switch (phase)
        {
            case AQ_OPEN_IF_CLOSED:
                // Check if AQ currently closed
                if (go->GetGoState() == GO_STATE_READY)
                {
                    AQopen = false;

                    // Set to open without animation
                    go->SetGoState(GO_STATE_ACTIVE);
                    go->SetVisible(false);
                }

                break;

            case AQ_PREPARE_CLOSE:
                // Prepare for close visual
                go->SetVisible(true);
                break;

            case AQ_CLOSE:

                // Close gates with animation
                go->ResetDoorOrButton();
                go->SetGoState(GO_STATE_READY);

                break;

            case AQ_CLOSE_QUIETLY:
                go->SetVisible(false);
                AnimateAQGate(go, AQ_CLOSE);
                go->SetVisible(true);

                break;

            case AQ_RESET:

                // Return to original state, if necessary
                if (AQopen)
                {
                    go->SetGoState(GO_STATE_ACTIVE);
                    go->SetVisible(false);
                    go->SetVisible(true);
                }
                break;

        }
    }

    void DoSummonDragons()
    {
        for (uint8 i = 0; i < MAX_DRAGONS; ++i)
            m_creature->SummonCreature(aEternalBoardNPCs[i].m_uiEntry, aEternalBoardNPCs[i].m_fX, aEternalBoardNPCs[i].m_fY, aEternalBoardNPCs[i].m_fZ, aEternalBoardNPCs[i].m_fO, TEMPSUMMON_CORPSE_DESPAWN, 0);
    }

    void DoSummonWarriors()
    {
        float fX, fY, fZ;
        // Summon kaldorei warriors
        for (uint8 i = 0; i < MAX_KALDOREI; ++i)
        {
            m_creature->GetRandomPoint(aQirajiWarriors[1].m_fX, aQirajiWarriors[1].m_fY, aQirajiWarriors[1].m_fZ, 20.0f, fX, fY, fZ);
            m_creature->SummonCreature(NPC_KALDOREI_INFANTRY, fX, fY, fZ, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 0);
        }

        // Summon Qiraji warriors
        for (uint8 i = 0; i < MAX_QIRAJI; ++i)
        {
            m_creature->GetRandomPoint(aQirajiWarriors[1].m_fX, aQirajiWarriors[1].m_fY, aQirajiWarriors[1].m_fZ, 20.0f, fX, fY, fZ);
            m_creature->SummonCreature(NPC_QIRAJI_WASP, fX, fY, fZ, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 0);

            m_creature->GetRandomPoint(aQirajiWarriors[1].m_fX, aQirajiWarriors[1].m_fY, aQirajiWarriors[1].m_fZ, 20.0f, fX, fY, fZ);
            m_creature->SummonCreature(NPC_QIRAJI_DRONE, fX, fY, fZ, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 0);

            m_creature->GetRandomPoint(aQirajiWarriors[1].m_fX, aQirajiWarriors[1].m_fY, aQirajiWarriors[1].m_fZ, 20.0f, fX, fY, fZ);
            m_creature->SummonCreature(NPC_QIRAJI_TANK, fX, fY, fZ, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 0);
        }

        // Also summon the 3 anubisath conquerors
        for (uint8 i = 0; i < MAX_CONQUERORS; ++i)
            m_creature->SummonCreature(NPC_ANUBISATH_CONQUEROR, aQirajiWarriors[i].m_fX, aQirajiWarriors[i].m_fY, aQirajiWarriors[i].m_fZ, 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
    }

    void DoUnsummonArmy()
    {
        for (std::list<uint64>::const_iterator itr = m_lQirajiWarriorsList.begin(); itr != m_lQirajiWarriorsList.end(); ++itr)
        {
            if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                pTemp->DisappearAndDie();
        }
    }

    void AddKaldoreiThreat(Creature* npc)
    {
        for (auto itr = m_lQirajiWarriorsList.begin(); itr != m_lQirajiWarriorsList.end(); itr++)
            if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                if (pTemp->GetEntry() == NPC_KALDOREI_INFANTRY)
                    npc->AddThreat(pTemp, 100.0f);

    }

    void JustSummoned(Creature* pSummoned)
    {
        // Also remove npc flags where needed
        switch (pSummoned->GetEntry())
        {
            case NPC_FANDRAL_STAGHELM:
                m_uiFandralGUID = pSummoned->GetObjectGuid();
                break;
            case NPC_MERITHRA_OF_THE_DREAM:
                m_uiMerithraGUID = pSummoned->GetObjectGuid();
                pSummoned->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
                break;
            case NPC_CAELESTRASZ:
                m_uiCaelestraszGUID = pSummoned->GetObjectGuid();
                pSummoned->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
                break;
            case NPC_ARYGOS:
                m_uiArygosGUID = pSummoned->GetObjectGuid();
                pSummoned->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
                break;
            case NPC_ANUBISATH_CONQUEROR:
            case NPC_QIRAJI_WASP:
            case NPC_QIRAJI_DRONE:
            case NPC_QIRAJI_TANK:
                pSummoned->setFaction(14);
                //pSummoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_lQirajiWarriorsList.push_back(pSummoned->GetObjectGuid());

                AddKaldoreiThreat(pSummoned);

                // Commence hostilities
                if(Creature* target = GetClosestCreatureWithEntry(pSummoned, NPC_KALDOREI_INFANTRY, 40.0f))
                {
                    pSummoned->AddThreat(target, 100.0f);
                    m_creature->GetMotionMaster()->MoveChase(target);
                    pSummoned->AI()->AttackStart(target);

                    target->AddThreat(pSummoned, 100.0f);
                    target->GetMotionMaster()->MoveChase(pSummoned);
                    target->AI()->AttackStart(pSummoned);
                }

                pSummoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);

                break;
            case NPC_KALDOREI_INFANTRY:
                pSummoned->setFaction(1608);
                m_lQirajiWarriorsList.push_back(pSummoned->GetObjectGuid());
                break;
        }

        pSummoned->SetRespawnDelay(DAY);
    }

    void DoCastTriggerSpellOnEnemies(uint32 spell)
    {
        for (std::list<uint64>::const_iterator itr = m_lQirajiWarriorsList.begin(); itr != m_lQirajiWarriorsList.end(); ++itr)
        {
            if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
            {
                // Cast trigger spell only on enemies
                if (pTemp->GetEntry() == NPC_ANUBISATH_CONQUEROR || pTemp->GetEntry() == NPC_QIRAJI_DRONE ||
                    pTemp->GetEntry() == NPC_QIRAJI_TANK || pTemp->GetEntry() == NPC_QIRAJI_WASP)
                {
                    if (spell == SPELL_WAKE_VISUAL)
                    {
                        if(pTemp->GetEntry() == NPC_ANUBISATH_CONQUEROR)
                        {
                            pTemp->CastSpell(pTemp, spell, true);
                            pTemp->SummonGameObject(GO_MERITHRA_WAKE, pTemp->GetPositionX(), pTemp->GetPositionY(), pTemp->GetPositionZ(), 0);
                        }
                    }
                    else
                        pTemp->CastSpell(pTemp, spell, true);
                }
            }
        }
    }

    void DoTimeStopArmy()
    {
        for (std::list<uint64>::const_iterator itr = m_lQirajiWarriorsList.begin(); itr != m_lQirajiWarriorsList.end(); ++itr)
        {
            if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
            {
                // Stop movement/attacks and freeze whole combat
                pTemp->RemoveAllAttackers();
                pTemp->AttackStop();
                pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                pTemp->AI()->EnterEvadeMode();
                pTemp->StopMoving();
                pTemp->GetMotionMaster()->Clear(true);
                pTemp->CastSpell(pTemp, 25171, true);
            }
        }
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if (uiType != POINT_MOTION_TYPE)
            return;

        switch (uiPointId)
        {
            case POINT_ID_GATE:
                // Cast time stop when he reaches the gate
                DoCastSpellIfCan(m_creature, SPELL_TIME_STOP);
                DoTimeStopArmy();
                m_uiEventTimer = 7000;
                break;
            case POINT_ID_SCEPTER_1:
                // Pickup the pieces

                DoScriptText(EMOTE_ANACHRONOS_PICKUP, m_creature);
                m_uiEventTimer = 2000;
                m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                break;

            case POINT_ID_SCEPTER_2:

                DoScriptText(SAY_ANACHRONOS_EPILOGUE_8, m_creature);
                m_uiEventTimer = 4000;
                m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                break;

            case POINT_ID_EXIT:
                m_uiEventTimer = 1000;
                break;
        }
    }

    void SummonedMovementInform(Creature* pSummoned, uint32 uiType, uint32 uiPointId)
    {
        if (uiType != POINT_MOTION_TYPE)
            return;

        if (pSummoned->GetEntry() == NPC_FANDRAL_STAGHELM)
        {
            switch (uiPointId)
            {
                case POINT_ID_EPILOGUE:
                    DoScriptText(SAY_FANDRAL_EPILOGUE_7, pSummoned);
                    pSummoned->SetFacingToObject(m_creature);
                    DoUnsummonArmy();
                    SetupAQGate(AQ_RESET);
                    m_uiEventTimer = 8000;
                    break;
                case POINT_ID_SCEPTER:
                    pSummoned->GetMotionMaster()->MovePoint(POINT_ID_EPILOGUE, aEternalBoardMovement[4].m_fX, aEternalBoardMovement[4].m_fY, aEternalBoardMovement[4].m_fZ);
                    break;
                case POINT_ID_EXIT:
                    pSummoned->ForcedDespawn();
                    break;
            }
        }
        else if (uiPointId == POINT_ID_DRAGON_ATTACK)
        {
            switch (pSummoned->GetEntry())
            {
                case NPC_MERITHRA_OF_THE_DREAM:
                    DoScriptText(SAY_MERITHRA_ATTACK_1, pSummoned);
                    m_uiEventTimer = 5000;
                    break;
                case NPC_CAELESTRASZ:
                    DoScriptText(SAY_CAELESTRASZ_ATTACK_5, pSummoned);
                    m_uiEventTimer = 3000;
                    break;
                case NPC_ARYGOS:
                    DoScriptText(SAY_ARYGOS_ATTACK_3, pSummoned);
                    m_uiEventTimer = 4000;
                    break;
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiEventTimer)
        {
            if (m_uiEventTimer <= uiDiff)
            {
                switch (m_uiEventStage)
                {
                    case 0:
                        // Summon the other dragons
                        DoSummonDragons();
                        m_uiEventTimer = 2000;
                        break;
                    case 1:
                        DoScriptText(SAY_ANACHRONOS_INTRO_1, m_creature);
                        m_uiEventTimer = 3000;
                        break;
                    case 2:
                        // Summon warriors
                        DoSummonWarriors();
                        SetupAQGate(AQ_OPEN_IF_CLOSED);

                        m_creature->HandleEmote(EMOTE_ONESHOT_SHOUT);
                        m_uiEventTimer = 3000;
                        break;
                    case 3:
                        if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_uiFandralGUID))
                        {
                            pFandral->SetFacingToObject(m_creature);
                            DoScriptText(SAY_FANDRAL_INTRO_2, pFandral);
                        }
                        m_uiEventTimer = 6000;
                        break;
                    case 4:
                        if (Creature* pMerithra = m_creature->GetMap()->GetCreature(m_uiMerithraGUID))
                        {
                            if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_uiFandralGUID))
                                pFandral->SetFacingToObject(pMerithra);

                            DoScriptText(EMOTE_MERITHRA_GLANCE, pMerithra);
                        }
                        m_uiEventTimer = 2000;
                        break;
                    case 5:
                        if (Creature* pMerithra = m_creature->GetMap()->GetCreature(m_uiMerithraGUID))
                            DoScriptText(SAY_MERITHRA_INTRO_3, pMerithra);
                        m_uiEventTimer = 3000;
                        break;
                    case 6:
                        if (Creature* pArygos = m_creature->GetMap()->GetCreature(m_uiArygosGUID))
                        {
                            DoScriptText(EMOTE_ARYGOS_NOD, pArygos);
                            pArygos->HandleEmote(EMOTE_ONESHOT_YES);
                        }
                        m_uiEventTimer = 4000;
                        break;
                    case 7:
                        if (Creature* pCaelestrasz = m_creature->GetMap()->GetCreature(m_uiCaelestraszGUID))
                            DoScriptText(SAY_CAELESTRASZ_INTRO_4, pCaelestrasz);
                        m_uiEventTimer = 9000;
                        break;
                    case 8:
                        if (Creature* pMerithra = m_creature->GetMap()->GetCreature(m_uiMerithraGUID))
                            DoScriptText(SAY_MERITHRA_INTRO_5, pMerithra);
                        m_uiEventTimer = 5000;
                        break;
                    case 9:
                        // Send Merithra to attack - continue when point is reached
                        if (Creature* pTrigger = GetClosestCreatureWithEntry(m_creature, NPC_ANACHRONOS_QUEST_TRIGGER, 60.0f))
                        {
                            m_uiTriggerGUID = pTrigger->GetObjectGuid();
                            if (Creature* pMerithra = m_creature->GetMap()->GetCreature(m_uiMerithraGUID))
                            {
                                pMerithra->SetWalk(false);
                                pMerithra->GetMotionMaster()->MovePoint(POINT_ID_DRAGON_ATTACK, pTrigger->GetPositionX(), pTrigger->GetPositionY(), pTrigger->GetPositionZ());
                                pMerithra->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                                pMerithra->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                pMerithra->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                            }
                        }
                        m_uiEventTimer = 0;
                        break;
                    case 10:
                        if (Creature* pMerithra = m_creature->GetMap()->GetCreature(m_uiMerithraGUID))
                            pMerithra->CastSpell(pMerithra, SPELL_GREEN_DRAGON_TRANSFORM, false);
                        m_uiEventTimer = 2000;
                        break;
                    case 11:
                        if (Creature* pMerithra = m_creature->GetMap()->GetCreature(m_uiMerithraGUID))
                            pMerithra->HandleEmote(EMOTE_ONESHOT_LIFTOFF);
                        m_uiEventTimer = 1000;
                        break;
                    case 12:
                        if (Creature* pArygos = m_creature->GetMap()->GetCreature(m_uiArygosGUID))
                            DoScriptText(SAY_ARYGOS_ATTACK_2, pArygos);

                        if (Creature* pMerithra = m_creature->GetMap()->GetCreature(m_uiMerithraGUID))
                        {
                            pMerithra->AddAura(17131); // hover
                            pMerithra->CastSpell(pMerithra, SPELL_MERITHRA_WAKE, false);
                        }
                        m_uiEventTimer = 3000;
                        break;

                    case 13:
                        DoCastTriggerSpellOnEnemies(SPELL_WAKE_VISUAL);
                        m_uiEventTimer = 5000;
                        break;

                    case 14:
                        // Send Merithra on flight
                        if (Creature* pMerithra = m_creature->GetMap()->GetCreature(m_uiMerithraGUID))
                        {
                            //pMerithra->SetByteValue(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND/* | UNIT_BYTE1_FLAG_UNK_2*/);
                            pMerithra->SetFly(true);
                            pMerithra->GetMotionMaster()->MovePoint(POINT_ID_EXIT, aEternalBoardMovement[0].m_fX, aEternalBoardMovement[0].m_fY, aEternalBoardMovement[0].m_fZ);
                            pMerithra->ForcedDespawn(20000);
                        }
                        m_uiEventTimer = 1000;
                        break;

                    case 15:
                        // Send Arygos in combat
                        if (Creature* pTrigger = m_creature->GetMap()->GetCreature(m_uiTriggerGUID))
                        {
                            if (Creature* pArygos = m_creature->GetMap()->GetCreature(m_uiArygosGUID))
                            {
                                pArygos->SetWalk(false);
                                pArygos->GetMotionMaster()->MovePoint(POINT_ID_DRAGON_ATTACK, pTrigger->GetPositionX(), pTrigger->GetPositionY(), pTrigger->GetPositionZ());
                                pArygos->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                                pArygos->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                pArygos->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                            }
                        }
                        m_uiEventTimer = 0;
                        break;

                    case 16:
                        if (Creature* pArygos = m_creature->GetMap()->GetCreature(m_uiArygosGUID))
                            pArygos->CastSpell(pArygos, SPELL_BLUE_DRAGON_TRANSFORM, false);
                        m_uiEventTimer = 2000;
                        break;
                    case 17:
                        if (Creature* pArygos = m_creature->GetMap()->GetCreature(m_uiArygosGUID))
                            pArygos->HandleEmote(EMOTE_ONESHOT_LIFTOFF);
                        m_uiEventTimer = 1000;
                        break;
                    case 18:
                        if (Creature* pArygos = m_creature->GetMap()->GetCreature(m_uiArygosGUID))
                        {
                            pArygos->AddAura(17131); // hover
                            pArygos->CastSpell(pArygos, SPELL_ARYGOS_VENGEANCE, false);
                        }
                        m_uiEventTimer = 3000;
                        break;

                    case 19:
                        DoCastTriggerSpellOnEnemies(SPELL_VENGEANCE_VISUAL);
                        m_uiEventTimer = 3000;
                        break;

                    case 20:
                        SetupAQGate(AQ_PREPARE_CLOSE);
                        // Send Arygos on flight
                        if (Creature* pArygos = m_creature->GetMap()->GetCreature(m_uiArygosGUID))
                        {
                            //pArygos->SetByteValue(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND/* | UNIT_BYTE1_FLAG_UNK_2*/);
                            pArygos->SetFly(true);
                            pArygos->GetMotionMaster()->MovePoint(POINT_ID_EXIT, aEternalBoardMovement[0].m_fX, aEternalBoardMovement[0].m_fY, aEternalBoardMovement[0].m_fZ);
                            pArygos->ForcedDespawn(20000);
                        }
                        m_uiEventTimer = 1000;
                        break;
                    case 21:
                        if (Creature* pCaelestrasz = m_creature->GetMap()->GetCreature(m_uiCaelestraszGUID))
                            DoScriptText(SAY_CAELESTRASZ_ATTACK_4, pCaelestrasz);
                        m_uiEventTimer = 1000;
                        break;
                    case 22:
                        // Send Caelastrasz in combat
                        if (Creature* pTrigger = m_creature->GetMap()->GetCreature(m_uiTriggerGUID))
                        {
                            if (Creature* pCaelestrasz = m_creature->GetMap()->GetCreature(m_uiCaelestraszGUID))
                            {
                                pCaelestrasz->SetWalk(false);
                                pCaelestrasz->GetMotionMaster()->MovePoint(POINT_ID_DRAGON_ATTACK, pTrigger->GetPositionX(), pTrigger->GetPositionY(), pTrigger->GetPositionZ());
                                pCaelestrasz->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                                pCaelestrasz->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                pCaelestrasz->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                            }
                        }
                        m_uiEventTimer = 0;
                        break;
                    case 23:
                        if (Creature* pCaelestrasz = m_creature->GetMap()->GetCreature(m_uiCaelestraszGUID))
                            pCaelestrasz->CastSpell(pCaelestrasz, SPELL_RED_DRAGON_TRANSFORM, false);
                        m_uiEventTimer = 2000;
                        break;
                    case 24:
                        if (Creature* pCaelestrasz = m_creature->GetMap()->GetCreature(m_uiCaelestraszGUID))
                            pCaelestrasz->HandleEmote(EMOTE_ONESHOT_LIFTOFF);
                        m_uiEventTimer = 1000;
                        break;
                    case 25:
                        if (Creature* pCaelestrasz = m_creature->GetMap()->GetCreature(m_uiCaelestraszGUID))
                        {
                            pCaelestrasz->AddAura(17131); // hover
                            pCaelestrasz->CastSpell(pCaelestrasz, SPELL_CAELESTRASZ_MOLTEN_RAIN, false);
                        }
                        m_uiEventTimer = 3000;
                        break;
                    case 26:
                        DoCastTriggerSpellOnEnemies(SPELL_MOLTEN_RAIN_VISUAL);
                        DoCastTriggerSpellOnEnemies(SPELL_MOLTEN_RAIN_DAMAGE);
                        m_uiEventTimer = 5000;
                        break;

                    case 27:
                        // Send Caelestrasz on flight
                        if (Creature* pCaelestrasz = m_creature->GetMap()->GetCreature(m_uiCaelestraszGUID))
                        {
                            //pCaelestrasz->SetByteValue(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND/* | UNIT_BYTE1_FLAG_UNK_2*/);
                            pCaelestrasz->SetFly(true);
                            pCaelestrasz->GetMotionMaster()->MovePoint(POINT_ID_EXIT, aEternalBoardMovement[0].m_fX, aEternalBoardMovement[0].m_fY, aEternalBoardMovement[0].m_fZ);
                            pCaelestrasz->ForcedDespawn(20000);
                        }
                        if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_uiFandralGUID))
                            m_creature->SetFacingToObject(pFandral);
                        DoScriptText(SAY_ANACHRONOS_SEAL_1, m_creature);
                        m_uiEventTimer = 5000;
                        break;
                    case 28:
                        if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_uiFandralGUID))
                        {
                            pFandral->SetFacingToObject(m_creature);
                            DoScriptText(SAY_FANDRAL_SEAL_2, pFandral);
                        }
                        m_uiEventTimer = 3000;
                        break;
                    case 29:
                        DoScriptText(SAY_ANACHRONOS_SEAL_3, m_creature);
                        m_uiEventTimer = 1000;
                        break;
                    case 30:
                        // Send Anachronos and Fandral to the gate
                        m_creature->SetWalk(false);
                        m_creature->GetMotionMaster()->MovePoint(POINT_ID_GATE, aEternalBoardMovement[1].m_fX, aEternalBoardMovement[1].m_fY, aEternalBoardMovement[1].m_fZ);
                        m_uiEventTimer = 1000;
                        break;
                    case 31:
                        if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_uiFandralGUID))
                        {
                            pFandral->SetWalk(false);
                            pFandral->GetMotionMaster()->MovePoint(POINT_ID_GATE, aEternalBoardMovement[2].m_fX, aEternalBoardMovement[2].m_fY, aEternalBoardMovement[2].m_fZ);
                        }
                        m_uiEventTimer = 0;
                        break;
                    case 32:
                        DoCastSpellIfCan(m_creature, SPELL_PRISMATIC_BARRIER);

                        AnimateAQGate(go_aq_barrier);


                        m_uiEventTimer = 5000;
                        break;
                    case 33:
                        DoCastSpellIfCan(m_creature, SPELL_GLYPH_OF_WARDING);

                        AnimateAQGate(go_aq_gate_runes);

                        m_uiEventTimer = 4000;
                        break;
                    case 34:
                        DoScriptText(SAY_ANACHRONOS_SEAL_5, m_creature);
                        m_uiEventTimer = 3000;
                        break;
                    case 35:
                        if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_uiFandralGUID))
                        {
                            pFandral->CastSpell(pFandral, SPELL_CALL_ANCIENTS, false);

                            AnimateAQGate(go_aq_gate_roots);
                            DoScriptText(SAY_FANDRAL_SEAL_6, pFandral);
                        }
                        m_uiEventTimer = 9000;
                        break;
                    case 36:
                        if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_uiFandralGUID))
                        {
                            DoScriptText(EMOTE_FANDRAL_EXHAUSTED, pFandral);
                            pFandral->SetStandState(UNIT_STAND_STATE_KNEEL);
                            m_creature->SetFacingToObject(pFandral);
                        }
                        m_uiEventTimer = 1000;
                        break;
                    case 37:
                        DoScriptText(SAY_ANACHRONOS_EPILOGUE_1, m_creature);
                        m_uiEventTimer = 6000;
                        break;
                    case 38:
                        DoScriptText(SAY_ANACHRONOS_EPILOGUE_2, m_creature);
                        m_uiEventTimer = 5000;
                        break;
                    case 39:
                        DoScriptText(SAY_ANACHRONOS_EPILOGUE_3, m_creature);
                        m_uiEventTimer = 15000;
                        break;
                    case 40:
                        // ToDo: Make Fandral equip the scepter
                        if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_uiFandralGUID))
                            DoScriptText(EMOTE_ANACHRONOS_SCEPTER, m_creature, pFandral);
                        m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                        m_uiEventTimer = 3000;
                        break;
                    case 41:
                        if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_uiFandralGUID))
                        {
                            pFandral->SetStandState(UNIT_STAND_STATE_STAND);
                            DoScriptText(SAY_FANDRAL_EPILOGUE_4, pFandral);
                        }
                        m_uiEventTimer = 3000;
                        break;
                    case 42:
                        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                        m_uiEventTimer = 4000;
                        break;
                    case 43:
                        if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_uiFandralGUID))
                            DoScriptText(SAY_FANDRAL_EPILOGUE_5, pFandral);
                        m_uiEventTimer = 9000;
                        break;
                    case 44:
                        if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_uiFandralGUID))
                        {
                            pFandral->CastSpell(pFandral, SPELL_SHATTER_HAMMER, false);
                            DoScriptText(EMOTE_FANDRAL_SHATTER, pFandral);
                        }
                        m_uiEventTimer = 3000;
                        break;
                    case 45:
                        if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_uiFandralGUID))
                        {
                            pFandral->SetWalk(true);
                            pFandral->GetMotionMaster()->MovePoint(POINT_ID_SCEPTER, aEternalBoardMovement[3].m_fX, aEternalBoardMovement[3].m_fY, aEternalBoardMovement[3].m_fZ);
                        }
                        DoScriptText(SAY_ANACHRONOS_EPILOGUE_6, m_creature);
                        m_uiEventTimer = 0;
                        break;
                    case 46:
                        if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_uiFandralGUID))
                        {
                            pFandral->GetMotionMaster()->MovePoint(POINT_ID_EXIT, aEternalBoardMovement[7].m_fX, aEternalBoardMovement[7].m_fY, aEternalBoardMovement[7].m_fZ);
                            pFandral->ForcedDespawn(15000);
                        }
                        m_uiEventTimer = 4000;
                        break;
                    case 47:
                        DoScriptText(EMOTE_ANACHRONOS_DISPPOINTED, m_creature);
                        m_uiEventTimer = 1000;
                        break;
                    case 48:
                        m_creature->SetWalk(true);
                        m_creature->GetMotionMaster()->MovePoint(POINT_ID_SCEPTER_1, aEternalBoardMovement[5].m_fX, aEternalBoardMovement[5].m_fY, aEternalBoardMovement[5].m_fZ);
                        m_uiEventTimer = 0;
                        break;
                    case 49:
                        m_creature->GetMotionMaster()->MovePoint(POINT_ID_SCEPTER_2, aEternalBoardMovement[6].m_fX, aEternalBoardMovement[6].m_fY, aEternalBoardMovement[6].m_fZ);
                        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                        m_uiEventTimer = 0;
                        break;
                    case 50:
                        // Complete quest and despawn gate
                        if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_uiPlayerGUID))
                            pPlayer->GroupEventHappens(QUEST_A_PAWN_ON_THE_ETERNAL_BOARD, m_creature);

                        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                        m_uiEventTimer = 4000;
                        break;
                    case 51:
                        // Move to exit
                        m_creature->SetWalk(false);
                        m_creature->GetMotionMaster()->MovePoint(POINT_ID_EXIT, aEternalBoardMovement[8].m_fX, aEternalBoardMovement[8].m_fY, aEternalBoardMovement[8].m_fZ);
                        m_uiEventTimer = 0;
                        break;
                    case 52:
                        // Spell was removed, manually change the display
                        //DoCastSpellIfCan(m_creature, SPELL_BRONZE_DRAGON_TRANSFORM);
                        //m_creature->SetDisplayId(DISPLAY_ID_BRONZE_DRAGON);
                        m_creature->SetWalk(true);
                        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                        m_creature->CastSpell(m_creature, SPELL_BRONZE_DRAGON_TRANSFORM, false);
                        m_uiEventTimer = 1000;
                        break;
                    case 53:
                        m_creature->HandleEmote(EMOTE_ONESHOT_LIFTOFF);
                        m_uiEventTimer = 1000;
                        break;
                    case 54:
                        m_creature->ForcedDespawn(5000);
                        m_creature->SetFly(true);
                        m_creature->GetMotionMaster()->MovePoint(0, aEternalBoardMovement[9].m_fX, aEternalBoardMovement[9].m_fY, aEternalBoardMovement[9].m_fZ);
                        //m_creature->SetByteValue(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND/* | UNIT_BYTE1_FLAG_UNK_2*/);
                        m_uiEventTimer = 0;
                        break;
                }
                ++m_uiEventStage;
            }
            else
                m_uiEventTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_npc_anachronos_the_ancient(Creature* pCreature)
{
    return new npc_anachronos_the_ancientAI(pCreature);
}

bool QuestAcceptGO_crystalline_tear(Player* pPlayer, GameObject* pGo, const Quest* pQuest)
{
    // Summon the controller dragon at GO position (orientation is wrong - hardcoded)
    if (pQuest->GetQuestId() == QUEST_A_PAWN_ON_THE_ETERNAL_BOARD)
    {
        // Check if event is already in progress first
        if (Creature* pAnachronos = GetClosestCreatureWithEntry(pGo, NPC_ANACHRONOS_THE_ANCIENT, 180.0f))
            return true;

        if (Creature* pAnachronos = pPlayer->SummonCreature(NPC_ANACHRONOS_THE_ANCIENT, pGo->GetPositionX(), pGo->GetPositionY(), pGo->GetPositionZ(), 3.75f, TEMPSUMMON_CORPSE_DESPAWN, 0))
        {
            // Send the player's guid in order to handle quest completion
            if (npc_anachronos_the_ancientAI* pAnachronosAI = dynamic_cast<npc_anachronos_the_ancientAI*>(pAnachronos->AI()))
            {
                pAnachronosAI->m_uiPlayerGUID = pPlayer->GetObjectGuid();
                pAnachronosAI->BeginScene();
            }

            return true;
        }
    }

    return false;
}

enum
{
    MAX_QIRAJI_CRYSTALS     = 6,
    VAR_CRYSTALS_AWARDED    = 1,

    QUEST_MIGHT_OF_KALIMDOR = 8742,
    QUEST_BANG_A_GONG       = 8743,

    GLOBAL_TEXT_CHAMPION    = -1000007,
    GLOBAL_TEXT_CRYSTALS    = -1000008,

    TEXT_COLOSSUS_ASHI      = -1000009,
    TEXT_COLOSSUS_REGAL     = -1000016,
    TEXT_COLOSSUS_ZORA      = -1000017,

    GAME_EVENT_AQ_WAR       = 61,
    GAME_EVENT_ASHI         = 62,
    GAME_EVENT_REGAL        = 63,
    GAME_EVENT_ZORA         = 64,
    GAME_EVENT_CRYSTALS     = 65,


    STAGE_OPEN_GATES        = 0,
    STAGE_WAR               = 1,
    STAGE_RESET             = 2,

    NPC_RAJAXX              = 15341,
};

static const EventLocations rajaxxSpawnPoint = { -8106.35f, 1523.41f, 2.61f, 0.06166f, 0 };

struct scarab_gongAI: public GameObjectAI
{
    scarab_gongAI(GameObject* go) : GameObjectAI(go)
    {
        eventTimer = 0;
        eventStep = 0;
        eventStage = STAGE_OPEN_GATES;
    }

    uint32 eventTimer;
    uint32 eventStep;
    uint32 eventStage;

    GameObject* go_aq_barrier;
    GameObject* go_aq_gate_runes;
    GameObject* go_aq_gate_roots;
    // Invisible AQ barrier
    GameObject* go_aq_ghost_gate;

    void UpdateAI(const uint32 uiDiff)
    {
        if (eventTimer)
        {
            if (eventTimer <= uiDiff)
            {
                switch (eventStage)
                {
                    case STAGE_OPEN_GATES: return HandleOpeningStage();
                    case STAGE_WAR:         return HandleWarStage();
                    case STAGE_RESET:      return ResetAQGates();
                }

            }
            else eventTimer -= uiDiff;
        }

    }

    void NextStage(uint32 timeUntil = 100)
    {
        eventStage++;
        eventStep = 0;
        eventTimer = timeUntil;
    }

    void HandleOpeningStage()
    {
        switch (eventStep)
        {
            case 0:
                go_aq_gate_roots->ResetDoorOrButton();
                go_aq_gate_roots->UseDoorOrButton();

                // Remove invisible barrier
                go_aq_ghost_gate->SetSpawnedByDefault(false);
                go_aq_ghost_gate->Despawn();
                go_aq_ghost_gate->SaveToDB();
                eventTimer = 5000;
                break;

            case 1:
                go_aq_gate_runes->ResetDoorOrButton();
                go_aq_gate_runes->UseDoorOrButton();

                eventTimer = 3000;
                break;

            case 2:
                sWorld.SendWorldText(GLOBAL_TEXT_CRYSTALS);
                eventTimer = 5000;

                break;

            case 3:
                go_aq_barrier->ResetDoorOrButton();
                go_aq_barrier->UseDoorOrButton();

                me->MonsterScriptToZone(TEXT_COLOSSUS_ASHI, CHAT_MSG_MONSTER_EMOTE);
                me->MonsterScriptToZone(TEXT_COLOSSUS_REGAL, CHAT_MSG_MONSTER_EMOTE);
                me->MonsterScriptToZone(TEXT_COLOSSUS_ZORA, CHAT_MSG_MONSTER_EMOTE);

                return NextStage(10000);

        }

        eventStep++;

    }

    void HandleWarStage()
    {
        switch (eventStep)
        {
            case 0:
                sGameEventMgr.StartEvent(GAME_EVENT_CRYSTALS, true);
                eventTimer = 10000;

                break;

            // Summon Rajaxx
            case 1:
                me->SummonCreature(NPC_RAJAXX, rajaxxSpawnPoint.m_fX, rajaxxSpawnPoint.m_fY, rajaxxSpawnPoint.m_fZ, rajaxxSpawnPoint.m_fO, TEMPSUMMON_CORPSE_DESPAWN, 0);

                return EventDone();
        }

        eventStep++;
    }


    void BeginAQOpeningEvent(Player* player)
    {
        if (!player)
            return;

        go_aq_barrier    = GetClosestGameObjectWithEntry(me, GO_AQ_BARRIER, 150);
        go_aq_gate_runes = GetClosestGameObjectWithEntry(me, GO_AQ_GATE_RUNES, 150);
        go_aq_gate_roots = GetClosestGameObjectWithEntry(me, GO_AQ_GATE_ROOTS, 150);
        go_aq_ghost_gate = GetClosestGameObjectWithEntry(me, GO_AQ_GHOST_GATE, 150);

        if (!go_aq_barrier || !go_aq_gate_runes || !go_aq_gate_roots || !go_aq_ghost_gate)
            return;

        // Abort "Pawn on the Eternal Board" scene if currently active.
        if (Creature* pAnachronos = GetClosestCreatureWithEntry(me, NPC_ANACHRONOS_THE_ANCIENT, 180.0f))
        {
            if (npc_anachronos_the_ancientAI* pAnachronosAI = dynamic_cast<npc_anachronos_the_ancientAI*>(pAnachronos->AI()))
            {
                pAnachronosAI->AbortScene();
                eventTimer += 8000;
            }
        }

        // Should not be open
        if (go_aq_barrier->GetGoState() == GO_STATE_ACTIVE)
            return;

        // Announce Champion to the world
        sWorld.SendWorldText(GLOBAL_TEXT_CHAMPION, player->GetName());

        eventTimer += 1000;
        eventStage = STAGE_OPEN_GATES;
    }

    void EventDone()
    {
        NextStage(0);
        eventStage = STAGE_OPEN_GATES;
    }

    void ResetAQGates()
    // For debugging
    {
        go_aq_ghost_gate->SetGoState(GO_STATE_READY);
        go_aq_barrier->SetGoState(GO_STATE_READY);
        go_aq_gate_runes->SetGoState(GO_STATE_READY);
        go_aq_gate_roots->ResetDoorOrButton();
        go_aq_gate_roots->SetGoState(GO_STATE_READY);

        EventDone();
    }
};

GameObjectAI* GetAIscarab_gong(GameObject *go)
{
    return new scarab_gongAI(go);
}

bool GOHello_scarab_gong(Player* player, GameObject* go)
{
    if (go->GetGoType() != GAMEOBJECT_TYPE_QUESTGIVER)
        return false;

    //uint32 crystalsAwarded = sObjectMgr.GetSavedVariable(VAR_CRYSTALS_AWARDED, MAX_QIRAJI_CRYSTALS);

    // Not used since we moved to time-based window
    //if (crystalsAwarded >= MAX_QIRAJI_CRYSTALS)
    //    return false;

    player->PrepareQuestMenu(go->GetObjectGuid());
    player->SendPreparedQuest(go->GetObjectGuid());

    return true;
}

bool QuestRewarded_scarab_gong(Player* player, GameObject* go, Quest const* quest)
{
    if (quest->GetQuestId() != QUEST_BANG_A_GONG)
        return false;

    uint32 crystalsAwarded = sObjectMgr.GetSavedVariable(VAR_CRYSTALS_AWARDED, MAX_QIRAJI_CRYSTALS);

    // Just to be sure
    // Not used since we moved to time-based window
    //if (crystalsAwarded >= MAX_QIRAJI_CRYSTALS)
    //    return false;

    // Increment number of black crystals given
    sObjectMgr.SetSavedVariable(VAR_CRYSTALS_AWARDED, crystalsAwarded + 1, true);

    // Special case for first to ring the Gong
    if (0 == crystalsAwarded)
    {
        if (scarab_gongAI* gongAI = dynamic_cast<scarab_gongAI*>(go->AI()))
            gongAI->BeginAQOpeningEvent(player);
    }

    return true;
}



/*########################
 ## npc_Krug_SkullSplit ##
 ########################*/

#define GOSSIP_ITEM_KRUG_SKULLSPLIT_1 "Continue."
#define GOSSIP_ITEM_KRUG_SKULLSPLIT_2 "Very well, let's go!"

/* Hunterkiller */
#define HUNTERKILLER_SPAWN_POS_X -7765.0f
#define HUNTERKILLER_SPAWN_POS_Y 536.0f
#define HUNTERKILLER_SPAWN_POS_Z -43.0f

#define HUNTERKILLER_DEST_POS_X -7549.78f
#define HUNTERKILLER_DEST_POS_Y 711.901f
#define HUNTERKILLER_DEST_POS_Z -16.4527f

struct HunterMove
{
    float X;
    float Y;
    float Z;
    float O;
};

#define HUNTERKILLER_WAYPOINTS_NUMBER 16
static HunterMove HunterKillerWaypoint[HUNTERKILLER_WAYPOINTS_NUMBER] =
{
    { -7769.14f, 549.365f, -44.1111f, 0.868761f},
    { -7762.88f, 564.709f, -45.9125f, 1.17899f},
    { -7749.28f, 589.14f, -47.1617f, 1.05334f},
    { -7748.5f, 622.853f, -45.2946f, 0.982653f},
    { -7730.37f, 643.343f, -46.7866f, 0.456442f},
    { -7694.24f, 660.321f, -51.0377f, 0.441519f},
    { -7674.25f, 671.264f, -50.4592f, 0.0841609f},
    { -7655.01f, 671.601f, -47.9161f, 1.13424f},
    { -7634.07f, 672.497f, -47.8617f, 0.0857299f},
    { -7603.06f, 667.321f, -48.6429f, 6.0249f},
    { -7571.59f, 665.263f, -48.3899f, 0.0480175f},
    { -7562.58f, 687.328f, -49.9864f, 1.36749f},
    { -7559.18f, 689.958f, -40.8654f, 1.44927f},
    { -7555.3f, 692.953f, -30.7562f, 0.671711f},
    { -7548.27f, 697.037f, -25.6632f, 1.09582f},
    { -7547.07f, 706.949f, -17.7001f, 4.68419f},
};

/* Shai */
#define SHAI_SPAWN_POS_X -7556.600098f
#define SHAI_SPAWN_POS_Y 749.007019f
#define SHAI_SPAWN_POS_Z -17.578800f

#define SHAI_DEST_POS_X -7553.46f
#define SHAI_DEST_POS_Y 720.924f
#define SHAI_DEST_POS_Z -16.715f

/* Merok */
#define MEROK_SPAWN_POS_X -7537.149902f
#define MEROK_SPAWN_POS_Y 731.184021f
#define MEROK_SPAWN_POS_Z -16.418100f

#define MEROK_DEST_POS_X -7542.45f
#define MEROK_DEST_POS_Y 720.155f
#define MEROK_DEST_POS_Z -15.6801f

enum
{
    QUEST_FIELD_DUTY = 8731,
    QUEST_FIELD_DUTY_PAPERS = 8732,

    NPC_HUNTERKILLER = 15620,
    NPC_KRUG_SKULLSPLIT = 15612,
    NPC_MEROK_LONGSTRIDE = 15613,
    NPC_SHADOW_PRIESTESS_SHAI = 15615,
    NPC_ORGRIMMAR_LEGION_GRUNT = 15616,

    /* Emotes */
    SAY_LINE_1 = -1780131, //Spawn
    SAY_LINE_2 = -1780132,
    SAY_LINE_3 = -1780133,
    SAY_LINE_4 = -1780134,
    SAY_LINE_5 = -1780135,
    SAY_LINE_6 = -1780136,
    SAY_LINE_7 = -1780137,
    SAY_LINE_8 = -1780138,
    SAY_LINE_9 = -1780139,

    /* Hunterkiller */
    SPELL_THUNDERCLAP = 15588,
    SPELL_CHARGE = 25821,
    SPELL_CLEAVE = 11427,
    SPELL_FEAR = 25815,

    /* Merok */
    SPELL_HEALING_WAVE = 15982,

    /* Shai */
    SPELL_FLASH_HEAL = 17138
};

struct mob_HiveRegal_HunterKillerAI : public ScriptedAI
{
    mob_HiveRegal_HunterKillerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiThunderClapTimer;
    uint32 m_uiChargeTimer;
    uint32 m_uiCleaveTimer;
    uint32 m_uiFearTimer;

    void Reset()
    {
        m_uiThunderClapTimer = 17000;
        m_uiChargeTimer = urand(15000, 20000);
        m_uiCleaveTimer = urand(4000, 8000);
        m_uiFearTimer = urand(5000, 10000);
        // m_creature->MonsterTextEmote("Reset", NULL); // pour debug
    }

    Unit* GetVictimInRangePlayerOnly(float min, float max)
    {
        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator itr = tList.begin(); itr != tList.end(); ++itr)
        {
            if (ObjectGuid uiTargetGuid = (*itr)->getUnitGuid())
            {
                if (Unit* pTarget = m_creature->GetMap()->GetUnit(uiTargetGuid))
                {
                    if (pTarget->GetTypeId() == TYPEID_PLAYER && m_creature->IsInRange(pTarget, min, max))
                        return pTarget;
                }
            }
        }
        return NULL;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->IsNonMeleeSpellCasted(false))
            return;

        // THUNDERCLAP
        if (m_uiThunderClapTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_THUNDERCLAP);
            m_uiThunderClapTimer = 20000;
        }
        else
            m_uiThunderClapTimer -= uiDiff;

        // CHARGE
        if (m_uiChargeTimer <= uiDiff)
        {
            if (Unit* pTarget = GetVictimInRangePlayerOnly(5.0f, 30.0f))
            {
                DoCastSpellIfCan(pTarget, SPELL_CHARGE);
                m_uiChargeTimer = urand(30000, 35000);
            }
        }
        else
            m_uiChargeTimer -= uiDiff;

        // CLEAVE
        if (m_uiCleaveTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE);
            m_uiCleaveTimer = urand(5000, 12000);
        }
        else
            m_uiCleaveTimer -= uiDiff;

        // FEAR
        if (m_uiFearTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_FEAR);
            m_uiFearTimer = urand(58000, 63000);
        }
        else
            m_uiFearTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

/* TODO: Fixer les lanceurs de hache au sol */

CreatureAI* GetAI_mob_HiveRegal_HunterKiller(Creature* pCreature)
{
    return new mob_HiveRegal_HunterKillerAI(pCreature);
}


typedef enum
{
    EVENT_NOT_STARTED,
    EVENT_STARTED,
    EVENT_COMPLETE
} FieldDutyPaperEventStatus;


struct npc_Krug_SkullSplitAI : public ScriptedAI
{
    /* Written by Ivina & Malkins < Nostalrius > */
    npc_Krug_SkullSplitAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
        ResetEvent();
    }

    FieldDutyPaperEventStatus eEventStatus;
    uint32 m_uiEventResetTimer;
    uint32 m_uiSpeachNum;
    uint32 m_uiGruntSpeachTimer;
    uint32 m_uiMoveTimer;
    uint32 InitTimer;
    uint32 m_uiSpeachTimer;
    bool m_bAlreadyMoved;
    bool m_bIsDoingSpeach;
    bool m_bGruntSpeech;
    ObjectGuid m_uiShaiGUID;
    ObjectGuid m_uiMerokGUID;
    ObjectGuid m_uiHunterKillerGUID;
    int waypoint;

    void Reset()
    {
    }

    /* Custom methods */

    FieldDutyPaperEventStatus GetEventStatus()
    {
        return eEventStatus;
    }

    void ResetEvent()
    {
        // m_creature->MonsterTextEmote("reset", NULL); // pour debug
        eEventStatus = EVENT_NOT_STARTED;
        if (Creature* pHunterKiller = m_creature->GetMap()->GetCreature(m_uiHunterKillerGUID))
        {
            pHunterKiller->ForcedDespawn();
            pHunterKiller->AddObjectToRemoveList();
        }
        m_uiSpeachTimer = 0;
        m_uiSpeachNum = 0;
        m_uiHunterKillerGUID.Clear();
        m_uiEventResetTimer = 120000;
        m_bAlreadyMoved = true;
        m_bIsDoingSpeach = false;
        m_bGruntSpeech = false;
        m_uiMoveTimer = 10000;
        ResetOtherNPCsPosition();
        InitTimer = 2000;
        waypoint = 0;
    }

    void StartEvent()
    {
        if (eEventStatus == EVENT_NOT_STARTED)
        {
            DoScriptText(SAY_LINE_1, m_creature);

            m_uiGruntSpeachTimer = 3000;
            m_uiSpeachTimer = 10000;
            m_uiSpeachNum = 0;
            m_bIsDoingSpeach = true;
            m_bGruntSpeech = false;

            Creature* pHunterKiller = m_creature->SummonCreature(NPC_HUNTERKILLER,
                                      HUNTERKILLER_SPAWN_POS_X, HUNTERKILLER_SPAWN_POS_Y, HUNTERKILLER_SPAWN_POS_Z, 0.8f,
                                      TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 450000, true);
            if (pHunterKiller)
            {
                eEventStatus = EVENT_STARTED;
                m_bAlreadyMoved = false;
                m_uiHunterKillerGUID = pHunterKiller->GetObjectGuid();
                InitOtherNPCsGuids();
                pHunterKiller->setFaction(35);
            }
        }
    }

    void CompleteEvent()
    {
        m_uiEventResetTimer = 1800000;
        eEventStatus = EVENT_COMPLETE;
        m_bIsDoingSpeach = false;
        m_bGruntSpeech = true;
    }

    void InitOtherNPCsGuids()
    {
        m_uiMerokGUID = 0;
        m_uiShaiGUID = 0;

        if (Creature* pMerok = m_creature->FindNearestCreature(15613, 100.0f, true))
            m_uiMerokGUID = pMerok->GetObjectGuid();

        if (Creature* pShai = m_creature->FindNearestCreature(15615, 100.0f, true))
            m_uiShaiGUID = pShai->GetObjectGuid();
    }

    void ResetOtherNPCsPosition()
    {
        if (Creature* pMerok = m_creature->GetMap()->GetCreature(m_uiMerokGUID))
            pMerok->GetMotionMaster()->MovePoint(0, MEROK_SPAWN_POS_X, MEROK_SPAWN_POS_Y, MEROK_SPAWN_POS_Z);

        if (Creature* pShai = m_creature->GetMap()->GetCreature(m_uiShaiGUID))
            pShai->GetMotionMaster()->MovePoint(0, SHAI_SPAWN_POS_X, SHAI_SPAWN_POS_Y, SHAI_SPAWN_POS_Z);
    }

    /* Standart AI methods */
    void SummonedCreatureJustDied(Creature* pSummoned)
    {

        if (pSummoned->GetObjectGuid() == m_uiHunterKillerGUID)
        {
            // m_creature->MonsterTextEmote("SumCreaJustDied", NULL); // pour debug
            CompleteEvent();
        }
    }

    void SummonedCreatureDespawn(Creature* pSummoned)
    {
        if (pSummoned->GetObjectGuid() == m_uiHunterKillerGUID)
        {
            // m_creature->MonsterTextEmote("SumCreaDespawn", NULL); // pour debug
            ResetEvent();
        }
    }

    void JustDied(Unit* pKiller)
    {
        // m_creature->MonsterTextEmote("JustDied", NULL); // pour debug
        ResetEvent();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        /* Event complete state is lasting 30 sec then event is reset */
        if (eEventStatus == EVENT_COMPLETE)
        {
            if (m_uiEventResetTimer <= uiDiff)
            {
                // m_creature->MonsterTextEmote("CompleteEnd", NULL); // pour debug
                ResetEvent();
            }
            else
                m_uiEventResetTimer -= uiDiff;
        }

        /* Speech */
        if (m_bIsDoingSpeach)
        {
            if (m_uiSpeachTimer < uiDiff)
            {
                switch (m_uiSpeachNum)
                {
                    case 0:
                        // 8 seconds till next line
                        DoScriptText(SAY_LINE_2, m_creature);
                        m_uiSpeachTimer = 10000;
                        ++m_uiSpeachNum;
                        break;
                    case 1:
                        // 8 seconds till next line
                        DoScriptText(SAY_LINE_3, m_creature);
                        m_uiSpeachTimer = 10000;
                        ++m_uiSpeachNum;
                        break;
                    case 2:
                        // 8 seconds till next line
                        DoScriptText(SAY_LINE_4, m_creature);
                        m_uiSpeachTimer = 10000;
                        ++m_uiSpeachNum;
                        break;
                    case 3:
                        // 6,5 seconds till next line
                        DoScriptText(SAY_LINE_5, m_creature);
                        m_uiSpeachTimer = 6500;
                        ++m_uiSpeachNum;
                        break;
                    case 4:
                        // 2 seconds till next line
                        DoScriptText(SAY_LINE_6, m_creature);
                        m_uiSpeachTimer = 1000;
                        ++m_uiSpeachNum;
                        break;
                    case 5:
                        if (Creature* pMerok = m_creature->GetMap()->GetCreature(m_uiMerokGUID))
                            pMerok->GetMotionMaster()->MovePoint(0, MEROK_DEST_POS_X, MEROK_DEST_POS_Y, MEROK_DEST_POS_Z);

                        if (Creature* pShai = m_creature->GetMap()->GetCreature(m_uiShaiGUID))
                            pShai->GetMotionMaster()->MovePoint(0, SHAI_DEST_POS_X, SHAI_DEST_POS_Y, SHAI_DEST_POS_Z);
                        // 7 seconds till next line
                        DoScriptText(SAY_LINE_7, m_creature);
                        m_uiSpeachTimer = 6000;
                        ++m_uiSpeachNum;
                        break;
                    case 6:
                        DoScriptText(SAY_LINE_8, m_creature);
                        ++m_uiSpeachNum;
                        break;
                }
            }
            else
                m_uiSpeachTimer -= uiDiff;
        }

        /* Grunt */
        if (m_bGruntSpeech)
        {
            if (m_uiGruntSpeachTimer < uiDiff)
            {
                std::list<Creature*> gruntList;
                m_creature->GetCreatureListWithEntryInGrid(gruntList, NPC_ORGRIMMAR_LEGION_GRUNT, 100.0f);

                if (!gruntList.empty())
                {
                    for (std::list<Creature*>::iterator itr = gruntList.begin(); itr != gruntList.end(); ++itr)
                    {
                        if ((*itr)->isAlive())
                            DoScriptText(SAY_LINE_9, (*itr));
                    }
                }
                m_bGruntSpeech = false;
            }
            else
                m_uiGruntSpeachTimer -= uiDiff;
        }

        /* Move */
        if ((eEventStatus == EVENT_STARTED) && (waypoint <= HUNTERKILLER_WAYPOINTS_NUMBER))
        {
            if (m_uiMoveTimer < uiDiff)
            {
                if (Creature* pHunterKiller = m_creature->GetMap()->GetCreature(m_uiHunterKillerGUID))
                {
                    if (waypoint == HUNTERKILLER_WAYPOINTS_NUMBER)
                    {
                        pHunterKiller->setFaction(14);
                        pHunterKiller->AI()->AttackStart(m_creature);
                    }
                    else
                    {
                        float X = 0.0f;
                        float Y = 0.0f;
                        pHunterKiller->GetMotionMaster()->MovePoint(0, HunterKillerWaypoint[waypoint].X, HunterKillerWaypoint[waypoint].Y, HunterKillerWaypoint[waypoint].Z);
                        X = pHunterKiller->GetPositionX() - HunterKillerWaypoint[waypoint].X;
                        Y = pHunterKiller->GetPositionY() - HunterKillerWaypoint[waypoint].Y;
                        m_uiMoveTimer = sqrt((X * X) + (Y * Y)) / (m_creature->GetSpeed(MOVE_WALK) * 0.003f);
                    }
                    waypoint++;
                }
            }
            else
                m_uiMoveTimer -= uiDiff;
        }

        /* Clean unwanted states */
        if (Creature* pHunterKiller = m_creature->GetMap()->GetCreature(m_uiHunterKillerGUID))
        {
            if (eEventStatus == EVENT_NOT_STARTED)
            {
                pHunterKiller->ForcedDespawn();
                pHunterKiller->AddObjectToRemoveList();
            }
        }
        else
        {
            if (eEventStatus == EVENT_STARTED)
            {
                // m_creature->MonsterTextEmote("Clean2", NULL); // pour debug
                ResetEvent();
            }
        }

        /* Start of combat script */
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_npc_Krug_SkullSplit(Creature* pCreature)
{
    return new npc_Krug_SkullSplitAI(pCreature);
}


bool GossipHello_npc_Krug_SkullSplit(Player* pPlayer, Creature* pCreature)
{
    /* TO DO : la tous les SEND_GOSSIP_MENU renvoient le menu par defaut,
       or il faut mettre le gossip adapte a chaque cas */
    FieldDutyPaperEventStatus eEventStatus = EVENT_NOT_STARTED;

    if (npc_Krug_SkullSplitAI* pKrugSkullSplitAI = dynamic_cast<npc_Krug_SkullSplitAI*>(pCreature->AI()))
        eEventStatus = pKrugSkullSplitAI->GetEventStatus();

    if ((pPlayer->GetQuestStatus(QUEST_FIELD_DUTY) == QUEST_STATUS_INCOMPLETE)
            && (eEventStatus == EVENT_NOT_STARTED))
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_KRUG_SKULLSPLIT_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    }
    else if ((pPlayer->GetQuestStatus(QUEST_FIELD_DUTY) == QUEST_STATUS_INCOMPLETE)
             && (eEventStatus == EVENT_COMPLETE))
    {
        if (pCreature->isQuestGiver())
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());
        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    }
    else
        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}


bool GossipSelect_npc_Krug_SkullSplit(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF + 1)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_KRUG_SKULLSPLIT_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    }

    if (uiAction == GOSSIP_ACTION_INFO_DEF + 2)
    {
        if (npc_Krug_SkullSplitAI* pKrugSkullSplitAI = dynamic_cast<npc_Krug_SkullSplitAI*>(pCreature->AI()))
            pKrugSkullSplitAI->StartEvent();
        pPlayer->CLOSE_GOSSIP_MENU();
    }
    return true;
}

struct npc_MerokAI : public ScriptedAI
{
    npc_MerokAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiHealingWave;

    void Reset()
    {
        m_uiHealingWave = 12000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->IsNonMeleeSpellCasted(false))
            return;

        // HEALING WAVE
        if (m_uiHealingWave <= uiDiff)
        {
            Unit* pUnit = DoSelectLowestHpFriendly(60.0f, 1);
            if (!pUnit)
                return;

            if (DoCastSpellIfCan(pUnit, SPELL_HEALING_WAVE) == CAST_OK)
                m_uiHealingWave = 12000;
        }
        else
            m_uiHealingWave -= uiDiff;

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_npc_Merok(Creature* pCreature)
{
    return new npc_MerokAI(pCreature);
}


struct npc_ShaiAI : public ScriptedAI
{
    npc_ShaiAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiFlashHeal;

    void Reset()
    {
        m_uiFlashHeal = 12000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->IsNonMeleeSpellCasted(false))
            return;

        // FLASH HEAL
        if (m_uiFlashHeal <= uiDiff)
        {
            Unit* pUnit = DoSelectLowestHpFriendly(60.0f, 1);
            if (!pUnit)
                return;

            if (DoCastSpellIfCan(pUnit, SPELL_FLASH_HEAL) == CAST_OK)
                m_uiFlashHeal = 12000;
        }
        else
            m_uiFlashHeal -= uiDiff;

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_npc_Shai(Creature* pCreature)
{
    return new npc_ShaiAI(pCreature);
}


//=====================------------------------------------
//Alita <Nostalrius>
// PNJ tp AQ20, AQ40.
//====================
enum
{
    CHRIS = 56000 //notre pnj de tp.
};
bool GossipHello_npc_chris_starlightshadow(Player* pPlayer, Creature* pCreature)
{

    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Guide me to the Ruins of Ahn'Qiraj", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Guide me to the Temple of Ahn'Qiraj", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_chris_starlightshadow(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF + 1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        if (pPlayer->TeleportTo(509, -8429.74f, 1512.14f, 31.9074f, 2.56f))
            if (!pPlayer->isAlive())
            {
                pPlayer->ResurrectPlayer(0.5f, false);
                pPlayer->SpawnCorpseBones();
            }
    }
    else if (uiAction == GOSSIP_ACTION_INFO_DEF + 2)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        if (pPlayer->TeleportTo(531, -8231.330078f, 2010.6f, 129.33051f, 0.95f))
            if (!pPlayer->isAlive())
            {
                pPlayer->ResurrectPlayer(0.5f, false);
                pPlayer->SpawnCorpseBones();
            }
    }

    return true;
}

/** EVENT NOSTALRIUS VAM ,SAND PRINCE */

enum
{
    SPELL_CHARGE_VAM     = 26561,
    SPELL_IMPALE         = 28783,
    SPELL_ENRAGE         = 34624,
};

struct boss_vamAI : public ScriptedAI
{
    boss_vamAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 Charge_Timer;
    uint32 KnockBack_Timer;
    uint32 Enrage_Timer;

    bool Enraged;

    void Reset()
    {
        Charge_Timer = urand(15000, 27000);
        KnockBack_Timer = urand(8000, 20000);
        Enrage_Timer = 240000;

        Enraged = false;
    }

    void Aggro(Unit *who)
    {
    }

    void JustDied(Unit* Killer)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Charge_Timer
        if (Charge_Timer < diff)
        {
            Charge_Timer = 10000;
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(target, SPELL_CHARGE_VAM) == CAST_OK)
                    Charge_Timer = urand(8000, 16000);
            }
        }
        else Charge_Timer -= diff;

        //KnockBack_Timer
        if (KnockBack_Timer < diff)
        {
            KnockBack_Timer = 15000;
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(target, SPELL_IMPALE) == CAST_OK)
                    KnockBack_Timer = urand(15000, 25000);
            }
        }
        else KnockBack_Timer -= diff;

        //Enrage_Timer
        if (!Enraged && Enrage_Timer < diff)
            Enraged = true;
        else if (Enraged)
            DoCastSpellIfCan(m_creature, SPELL_ENRAGE, CAST_AURA_NOT_PRESENT);
        else
            Enrage_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_vamAI(Creature* pCreature)
{
    return new boss_vamAI(pCreature);
}


void AddSC_silithus()
{
    Script *pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_highlord_demitrian";
    pNewScript->pGossipHello =  &GossipHello_npc_highlord_demitrian;
    pNewScript->pGossipSelect = &GossipSelect_npc_highlord_demitrian;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npcs_rutgar_and_frankal";
    pNewScript->pGossipHello =  &GossipHello_npcs_rutgar_and_frankal;
    pNewScript->pGossipSelect = &GossipSelect_npcs_rutgar_and_frankal;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_pierre_vents";
    pNewScript->pGOGossipSelect =  &GossipSelect_go_pierre_vents;
    pNewScript->GOGetAI = &GetAIgo_pierre_vents;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_cenarion_scout_azenel";
    pNewScript->pGossipHello =  &GossipHello_npc_cenarion_scout_azenel;
    pNewScript->pGossipSelect = &GossipSelect_npc_cenarion_scout_azenel;
    pNewScript->RegisterSelf();


    /*########################
    ##      Nostalrius      ##
    ########################*/

    pNewScript = new Script;
    pNewScript->Name = "npc_prince_thunderaan";
    pNewScript->GetAI = &GetAI_npc_prince_thunderaan;
    pNewScript->RegisterSelf();

    // AQ WAR
    pNewScript = new Script;
    pNewScript->Name = "npc_colossus";
    pNewScript->GetAI = &GetAI_npc_colossus;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_resonating_Crystal";
    pNewScript->GetAI = &GetAI_npc_resonating_Crystal;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_AQwar_effort";
    pNewScript->pGossipHello = &GossipHello_npc_AQwar_effort;
    pNewScript->pGossipSelect = &GossipSelect_npc_AQwar_effort;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_AQwar_collector";
    pNewScript->pGossipHello = &GossipHello_npc_AQwar_collector;
    pNewScript->pQuestRewardedNPC = &QuestComplete_npc_AQwar_collector;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_Geologist_Larksbane";
    pNewScript->GetAI = &GetAI_npc_Geologist_Larksbane;
    pNewScript->pQuestRewardedNPC = &QuestComplete_npc_Geologist_Larksbane;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_Emissary_Romankhan";
    pNewScript->GetAI = &GetAI_npc_Emissary_Romankhan;
    pNewScript->RegisterSelf();


    // FIELD PAPERS - Ivina & Malkins
    pNewScript = new Script;
    pNewScript->Name = "npc_Krug_SkullSplit";
    pNewScript->GetAI = &GetAI_npc_Krug_SkullSplit;
    pNewScript->pGossipHello = &GossipHello_npc_Krug_SkullSplit;
    pNewScript->pGossipSelect = &GossipSelect_npc_Krug_SkullSplit;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_HiveRegal_HunterKiller";
    pNewScript->GetAI = &GetAI_mob_HiveRegal_HunterKiller;
    pNewScript->RegisterSelf();
    /*
    pNewScript = new Script;
    pNewScript->Name = "npc_Merok";
    pNewScript->GetAI = &GetAI_npc_Merok;
    pNewScript->RegisterSelf();
    */
    pNewScript = new Script;
    pNewScript->Name = "npc_Shai";
    pNewScript->GetAI = &GetAI_npc_Shai;
    pNewScript->RegisterSelf();

    //--Alita------
    pNewScript = new Script;
    pNewScript->Name = "npc_chris_starlightshadow";
    pNewScript->pGossipHello =  &GossipHello_npc_chris_starlightshadow;
    pNewScript->pGossipSelect = &GossipSelect_npc_chris_starlightshadow;
    pNewScript->RegisterSelf();
    //------------

    /** Event Nostalrius */
    pNewScript = new Script;
    pNewScript->Name = "npc_boss_vam";
    pNewScript->GetAI = &GetAI_boss_vamAI;
    pNewScript->RegisterSelf();

    // End Nostalrius

    pNewScript = new Script;
    pNewScript->Name = "npc_anachronos_the_ancient";
    pNewScript->GetAI = &GetAI_npc_anachronos_the_ancient;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_crystalline_tear";
    pNewScript->pGOQuestAccept = &QuestAcceptGO_crystalline_tear;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_solenor"; // npc_solenor_the_slayer
    pNewScript->GetAI = &GetAI_npc_solenor;
    pNewScript->pGossipHello =  &GossipHello_npc_solenor;
    pNewScript->pGossipSelect = &GossipSelect_npc_solenor;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_creeping_doom";
    pNewScript->GetAI = &GetAI_npc_creeping_doom;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_scarab_gong";
    pNewScript->GOGetAI = &GetAIscarab_gong;
    pNewScript->pQuestRewardedGO = &QuestRewarded_scarab_gong;
    pNewScript->pGOGossipHello = &GOHello_scarab_gong;
    pNewScript->RegisterSelf();
}
