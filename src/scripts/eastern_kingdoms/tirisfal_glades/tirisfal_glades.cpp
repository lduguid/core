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
SDName: Tirisfal_Glades
SD%Complete: 100
SDComment: Quest support: 590, 1819
SDCategory: Tirisfal Glades
EndScriptData */

/* ContentData
go_mausoleum_door
go_mausoleum_trigger
npc_calvin_montague
EndContentData */

#include "scriptPCH.h"

/*######
## go_mausoleum_door
## go_mausoleum_trigger
######*/

enum
{
    QUEST_ULAG      = 1819,
    NPC_ULAG        = 6390,
    GO_TRIGGER      = 104593,
    GO_DOOR         = 176594
};

bool GOHello_go_mausoleum_door(Player* pPlayer, GameObject* pGo)
{
    if (pPlayer->GetQuestStatus(QUEST_ULAG) != QUEST_STATUS_INCOMPLETE)
        return false;

    if (GameObject* pTrigger = GetClosestGameObjectWithEntry(pPlayer, GO_TRIGGER, 30.0f))
    {
        pTrigger->SetGoState(GO_STATE_READY);

        Creature* ulag = pPlayer->FindNearestCreature(NPC_ULAG, 100.0f);
        if (!ulag || ulag->isDead())
            pPlayer->SummonCreature(NPC_ULAG, 2390.26f, 336.47f, 40.01f, 2.26f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000);       
        return false;
    }

    return false;
}

bool GOHello_go_mausoleum_trigger(Player* pPlayer, GameObject* pGo)
{
    if (pPlayer->GetQuestStatus(QUEST_ULAG) != QUEST_STATUS_INCOMPLETE)
        return false;

    if (GameObject* pDoor = GetClosestGameObjectWithEntry(pPlayer, GO_DOOR, 30.0f))
    {
        pGo->SetGoState(GO_STATE_ACTIVE);
        pDoor->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
        return true;
    }

    return false;
}

/*######
## npc_calvin_montague
######*/

enum
{
    SAY_COMPLETE        = -1000356,
    SPELL_QUEST_DRINK   = 2639,                             // possibly not correct spell (but iconId is correct)
    QUEST_590           = 590,
    FACTION_HOSTILE     = 168
};

struct npc_calvin_montagueAI : public ScriptedAI
{
    npc_calvin_montagueAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_uiNormFaction = pCreature->getFaction();
        Reset();
    }

    uint32 m_uiNormFaction;
    uint32 m_uiPhase;
    uint32 m_uiPhaseTimer;
    uint64 m_uiPlayerGUID;

    void Reset()
    {
        m_uiPhase = 0;
        m_uiPhaseTimer = 1000;
        m_uiPlayerGUID = 0;

        if (m_creature->getFaction() != m_uiNormFaction)
            m_creature->setFaction(m_uiNormFaction);

        if (!m_creature->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER))
            m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
    }

    void AttackedBy(Unit* pAttacker)
    {
        if (m_creature->getVictim() || m_creature->IsFriendlyTo(pAttacker))
            return;

        AttackStart(pAttacker);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (uiDamage > m_creature->GetHealth() || ((m_creature->GetHealth() - uiDamage) * 100 / m_creature->GetMaxHealth() < 15))
        {
            uiDamage = 0;

            m_creature->setFaction(m_uiNormFaction);
            m_creature->CombatStop(true);

            m_uiPhase = 1;

            if (pDoneBy->GetTypeId() == TYPEID_PLAYER)
                m_uiPlayerGUID = pDoneBy->GetGUID();
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiPhase)
        {
            if (m_uiPhaseTimer < uiDiff)
            {
                switch (m_uiPhase)
                {
                    case 1:
                        m_creature->HandleEmoteCommand(EMOTE_ONESHOT_RUDE);
                        DoScriptText(SAY_COMPLETE, m_creature);
                        m_uiPhaseTimer = 4500;
                        ++m_uiPhase;
                        break;
                    case 2:
                        if (Unit* unit = m_creature->GetMap()->GetUnit(m_uiPlayerGUID))
                            if (Player* player = unit->ToPlayer())
                                player->AreaExploredOrEventHappens(QUEST_590);

                        m_creature->CastSpell(m_creature, SPELL_QUEST_DRINK, true);
                        m_uiPhaseTimer = 15000;
                        ++m_uiPhase;
                        break;
                    case 3:
                        m_creature->SetStandState(UNIT_STAND_STATE_STAND); //otherwise he is sitting until server restart
                        EnterEvadeMode();
                        break;
                }
            }
            else
                m_uiPhaseTimer -= uiDiff;

            return;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_calvin_montague(Creature* pCreature)
{
    return new npc_calvin_montagueAI(pCreature);
}

bool QuestAccept_npc_calvin_montague(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_590)
    {
        pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
        pCreature->setFaction(FACTION_HOSTILE);
        pCreature->AI()->AttackStart(pPlayer);
    }
    return true;
}

void AddSC_tirisfal_glades()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "go_mausoleum_door";
    newscript->pGOHello = &GOHello_go_mausoleum_door;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_mausoleum_trigger";
    newscript->pGOHello = &GOHello_go_mausoleum_trigger;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_calvin_montague";
    newscript->GetAI = &GetAI_npc_calvin_montague;
    newscript->pQuestAcceptNPC = &QuestAccept_npc_calvin_montague;
    newscript->RegisterSelf();
}
