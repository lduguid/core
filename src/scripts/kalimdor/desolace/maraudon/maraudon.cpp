/*
 *
 */

#include "scriptPCH.h"

 /*
 * Primordial Behemoth
 */

enum
{
    SPELL_BOULDER = 21071,
    SPELL_TRAMPLE = 5568
};

struct npc_primordial_behemothAI : ScriptedAI
{
    explicit npc_primordial_behemothAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        npc_primordial_behemothAI::Reset();
    }

    uint32 m_uiBoulderTimer;
    uint32 m_uiTrampleTimer;

    void Reset() override
    {
        m_uiBoulderTimer = urand(50, 150);
        m_uiTrampleTimer = urand(50, 150);
        m_creature->SetCasterChaseDistance(25.0f);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        auto pMainTarget = m_creature->getVictim();

        if (pMainTarget->IsWithinMeleeRange(m_creature) && pMainTarget->IsWithinLOSInMap(m_creature))
        {
            m_creature->SetCasterChaseDistance(0.0f);

            DoMeleeAttackIfReady();

            if (m_uiTrampleTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_TRAMPLE) == CAST_OK)
                    m_uiTrampleTimer = urand(12000, 15000);
            }
            else
                m_uiTrampleTimer -= uiDiff;

            return;
        }

        if (!pMainTarget->IsWithinDistInMap(m_creature, 20.0f))
        {
            m_creature->SetCasterChaseDistance(25.0f);

            // cast Boulder if all targets are far away
            if (m_uiBoulderTimer < uiDiff)
            {
                if (DoCastSpellIfCan(pMainTarget, SPELL_BOULDER) == CAST_OK)
                    m_uiBoulderTimer = urand(2000, 2500);
            }
            else
                m_uiBoulderTimer -= uiDiff;
        }
        else
            m_creature->SetCasterChaseDistance(0.0f);
    }
};

CreatureAI* GetAI_npc_primordial_behemoth(Creature* pCreature)
{
    return new npc_primordial_behemothAI(pCreature);
}

void AddSC_maraudon()
{
    //Script* pNewScript;

    //pNewScript = new Script;
    //pNewScript->Name = "npc_primordial_behemoth";
    //pNewScript->GetAI = &GetAI_npc_primordial_behemoth;
    //pNewScript->RegisterSelf();
}
