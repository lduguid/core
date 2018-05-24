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

#ifndef DATABASE_H
#define DATABASE_H

#include "Threading.h"
#include "Utilities/UnorderedMapSet.h"
#include "Database/SqlDelayThread.h"
#include <ace/Recursive_Thread_Mutex.h>
#include "Policies/ThreadingModel.h"
#include <ace/TSS_T.h>
#include <ace/Atomic_Op.h>
#include "SqlPreparedStatement.h"

class SqlTransaction;
class SqlResultQueue;
class SqlQueryHolder;
class SqlStmtParameters;
class SqlParamBinder;
class Database;

#define MAX_QUERY_LEN   32*1024

typedef ACE_Based::LockedQueue<SqlOperation*, ACE_Thread_Mutex> SqlQueue;

//
class MANGOS_DLL_SPEC SqlConnection
{
    public:
        virtual ~SqlConnection() {}

        //method for initializing DB connection
        bool Initialize(const char *infoString);
        virtual bool OpenConnection(bool reconnect) = 0;

        //public methods for making queries
        virtual QueryResult* Query(const char *sql) = 0;
        virtual QueryNamedResult* QueryNamed(const char *sql) = 0;

        //public methods for making requests
        virtual bool Execute(const char *sql) = 0;

        //escape string generation
        virtual unsigned long escape_string(char *to, const char *from, unsigned long length) { strncpy(to,from,length); return length; }

        // nothing do if DB not support transactions
        virtual bool BeginTransaction() { return true; }
        virtual bool CommitTransaction() { return true; }
        // can't rollback without transaction support
        virtual bool RollbackTransaction() { return true; }

        //methods to work with prepared statements
        bool ExecuteStmt(int nIndex, const SqlStmtParameters& id);

        //SqlConnection object lock
        class Lock
        {
            public:
                Lock(SqlConnection * conn) : m_pConn(conn) { m_pConn->m_mutex.acquire(); }
                ~Lock() { m_pConn->m_mutex.release(); }

                SqlConnection * operator->() const { return m_pConn; }

            private:
                SqlConnection * const m_pConn;
        };

        //get DB object
        Database& DB() { return m_db; }

    protected:
        SqlConnection(Database& db) : m_db(db) {}

        virtual SqlPreparedStatement * CreateStatement(const std::string& fmt);
        //allocate prepared statement and return statement ID
        SqlPreparedStatement * GetStmt(int nIndex);

        Database& m_db;

        //free prepared statements objects
        void FreePreparedStatements();

        std::string m_host;
        std::string m_port_or_socket;
        int         m_port;
        bool        m_use_socket;
        std::string m_user;
        std::string m_password;
        std::string m_database;

    private:
        typedef ACE_Recursive_Thread_Mutex LOCK_TYPE;
        LOCK_TYPE m_mutex;

        typedef std::vector<SqlPreparedStatement * > StmtHolder;
        StmtHolder m_holder;
};

class MANGOS_DLL_SPEC Database
{
    public:
        virtual ~Database();

        virtual bool Initialize(const char *infoString, int nConns = 1, int nWorkers = 1);
        //start worker thread for async DB request execution
        virtual bool InitDelayThread(int i, std::string const& infoString);
        //stop worker thread
        virtual void HaltDelayThread();

        /// Synchronous DB queries
        inline QueryResult* Query(const char *sql)
        {
            SqlConnection::Lock guard(getQueryConnection());
            return guard->Query(sql);
        }

        inline QueryNamedResult* QueryNamed(const char *sql)
        {
            SqlConnection::Lock guard(getQueryConnection());
            return guard->QueryNamed(sql);
        }

        QueryResult* PQuery(const char *format,...) ATTR_PRINTF(2,3);
        QueryNamedResult* PQueryNamed(const char *format,...) ATTR_PRINTF(2,3);

        inline bool DirectExecute(const char* sql)
        {
            if(!m_pAsyncConn)
                return false;

            SqlConnection::Lock guard(m_pAsyncConn);
            return guard->Execute(sql);
        }

        bool DirectPExecute(const char *format,...) ATTR_PRINTF(2,3);

        /// Async queries and query holders, implemented in DatabaseImpl.h

        // Query / member
        template<class Class>
            bool AsyncQuery(Class *object, void (Class::*method)(QueryResult*), const char *sql);
        template<class Class>
            bool AsyncQueryUnsafe(Class *object, void (Class::*method)(QueryResult*), const char *sql);
        template<class Class, typename ParamType1>
            bool AsyncQuery(Class *object, void (Class::*method)(QueryResult*, ParamType1), ParamType1 param1, const char *sql);
        template<class Class, typename ParamType1>
            bool AsyncQueryUnsafe(Class *object, void (Class::*method)(QueryResult*, ParamType1), ParamType1 param1, const char *sql);
        template<class Class, typename ParamType1, typename ParamType2>
            bool AsyncQuery(Class *object, void (Class::*method)(QueryResult*, ParamType1, ParamType2), ParamType1 param1, ParamType2 param2, const char *sql);
        template<class Class, typename ParamType1, typename ParamType2, typename ParamType3>
            bool AsyncQuery(Class *object, void (Class::*method)(QueryResult*, ParamType1, ParamType2, ParamType3), ParamType1 param1, ParamType2 param2, ParamType3 param3, const char *sql);
        // Query / static
        template<typename ParamType1>
            bool AsyncQuery(void (*method)(QueryResult*, ParamType1), ParamType1 param1, const char *sql);
        template<typename ParamType1, typename ParamType2>
            bool AsyncQuery(void (*method)(QueryResult*, ParamType1, ParamType2), ParamType1 param1, ParamType2 param2, const char *sql);
        template<typename ParamType1, typename ParamType2, typename ParamType3>
            bool AsyncQuery(void (*method)(QueryResult*, ParamType1, ParamType2, ParamType3), ParamType1 param1, ParamType2 param2, ParamType3 param3, const char *sql);
        // PQuery / member
        template<class Class>
            bool AsyncPQuery(Class *object, void (Class::*method)(QueryResult*), const char *format,...) ATTR_PRINTF(4,5);
        template<class Class>
            bool AsyncPQueryUnsafe(Class *object, void (Class::*method)(QueryResult*), const char *format,...) ATTR_PRINTF(4,5);
        template<class Class, typename ParamType1>
            bool AsyncPQuery(Class *object, void (Class::*method)(QueryResult*, ParamType1), ParamType1 param1, const char *format,...) ATTR_PRINTF(5,6);
        template<class Class, typename ParamType1>
            bool AsyncPQueryUnsafe(Class *object, void (Class::*method)(QueryResult*, ParamType1), ParamType1 param1, const char *format,...) ATTR_PRINTF(5,6);
        template<class Class, typename ParamType1, typename ParamType2>
            bool AsyncPQuery(Class *object, void (Class::*method)(QueryResult*, ParamType1, ParamType2), ParamType1 param1, ParamType2 param2, const char *format,...) ATTR_PRINTF(6,7);
        template<class Class, typename ParamType1, typename ParamType2, typename ParamType3>
            bool AsyncPQuery(Class *object, void (Class::*method)(QueryResult*, ParamType1, ParamType2, ParamType3), ParamType1 param1, ParamType2 param2, ParamType3 param3, const char *format,...) ATTR_PRINTF(7,8);
        // PQuery / static
        template<typename ParamType1>
            bool AsyncPQuery(void (*method)(QueryResult*, ParamType1), ParamType1 param1, const char *format,...) ATTR_PRINTF(4,5);
        template<typename ParamType1, typename ParamType2>
            bool AsyncPQuery(void (*method)(QueryResult*, ParamType1, ParamType2), ParamType1 param1, ParamType2 param2, const char *format,...) ATTR_PRINTF(5,6);
        template<typename ParamType1, typename ParamType2, typename ParamType3>
            bool AsyncPQuery(void (*method)(QueryResult*, ParamType1, ParamType2, ParamType3), ParamType1 param1, ParamType2 param2, ParamType3 param3, const char *format,...) ATTR_PRINTF(6,7);
        template<class Class>
        // QueryHolder
            bool DelayQueryHolder(Class *object, void (Class::*method)(QueryResult*, SqlQueryHolder*), SqlQueryHolder *holder);
        template<class Class>
            bool DelayQueryHolderUnsafe(Class *object, void (Class::*method)(QueryResult*, SqlQueryHolder*), SqlQueryHolder *holder);
        template<class Class, typename ParamType1>
            bool DelayQueryHolder(Class *object, void (Class::*method)(QueryResult*, SqlQueryHolder*, ParamType1), SqlQueryHolder *holder, ParamType1 param1);

        bool Execute(const char *sql);
        bool PExecute(const char *format,...) ATTR_PRINTF(2,3);

        // Writes SQL commands to a LOG file (see mangosd.conf "LogSQL")
        bool PExecuteLog(const char *format,...) ATTR_PRINTF(2,3);

        bool BeginTransaction(uint32 serialId = 0);
        bool InTransaction();
        uint32 GetTransactionSerialId();
        bool CommitTransaction();
        bool RollbackTransaction();
        //for sync transaction execution
        bool CommitTransactionDirect();

        //PREPARED STATEMENT API

        //allocate index for prepared statement with SQL request 'fmt'
        SqlStatement CreateStatement(SqlStatementID& index, const char * fmt);
        //get prepared statement format string
        std::string GetStmtString(const int stmtId) const;

        operator bool () const { return m_pQueryConnections.size() && m_pAsyncConn != 0; }

        //escape string generation
        void escape_string(std::string& str);

        // must be called before first query in thread (one time for thread using one from existing Database objects)
        virtual void ThreadStart();
        // must be called before finish thread run (one time for thread using one from existing Database objects)
        virtual void ThreadEnd();

        // set database-wide result queue. also we should use object-bases and not thread-based result queues
        void ProcessResultQueue(uint32 maxTime = 0);

        bool CheckRequiredMigrations(const char **migrations);
        uint32 GetPingIntervall() { return m_pingIntervallms; }

        //function to ping database connections
        void Ping();

        //set this to allow async transactions
        //you should call it explicitly after your server successfully started up
        //NO ASYNC TRANSACTIONS DURING SERVER STARTUP - ONLY DURING RUNTIME!!!
        void AllowAsyncTransactions() { m_bAllowAsyncTransactions = true; }
        inline void AddToDelayQueue(SqlOperation* op) { m_delayQueue->add(op); }
        inline bool NextDelayedOperation(SqlOperation*& op) { return m_delayQueue->next(op); }

        inline void AddToSerialDelayQueue(int workerId, SqlOperation *op) { m_serialDelayQueue[workerId]->add(op); }
        bool NextSerialDelayedOperation(int workerId, SqlOperation*& op);

        bool HasAsyncQuery();

        void AddToSerialDelayQueue(SqlOperation *op);

        // Frees data, cancels scheduled queries, closes connection
        void StopServer();
    protected:
        Database() : m_pAsyncConn(NULL), m_pResultQueue(NULL), m_threadsBodies(NULL), m_delayThreads(NULL), m_numAsyncWorkers(0),
            m_serialDelayQueue(NULL), m_delayQueue(new SqlQueue()), m_logSQL(false), m_pingIntervallms(0), m_nQueryConnPoolSize(1),
            m_bAllowAsyncTransactions(false), m_iStmtIndex(-1)
        {
            m_nQueryCounter = -1;
        }

        //factory method to create SqlConnection objects
        virtual SqlConnection * CreateConnection() = 0;

        class MANGOS_DLL_SPEC TransHelper
        {
            public:
                TransHelper() : m_pTrans(NULL) {}
                ~TransHelper();

                //initializes new SqlTransaction object
                SqlTransaction * init(uint32 serialId);
                //gets pointer on current transaction object. Returns NULL if transaction was not initiated
                SqlTransaction * get() const { return m_pTrans; }
                //detaches SqlTransaction object allocated by init() function
                //next call to get() function will return NULL!
                //do not forget to destroy obtained SqlTransaction object!
                SqlTransaction * detach();
                //destroyes SqlTransaction allocated by init() function
                void reset();

            private:
                SqlTransaction * m_pTrans;
        };

        //per-thread based storage for SqlTransaction object initialization - no locking is required
        typedef ACE_TSS<Database::TransHelper> DBTransHelperTSS;
        Database::DBTransHelperTSS m_TransStorage;

        ///< DB connections

        //round-robin connection selection
        SqlConnection * getQueryConnection();
        //for now return one single connection for async requests
        SqlConnection * getAsyncConnection() const { return m_pAsyncConn; }

        friend class SqlStatement;
        //PREPARED STATEMENT API
        //query function for prepared statements
        bool ExecuteStmt(const SqlStatementID& id, SqlStmtParameters * params);
        bool DirectExecuteStmt(const SqlStatementID& id, SqlStmtParameters * params);

        //connection helper counters
        int m_nQueryConnPoolSize;                               //current size of query connection pool
        ACE_Atomic_Op<ACE_Thread_Mutex, int> m_nQueryCounter;  //counter for connection selection

        //lets use pool of connections for sync queries
        typedef std::vector< SqlConnection * > SqlConnectionContainer;
        SqlConnectionContainer m_pQueryConnections;

        SqlQueue* m_delayQueue;
        SqlQueue** m_serialDelayQueue; // simple mapping for worker ID -> serialized queue (only executed in set worker thread)

        SqlConnection * m_pAsyncConn;

        SqlResultQueue *    m_pResultQueue;                  ///< Transaction queues from diff. threads
        uint32              m_numAsyncWorkers;
        SqlDelayThread**    m_threadsBodies;                  ///< Pointer to delay sql executer (owned by m_delayThread)
        ACE_Based::Thread** m_delayThreads;                   ///< Pointer to executer thread

        bool m_bAllowAsyncTransactions;                      ///< flag which specifies if async transactions are enabled

        //PREPARED STATEMENT REGISTRY
        typedef ACE_Thread_Mutex LOCK_TYPE;
        typedef ACE_Guard<LOCK_TYPE> LOCK_GUARD;

        mutable LOCK_TYPE m_stmtGuard;

        typedef UNORDERED_MAP<std::string, int> PreparedStmtRegistry;
        PreparedStmtRegistry m_stmtRegistry;                 ///<

        int m_iStmtIndex;

    private:

        bool m_logSQL;
        std::string m_logsDir;
        uint32 m_pingIntervallms;
};
#endif
