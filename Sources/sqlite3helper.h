//-----------------------------------------------------------------------------
#ifndef __sqlite3_helper_h__
#define __sqlite3_helper_h__
//-----------------------------------------------------------------------------

#include "sqlite3db.h"
#include "S3DXAIPackage.h"

#define DMaxSqliteDbHandler 3

struct Sqlite3DbInfo
{
	int		      m_iSqlite3LastResult;
	sqlite3*      m_pDbHandle;
	int           m_iTransCount;
	sqlite3*      m_pDb;
	bool          m_bInited;
	bool          m_bInMemory;

	Sqlite3DbInfo()
	{
		m_iSqlite3LastResult = 0;
		m_pDbHandle   = nullptr;
		m_bInMemory   = false;
		m_iTransCount = 0;
		m_pDb         = nullptr;
		m_bInited     = false;
	}
	void Commit();
	void StartTransaction();
	void Rollback();

	int  OpenConnection(const char* sDbName, bool bInMemory);
	int  CloseConnection();

};

struct Sqlite3Helper 
{	
	Sqlite3Helper() {}
	~Sqlite3Helper() { CloseAllConnection(); }

	Sqlite3DbInfo m_DbHandler[DMaxSqliteDbHandler];
	
	static int		FindStateColumn(const char* ColumnName, sqlite3_stmt* pStmt);
	static int		ExecuteSQL(sqlite3 *pDb, const char *sSQL );
	static int		FindBindParam(const char* sParamName, sqlite3_stmt* pStmt);

	Sqlite3DbInfo*	GetConnection(int iIndex);
	int				HasFreeConnection();
	void			CloseAllConnection();
	int				CloseConnection(int iIndex);
	bool			IsValidDatabaseIndex(int iIndex);
	void			SetLastResult(int iIndex, int iValue);
	int				OpenConnection(const char* sDbName, const bool bInMemory, int& iDbIndex );
	int				StartTransaction(int iIndex);
	int				Rollback(int iIndex);
	int				Commit(int iIndex);
	int             ExecSql(int iIndex, const char* sSql);
	sqlite3_stmt*   Query(int iIndex, const char* sSql);
	int             QueryExecute(sqlite3_stmt* pStmt);
	int             QueryNext(sqlite3_stmt* pStmt);
	int             QueryClose(sqlite3_stmt* pStmt);
	bool			InTrasaction(int iIndex);
	int             Bind( sqlite3_stmt* pState, int iParam, const char* sParamValue );
};


#endif