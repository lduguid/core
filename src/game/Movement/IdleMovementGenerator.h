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

#ifndef MANGOS_IDLEMOVEMENTGENERATOR_H
#define MANGOS_IDLEMOVEMENTGENERATOR_H

#include "MovementGenerator.h"

class MANGOS_DLL_SPEC IdleMovementGenerator : public MovementGenerator
{
    public:

        void Initialize(Unit &) {}
        void Finalize(Unit &) {}
        void Interrupt(Unit &) {}
        void Reset(Unit &);
        bool Update(Unit &, const uint32 &) { return true; }
        MovementGeneratorType GetMovementGeneratorType() const { return IDLE_MOTION_TYPE; }
};

extern IdleMovementGenerator si_idleMovement;

class MANGOS_DLL_SPEC DistractMovementGenerator : public MovementGenerator
{
    public:
        explicit DistractMovementGenerator(uint32 timer) : m_timer(timer) {}

        void Initialize(Unit& owner);
        void Finalize(Unit& owner);
        void Interrupt(Unit& );
        void Reset(Unit& );
        bool Update(Unit& owner, const uint32& time_diff);
        MovementGeneratorType GetMovementGeneratorType() const { return DISTRACT_MOTION_TYPE; }

    private:
        uint32 m_timer;
};

class MANGOS_DLL_SPEC AssistanceDistractMovementGenerator : public DistractMovementGenerator
{
    public:
        AssistanceDistractMovementGenerator(uint32 timer) :
            DistractMovementGenerator(timer) {}

        MovementGeneratorType GetMovementGeneratorType() const { return ASSISTANCE_DISTRACT_MOTION_TYPE; }
        void Finalize(Unit& unit);
};

#endif
