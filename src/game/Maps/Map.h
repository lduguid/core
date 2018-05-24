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

#ifndef MANGOS_MAP_H
#define MANGOS_MAP_H

#include "Common.h"
#include "Platform/Define.h"
#include "Policies/ThreadingModel.h"
#include "ace/RW_Thread_Mutex.h"
#include "ace/Thread_Mutex.h"

#include "DBCStructure.h"
#include "GridDefines.h"
#include "Cell.h"
#include "Object.h"
#include "Timer.h"
#include "SharedDefines.h"
#include "GridMap.h"
#include "GameSystem/GridRefManager.h"
#include "MapRefManager.h"
#include "Utilities/TypeList.h"
#include "ScriptMgr.h"
#include "vmap/DynamicTree.h"
#include "MoveSplineInitArgs.h"
#include "WorldSession.h"
#include "SQLStorages.h"

#include <bitset>
#include <list>
#include <set>

using Movement::Vector3;

struct CreatureInfo;
class Creature;
class Unit;
class WorldPacket;
class InstanceData;
class Group;

class CreatureGroup;

class MapPersistentState;
class WorldPersistentState;
class DungeonPersistentState;
class BattleGroundPersistentState;
class ChatHandler;

struct ScriptInfo;
class BattleGround;
class GridMap;
class Transport;

namespace VMAP
{
    class ModelInstance;
};

// GCC have alternative #pragma pack(N) syntax and old gcc version not support pack(push,N), also any gcc version not support it at some platform
#if defined( __GNUC__ )
#pragma pack(1)
#else
#pragma pack(push,1)
#endif

struct MapEntry
{
    uint32 id;
    uint32 parent;
    uint32 mapType;
    uint32 linkedZone;
    uint32 levelMin;
    uint32 levelMax;
    uint32 maxPlayers;
    uint32 resetDelay;
    int32 ghostEntranceMap;
    float ghostEntranceX;
    float ghostEntranceY;
    char*  name;
    uint32 scriptId;

    bool IsDungeon() const { return mapType == MAP_INSTANCE || mapType == MAP_RAID; }
    bool IsNonRaidDungeon() const { return mapType == MAP_INSTANCE; }
    bool Instanceable() const { return mapType == MAP_INSTANCE || mapType == MAP_RAID || mapType == MAP_BATTLEGROUND; }
    bool IsRaid() const { return mapType == MAP_RAID; }
    bool IsBattleGround() const { return mapType == MAP_BATTLEGROUND; }
    bool IsMountAllowed() const { return !IsDungeon() || id == 309 || id == 209 || id == 509 || id == 269; }
    bool IsContinent() const { return id == 0 || id == 1; }
};

typedef std::map<uint32, uint32> AreaFlagByMapId;
static AreaFlagByMapId sAreaFlagByMapId;

struct AreaEntry
{
    uint32 Id;
    uint32 MapId;
    uint32 ZoneId;
    uint32 ExploreFlag;
    uint32 Flags;
    int32  AreaLevel;
    char*  Name;
    uint32 Team;
    uint32 LiquidTypeId;

    bool IsZone() const { return ZoneId == 0; }

    static int32 GetFlagById(uint32 id)
    {
        const auto *areaEntry = sAreaStorage.LookupEntry<AreaEntry>(id);
        if (!areaEntry)
            return -1;

        return areaEntry->ExploreFlag;
    }

    static uint32 GetFlagByMapId(uint32 mapId)
    {
        auto itr = sAreaFlagByMapId.find(mapId);
        if (itr == sAreaFlagByMapId.end())
            return 0;
        return itr->second;
    }

    static const AreaEntry* GetById(uint32 id)
    {
        return sAreaStorage.LookupEntry<AreaEntry>(id);
    }

    static const AreaEntry* GetByAreaFlagAndMap(uint32 areaFlag, uint32 mapId)
    {
        AreaEntry const* areaEntry = nullptr;
        // 1.12.1 areatable have duplicates for areaflag
        for (auto itr = sAreaStorage.begin<AreaEntry>(); itr < sAreaStorage.end<AreaEntry>(); ++itr)
        {
            if (areaFlag && areaFlag == itr->ExploreFlag)
            {
                if (itr->MapId == mapId)
                    return *itr;

                areaEntry = *itr;
            }
        }

        if (areaEntry)
            return areaEntry;

        if (const auto *mapEntry = sMapStorage.LookupEntry<MapEntry>(mapId))
            return sAreaStorage.LookupEntry<AreaEntry>(mapEntry->linkedZone);

        return nullptr;
    }
};

#if defined( __GNUC__ )
#pragma pack()
#else
#pragma pack(pop)
#endif

struct AreaLocale
{
    std::vector<std::string> Name;
};

#define MIN_UNLOAD_DELAY      1                             // immediate unload

typedef std::map<uint32, CreatureGroup*> CreatureGroupHolderType;
typedef ACE_Thread_Mutex MapMutexType; // Use ACE_Null_Mutex to disable locks

// Instance IDs reserved for internal use (instanced continent parts, ...)
#define RESERVED_INSTANCES_LAST 100

class MANGOS_DLL_SPEC Map : public GridRefManager<NGridType>, public MaNGOS::ObjectLevelLockable<Map, ACE_Thread_Mutex>
{
    friend class MapReference;
    friend class ObjectGridLoader;
    friend class ObjectWorldLoader;

    protected:
        Map(uint32 id, time_t, uint32 InstanceId);

    public:
        virtual ~Map();
        void PrintInfos(ChatHandler& handler);
        void SpawnActiveObjects();
        // currently unused for normal maps
        bool CanUnload(uint32 diff)
        {
            if(!m_unloadTimer) return false;
            if(m_unloadTimer <= diff) return true;
            m_unloadTimer -= diff;
            return false;
        }

        void ExistingPlayerLogin(Player*);
        virtual bool Add(Player*);
        virtual void Remove(Player*, bool);
        template<class T> void Add(T*);
        template<class T> void Remove(T*, bool);

        static void DeleteFromWorld(Player* player);        // player object will deleted at call

        inline void UpdateCellsAroundObject(uint32 now, uint32 diff, WorldObject const* object);
        inline void UpdateActiveCellsSynch(uint32 now, uint32 diff);
        inline void MarkCellsAroundObject(WorldObject const* object);
        inline void UpdateActiveCellsAsynch(uint32 now, uint32 diff);
        inline void UpdateActiveCellsCallback(uint32 diff, uint32 now, uint32 threadId, uint32 totalThreads, uint32 step);
        inline void UpdateCells(uint32 diff);
        void UpdateSync(const uint32);
        void UpdatePlayers();
        void DoUpdate(uint32 maxDiff);
        virtual void Update(uint32);
        void UpdateSessionsMovementAndSpellsIfNeeded();
        void ProcessSessionPackets(PacketProcessing type);

        void MessageBroadcast(Player*, WorldPacket*, bool to_self);
        void MessageBroadcast(WorldObject*, WorldPacket*);
        void MessageDistBroadcast(Player*, WorldPacket*, float dist, bool to_self, bool own_team_only = false);
        void MessageDistBroadcast(WorldObject*, WorldPacket*, float dist);

        float GetVisibilityDistance() const { return m_VisibleDistance; }
        float GetGridActivationDistance() const { return m_GridActivationDistance; }

        //function for setting up visibility distance for maps on per-type/per-Id basis
        virtual void InitVisibilityDistance();

        void PlayerRelocation(Player *, float x, float y, float z, float angl);
        // Used at interpolation.
        void DoPlayerGridRelocation(Player*, float x, float y, float z, float angl);
        void CreatureRelocation(Creature *creature, float x, float y, float z, float orientation);

        template<class T, class CONTAINER> void Visit(const Cell& cell, TypeContainerVisitor<T, CONTAINER> &visitor);

        bool IsRemovalGrid(float x, float y) const
        {
            GridPair p = MaNGOS::ComputeGridPair(x, y);
            return !getNGrid(p.x_coord, p.y_coord) || getNGrid(p.x_coord, p.y_coord)->GetGridState() == GRID_STATE_REMOVAL;
        }

        bool IsLoaded(float x, float y) const
        {
            GridPair p = MaNGOS::ComputeGridPair(x, y);
            return loaded(p);
        }

        bool GetUnloadLock(const GridPair &p) const { return getNGrid(p.x_coord, p.y_coord)->getUnloadLock(); }
        void SetUnloadLock(const GridPair &p, bool on) { getNGrid(p.x_coord, p.y_coord)->setUnloadExplicitLock(on); }
        void LoadGrid(const Cell& cell, bool no_unload = false);
        bool UnloadGrid(const uint32 &x, const uint32 &y, bool pForce);
        virtual void UnloadAll(bool pForce);

        void ResetGridExpiry(NGridType &grid, float factor = 1) const
        {
            grid.ResetTimeTracker((time_t)((float)i_gridExpiry*factor));
        }

        time_t GetGridExpiry(void) const { return i_gridExpiry; }
        uint32 GetId(void) const { return i_id; }

        // some calls like isInWater should not use vmaps due to processor power
        // can return INVALID_HEIGHT if under z+2 z coord not found height

        virtual void RemoveAllObjectsInRemoveList();

        bool CreatureRespawnRelocation(Creature *c, bool forGridUnload = false);        // used only in CreatureRelocation and ObjectGridUnloader

        bool CheckGridIntegrity(Creature* c, bool moved) const;

        uint32 GetInstanceId() const { return i_InstanceId; }
        virtual bool CanEnter(Player* /*player*/) { return true; }
        const char* GetMapName() const;

        const MapEntry* GetMapEntry() const { return i_mapEntry; }
        bool Instanceable() const { return i_mapEntry && i_mapEntry->Instanceable(); }
        bool IsNonRaidDungeon() const { return i_mapEntry && i_mapEntry->IsNonRaidDungeon(); }
        bool IsDungeon() const { return i_mapEntry && i_mapEntry->IsDungeon(); }
        bool IsRaid() const { return i_mapEntry && i_mapEntry->IsRaid(); }
        bool IsBattleGround() const { return i_mapEntry && i_mapEntry->IsBattleGround(); }
        bool IsContinent() const { return i_mapEntry && i_mapEntry->IsContinent(); }

        // can't be NULL for loaded map
        MapPersistentState* GetPersistentState() const { return m_persistentState; }

        void AddObjectToRemoveList(WorldObject *obj);

        void UpdateObjectVisibility(WorldObject* obj, Cell cell, CellPair cellpair);

        void resetMarkedCells() { marked_cells.reset(); }
        bool isCellMarked(uint32 pCellId) { return marked_cells.test(pCellId); }
        void markCell(uint32 pCellId) { marked_cells.set(pCellId); }

        bool HavePlayers() const { return !m_mapRefManager.isEmpty(); }
        uint32 GetPlayersCountExceptGMs() const;
        bool ActiveObjectsNearGrid(uint32 x,uint32 y) const;

        void SendToPlayers(WorldPacket const* data) const;

        typedef MapRefManager PlayerList;
        PlayerList const& GetPlayers() const { return m_mapRefManager; }

        //per-map script storage
        void ScriptsStart(std::map<uint32, std::multimap<uint32, ScriptInfo> > const& scripts, uint32 id, Object* source, Object* target);
        void ScriptCommandStart(ScriptInfo const& script, uint32 delay, Object* source, Object* target);

        // must called with AddToWorld
        void AddToActive(WorldObject* obj);
        // must called with RemoveFromWorld
        void RemoveFromActive(WorldObject* obj);

        Creature* SummonCreature(uint32 entry, float x, float y, float z, float ang, TempSummonType spwtype = TEMPSUMMON_DEAD_DESPAWN, uint32 despwtime = 25000, bool asActiveObject = false);

        Player* GetPlayer(ObjectGuid guid);
        GameObject* GetGameObject(ObjectGuid const& guid) { return GetObject<GameObject>(guid); }
        Creature* GetCreature(ObjectGuid const& guid) { return GetObject<Creature>(guid); }
        Pet* GetPet(ObjectGuid const& guid) { return GetObject<Pet>(guid); }
        Creature* GetAnyTypeCreature(ObjectGuid guid);      // normal creature or pet
        Transport* GetTransport(ObjectGuid guid);
        DynamicObject* GetDynamicObject(ObjectGuid guid);
        Corpse* GetCorpse(ObjectGuid guid);                 // !!! find corpse can be not in world
        Unit* GetUnit(ObjectGuid guid);                     // only use if sure that need objects at current map, specially for player case
        WorldObject* GetWorldObject(ObjectGuid guid);       // only use if sure that need objects at current map, specially for player case

        template <typename T> void InsertObject(ObjectGuid const& guid, T* ptr)
        {
            m_objectsStore_lock.acquire_write();
            m_objectsStore.insert<T>(guid, ptr);
            m_objectsStore_lock.release();
        }
        template <typename T> void EraseObject(ObjectGuid const& guid)
        {
            m_objectsStore_lock.acquire_write();
            m_objectsStore.erase<T>(guid, (T*)nullptr);
            m_objectsStore_lock.release();
        }
        template <typename T> T* GetObject(ObjectGuid const& guid)
        {
            m_objectsStore_lock.acquire_read();
            T* ptr = m_objectsStore.find<T>(guid, (T*)nullptr);
            m_objectsStore_lock.release();
            return ptr;
        }
        void AddUpdateObject(Object *obj)
        {
            if (_processingSendObjUpdates)
                return;
            i_objectsToClientUpdate_lock.acquire();
            i_objectsToClientUpdate.insert(obj);
            i_objectsToClientUpdate_lock.release();
        }

        void RemoveUpdateObject(Object *obj)
        {
            ASSERT(!_processingSendObjUpdates);
            i_objectsToClientUpdate_lock.acquire();
            i_objectsToClientUpdate.erase( obj );
            i_objectsToClientUpdate_lock.release();
        }
        // May be called from a different map ...
        void AddRelocatedUnit(Unit* obj)
        {
            if (_processingUnitsRelocation)
                return;
            i_unitsRelocated_lock.acquire();
            i_unitsRelocated.insert(obj);
            i_unitsRelocated_lock.release();
        }
        void RemoveRelocatedUnit(Unit* obj)
        {
            ASSERT(!_processingUnitsRelocation);
            i_unitsRelocated_lock.acquire();
            i_unitsRelocated.erase(obj);
            i_unitsRelocated_lock.release();
        }

        void AddUnitToMovementUpdate(Unit* unit)
        {
            unitsMvtUpdate_lock.acquire();
            unitsMvtUpdate.insert(unit);
            unitsMvtUpdate_lock.release();
        }
        void RemoveUnitFromMovementUpdate(Unit* unit)
        {
            unitsMvtUpdate_lock.acquire();
            unitsMvtUpdate.erase(unit);
            unitsMvtUpdate_lock.release();
        }
        // DynObjects currently
        uint32 GenerateLocalLowGuid(HighGuid guidhigh);

        //get corresponding TerrainData object for this particular map
        const TerrainInfo * GetTerrain() const { return m_TerrainData; }

        void CreateInstanceData(bool load);
        InstanceData* GetInstanceData() { return i_data; }
        InstanceData const* GetInstanceData() const { return i_data; }
        uint32 GetScriptId() const { return i_script_id; }

        void MonsterYellToMap(ObjectGuid guid, int32 textId, uint32 language, Unit* target);
        void MonsterYellToMap(CreatureInfo const* cinfo, int32 textId, uint32 language, Unit* target, uint32 senderLowGuid = 0);
        void PlayDirectSoundToMap(uint32 soundId);

        // GameObjectCollision
        float GetHeight(float x, float y, float z, bool vmap = true, float maxSearchDist = DEFAULT_HEIGHT_SEARCH) const;
        bool isInLineOfSight(float x1, float y1, float z1, float x2, float y2, float z2, bool checkDynLos = true) const;
        // First collision with object
        bool GetLosHitPosition(float srcX, float srcY, float srcZ, float& destX, float& destY, float& destZ, float modifyDist) const;
        // Use navemesh to walk
        bool GetWalkHitPosition(Transport* t, float srcX, float srcY, float srcZ, float& destX, float& destY, float& destZ, 
            uint32 moveAllowedFlags = 0xF /*NAV_GROUND | NAV_WATER | NAV_MAGMA | NAV_SLIME*/, float zSearchDist = 20.0f, bool locatedOnSteepSlope = true) const;
        bool GetWalkRandomPosition(Transport* t, float &x, float &y, float &z, float maxRadius, uint32 moveAllowedFlags = 0xF) const;
        VMAP::ModelInstance* FindCollisionModel(float x1, float y1, float z1, float x2, float y2, float z2);

        void Balance() { _dynamicTree.balance(); }
        void RemoveGameObjectModel(const GameObjectModel& model)
        {
            _dynamicTree_lock.acquire_write();
            _dynamicTree.remove(model);
            _dynamicTree.balance();
            _dynamicTree_lock.release();
        }
        void InsertGameObjectModel(const GameObjectModel& model)
        {
            _dynamicTree_lock.acquire_write();
            _dynamicTree.insert(model);
            _dynamicTree.balance();
            _dynamicTree_lock.release();
        }
        bool ContainsGameObjectModel(const GameObjectModel& model) const
        {
            _dynamicTree_lock.acquire_read();
            bool r = _dynamicTree.contains(model);
            _dynamicTree_lock.release();
            return r;
        }
        bool GetDynamicObjectHitPos(Vector3 start, Vector3 end, Vector3& out, float finalDistMod) const
        {
            _dynamicTree_lock.acquire_read();
            bool r = _dynamicTree.getObjectHitPos(start, end, out, finalDistMod);
            _dynamicTree_lock.release();
            return r;
        }
        float GetDynamicTreeHeight(float x, float y, float z, float maxSearchDist) const
        {
            _dynamicTree_lock.acquire_read();
            float r = _dynamicTree.getHeight(x, y, z, maxSearchDist);
            _dynamicTree_lock.release();
            return r;
        }
        bool CheckDynamicTreeLoS(float x1, float y1, float z1, float x2, float y2, float z2) const
        {
            _dynamicTree_lock.acquire_read();
            bool r = _dynamicTree.isInLineOfSight(x1, y1, z1, x2, y2, z2);
            _dynamicTree_lock.release();
            return r;
        }
        bool IsUnloading() const { return m_unloading; }
        void MarkAsCrashed() { m_crashed = true; }
        bool IsCrashed() const { return m_crashed; }
        void CrashUnload();
        bool IsUpdateFinished() const { return m_updateFinished; }
        void MarkNotUpdated() { m_updateFinished = false; }
        void SetUpdateDiffMod(int32 d) { m_updateDiffMod = d; }
        uint32 GetUpdateDiffMod() const { return m_updateDiffMod; }
        void BindToInstanceOrRaid(Player* player, time_t objectResetTime, bool permBindToRaid);
        void TeleportAllPlayersToHomeBind();

        void SetMapUpdateIndex(int idx) { _updateIdx = idx; }

    private:
        void LoadMapAndVMap(int gx, int gy);

        void SetTimer(uint32 t) { i_gridExpiry = t < MIN_GRID_DELAY ? MIN_GRID_DELAY : t; }

        void SendInitSelf( Player * player );

        void SendInitTransports(Player * player);
        void SendRemoveTransports(Player * player);

        bool CreatureCellRelocation(Creature *creature, Cell new_cell);

        bool loaded(const GridPair &) const;
        void EnsureGridCreated(const GridPair &);
        bool EnsureGridLoaded(Cell const&);
        void EnsureGridLoadedAtEnter(Cell const&, Player* player = nullptr);

        void buildNGridLinkage(NGridType* pNGridType) { pNGridType->link(this); }

        template<class T> void AddType(T *obj);
        template<class T> void RemoveType(T *obj, bool);

        NGridType* getNGrid(uint32 x, uint32 y) const
        {
            MANGOS_ASSERT(x < MAX_NUMBER_OF_GRIDS);
            MANGOS_ASSERT(y < MAX_NUMBER_OF_GRIDS);
            return i_grids[x][y];
        }

        bool isGridObjectDataLoaded(uint32 x, uint32 y) const { return getNGrid(x,y)->isGridObjectDataLoaded(); }
        void setGridObjectDataLoaded(bool pLoaded, uint32 x, uint32 y) { getNGrid(x,y)->setGridObjectDataLoaded(pLoaded); }

        void setNGrid(NGridType* grid, uint32 x, uint32 y);
        void ScriptsProcess();

        void SendObjectUpdates();
        void UpdateVisibilityForRelocations();

        bool                    _processingSendObjUpdates;
        uint32                  _objUpdatesThreads;
        mutable MapMutexType    i_objectsToClientUpdate_lock;
        std::set<Object *>      i_objectsToClientUpdate;

        bool                    _processingUnitsRelocation;
        uint32                  _unitRelocationThreads;
        mutable MapMutexType    i_unitsRelocated_lock;
        std::set<Unit* >        i_unitsRelocated;

        mutable MapMutexType    unitsMvtUpdate_lock;
        std::set<Unit*>         unitsMvtUpdate;

    protected:
        MapEntry const* i_mapEntry;
        uint32 i_id;
        uint32 i_InstanceId;
        uint32 m_unloadTimer;
        float m_VisibleDistance;
        float m_GridActivationDistance;

        mutable ACE_RW_Mutex   _dynamicTree_lock;
        DynamicMapTree _dynamicTree;

        MapPersistentState* m_persistentState;

        MapRefManager m_mapRefManager;
        MapRefManager::iterator m_mapRefIter;

        typedef std::set<WorldObject*> ActiveNonPlayers;
        ActiveNonPlayers m_activeNonPlayers;
        ActiveNonPlayers::iterator m_activeNonPlayersIter;

        typedef TypeUnorderedMapContainer<AllMapStoredObjectTypes, ObjectGuid> MapStoredObjectTypesContainer;
        ACE_RW_Mutex                    m_objectsStore_lock;
        MapStoredObjectTypesContainer   m_objectsStore;

        // Objects that must update even in inactive grids without activating them
        typedef std::set<Transport*> TransportsContainer;
        TransportsContainer _transports;
        TransportsContainer::iterator _transportsUpdateIter;
        bool m_unloading;
        bool m_crashed;
        bool m_updateFinished;
        uint32 m_updateDiffMod;
        uint32 m_lastMvtSpellsUpdate;
    private:
        time_t i_gridExpiry;

        NGridType* i_grids[MAX_NUMBER_OF_GRIDS][MAX_NUMBER_OF_GRIDS];

        //Shared geodata object with map coord info...
        TerrainInfo * const m_TerrainData;
        bool m_bLoadedGrids[MAX_NUMBER_OF_GRIDS][MAX_NUMBER_OF_GRIDS];

        std::bitset<TOTAL_NUMBER_OF_CELLS_PER_MAP*TOTAL_NUMBER_OF_CELLS_PER_MAP> marked_cells;

        mutable MapMutexType    i_objectsToRemove_lock;
        std::set<WorldObject *> i_objectsToRemove;

        typedef std::multimap<time_t, ScriptAction> ScriptScheduleMap;
        MapMutexType      m_scriptSchedule_lock;
        ScriptScheduleMap m_scriptSchedule;

        InstanceData* i_data;
        uint32 i_script_id;

        // Map local low guid counters
        mutable MapMutexType    m_guidGenerators_lock;
        ObjectGuidGenerator<HIGHGUID_UNIT> m_CreatureGuids;
        ObjectGuidGenerator<HIGHGUID_GAMEOBJECT> m_GameObjectGuids;
        ObjectGuidGenerator<HIGHGUID_DYNAMICOBJECT> m_DynObjectGuids;
        ObjectGuidGenerator<HIGHGUID_PET> m_PetGuids;

        // Type specific code for add/remove to/from grid
        template<class T>
            void AddToGrid(T*, NGridType *, Cell const&);

        template<class T>
            void RemoveFromGrid(T*, NGridType *, Cell const&);
        // Custom
        uint32 _lastMapUpdate;
        uint32 _lastPlayerLeftTime;
        uint32 _lastPlayersUpdate;
        uint32 _inactivePlayersSkippedUpdates;
        uint32 _lastCellsUpdate;

        int8 _updateIdx;
    public:
        CreatureGroupHolderType CreatureGroupHolder;
        uint32 GetLastPlayerLeftTime() const { return _lastPlayerLeftTime; }
};

class MANGOS_DLL_SPEC WorldMap : public Map
{
    using Map::GetPersistentState;                      // hide in subclass for overwrite

    public:
        WorldMap(uint32 id, time_t expiry, uint32 instance = 0) : Map(id, expiry, instance) {}
        ~WorldMap() {}

        // can't be NULL for loaded map
        WorldPersistentState* GetPersistanceState() const;
};

class MANGOS_DLL_SPEC DungeonMap : public Map
{
    using Map::GetPersistentState;                      // hide in subclass for overwrite

    public:
        DungeonMap(uint32 id, time_t, uint32 InstanceId);
        ~DungeonMap();
        bool Add(Player*) override;
        void Remove(Player*, bool) override;
        void Update(uint32) override;
        bool Reset(InstanceResetMethod method);
        void PermBindAllPlayers(Player *player);
        void UnloadAll(bool pForce) override;
        bool CanEnter(Player* player) override;
        void SendResetWarnings(uint32 timeLeft) const;
        void SetResetSchedule(bool on);
        uint32 GetMaxPlayers() const;

        // can't be NULL for loaded map
        DungeonPersistentState* GetPersistanceState() const;

        void InitVisibilityDistance() override;
        // Activated at raid expiration. No one can enter.
        bool IsUnloadingBeforeReset() const { return m_resetAfterUnload; }
    private:
        bool m_resetAfterUnload;
        bool m_unloadWhenEmpty;
};

class MANGOS_DLL_SPEC BattleGroundMap : public Map
{
    using Map::GetPersistentState;                      // hide in subclass for overwrite

    public:
        BattleGroundMap(uint32 id, time_t, uint32 InstanceId);
        ~BattleGroundMap();

        void Update(uint32) override;
        bool Add(Player*) override;
        void Remove(Player*, bool) override;
        bool CanEnter(Player* player) override;
        void SetUnload();
        void UnloadAll(bool pForce) override;

        void InitVisibilityDistance() override;
        BattleGround* GetBG() const { return m_bg; }
        void SetBG(BattleGround* bg) { m_bg = bg; }

        // can't be NULL for loaded map
        BattleGroundPersistentState* GetPersistanceState() const;

    private:
        BattleGround* m_bg;
};

template<class T, class CONTAINER>
void Map::Visit(const Cell& cell, TypeContainerVisitor<T, CONTAINER> &visitor)
{
    const uint32 x = cell.GridX();
    const uint32 y = cell.GridY();
    const uint32 cell_x = cell.CellX();
    const uint32 cell_y = cell.CellY();

    if( !cell.NoCreate() || loaded(GridPair(x,y)) )
    {
        EnsureGridLoaded(cell);
        getNGrid(x, y)->Visit(cell_x, cell_y, visitor);
    }
}
#endif
