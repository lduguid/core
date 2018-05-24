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
SDName: mob_anubisath_sentinel
SD%Complete: 95
SDComment: Shadow storm is not properly implemented in core it should only target ppl outside of melee range.
SDCategory: Temple of Ahn'Qiraj
EndScriptData */

#include "scriptPCH.h"

#define SPELL_MENDING_BUFF      2147

#define SPELL_KNOCK_BUFF        21737
#define SPELL_KNOCK             23382
#define SPELL_MANAB_BUFF        812
#define SPELL_MANAB             25779

#define SPELL_REFLECTAF_BUFF    13022
#define SPELL_REFLECTSFr_BUFF   19595
#define SPELL_THORNS_BUFF       25777

#define SPELL_THUNDER_BUFF      2834
#define SPELL_THUNDER           23931

#define SPELL_MSTRIKE_BUFF      9347
#define SPELL_MSTRIKE           24573

#define SPELL_STORM_BUFF        2148
#define SPELL_STORM             26546

#define SPELL_ENRAGE            24318
#define EMOTE_ENRAGE            -1000003

#define SPELL_TRANSFER          2400
#define EMOTE_TRANSFER          -1388101

struct aqsentinelAI;
class SentinelAbilityAura : public Aura
{
public:
    ~SentinelAbilityAura();
    Unit* GetTriggerTarget() const;
    SentinelAbilityAura(aqsentinelAI *abilityOwner, SpellEntry *spell, uint32 ability, SpellEffectIndex eff);
protected:
    aqsentinelAI *aOwner;
    int32 currentBasePoints;
    uint32 abilityId;
};

struct aqsentinelAI : public ScriptedAI
{
    uint32 ability;
    int abselected;
    uint32 m_uiKnock_Timer;
    uint32 m_uiDrainMana_Timer;
    uint8 m_uiDrainCount;
    std::vector<ObjectGuid> _playerList;
    bool m_bEnraged;
    bool m_bAlone;
    bool gatherOthersWhenAggro;
    Creature *nearby[3];

    void selectAbility(int asel)
    {
        switch (asel)
        {
        case 0:
            ability = SPELL_MENDING_BUFF;
            break;
        case 1:
            ability = SPELL_MSTRIKE_BUFF;
            break;
        case 2:
            ability = SPELL_STORM_BUFF;
            break;
        case 3:
            ability = SPELL_REFLECTAF_BUFF;
            break;
        case 4:
            ability = SPELL_REFLECTSFr_BUFF;
            break;
        case 5:
            ability = SPELL_THORNS_BUFF;
            break;
        case 6:
            ability = SPELL_THUNDER_BUFF;
            break;
        case 7:
            ability = SPELL_KNOCK_BUFF;
            break;
        case 8:
            ability = SPELL_MANAB_BUFF;
            break;
        }
    }

    aqsentinelAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        ClearBudyList();
        abselected = 0;                                     // just initialization of variable
        m_bEnraged = false;
        Reset();
    }

    void MoveInLineOfSight(Unit* pWho) override
    {
        // Increase aggro radius
        if (pWho->GetTypeId() == TYPEID_PLAYER && !m_creature->isInCombat() && m_creature->IsWithinDistInMap(pWho, 45.0f) && m_creature->IsWithinLOSInMap(pWho) && !pWho->HasAuraType(SPELL_AURA_FEIGN_DEATH))
        {
            AttackStart(pWho);
        }
        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void ClearBudyList()
    {
        nearby[0] = nearby[1] = nearby[2] = NULL;
    }

    void AddBuddyToList(Creature *c)
    {
        if (c == m_creature)
            return;
        for (int i = 0; i < 3; ++i)
        {
            if (nearby[i] == c)
                return;
            if (!nearby[i])
            {
                nearby[i] = c;
                return;
            }
        }
    }

    void GiveBuddyMyList(Creature *c)
    {
        aqsentinelAI *cai = (aqsentinelAI *)(c->AI());
        for (int i = 0; i < 3; ++i)
            if (nearby[i] && nearby[i] != c)
                cai->AddBuddyToList(nearby[i]);
        cai->AddBuddyToList(m_creature);
    }

    void SendMyListToBuddies()
    {
        for (int i = 0; i < 3; ++i)
            if (nearby[i])
                GiveBuddyMyList(nearby[i]);
    }

    void CallBuddiesToAttack(Unit *who)
    {
        for (int i = 0; i < 3; ++i)
        {
            Creature *c = nearby[i];
            if (c)
            {
                if (!c->isInCombat())
                {
                    c->SetNoCallAssistance(true);
                    if (c->AI())
                        c->AI()->AttackStart(who);
                }
            }
        }
    }

    void AddSentinelsNear(Unit *nears)
    {
        std::list<Creature*> assistList;
        GetCreatureListWithEntryInGrid(assistList, m_creature, 15264, 90.0f);

        if (assistList.empty())
            return;

        for (std::list<Creature*>::iterator iter = assistList.begin(); iter != assistList.end(); ++iter)
            AddBuddyToList((*iter));
    }

    int pickAbilityRandom(bool *chosenAbilities)
    {
        for (int t = 0; t < 2; ++t)
        {
            // should be (rand() % 9) : 0; i < 9;
            // temporarily precluding sentinels from rolling semi-broken mana burn
            for (int i = !t ? (rand() % 8) : 0; i < 8; ++i)
            {
                if (!chosenAbilities[i])
                {
                    chosenAbilities[i] = true;
                    return i;
                }
            }
        }
        return 0;                                           // should never happen
    }

    void GetOtherSentinels(Unit *who)
    {
        bool *chosenAbilities = new bool[9];
        memset(chosenAbilities, 0, 9 * sizeof(bool));
        selectAbility(pickAbilityRandom(chosenAbilities));

        ClearBudyList();
        AddSentinelsNear(m_creature);
        int bli;
        for (bli = 0; bli < 3; ++bli)
        {
            if (!nearby[bli])
                break;
            AddSentinelsNear(nearby[bli]);
            ((aqsentinelAI *)nearby[bli]->AI())->gatherOthersWhenAggro = false;
            ((aqsentinelAI *)nearby[bli]->AI())->selectAbility(pickAbilityRandom(chosenAbilities));
        }
        /*if (bli < 3)
            DoYell("I dont have enough buddies.", LANG_NEUTRAL, 0);*/
        SendMyListToBuddies();
        CallBuddiesToAttack(who);

        delete[] chosenAbilities;
    }

    void Reset()
    {
        if (!m_creature->isDead())
        {
            for (int i = 0; i < 3; ++i)
            {
                if (!nearby[i])
                    continue;
                if (nearby[i]->isDead())
                    nearby[i]->Respawn();
            }
        }
        ClearBudyList();
        gatherOthersWhenAggro = true;
        m_uiKnock_Timer = 13000;
        m_uiDrainMana_Timer = 11000;
        m_bEnraged = 0;
        m_uiDrainCount = 0;
    }

    void GainSentinelAbility(uint32 id)
    {
        m_creature->AddAura(id);
    }

    void cast_knock()
    {
        if (Unit* victim = m_creature->getVictim())
            m_creature->CastSpell(victim, SPELL_KNOCK, false);
    }

    // Threat reduction for Knock Away
    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell) override
    {
        if ((pSpell->Id == SPELL_KNOCK) && pTarget->GetTypeId() == TYPEID_PLAYER)
        {
            if (m_creature->getThreatManager().getThreat(m_creature->getVictim()))
                m_creature->getThreatManager().modifyThreatPercent(m_creature->getVictim(), -20);
        }
    }

    void Aggro(Unit* pWho)
    {
        if (gatherOthersWhenAggro)
            GetOtherSentinels(pWho);

        GainSentinelAbility(ability);

        m_creature->SetInCombatWithZone();
    }

    void FillPlayerList()
    {
        Map::PlayerList const &liste = m_creature->GetMap()->GetPlayers();
        for (Map::PlayerList::const_iterator i = liste.begin(); i != liste.end(); ++i)
        {
            if (Player* player = i->getSource())
                if (player->isAlive() && player->getPowerType() == POWER_MANA)
                    _playerList.push_back(player->GetObjectGuid());
        }
    }

    // Transfer powers on death
    void JustDied(Unit*)
    {
        m_bAlone = true;
        for (int ni = 0; ni < 3; ++ni)
        {
            Creature *sent = nearby[ni];
            if (!sent)
                continue;
            if (sent->isDead())
                continue;
            m_bAlone = false;
            DoCastSpellIfCan(sent, SPELL_TRANSFER, CAST_TRIGGERED);
            uint32 h = sent->GetHealth() + (sent->GetMaxHealth() / 2);
            if (h > sent->GetMaxHealth())
                h = sent->GetMaxHealth();
            sent->SetHealth(h);
            ((aqsentinelAI *)sent->AI())->GainSentinelAbility(ability);
        }
        if (!m_bAlone)
            DoScriptText(EMOTE_TRANSFER, m_creature);
    }

    Unit *GetHatedManaUser()
    {
        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator i = tList.begin(); i != tList.end(); ++i)
        {
            Unit* pUnit = m_creature->GetMap()->GetUnit((*i)->getUnitGuid());
            if (pUnit->getPowerType() == POWER_MANA)
                return pUnit;
        }
        return NULL;
    }
    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (ability == SPELL_KNOCK_BUFF)
        {
            if (m_uiKnock_Timer < uiDiff)
            {
                //cast_knock();
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_KNOCK);
                m_uiKnock_Timer = 13000;
            }
            else
                m_uiKnock_Timer -= uiDiff;

        }

        if (ability == SPELL_MANAB_BUFF)
        {
            if (m_uiDrainMana_Timer < uiDiff)
            {
                if (!m_uiDrainCount)
                    FillPlayerList();
                if (!_playerList.empty() && m_uiDrainCount < 6)
                {
                    uint32 Rand = urand(0, _playerList.size() - 1);
                    if (Player* Plr = m_creature->GetMap()->GetPlayer(_playerList[Rand]))
                    {
                        if (Plr->isAlive())
                        {
                            m_uiDrainCount++;
                            uint32 Mana = Plr->GetPower(POWER_MANA);
                            if (Mana > 788)
                            {
                                Plr->SetPower(POWER_MANA, Plr->GetPower(POWER_MANA) - 788);
                                Mana = 788;
                            }
                            else
                                Plr->SetPower(POWER_MANA, 0);
                            Mana *= 2;
                            if (m_creature->GetPower(POWER_MANA) + Mana < m_creature->GetMaxPower(POWER_MANA))
                                m_creature->SetPower(POWER_MANA, m_creature->GetPower(POWER_MANA) + Mana);
                            else if (m_creature->GetPower(POWER_MANA) < m_creature->GetMaxPower(POWER_MANA))
                                m_creature->SetPower(POWER_MANA, m_creature->GetMaxPower(POWER_MANA));
                            Plr->CastSpell(m_creature, SPELL_MANAB, true);
                        }
                    }
                    _playerList.erase(_playerList.begin() + Rand);
                }
                else
                {
                    _playerList.clear();
                    m_uiDrainCount = 0;
                    m_uiDrainMana_Timer = 11000;
                }
            }
            else
                m_uiDrainMana_Timer -= uiDiff;
        }

        if (!m_bEnraged && m_creature->GetHealthPercent() < 30.0f)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_ENRAGE) == CAST_OK)
            {
                DoScriptText(EMOTE_ENRAGE, m_creature);
                m_bEnraged = true;
            }
        }

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_mob_anubisath_sentinelAI(Creature* pCreature)
{
    return new aqsentinelAI(pCreature);
}

void AddSC_mob_anubisath_sentinel()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "mob_anubisath_sentinel";
    newscript->GetAI = &GetAI_mob_anubisath_sentinelAI;
    newscript->RegisterSelf();
}

SentinelAbilityAura::~SentinelAbilityAura() {}
Unit* SentinelAbilityAura::GetTriggerTarget() const
{
    switch (abilityId)
    {
    case SPELL_KNOCK_BUFF:
    case SPELL_THUNDER_BUFF:
    case SPELL_MSTRIKE_BUFF:
    case SPELL_STORM_BUFF:
        return aOwner->m_creature->getVictim();

    case SPELL_MANAB_BUFF:
        return aOwner->GetHatedManaUser();
    case SPELL_MENDING_BUFF:
    case SPELL_REFLECTAF_BUFF:
    case SPELL_REFLECTSFr_BUFF:
    case SPELL_THORNS_BUFF:
    default:
        return aOwner->m_creature;
    }
}

/*SentinelAbilityAura::SentinelAbilityAura(aqsentinelAI *abilityOwner, SpellEntry *spell, uint32 ability, SpellEffectIndex eff)
: Aura(spell, eff, NULL, abilityOwner->m_creature, abilityOwner->m_creature, NULL)
{
    aOwner = abilityOwner;
    abilityId = ability;
    currentBasePoints = 0;
}*/
