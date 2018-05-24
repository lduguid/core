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

#ifndef MANGOSSERVER_LOG_H
#define MANGOSSERVER_LOG_H

#include "Common.h"
#include "Policies/Singleton.h"

class Config;
class ByteBuffer;

enum LogLevel
{
    LOG_LVL_MINIMAL = 0,                                    // unconditional and errors
    LOG_LVL_BASIC   = 1,
    LOG_LVL_DETAIL  = 2,
    LOG_LVL_DEBUG   = 3
};

// bitmask (not forgot update logFilterData content)
enum LogFilters
{
    LOG_FILTER_TRANSPORT_MOVES    = 0x0001,                 // any related to transport moves
    LOG_FILTER_CREATURE_MOVES     = 0x0002,                 // creature move by cells
    LOG_FILTER_VISIBILITY_CHANGES = 0x0004,                 // update visibility for diff objects and players
    // reserved for future version
    LOG_FILTER_WEATHER            = 0x0010,                 // weather changes
    LOG_FILTER_PLAYER_STATS       = 0x0020,                 // player save data
    LOG_FILTER_SQL_TEXT           = 0x0040,                 // raw SQL text send to DB engine
    LOG_FILTER_PLAYER_MOVES       = 0x0080,                 // player moves by grid/cell
    LOG_FILTER_PERIODIC_AFFECTS   = 0x0100,                 // DoT/HoT apply trace
    LOG_FILTER_AI_AND_MOVEGENSS   = 0x0200,                 // AI/movement generators debug output
    LOG_FILTER_DAMAGE             = 0x0400,                 // Direct/Area damage trace
    LOG_FILTER_COMBAT             = 0x0800,                 // attack states/roll attack results/etc
    LOG_FILTER_SPELL_CAST         = 0x1000,                 // spell cast/aura apply/spell proc events
    LOG_FILTER_DB_STRICTED_CHECK  = 0x2000,                 // stricted DB data checks output (with possible false reports) for DB devs
    LOG_FILTER_PATHFINDING        = 0x4000,
    LOG_FILTER_HONOR              = 0x8000,                 // Honor
};

#define LOG_FILTER_COUNT            16

struct LogFilterData
{
    char const* name;
    char const* configName;
    bool defaultState;
};

extern LogFilterData logFilterData[LOG_FILTER_COUNT];

enum Color
{
    BLACK,
    RED,
    GREEN,
    BROWN,
    BLUE,
    MAGENTA,
    CYAN,
    GREY,
    YELLOW,
    LRED,
    LGREEN,
    LBLUE,
    LMAGENTA,
    LCYAN,
    WHITE
};

enum LogFile
{
    LOG_CHAT            = 0,
    LOG_BG,
    LOG_CHAR,
    LOG_RA,
    LOG_DBERRFIX,
    LOG_CLIENT_IDS,
    LOG_LOOTS,
    LOG_LEVELUP,
    LOG_PERFORMANCE,
    LOG_MONEY_TRADES,
    LOG_ANTICHEAT,
    LOG_GM_CRITICAL,
    LOG_CHAT_SPAM,
    LOG_EXPLOITS,
    LOG_MAX_FILES
};

enum LogType
{
    LogNormal = 0,
    LogDetails,
    LogDebug,
    LogError,
    LogWarden,
    LOG_TYPE_MAX // add new entries *before* this value!
};

const int Color_count = int(WHITE)+1;

class Log : public MaNGOS::Singleton<Log, MaNGOS::ClassLevelLockable<Log, ACE_Thread_Mutex> >
{
    friend class MaNGOS::OperatorNew<Log>;
    Log();

    ~Log()
    {
        if( logfile != nullptr )
            fclose(logfile);
        logfile = nullptr;

        if( gmLogfile != nullptr )
            fclose(gmLogfile);
        gmLogfile = nullptr;

        if( dberLogfile != nullptr )
            fclose(dberLogfile);
        dberLogfile = nullptr;

        if (worldLogfile != nullptr)
            fclose(worldLogfile);
        worldLogfile = nullptr;

        if (nostalriusLogFile != nullptr)
            fclose(nostalriusLogFile);
        nostalriusLogFile = nullptr;

        if (honorLogfile != nullptr)
            fclose(honorLogfile);
        honorLogfile = nullptr;

        for (int i = 0; i < LOG_MAX_FILES; ++i)
            if (logFiles[i] != nullptr)
            {
                fclose(logFiles[i]);
                logFiles[i] = nullptr;
            }
    }
    public:
        void Initialize();
        void InitColors(const std::string& init_str);

        void InitSmartlogEntries(const std::string& str);
        void InitSmartlogGuids(const std::string& str);

        void out(LogFile t, const char* format, ...) ATTR_PRINTF(3,4);
        void outCommand( uint32 account, const char * str, ...) ATTR_PRINTF(3,4);
        void outString();                                   // any log level
                                                            // any log level
        void outString( const char * str, ... )      ATTR_PRINTF(2,3);
        void outInfo( const char * str, ...)      ATTR_PRINTF(2,3);
        void outHonor(const char* str, ...)       ATTR_PRINTF(2, 3);
                                                            // any log level
        void outError( const char * err, ... )       ATTR_PRINTF(2,3);
                                                            // log level >= 1
        void outBasic( const char * str, ... )       ATTR_PRINTF(2,3);
                                                            // log level >= 2
        void outDetail( const char * str, ... )      ATTR_PRINTF(2,3);
                                                            // log level >= 3
        void outDebug( const char * str, ... )       ATTR_PRINTF(2,3);
        void outWarden(const char * wrd, ...)        ATTR_PRINTF(2,3);

        void outErrorDb();                                  // any log level
                                                            // any log level
        void outErrorDb( const char * str, ... )     ATTR_PRINTF(2,3);
                                                            // any log level
        void outWorldPacketDump(ACE_HANDLE socketHandle, uint32 opcode,
                                char const* opcodeName,
                                ByteBuffer const* packet, bool incoming);
        // any log level
        uint32 GetLogLevel() const { return m_logLevel; }
        void SetLogLevel(char * Level);
        void SetLogFileLevel(char * Level);
        void SetColor(bool stdout_stream, Color color);
        void ResetColor(bool stdout_stream);
        void outTime(FILE* where);
        static void outTimestamp(FILE* file);
        static std::string GetTimestampStr();
        bool HasLogFilter(uint32 filter) const { return m_logFilter & filter; }
        void SetLogFilter(LogFilters filter, bool on) { if (on) m_logFilter |= filter; else m_logFilter &= ~filter; }
        bool HasLogLevelOrHigher(LogLevel loglvl) const { return m_logLevel >= loglvl || (m_logFileLevel >= loglvl && logfile); }
        bool IsIncludeTime() const { return m_includeTime; }

        static void WaitBeforeContinueIfNeed();

        std::list<uint32> m_smartlogExtraEntries;
        std::list<uint32> m_smartlogExtraGuids;

    private:
        FILE* openLogFile(char const* configFileName,char const* configTimeStampFlag, char const* mode);
        FILE* openGmlogPerAccount(uint32 account);

        FILE* logfile;
        FILE* gmLogfile;
        FILE* dberLogfile;
        FILE* wardenLogfile;
        FILE* worldLogfile;
        FILE* nostalriusLogFile;
        FILE* honorLogfile;
        FILE* logFiles[LOG_MAX_FILES];
        bool  timestampPrefix[LOG_MAX_FILES];

        bool m_bIsChatLogFileActivated;

        // log/console control
        LogLevel m_logLevel;
        LogLevel m_logFileLevel;
        bool m_colored;
        bool m_includeTime;
        Color m_colors[LOG_TYPE_MAX];
        uint32 m_logFilter;

        // cache values for after initilization use (like gm log per account case)
        std::string m_logsDir;
        std::string m_logsTimestamp;

        // char log control
        bool m_charLog_Dump;

        // gm log control
        bool m_gmlog_per_account;
        std::string m_gmlog_filename_format;
};

#define sLog MaNGOS::Singleton<Log>::Instance()

#define BASIC_LOG(...)                                  \
    do {                                                \
        if (sLog.HasLogLevelOrHigher(LOG_LVL_BASIC))    \
            sLog.outBasic(__VA_ARGS__);                 \
    } while(0)

#define BASIC_FILTER_LOG(F,...)                         \
    do {                                                \
        if (sLog.HasLogLevelOrHigher(LOG_LVL_BASIC) && !sLog.HasLogFilter(F)) \
            sLog.outBasic(__VA_ARGS__);                 \
    } while(0)

#define DETAIL_LOG(...)                                 \
    do {                                                \
        if (sLog.HasLogLevelOrHigher(LOG_LVL_DETAIL))   \
            sLog.outDetail(__VA_ARGS__);                \
    } while(0)

#define DETAIL_FILTER_LOG(F,...)                        \
    do {                                                \
        if (sLog.HasLogLevelOrHigher(LOG_LVL_DETAIL) && !sLog.HasLogFilter(F)) \
            sLog.outDetail(__VA_ARGS__);                \
    } while(0)

#define DEBUG_LOG(...)                                  \
    do {                                                \
        if (sLog.HasLogLevelOrHigher(LOG_LVL_DEBUG))    \
            sLog.outDebug(__VA_ARGS__);                 \
    } while(0)

#define DEBUG_FILTER_LOG(F,...)                         \
    do {                                                \
        if (sLog.HasLogLevelOrHigher(LOG_LVL_DEBUG) && !sLog.HasLogFilter(F)) \
            sLog.outDebug(__VA_ARGS__);                 \
    } while(0)

#define ERROR_DB_FILTER_LOG(F,...)                      \
    do {                                                \
        if (!sLog.HasLogFilter(F))                      \
            sLog.outErrorDb(__VA_ARGS__);               \
    } while(0)

#define ERROR_DB_STRICT_LOG(...) \
    ERROR_DB_FILTER_LOG(LOG_FILTER_DB_STRICTED_CHECK, __VA_ARGS__)

// primary for script library
void MANGOS_DLL_SPEC outstring_log(const char* str, ...) ATTR_PRINTF(1, 2);
void MANGOS_DLL_SPEC detail_log(const char* str, ...) ATTR_PRINTF(1, 2);
void MANGOS_DLL_SPEC debug_log(const char* str, ...) ATTR_PRINTF(1, 2);
void MANGOS_DLL_SPEC error_log(const char* str, ...) ATTR_PRINTF(1, 2);
void MANGOS_DLL_SPEC error_db_log(const char* str, ...) ATTR_PRINTF(1, 2);
void MANGOS_DLL_SPEC setScriptLibraryErrorFile(char const* fname, char const* libName);
void MANGOS_DLL_SPEC script_error_log(const char* str, ...) ATTR_PRINTF(1, 2);

#endif
