//-----------------------------------------------------------------------------
#include "PrecompiledHeader.h"
//-----------------------------------------------------------------------------
#include "sqlite3helper.h"
#include <string.h>
#include <assert.h>


//-----------------------------------------------------------------------------
//  FindStateColumn
//-----------------------------------------------------------------------------
int Sqlite3Helper::FindStateColumn(const char* sColumnName, sqlite3_stmt* pStmt)
{
	assert(pStmt);
	if (!pStmt)
		return -1;

	for (int iIndex = 0; iIndex < sqlite3_column_count(pStmt); iIndex++ )
	{
		const char* sSqlColName = sqlite3_column_name(pStmt, iIndex);
		if (strcmp(sSqlColName, sColumnName) == 0)
			return iIndex;
	}
	return -1;
}


//-----------------------------------------------------------------------------
//  ExecuteSQL
//-----------------------------------------------------------------------------
int Sqlite3Helper::ExecuteSQL(sqlite3 *pDb, const char *sSQL )
{
	assert(pDb);

	if (!pDb)
		return SQLITE_ERROR;
	
	sqlite3_stmt *pStmt = nullptr;
	const char *tail    = nullptr;

	int iResult = sqlite3_prepare_v2( pDb, sSQL, strlen(sSQL), &pStmt, &tail);
	
	if ( iResult == SQLITE_OK && pStmt )
	{
		iResult = sqlite3_step(pStmt);
		if (iResult == SQLITE_DONE)
			iResult = SQLITE_OK;
	}
	
	if (pStmt)
		sqlite3_finalize(pStmt);
	
	return iResult;
}

//-----------------------------------------------------------------------------
//  FindBindParam
//-----------------------------------------------------------------------------
int Sqlite3Helper::FindBindParam(const char* sParamName, sqlite3_stmt* pStmt)
{
	return sqlite3_bind_parameter_index( pStmt, sParamName );
}


//-----------------------------------------------------------------------------
//  HasFreeConnection
//-----------------------------------------------------------------------------
int Sqlite3Helper::HasFreeConnection()
{
	for (int iIndex = 0; iIndex < DMaxSqliteDbHandler; iIndex++)
	{
		if (m_DbHandler[ iIndex ].m_bInited == false)
			return iIndex;
	}
	return -1;
}

//-----------------------------------------------------------------------------
//  HasFreeConnection
//-----------------------------------------------------------------------------
void  Sqlite3Helper::CloseAllConnection()
{
	for (int iIndex = 0; iIndex < DMaxSqliteDbHandler; iIndex++)
	{
		Sqlite3DbInfo& DbInfo = m_DbHandler[iIndex];
		if ( DbInfo.m_bInited )
		{
			DbInfo.CloseConnection();
		}
	}
}

//-----------------------------------------------------------------------------
//  GetConnection
//-----------------------------------------------------------------------------
Sqlite3DbInfo* Sqlite3Helper::GetConnection(int iIndex)
{
	if (iIndex < 0 || iIndex >= DMaxSqliteDbHandler)
		return nullptr;
	return &m_DbHandler[iIndex];
}

//-----------------------------------------------------------------------------
//  CloseConnection
//-----------------------------------------------------------------------------
int Sqlite3DbInfo::CloseConnection()
{
	if ( m_bInited && m_pDb )
	{ 
		
		int m_iSqlite3LastResult = sqlite3_close(m_pDb);
		if ( m_iSqlite3LastResult == SQLITE_OK)
		{
			m_pDb = nullptr;
			m_bInited = false;
		}
		return m_iSqlite3LastResult;
	}
	return SQLITE_ERROR;
}

//-----------------------------------------------------------------------------
//  IsValidDatabaseIndex
//-----------------------------------------------------------------------------
bool Sqlite3Helper::IsValidDatabaseIndex(int iIndex)
{
	if (iIndex < 0 || iIndex >= DMaxSqliteDbHandler)
		return false;
	return m_DbHandler[iIndex].m_bInited;
}

//-----------------------------------------------------------------------------
//  CloseConnection
//-----------------------------------------------------------------------------
int Sqlite3Helper::CloseConnection(int iIndex)
{
	return m_DbHandler[iIndex].CloseConnection();
}

//-----------------------------------------------------------------------------
//  SetLastResult
//-----------------------------------------------------------------------------
void Sqlite3Helper::SetLastResult(int iIndex, int iValue)
{
	m_DbHandler[iIndex].m_iSqlite3LastResult = iValue;
}

//-----------------------------------------------------------------------------
//  OpenConnection
//-----------------------------------------------------------------------------
int Sqlite3Helper::OpenConnection(const char* sDbName, const bool bInMemory, int& iDbIndex)
{
	for (int iIndex = 0; iIndex < DMaxSqliteDbHandler; iIndex++ )
	{
		if (!m_DbHandler[iIndex].m_bInited)
		{
			iDbIndex = iIndex;
			break;
		}
	}

	if (iDbIndex > -1)
		return m_DbHandler[iDbIndex].OpenConnection(sDbName, bInMemory);

	return SQLITE_ERROR;
}

//-----------------------------------------------------------------------------
//  StartTransaction
//-----------------------------------------------------------------------------
int Sqlite3Helper::StartTransaction(int iIndex)
{
	char* cerr = 0;
	int iResult = sqlite3_exec( m_DbHandler[iIndex].m_pDb , "BEGIN TRANSACTION;", NULL, 0, &cerr);
	if (iResult == SQLITE_OK)
		m_DbHandler[iIndex].StartTransaction();
	sqlite3_free(cerr);
	return iResult;
}

//-----------------------------------------------------------------------------
//  Rollback
//-----------------------------------------------------------------------------
int Sqlite3Helper::Rollback(int iIndex)
{
	char* cerr = 0;
	int iResult = sqlite3_exec(m_DbHandler[iIndex].m_pDb, "ROLLBACK TRANSACTION;", NULL, 0, &cerr);
	if (iResult == SQLITE_OK)
		m_DbHandler[iIndex].Rollback();
	sqlite3_free(cerr);
	return iResult;
}

//-----------------------------------------------------------------------------
//  Commit
//-----------------------------------------------------------------------------
int Sqlite3Helper::Commit(int iIndex)
{
	char* cerr = 0;
	int iResult = sqlite3_exec(m_DbHandler[iIndex].m_pDb, "COMMIT TRANSACTION;", NULL, 0, &cerr);
	if (iResult == SQLITE_OK)
		m_DbHandler[iIndex].Rollback();
	sqlite3_free(cerr);
	return iResult;
}

//-----------------------------------------------------------------------------
//  ExecSql
//-----------------------------------------------------------------------------
int  Sqlite3Helper::ExecSql(int iIndex, const char* sSql)
{
	m_DbHandler[iIndex].m_iSqlite3LastResult = Sqlite3Helper::ExecuteSQL( m_DbHandler[iIndex].m_pDb, sSql );
	return m_DbHandler[iIndex].m_iSqlite3LastResult;
}

//-----------------------------------------------------------------------------
//  Query
//-----------------------------------------------------------------------------
sqlite3_stmt* Sqlite3Helper::Query(int iIndex, const char* sSql )
{
	const char*   pDbTail = nullptr;
	sqlite3_stmt* pDbStmt = nullptr;
	int iResult = sqlite3_prepare_v2( m_DbHandler[iIndex].m_pDb, sSql, strlen(sSql), &pDbStmt, &pDbTail);
	m_DbHandler[iIndex].m_iSqlite3LastResult = iResult;
	return pDbStmt;
}

//-----------------------------------------------------------------------------
//  QueryExecute
//-----------------------------------------------------------------------------
int  Sqlite3Helper::QueryExecute(sqlite3_stmt* pStmt)
{
	if (!pStmt)
		return SQLITE_ERROR;
	int iResult = sqlite3_step(pStmt);
	if (iResult == SQLITE_DONE || iResult == SQLITE_ROW )
		iResult = SQLITE_OK;
	return iResult;
}

//-----------------------------------------------------------------------------
//  Bind
//-----------------------------------------------------------------------------
int Sqlite3Helper::Bind( sqlite3_stmt* pStmt, int iParam, const char* sParamValue)
{
	return sqlite3_bind_text(pStmt, iParam, sParamValue, strlen(sParamValue), SQLITE_TRANSIENT);
}

//-----------------------------------------------------------------------------
//  QueryClose
//-----------------------------------------------------------------------------
int Sqlite3Helper::QueryClose(sqlite3_stmt* pStmt)
{
	if (!pStmt)
		return SQLITE_ERROR;

	return sqlite3_finalize(pStmt);
}

//-----------------------------------------------------------------------------
//  QueryNext
//-----------------------------------------------------------------------------
int Sqlite3Helper::QueryNext(sqlite3_stmt* pStmt)
{
	if (!pStmt)
		return SQLITE_ERROR;

	return sqlite3_step(pStmt);
}

//-----------------------------------------------------------------------------
//  InTrasaction
//-----------------------------------------------------------------------------
bool Sqlite3Helper::InTrasaction(int iIndex)
{
	return m_DbHandler[iIndex].m_iTransCount > 0;
}

//-----------------------------------------------------------------------------
//  Sqlite3DbInfo
//-----------------------------------------------------------------------------

#define DDecrementTransaction 	m_iTransCount--; if (m_iTransCount < 0) m_iTransCount = 0;

//-----------------------------------------------------------------------------
//  Commit
//-----------------------------------------------------------------------------
void Sqlite3DbInfo::Commit()
{
	DDecrementTransaction
}

//-----------------------------------------------------------------------------
//  StartTransaction
//-----------------------------------------------------------------------------
void Sqlite3DbInfo::StartTransaction()
{
	m_iTransCount++;
}

//-----------------------------------------------------------------------------
//  Rollback
//-----------------------------------------------------------------------------
void Sqlite3DbInfo::Rollback()
{
	DDecrementTransaction
}

//-----------------------------------------------------------------------------
//  OpenConnection
//-----------------------------------------------------------------------------
int Sqlite3DbInfo::OpenConnection(const char* sDbName, bool bInMemory)
{
	assert(!m_pDb);
	if (m_bInited)
		return SQLITE_ERROR;

	m_iSqlite3LastResult = sqlite3_open(sDbName, &m_pDb);
	m_bInited = m_iSqlite3LastResult == SQLITE_OK;
	m_bInMemory = bInMemory;
	return m_iSqlite3LastResult;
}



