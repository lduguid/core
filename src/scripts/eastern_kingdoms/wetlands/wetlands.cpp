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
SDName: Wetlands
SD%Complete: 100
SDComment: Quest support: 1249
SDCategory: Wetlands
EndScriptData */

/* ContentData
npc_mikhail
npc_tapoke_slim_jahn
EndContentData */

#include "scriptPCH.h"

//-----------------------------------------------------------------------------
// Full quest event implementation (Missing Diplomat part 11 id:1249).
// Author: Kampeador
//-----------------------------------------------------------------------------

// On elysium core, JustRespawned() isn't called on spawn. Only on respawn.
// if it is safe to reduce a hardcoded 2500ms delay before the first waypoint to 750ms-1000ms,
// then this and setDelayBeforeTheFirstWaypoint(750) function can be removed.
#define ELYSIUM_CORE_JUST_RESPAWNED

enum
{
    // ids from "script_texts" table
    SAY_PROGRESS_1_TAP = -1999906, // Oh, it's on now! But you thought I'd be alone too, huh?!
    SAY_PROGRESS_2_FRI = -1999907, // Whoa! This is way more than what I bargained for, you're on your own, Slim!
    SAY_PROGRESS_3_TAP = -1999908, // Okay, okay! No need to get all violent. I'll talk. I'll talk!
    SAY_PROGRESS_4_TAP = -1999909, // I have a few notes from the job back at my place. I'll get them and then meet you back in the inn.
    SAY_PROGRESS_5_MIC = -1999910, // I'm glad the commotions died down some around here. The last thing this place needs is another brawl.
    // quest id
    QUEST_MISSING_DIPLOMAT_PART11 = 1249,
    // factions
    FACTION_FRIENDLY = 11, // taken from DB
    FACTION_FRIENDLY_TO_ALL = 35,
    // Blizzlike number will probably be 34, but there are some collisions in current db with other NPCs in this area,
    // they will attack Tapoke Slim Jahn during the event.
    FACTION_NEUTRAL = 189,
    // spells used by Tapoke "Slim: Jahn
    SPELL_STEALTH = 6634, // used during escape from the inn.
    SPELL_CALL_FRIENDS = 16457, // summons 1x friend
    SPELL_PUMMEL = 12555, // used to interrupt enemy spells.
    // spells used by Slim's Friend
    SPELL_SLOWING_POISON = 7992,
    SPELL_POISON_PROC = 3616, // Aura used on spawn, which procs spell id:744.
    SPELL_BACKSTAB = 15582,//15657, // or 15582? not sure which one.
    // NPC ids
    NPC_MIKHAIL = 4963, // Quest giver, starts an event
    NPC_SLIMS_FRIEND = 4971, // NPC that helps Tapoke Slim
    NPC_TAPOKE_SLIM_JAHN = 4962,
    // Waypoint Ids
    WAYPOINT_MAILBOX = 3,
    WAYPOINT_GATE = 9
};
//-----------------------------------------------------------------------------
// Slim's Friend (guardian AI)
class npc_slims_friendAI : public ScriptedAI
{
public:
    npc_slims_friendAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    void Reset()
    {
        m_slowingPoisonTimer = urand(5000, 8900);
        m_backstabTimer = 500;
        m_creature->CastSpell(m_creature, SPELL_POISON_PROC, false);
    }

    void AttackStart(Unit* pWho)
    {
        if (pWho && m_creature->Attack(pWho, true))
            m_creature->GetMotionMaster()->MoveChase(pWho);
    }

    void AttackedBy(Unit* pAttacker)
    {
        if (!pAttacker || m_creature->getVictim())
            return;

        if (m_creature->GetCharmInfo() && m_creature->CanReachWithMeleeAttack(pAttacker))
            AttackStart(pAttacker);
    }

    void UpdateCombatAI(const uint32 uiDiff)
    {
        // slowing poison timer
        if (m_slowingPoisonTimer < uiDiff)
        {
            CanCastResult castResult = DoCastSpellIfCan(m_creature->getVictim(), SPELL_SLOWING_POISON, CAST_AURA_NOT_PRESENT);
            if (castResult == CAST_OK)
                m_slowingPoisonTimer = urand(8400, 15300);
        }
        else
            m_slowingPoisonTimer -= uiDiff;

        // backstab timer
        if (m_backstabTimer < uiDiff)
        {
            CanCastResult castResult = DoCastSpellIfCan(m_creature->getVictim(), SPELL_BACKSTAB);
            if (castResult == CAST_OK)
                m_backstabTimer = urand(2100, 5600);
        }
        else
            m_backstabTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }


    void UpdateAI(const uint32 uiDiff) override
    {
        Unit* pTarget = m_creature->getVictim();
        if (pTarget) // in combat
        {
            // update when in combat
            UpdateCombatAI(uiDiff);
        }
        else if (m_creature->GetCharmInfo())
        {
            Unit* pOwner = m_creature->GetCharmerOrOwner();

            if (!pOwner)
                return;

            if (pOwner->isInCombat())
            {
                Unit* pTarget = pOwner->getAttackerForHelper();
                if (pTarget)
                {
                    AttackStart(pTarget);
                }
            }
            else // not in combat
            {
                // if not following, start follow
                if (!m_creature->hasUnitState(UNIT_STAT_FOLLOW))
                    m_creature->GetMotionMaster()->MoveFollow(pOwner, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
            }
        }
    }

private:
    // slowing poison spell timer
    uint32 m_slowingPoisonTimer;
    // backstab spell timer
    uint32 m_backstabTimer;
};

CreatureAI* GetAI_npc_slims_friend(Creature* pCreature)
{
    return new npc_slims_friendAI(pCreature);
}
//-----------------------------------------------------------------------------
// Tapoke "Slim: Jahn AI
struct npc_tapoke_slim_jahnAI : public npc_escortAI
{
    // flag defines, whether players have won the fight
    bool m_isBeaten;
    // event timmers
    uint32 m_nextPhaseDelay;
    // missing diplomat quest event phase index
    uint32 m_mdDialogPhase;
    // Slim's friend
    Creature* m_slimsFriend;
    // pummel spell timer
    uint32 m_pummelTimer;
    // respawn delay from DB
    uint32 m_respawnDelay;

#ifdef ELYSIUM_CORE_JUST_RESPAWNED  
    bool m_justCreated;
#endif

    npc_tapoke_slim_jahnAI(Creature* pCreature) : npc_escortAI(pCreature)
    {
        // disable pathfinding between waypoints to prevent wrong behavior.
        SetPathfindingEnabledBetweenWaypoints(false);
        // set delay before the first waypoint (index: 0).
        // It will get updated only after JustRespawned() call!
        SetDelayBeforeTheFirstWaypoint(750);

        // backup the original respawn delay
        m_respawnDelay = m_creature->GetRespawnDelay();

        m_slimsFriend = nullptr;

#ifdef ELYSIUM_CORE_JUST_RESPAWNED
        m_justCreated = true;
#endif

        Reset();
    }

    void Reset()
    {
        m_nextPhaseDelay = 0;
        m_mdDialogPhase = 0;
        m_isBeaten = false;
        m_pummelTimer = urand(1400, 2700);
    }

    // This function is also called when NPC runs away from player/group range.
    void JustDied(Unit*) override
    {
        if (HasEscortState(STATE_ESCORT_ESCORTING))
        {
            // Quest will also fail if Tapoke "Slim" Jahn ran away from player / group range.
            Player* player = GetPlayerForEscort();
            if (player)
                player->GroupEventFailHappens(QUEST_MISSING_DIPLOMAT_PART11);

            // remove slim's friend
            if (m_slimsFriend)
            {
                static_cast<TemporarySummon*>(m_slimsFriend)->UnSummon();
                m_slimsFriend = nullptr;
            }
        }
    }

    void JustRespawned()
    {
        npc_escortAI::JustRespawned(); // calls Reset()

        // restore original respawn delay.
        m_creature->SetRespawnDelay(m_respawnDelay);
        // restore faction, which usually getting restored automatically, but in rare cases it still can fail.
        m_creature->setFaction(FACTION_FRIENDLY);
        // "announce" that Tapoke Slim Jahn is back and event is ready to start.
        // distance between Mikhail and Tapoke "Slim" Jahn is about 16 yards, 20 used for "safety".
        Creature* npcMikhail = GetClosestCreatureWithEntry(m_creature, NPC_MIKHAIL, 20.0f);
        // if Mikhail is alive and in range
        if (npcMikhail)
            DoScriptText(SAY_PROGRESS_5_MIC, npcMikhail);
    }

    void WaypointReached(uint32 uiPointId)
    {
        switch (uiPointId)
        {
        case WAYPOINT_MAILBOX:
        {
            SetRun();
            // change faction, which makes him attackable.
            m_creature->setFaction(FACTION_NEUTRAL);
        }break;
        case WAYPOINT_GATE:
        {
            // set quest failed if tapoke slim escaped
            Player* player = GetPlayerForEscort();
            if (player)
                player->GroupEventFailHappens(QUEST_MISSING_DIPLOMAT_PART11);

            m_creature->ForcedDespawn(5000);
        }break;
        }
    }

    void Aggro(Unit* pWho)
    {
        if (HasEscortState(STATE_ESCORT_ESCORTING))
        {
            // This function is also called when Tapoke Slim Jahn has been defeated!
            if (!m_slimsFriend)
            {
                // calls a friend
                CanCastResult castResult = DoCastSpellIfCan(m_creature, SPELL_CALL_FRIENDS);
                if (castResult == CAST_OK)
                {
                    DoScriptText(SAY_PROGRESS_1_TAP, m_creature);
                }
            }
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        m_slimsFriend = pSummoned;
    }

    void AttackedBy(Unit* pAttacker)
    {
        if (!pAttacker || m_creature->getVictim() || m_creature->IsFriendlyTo(pAttacker))
            return;

        AttackStart(pAttacker);
    }

    void UpdateEscortAI(const uint32 uiDiff)
    {
#ifdef ELYSIUM_CORE_JUST_RESPAWNED
        if (m_justCreated)
        {
            m_justCreated = false;
            JustRespawned();
        }
#endif
        if (m_isBeaten)
        {
            if (m_nextPhaseDelay < uiDiff)
            {
                switch (m_mdDialogPhase)
                {
                case 0:
                {
                    // Set Tapoke Tapoke Slim Jahn and his friend facing to player character.
                    // An better option can be to move this to DamageTaken(), but it will not work.
                    // It's probably because Aggro() is being called after DamageTaken().
                    Player* player = GetPlayerForEscort();
                    if (player)
                    {
                        m_creature->SetFacingToObject(player);

                        if (m_slimsFriend)
                        {
                            if (m_slimsFriend->isAlive())
                                m_slimsFriend->SetFacingToObject(player);
                        }

                    }
                    m_nextPhaseDelay = 2000;
                }break;
                case 1: // Say_0
                {
                    // despawn Slims friend
                    if (m_slimsFriend)
                    {
                        static_cast<TemporarySummon*>(m_slimsFriend)->UnSummon();
                        m_slimsFriend = nullptr;
                    }

                    m_creature->HandleEmote(EMOTE_ONESHOT_BEG);
                    DoScriptText(SAY_PROGRESS_3_TAP, m_creature);
                    m_nextPhaseDelay = 4000;
                }break;
                case 2: // Say_1
                {
                    m_creature->HandleEmote(EMOTE_ONESHOT_TALK);
                    DoScriptText(SAY_PROGRESS_4_TAP, m_creature);
                    m_nextPhaseDelay = 6000;
                }break;
                case 3: // The End
                {
                    Player* player = GetPlayerForEscort();
                    if (player)
                        player->GroupEventHappens(QUEST_MISSING_DIPLOMAT_PART11, m_creature);

                    // make an illusion returning him back to the inn
                    Stop();

                    // despawn and respawn at inn
                    m_creature->ForcedDespawn(1000);
                    // respawn in 2 seconds.
                    m_creature->SetRespawnDelay(2);

                    m_nextPhaseDelay = 0;
                    m_mdDialogPhase = 0;
                    m_isBeaten = false;
                }break;
                }
                // move to the next phase
                ++m_mdDialogPhase;
            }
            else
                m_nextPhaseDelay -= uiDiff;
        }
        else
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

            // Pummel timer
            if (m_pummelTimer < uiDiff)
            {
                CanCastResult castResult = DoCastSpellIfCan(m_creature->getVictim(), SPELL_PUMMEL);
                if (castResult == CAST_OK)
                    m_pummelTimer = urand(7300, 15000);
            }
            else
                m_pummelTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    }

    void JustStartedEscort()
    {
        // Once event starts, NPC will use his Stealth spell.
        m_creature->CastSpell(m_creature, SPELL_STEALTH, false);
    }

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
    {
        if (!HasEscortState(STATE_ESCORT_ESCORTING))
            return;

        if (uiDamage > m_creature->GetHealth() || m_creature->GetHealthPercent() < 20.0f)
        {
            uiDamage = 0;

            m_isBeaten = true;

            if (m_slimsFriend)
            {
                if (m_slimsFriend->isAlive())
                {
                    // stop combat
                    m_slimsFriend->CombatStop(true);
                    // remove all dots, etc
                    m_slimsFriend->RemoveAllAuras();
                    // clean thread list
                    m_slimsFriend->DeleteThreatList();
                    // make him unattackable
                    m_slimsFriend->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_PASSIVE);

                    DoScriptText(SAY_PROGRESS_2_FRI, m_slimsFriend);
                }
            }

            // pause escort
            SetEscortPaused(true);

            // prevent an inconsistent behavior
            m_creature->setFaction(FACTION_FRIENDLY_TO_ALL);
            // remove all dots, etc
            m_creature->RemoveAllAuras();
            // clean thread list
            m_creature->DeleteThreatList();
            // stop combat
            m_creature->CombatStop(true);

            SetRun(false);
        }
    }
};

CreatureAI* GetAI_npc_tapoke_slim_jahn(Creature* pCreature)
{
    return new npc_tapoke_slim_jahnAI(pCreature);
}
//-----------------------------------------------------------------------------
// Mikhail gossip scripts
bool QuestAccept_npc_mikhail(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (!pPlayer || !pCreature || !pQuest)
        return false;

    if (pQuest->GetQuestId() == QUEST_MISSING_DIPLOMAT_PART11)
    {
        // distance between Mikhail and Tapoke "Slim" Jahn is about 16 yards, 20 used for "safety".
        Creature* pSlim = GetClosestCreatureWithEntry(pCreature, NPC_TAPOKE_SLIM_JAHN, 20.0f);
        if (!pSlim)
        {
            // Rare case: This will happen, when Tapoke "Slim" Jahn become not available, during reading a quest text.
            return false;
        }

        npc_tapoke_slim_jahnAI* tapokeSlimJahnAI = dynamic_cast<npc_tapoke_slim_jahnAI*>(pSlim->AI());
        if (tapokeSlimJahnAI)
        {
            // start escort
            tapokeSlimJahnAI->Start(false, pPlayer->GetGUID(), pQuest);
        }
        else
            return false;
    }
    return true;
}
//-----------------------------------------------------------------------------
bool GossipHello_npc_mikhail(Player* pPlayer, Creature* pCreature)
{
    // if player has completed a previous The Missing Diplomat part 10
    if (pPlayer->GetQuestStatus(1248) == QUEST_STATUS_COMPLETE)
    {
        // if player has not completed The Missing Diplomat part 11 and doesn't have it in a quest log
        QuestStatus mdPT11 = pPlayer->GetQuestStatus(QUEST_MISSING_DIPLOMAT_PART11);
        if (mdPT11 == QUEST_STATUS_NONE)
        {
            // check if a quest event can be started
            // If Tapoke Slim Jahn is alive and in range.
            Creature* pSlim = GetClosestCreatureWithEntry(pCreature, NPC_TAPOKE_SLIM_JAHN, 20.0f);
            if (!pSlim)
            {
                // This case will happen, if player from the opposite faction will pull Tapoke "Slim" Jahn from the inn,
                // while the other player will try to start the event.
                // id: 1713 was taken from the npc_text table.
                pPlayer->SEND_GOSSIP_MENU(1713, pCreature->GetGUID());
            }
            else
            {
                npc_tapoke_slim_jahnAI* tapokeSlimJahnAI = dynamic_cast<npc_tapoke_slim_jahnAI*>(pSlim->AI());
                if (tapokeSlimJahnAI)
                {
                    // if event has already started.
                    if (tapokeSlimJahnAI->HasEscortState(STATE_ESCORT_ESCORTING))
                    {
                        // id: 1713 was taken from the npc_text table.
                        pPlayer->SEND_GOSSIP_MENU(1713, pCreature->GetGUID());
                    }
                    else
                    {
                        // event is ready to start, send quest menu
                        pPlayer->PrepareQuestMenu(pCreature->GetGUID());
                        pPlayer->SendPreparedQuest(pCreature->GetGUID());
                    }
                }
                else
                {
                    // something weird happened.
                    pPlayer->SEND_GOSSIP_MENU(1713, pCreature->GetGUID());
                }
            }
        }
        else
        {
            // show quest menu as usual
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());
            pPlayer->SendPreparedQuest(pCreature->GetGUID());
        }
    }
    else
    {
        // show quest menu as usual
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());
        pPlayer->SendPreparedQuest(pCreature->GetGUID());
    }

    return true;
}
//-----------------------------------------------------------------------------
// AddSC
//-----------------------------------------------------------------------------
void AddSC_wetlands()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_slims_friend";
    newscript->GetAI = &GetAI_npc_slims_friend;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_tapoke_slim_jahn";
    newscript->GetAI = &GetAI_npc_tapoke_slim_jahn;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_mikhail";
    newscript->pQuestAcceptNPC = &QuestAccept_npc_mikhail;
    newscript->pGossipHello = &GossipHello_npc_mikhail;
    newscript->RegisterSelf();
}
