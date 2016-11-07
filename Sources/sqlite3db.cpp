//-----------------------------------------------------------------------------
#include "PrecompiledHeader.h"
#include "sqlite3db.h"
#include "sqlite3helper.h"

using namespace std;

//-----------------------------------------------------------------------------

#ifdef S3DX_DLL
	sqlite3dbAPI sqlite3db ;
#endif

	static Sqlite3Helper SqliteHelper;

#if defined(_DEBUG)	
	#define SQLLOG(AMSG) S3DX::log.message( S3DX::AIVariable(AMSG) )
	#define SQLLOG1(AMSG,APARAM1) S3DX::log.message( S3DX::AIVariable(AMSG), APARAM1 )
	#define SQLLOG2(AMSG,APARAM1,APARAM2) S3DX::log.message( S3DX::AIVariable(AMSG), APARAM1, APARAM2 )
#else
	#define SQLLOG
	#define SQLLOG1
	#define SQLLOG2
#endif


//-----------------------------------------------------------------------------
//  Callbacks
//-----------------------------------------------------------------------------


	//-----------------------------------------------------------------------------
	//  Sqlite3 Get Statement Column type
	//-----------------------------------------------------------------------------
int Callback_sqlite3db_GetColumnType ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.GetColumnType" ) ;

    // Input Parameters 
    int iInputCount = 0 ;
    S3DX::AIVariable iCmdHandle  = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;
    S3DX::AIVariable nsIndexName = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable nType ;

#   pragma TODO( write the code for nType = sqlite3db.GetColumnType ( iCmdHandle, nsIndexName ) )
	if (iCmdHandle.IsHandle() && (nsIndexName.IsNumber() || nsIndexName.IsString()  ) )
	{
		sqlite3_stmt *pStmt = (sqlite3_stmt *) iCmdHandle.GetHandleValue();
		if ( pStmt == nullptr ) 
			nType.SetNil();
		else
		{
			if ( nsIndexName.IsNumber() )
				nType = sqlite3_column_type( pStmt, nsIndexName.GetNumberValue() );
			else 
			{
				const char *sName = nsIndexName.GetStringValue();
				int iIndex = Sqlite3Helper::FindStateColumn( sName, pStmt );
				if ( iIndex != -1)
					 nType = sqlite3_column_type(pStmt, nsIndexName.GetNumberValue()) ;
				else 
					nType.SetNil(); 
			}							
		}
	}
	else 
		nType.SetNil();

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = nType ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
	//  Sqlite3 attach more database to opened database not work with mem database
	//-----------------------------------------------------------------------------


int Callback_sqlite3db_AttachDatabase ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.AttachDatabase" ) ;

    // Input Parameters 
    int iInputCount = 0 ;
	S3DX::AIVariable iDbIndex      = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( );
    S3DX::AIVariable sFileName     = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;
    S3DX::AIVariable sDatabaseName = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable bOk ;

#   pragma TODO( write the code for bOk = sqlite3db.AttachDatabase ( sFileName, sDatabaseName ) )

	if ( !iDbIndex.IsNumber() || !SqliteHelper.IsValidDatabaseIndex( iDbIndex) || !sFileName.IsString() || !sDatabaseName.IsString() )
	{
		SQLLOG("AttachDatabase failed, invalid parameters!");
		bOk.SetBooleanValue(false);
		if (iDbIndex.IsNumber() )
			SqliteHelper.SetLastResult(iDbIndex, SQLITE_ERROR);
	} 
	else
	{
		SQLLOG1("AttachDatabase success ", sFileName);
		S3DX::AIVariable sSql = S3DX::string.format( S3DX::AIVariable("ATTACH '%s' AS '%s';"),sFileName,sDatabaseName);
		bOk = SqliteHelper.ExecSql(iDbIndex, sSql.GetStringValue()) == SQLITE_OK;
	}  

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = bOk ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
	//  Sqlite3 detach selected database not work with memory database
	//-----------------------------------------------------------------------------

int Callback_sqlite3db_DetachDatabase ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.DetachDatabase" ) ;

    // Input Parameters 
    int iInputCount = 0 ;
	S3DX::AIVariable iDbIndex      = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( );
    S3DX::AIVariable sDatabaseName = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable bOk;

#   pragma TODO( write the code for bOk = sqlite3db.DetachDatabase ( sDatabaseName ) )
	
	if ( !iDbIndex.IsNumber() || !SqliteHelper.IsValidDatabaseIndex( iDbIndex ) || !sDatabaseName.IsString() )
	{
		SQLLOG("DetachDatabase failed, invalid parameters!");
		bOk.SetBooleanValue(false);
		if ( iDbIndex.IsNumber() )
			SqliteHelper.SetLastResult(iDbIndex, SQLITE_ERROR);
	} 
	else
	{
		SQLLOG("DetachDatabase success!");
		 S3DX::AIVariable sSql = S3DX::string.format(S3DX::AIVariable("DETACH '%s';"),sDatabaseName);
		 bOk = SqliteHelper.ExecSql(iDbIndex, sSql.GetStringValue()) == SQLITE_OK;
	}  

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = bOk ;

    S3DX_API_PROFILING_STOP( );
    return iReturnCount;
}

//-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
	//  Sqlite3 set special database parameters
	//-----------------------------------------------------------------------------

int Callback_sqlite3db_Pragma ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.Pragma" ) ;

    // Input Parameters 
    int iInputCount = 0 ;
	S3DX::AIVariable iDbIndex = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( );
    S3DX::AIVariable sName    = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;
    S3DX::AIVariable sValue   = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable bOk ;


#   pragma TODO( write the code for bOk = sqlite3db.Pragma ( sName, sValue ) )

	if ( !iDbIndex.IsNumber() || !SqliteHelper.IsValidDatabaseIndex( iDbIndex ) || !sName.IsString() || !sValue.IsString() )
	{
		SQLLOG("Pragma failed, invalid parameters!");
		bOk.SetBooleanValue(false);
		if (iDbIndex.IsNumber())
			SqliteHelper.SetLastResult(iDbIndex, SQLITE_ERROR);
	} 
	else
	{
		S3DX::AIVariable sSql = S3DX::string.format(S3DX::AIVariable("PRAGMA %s = %s ;"),sName,sValue);
		SQLLOG1("Pragma success ", sSql);
		bOk = SqliteHelper.ExecSql(iDbIndex, sSql.GetStringValue()) == SQLITE_OK;
	} 

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = bOk ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
	//  Sqlite3 rollback database state to saved point
	//-----------------------------------------------------------------------------


int Callback_sqlite3db_RollbackSavePoint ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.RollbackSavePoint" ) ;

    // Input Parameters 
    int iInputCount = 0 ;
	S3DX::AIVariable iDbIndex = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( );
    S3DX::AIVariable sName    = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable bOk ;

#   pragma TODO( write the code for bOk = sqlite3db.RollbackSavePoint ( sName ) )

	if (  !iDbIndex.IsNumber() || !SqliteHelper.IsValidDatabaseIndex( iDbIndex) || !sName.IsString() )
	{
		bOk = false;
		SQLLOG("RollbackSavePoint failed, invalid parameters");
		if ( iDbIndex.IsNumber() )
			SqliteHelper.SetLastResult( iDbIndex, SQLITE_ERROR);
	} 
	else
	{
		S3DX::AIVariable sSQL = S3DX::string.format(S3DX::AIVariable("ROLLBACK TO %s ; "),sName);
		bOk = SqliteHelper.ExecSql(iDbIndex, sSQL.GetStringValue()) == SQLITE_OK;
	}

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = bOk ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_sqlite3db_ReleaseSavePoint ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.ReleaseSavePoint" ) ;

    // Input Parameters 
    int iInputCount = 0 ;
	S3DX::AIVariable iDbIndex = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable( );
    S3DX::AIVariable sName    = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable( ) ;

    // Output Parameters 
    S3DX::AIVariable bOk ;

#   pragma TODO( write the code for bOk = sqlite3db.Release ( sName ) )

	if ( !iDbIndex.IsNumber() || !sName.IsString() )
	{
		bOk = false;
		SqliteHelper.SetLastResult( iDbIndex, SQLITE_ERROR );
	} 
	else
	{
		S3DX::AIVariable sSQL = S3DX::string.format(S3DX::AIVariable("RELEASE %s ; "), sName);
		bOk = SqliteHelper.ExecSql(iDbIndex, sSQL.GetStringValue()) == SQLITE_OK;
	}

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = bOk ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_sqlite3db_SavePoint ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.SavePoint" ) ;

    // Input Parameters 
	int iInputCount = 0;
	S3DX::AIVariable iDbIndex = ( iInputCount < _iInCount)  ? _pIn[iInputCount++] : S3DX::AIVariable ( );
    S3DX::AIVariable sName    = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable bOk ;


#   pragma TODO( write the code for bOk = sqlite3db.SavePoint ( iDbIndex, sName ) )

	if (  !iDbIndex.IsNumber() || !SqliteHelper.IsValidDatabaseIndex( iDbIndex) || !sName.IsString() )
	{
		bOk.SetBooleanValue(false);
		if ( iDbIndex.IsNumber() )
		 SqliteHelper.SetLastResult( iDbIndex, SQLITE_ERROR );
	} 
	else
	{
		S3DX::AIVariable sSql = S3DX::string.format(S3DX::AIVariable("SAVEPOINT %s ;"), sName);
		bOk = SqliteHelper.ExecSql(iDbIndex, sSql.GetStringValue()) == SQLITE_OK;
	}

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = bOk ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_sqlite3db_isInTransaction ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.isInTransaction" ) ;
  
	// Input Parameters 
	int iInputCount = 0;
	S3DX::AIVariable iDbIndex = (iInputCount < _iInCount) ? _pIn[iInputCount++] : S3DX::AIVariable();

    // Output Parameters 
    S3DX::AIVariable bYes = false;

#   pragma TODO( write the code for bYes = sqlite3db.isInTransaction ( iDbIndex ) )
	
	if ( iDbIndex.IsNumber() && SqliteHelper.IsValidDatabaseIndex( iDbIndex) )
		bYes = SqliteHelper.InTrasaction(iDbIndex);
	
    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = bYes ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_sqlite3db_GetColumnCount ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.GetColumnCount" ) ;

    // Input Parameters 
    int iInputCount = 0 ;
    S3DX::AIVariable iCmdHandle = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable nCount ;

#   pragma TODO( write the code for nCount = sqlite3db.GetColumnCount ( iCmdHandle ) )
	if ( !iCmdHandle.IsHandle())
		nCount = 0;
	else
	{
		if (sqlite3_stmt *pstmt = (sqlite3_stmt*)iCmdHandle.GetHandleValue())
			nCount = 0;
		else
			nCount = sqlite3_column_count(pstmt);
	}

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = nCount ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_sqlite3db_GetColumnAt ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.GetColumnAt" ) ;

    // Input Parameters 
    int iInputCount = 0 ;
    S3DX::AIVariable iCmdHandle = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;
    S3DX::AIVariable nIndex     = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable sColumn ;

#   pragma TODO( write the code for sColumn = sqlite3db.GetColumnAt ( iCmdHandle, nIndex ) )
    
	if ( !iCmdHandle.IsHandle())
		sColumn.SetNil();
	else
	{
		if ( sqlite3_stmt *pStmt = (sqlite3_stmt*)iCmdHandle.GetHandleValue() )
			sColumn.SetNil();
		else
			sColumn = sqlite3_column_name(pStmt, nIndex.GetNumberValue());
	}

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = sColumn ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_sqlite3db_RowChanges ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.RowChanges" ) ;
  
	// Input Parameters 
	int iInputCount = 0;
	S3DX::AIVariable iDbIndex = (iInputCount < _iInCount) ? _pIn[iInputCount++] : S3DX::AIVariable();

    // Output Parameters 
	S3DX::AIVariable nCount;

#   pragma TODO( write the code for nCount = sqlite3db.RowChanges ( iDbIndex ) )
	
	if ( !iDbIndex.IsNumber() || !SqliteHelper.IsValidDatabaseIndex(iDbIndex) )
		nCount = 0;
	else
		nCount.SetNumberValue(sqlite3_changes(SqliteHelper.GetConnection(iDbIndex)->m_pDb));
	
    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = nCount ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_sqlite3db_RowCount ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.RowCount" ) ;

    // Input Parameters 
    int iInputCount = 0 ;
    S3DX::AIVariable iCmdHandle = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable nCount = 0;

#   pragma TODO( write the code for nCount = sqlite3db.RowCount ( iCmdHandle ) )
	if ( iCmdHandle.IsHandle() ) {
		if ( sqlite3_stmt *pstmt = (sqlite3_stmt*)iCmdHandle.GetHandleValue() )
		{
			#pragma warning( disable : 4244 )
			nCount.SetNumberValue(sqlite3_data_count(pstmt) );
		}
	}

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = nCount ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_sqlite3db_GetFieldValue ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.GetFieldValue" ) ;

    // Input Parameters 
    int iInputCount = 0 ;
    S3DX::AIVariable iCmdHandle       = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;
    S3DX::AIVariable siFieldIndexName = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable sValue ;

#   pragma TODO( write the code for sValue = sqlite3db.GetFieldValue ( iCmdHandle, siFieldIndexName ) )

	if (iCmdHandle.IsHandle() && (siFieldIndexName.IsNumber() || siFieldIndexName.IsString()  ) )
	{
		
		sqlite3_stmt *pStmt = (sqlite3_stmt*)iCmdHandle.GetHandleValue();
			
		if ( pStmt == nullptr )
		{
			sValue.SetNil();
		} 
		else
		{
			int iFieldIndex = -1;

			if (siFieldIndexName.IsNumber())
				iFieldIndex = siFieldIndexName.GetNumberValue();
			else if (siFieldIndexName.IsString())
				iFieldIndex = Sqlite3Helper::FindStateColumn(siFieldIndexName.GetStringValue(), pStmt);

			if (iFieldIndex > -1)
			{
				
				#pragma warning( disable : 4244 )
				sqlite3_value *fv = sqlite3_column_value(pStmt, iFieldIndex );

				switch (sqlite3_value_type(fv))
				{
				case SQLITE_NULL:
				{
					sValue.SetNil();
					break;
				}
				case SQLITE_INTEGER:
				{
					sqlite3_int64 va = sqlite3_value_int64(fv);
					sValue.SetNumberValue(va);
					break;
				}
				case SQLITE_FLOAT:
				{
					double vd = sqlite3_value_double(fv);
					sValue.SetNumberValue(vd);
					break;
				}
				case SQLITE3_TEXT:
				{
					const unsigned char * text = sqlite3_value_text(fv);
					sValue.SetStringValue((const char*)text);
					break;
				}
				case SQLITE_BLOB:
				{
					const void *vp = sqlite3_value_blob(fv);
					int   abytes = sqlite3_value_bytes(fv);
					sValue.SetStringValue((const char*)vp);
					break;
				}
				default:
					sValue.SetNil();
					break;
				}

			}
			else
				sValue.SetNil();
		}
	}
	else
	{
		sValue.SetNil();
	}

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = sValue ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_sqlite3db_QueryNext ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.QueryNext" ) ;

    // Input Parameters 
    int iInputCount = 0 ;
    S3DX::AIVariable iCmdHandle = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable bOk ;
	
#   pragma TODO( write the code for bok = sqlite3db.SqlCommandNext ( iCmdHandle ) )

	if (!iCmdHandle.IsHandle())
	{ 
		bOk = false;
		SQLLOG("QueryNext wrong parameters");
	}
	else
	{ 
		int iResult = SqliteHelper.QueryNext((sqlite3_stmt*)iCmdHandle.GetHandleValue());
		SQLLOG("QueryNext result ", S3DX::AIVariable(iResult) );
		bOk = iResult == SQLITE_ROW;
	}
    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = bOk ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_sqlite3db_QueryClose ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.QueryClose" ) ;

    // Input Parameters 
    int iInputCount = 0 ;
    S3DX::AIVariable iCmdHandle = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable bOk ;


#   pragma TODO( write the code for bok = sqlite3db.SqlCommandClose ( iCmdHandle ) )
	if (!iCmdHandle.IsHandle())
		bOk = false;
	else
		bOk = SqliteHelper.QueryClose((sqlite3_stmt*)iCmdHandle.GetHandleValue()) == SQLITE_OK;

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = bOk ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}


//-----------------------------------------------------------------------------

int Callback_sqlite3db_Query ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.Query" ) ;

    // Input Parameters 
    int iInputCount = 0 ;
	S3DX::AIVariable iDbIndex  = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;
    S3DX::AIVariable sParamSql = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable iCmdHandle ;

#   pragma TODO( write the code for iCmdHandle = sqlite3db.Query ( iDbIndex, sParamSql ) )

	// param check
	if (!iDbIndex.IsNumber() || !sParamSql.IsString() || !SqliteHelper.IsValidDatabaseIndex(iDbIndex))
	{
		iCmdHandle.SetNil();
		if (iDbIndex.IsNumber())
			SqliteHelper.SetLastResult(iDbIndex, SQLITE_ERROR);
	}
	else
		iCmdHandle.SetHandleValue( SqliteHelper.Query(iDbIndex, sParamSql.GetStringValue() ));
	
    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = iCmdHandle ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_sqlite3db_QueryExecute ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.QueryExecute" ) ;

    // Input Parameters 
    int iInputCount = 0 ;
    S3DX::AIVariable iCmdHandle = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable bOk ;

#   pragma TODO( write the code for bok = sqlite3db.QueryExecute ( iCmdHandle ) )

	if (!iCmdHandle.IsHandle())
		bOk = false;
	else
		bOk = SqliteHelper.QueryExecute((sqlite3_stmt*)iCmdHandle.GetHandleValue()) == SQLITE_OK;

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = bOk ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_sqlite3db_Bind ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.Bind" ) ;

    // Input Parameters 
    int iInputCount = 0 ;
    S3DX::AIVariable iCmdHandle  = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;
    S3DX::AIVariable sParamName  = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;
    S3DX::AIVariable sParamValue = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable bOk ;


#   pragma TODO( write the code for bok = sqlite3db.QueryParam ( iCmdHandle, sParamName, sParamValue ) )

	if ( iCmdHandle.IsHandle() && ( sParamName.IsString() || sParamName.IsNumber()  ) && sParamValue.IsString() )
	{
		sqlite3_stmt *pStmt = (sqlite3_stmt*)iCmdHandle.GetHandleValue();
		bOk = false;

		if ( pStmt )
		{
			const char *sparam = sParamValue.GetStringValue();
			#pragma warning( disable : 4244 )
			
			if (sParamName.IsNumber())
			{				
				if (sParamName.GetNumberValue() == 0)
					bOk = false;
				else
					bOk = SqliteHelper.Bind(pStmt, sParamName, sparam) == SQLITE_OK;

				SQLLOG1("Bind by index ", bOk);
			}
			else
			{
				int iParam = Sqlite3Helper::FindBindParam(sParamName.GetStringValue(), pStmt);
				if (iParam > 0)
				{
					bOk = SqliteHelper.Bind(pStmt, iParam, sparam) == SQLITE_OK;
				}
				SQLLOG1("Bind by name ", bOk);
			}

		}
	} 
	else
	{
		SQLLOG("Bind failed, invalid parameters!");
		bOk = false;
	}
    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = bOk;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_sqlite3db_GetLastErrorStr ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.GetLastErrorStr" ) ;

	// Input Parameters 
	int iInputCount = 0;
	S3DX::AIVariable iDbIndex = (iInputCount < _iInCount) ? _pIn[iInputCount++] : S3DX::AIVariable();
	
	// Output Parameters 
    S3DX::AIVariable sMessage ;
	#   pragma TODO( write the code for sMessage = sqlite3db.GetLastErrorStr ( ) )
	
	if (!iDbIndex.IsNumber() || !SqliteHelper.IsValidDatabaseIndex(iDbIndex))
		sMessage.SetNil();
	else
		sMessage = sqlite3_errmsg(SqliteHelper.GetConnection(iDbIndex)->m_pDb);
	
    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = sMessage ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_sqlite3db_GetLastErrorCode ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.GetLastErrorCode" ) ;

	// Input Parameters 
	int iInputCount = 0;
	S3DX::AIVariable iDbIndex = (iInputCount < _iInCount) ? _pIn[iInputCount++] : S3DX::AIVariable();

    // Output Parameters 
    S3DX::AIVariable iCode ;
	
	#pragma warning( disable : 4244 )
	
	if (!iDbIndex.IsNumber() || !SqliteHelper.IsValidDatabaseIndex(iDbIndex))
		iCode = -1;
	else
		iCode = SqliteHelper.GetConnection(iDbIndex)->m_iSqlite3LastResult;

#   pragma TODO( write the code for iCode = sqlite3db.GetLastErrorCode (  ) )


    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = iCode ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_sqlite3db_Commit ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.Commit" ) ;

	// Input Parameters 
	int iInputCount = 0;
	S3DX::AIVariable iDbIndex = (iInputCount < _iInCount) ? _pIn[iInputCount++] : S3DX::AIVariable();

    // Output Parameters 
    S3DX::AIVariable bOk ;

	#   pragma TODO( write the code for bOk = sqlite3db.Commit () )
	
	if (!iDbIndex.IsNumber() || !SqliteHelper.IsValidDatabaseIndex(iDbIndex))
	{
		bOk = false;
		SQLLOG("Commint invalid parameters!");
	}
	else
	{
		int iResult = SqliteHelper.Commit(iDbIndex);
		SQLLOG("Commit result ", S3DX::AIVariable( iResult) );
		bOk = iResult == SQLITE_OK;
	}
	
    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = bOk ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_sqlite3db_Rollback ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.Rollback" ) ;

	// Input Parameters 
	int iInputCount = 0;
	S3DX::AIVariable iDbIndex = (iInputCount < _iInCount) ? _pIn[iInputCount++] : S3DX::AIVariable();

    // Output Parameters 
    S3DX::AIVariable bOk ;

#   pragma TODO( write the code for bOk = sqlite3db.Rollback () )
	
	if (!iDbIndex.IsNumber() || !SqliteHelper.IsValidDatabaseIndex(iDbIndex))
	{
		bOk = false;
		SQLLOG("Rollback failed, invalid parameters!");
	}
	else
	{
		int iResult = SqliteHelper.Rollback(iDbIndex);
		SQLLOG1("Rollback code", S3DX::AIVariable(iResult));
		bOk = iResult == SQLITE_OK;
	}
	
    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = bOk ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_sqlite3db_StartTransaction ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.StartTransaction" ) ;

	// Input Parameters 
	int iInputCount = 0;
	S3DX::AIVariable iDbIndex = (iInputCount < _iInCount) ? _pIn[iInputCount++] : S3DX::AIVariable();

    // Output Parameters 
    S3DX::AIVariable bOk ;

#   pragma TODO( write the code for bOk = sqlite3db.StartTransaction () )
	
	if (!iDbIndex.IsNumber() || !SqliteHelper.IsValidDatabaseIndex(iDbIndex))
	{
		bOk = false;
		SQLLOG("StartTransaction failed, invalid parameters");
	}
	else
	{
		int iResult = SqliteHelper.StartTransaction(iDbIndex);
		SQLLOG1("StartTransaction code ", S3DX::AIVariable(iResult));
		bOk =  iResult == SQLITE_OK;
	}

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = bOk ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------
int Callback_sqlite3db_SqlExecute ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.SqlExecute" ) ;

    // Input Parameters 
    int iInputCount = 0 ;
	S3DX::AIVariable iDbIndex    = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;
	S3DX::AIVariable sParamSql   = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;
	
	// Output Parameters 
    S3DX::AIVariable bOk ;
	
#   pragma TODO( write the code for bok = sqlite3db.SqlExecute ( iDbIndex, sParamSql ) )
	
	if (!iDbIndex.IsNumber() || !sParamSql.IsString() || !SqliteHelper.IsValidDatabaseIndex(iDbIndex))
	{
		bOk = false;
		SQLLOG("SQLExecute falied , invalid params");
	}
	else
	{
		int iResult = SqliteHelper.ExecSql(iDbIndex, sParamSql.GetStringValue());
		bOk = iResult == SQLITE_OK;
		SQLLOG1("SQLExecute code ", S3DX::AIVariable(iResult));
	}
    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = bOk ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_sqlite3db_CreateMemDatabase ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.CreateMemDatabase" ) ;

    // Output Parameters 
    S3DX::AIVariable bOk ;
	S3DX::AIVariable iDatabaseIndex;

#   pragma TODO( write the code for bOk - iDbIndex = sqlite3db.CreateMemDatabase (  ) )
	

	const char* sMemory = ":memory:";
	
	if ( SqliteHelper.HasFreeConnection() == -1 )
	{
		bOk = false;
		iDatabaseIndex = -1;
	}
	else
	{
		int iDbIndex = -1;
		int iResult = SqliteHelper.OpenConnection( sMemory, true , iDbIndex);
		bOk.SetBooleanValue(iResult == SQLITE_OK);
		iDatabaseIndex = iDbIndex;
	}

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = bOk ;
	_pOut[iReturnCount++] = iDatabaseIndex;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}


//-----------------------------------------------------------------------------

int Callback_sqlite3db_OpenDatabase ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.OpenDatabase" ) ;

    // Input Parameters 
    int iInputCount = 0 ;
    S3DX::AIVariable sDatabase = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable bOk ;
	S3DX::AIVariable iDatabaseIndex;

#   pragma TODO( write the code for bOk - iDbIndex = sqlite3db.OpenDatabase ( sDatabase ) )
	S3DX::log.message(" Open sqlite database free connections ", S3DX::AIVariable(SqliteHelper.HasFreeConnection()) );
	if (!sDatabase.IsString() || SqliteHelper.HasFreeConnection() == -1 )
	{
		S3DX::log.message(" Open sqlite database  failed ", !sDatabase);
		bOk = false;
		iDatabaseIndex = -1;
	}
	else
	{
		int iDbIndex = -1;
		int iResult = SqliteHelper.OpenConnection(sDatabase.GetStringValue(), false, iDbIndex);
		S3DX::log.message(" Open sqlite database ",  sDatabase, S3DX::AIVariable(" "),S3DX::AIVariable(iResult));
		bOk.SetBooleanValue(iResult == SQLITE_OK);
		iDatabaseIndex = iDbIndex;
	}
    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = bOk ;
	_pOut[iReturnCount++] = iDatabaseIndex;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}


//-----------------------------------------------------------------------------

int Callback_sqlite3db_CloseDatabase ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "sqlite3db.CloseDatabase" ) ;
	
	// Input Parameters 
	int iInputCount = 0;
	S3DX::AIVariable iDatabaseIndex = (iInputCount < _iInCount) ? _pIn[iInputCount++] : S3DX::AIVariable();

	// Output Parameters 
    S3DX::AIVariable bOk ;

	#   pragma TODO( write the code for bok = sqlite3db.CloseDatabase ( iDatabaseIndex ) )

	if ( !iDatabaseIndex.IsNumber() || !SqliteHelper.IsValidDatabaseIndex(iDatabaseIndex ) )
	{
		bOk.SetBooleanValue(false);
		SqliteHelper.SetLastResult( iDatabaseIndex, SQLITE_ERROR );
		SQLLOG("CloseDatabase failed, invalid parameters!");
	}
	else
	{
		int iResult = SqliteHelper.CloseConnection(iDatabaseIndex);
		bOk.SetBooleanValue( iResult == SQLITE_OK);
		SQLLOG1("Close database code ", S3DX::AIVariable(iResult));
	}

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = bOk ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------
//  Constructor / Destructor
//-----------------------------------------------------------------------------

sqlite3dbPackage::sqlite3dbPackage ( )
{
#ifdef S3DX_DLL
    sqlite3db.pfn_sqlite3db_GetColumnType     = Callback_sqlite3db_GetColumnType     ;    
	sqlite3db.pfn_sqlite3db_AttachDatabase    = Callback_sqlite3db_AttachDatabase    ;
    sqlite3db.pfn_sqlite3db_DetachDatabase    = Callback_sqlite3db_DetachDatabase    ;
    sqlite3db.pfn_sqlite3db_Pragma            = Callback_sqlite3db_Pragma            ;
    sqlite3db.pfn_sqlite3db_RollbackSavePoint = Callback_sqlite3db_RollbackSavePoint ;
    sqlite3db.pfn_sqlite3db_ReleaseSavePoint  = Callback_sqlite3db_ReleaseSavePoint  ;
    sqlite3db.pfn_sqlite3db_SavePoint         = Callback_sqlite3db_SavePoint         ;
    sqlite3db.pfn_sqlite3db_isInTransaction   = Callback_sqlite3db_isInTransaction   ;
    sqlite3db.pfn_sqlite3db_GetColumnCount    = Callback_sqlite3db_GetColumnCount    ;
    sqlite3db.pfn_sqlite3db_GetColumnAt       = Callback_sqlite3db_GetColumnAt       ;
    sqlite3db.pfn_sqlite3db_RowChanges        = Callback_sqlite3db_RowChanges        ;
    sqlite3db.pfn_sqlite3db_RowCount          = Callback_sqlite3db_RowCount          ;
    sqlite3db.pfn_sqlite3db_GetFieldValue     = Callback_sqlite3db_GetFieldValue     ;
	sqlite3db.pfn_sqlite3db_QueryNext         = Callback_sqlite3db_QueryNext         ;
    sqlite3db.pfn_sqlite3db_QueryClose		  = Callback_sqlite3db_QueryClose		 ;
    sqlite3db.pfn_sqlite3db_Query			  = Callback_sqlite3db_Query             ;
    sqlite3db.pfn_sqlite3db_QueryExecute      = Callback_sqlite3db_QueryExecute      ;
    sqlite3db.pfn_sqlite3db_Bind			  = Callback_sqlite3db_Bind              ;
    sqlite3db.pfn_sqlite3db_GetLastErrorStr   = Callback_sqlite3db_GetLastErrorStr   ;
    sqlite3db.pfn_sqlite3db_GetLastErrorCode  = Callback_sqlite3db_GetLastErrorCode  ;
    sqlite3db.pfn_sqlite3db_Commit            = Callback_sqlite3db_Commit            ;
    sqlite3db.pfn_sqlite3db_Rollback          = Callback_sqlite3db_Rollback          ;
    sqlite3db.pfn_sqlite3db_StartTransaction  = Callback_sqlite3db_StartTransaction  ;
    sqlite3db.pfn_sqlite3db_SqlExecute        = Callback_sqlite3db_SqlExecute        ;
    sqlite3db.pfn_sqlite3db_CreateMemDatabase = Callback_sqlite3db_CreateMemDatabase ;
    sqlite3db.pfn_sqlite3db_OpenDatabase	  = Callback_sqlite3db_OpenDatabase   ;
    sqlite3db.pfn_sqlite3db_CloseDatabase     = Callback_sqlite3db_CloseDatabase  ;
	sqlite3db.kSqlite_ErrorCalling		= -1;
	sqlite3db.kSqlite_Ok				= 0;
	sqlite3db.kSqlite_SQLITE_ERROR		= 1;
	sqlite3db.kSqlite_SQLITE_INTERNAL	= 2;
	sqlite3db.kSqlite_SQLITE_PERM		= 3;
	sqlite3db.kSqlite_SQLITE_ABORT		= 4;
	sqlite3db.kSqlite_SQLITE_BUSY		= 5;
	sqlite3db.kSqlite_SQLITE_LOCKED		= 6;
	sqlite3db.kSqlite_SQLITE_NOMEM		= 7;
	sqlite3db.kSqlite_SQLITE_READONLY	= 8;
	sqlite3db.kSqlite_SQLITE_INTERRUPT	= 9;
	sqlite3db.kSqlite_SQLITE_IOERR		= 10;
	sqlite3db.kSqlite_SQLITE_CORRUPT	= 11;
	sqlite3db.kSqlite_SQLITE_NOTFOUND	= 12;
	sqlite3db.kSqlite_SQLITE_FULL		= 13;
	sqlite3db.kSqlite_SQLITE_CANTOPEN	= 14;
	sqlite3db.kSqlite_SQLITE_PROTOCOL	= 15;
	sqlite3db.kSqlite_SQLITE_EMPTY		= 16;
	sqlite3db.kSqlite_SQLITE_SCHEMA		= 17;
	sqlite3db.kSqlite_SQLITE_TOOBIG		= 18;
	sqlite3db.kSqlite_SQLITE_CONSTRAINT	= 19;
	sqlite3db.kSqlite_SQLITE_MISMATCH	= 20;
	sqlite3db.kSqlite_SQLITE_MISUSE		= 21;
	sqlite3db.kSqlite_SQLITE_NOLFS		= 22;
	sqlite3db.kSqlite_SQLITE_AUTH		= 23;
	sqlite3db.kSqlite_SQLITE_FORMAT		= 24;
	sqlite3db.kSqlite_SQLITE_RANGE		= 25;
	sqlite3db.kSqlite_SQLITE_NOTADB		= 26;	
	sqlite3db.kDataType_Integer			= 1;
	sqlite3db.kDataType_Float			= 2;
	sqlite3db.kDataType_Text			= 3;
	sqlite3db.kDataType_Blob			= 4;
	sqlite3db.kDataType_Null			= 5;
#endif
}

//-----------------------------------------------------------------------------

sqlite3dbPackage::~sqlite3dbPackage ( )
{

}

//-----------------------------------------------------------------------------
//  Functions table
//-----------------------------------------------------------------------------

static S3DX::AIFunction aMyFunctions [ ] =
{
    { "GetColumnType"    , Callback_sqlite3db_GetColumnType    , "nType"     , "iCmdHandle,nsIndexName"				, "Get column type by index or name"                   , 0 }, 
    { "AttachDatabase"   , Callback_sqlite3db_AttachDatabase   , "bOk"       , "iDbIndex,sFileName,sDatabaseName"	, "Attach new database to opened database"             , 0 }, 
    { "DetachDatabase"   , Callback_sqlite3db_DetachDatabase   , "bOk"       , "iDbIndex,sDatabaseName"				, "Detach  database from  opened databases"            , 0 }, 
    { "Pragma"           , Callback_sqlite3db_Pragma           , "bOk"       , "sName,sValue"						, "Set special database settings"                      , 0 }, 
    { "RollbackSavePoint", Callback_sqlite3db_RollbackSavePoint, "bOk"       , "iDbIndex,sName"						, "Rollback save point to specified"                   , 0 }, 
    { "ReleaseSavePoint" , Callback_sqlite3db_ReleaseSavePoint , "bOk"       , "iDbIndex,sName"						, "Release an asql savepoint"                          , 0 }, 
    { "SavePoint"        , Callback_sqlite3db_SavePoint        , "bOk"       , "iDbIndex,sName"					    , "Create restore point"                               , 0 }, 
    { "isInTransaction"  , Callback_sqlite3db_isInTransaction  , "bYes"      , "iDbIndex"							, "Get transaction state of database"                  , 0 }, 
    { "GetColumnCount"   , Callback_sqlite3db_GetColumnCount   , "nCount"    , "iCmdHandle"							, "Get active statement column count"                  , 0 }, 
    { "GetColumnAt"      , Callback_sqlite3db_GetColumnAt      , "sColumn"   , "iCmdHandle,nIndex"					, "Get column name at index"                           , 0 }, 
    { "RowChanges"       , Callback_sqlite3db_RowChanges       , "nCount"    , "iDbIndex"							, "Return how many rows affecter by last action"       , 0 },
	{ "RowCount"         , Callback_sqlite3db_RowCount         , "nCount"    , "iCmdHandle"							, "Return how many rows selected by last action"       , 0 },
	{ "GetFieldValue"    , Callback_sqlite3db_GetFieldValue    , "sValue"    , "iCmdHandle,siFieldIndexName"		, "Get current row field value"                        , 0 },
	{ "QueryNext"        , Callback_sqlite3db_QueryNext        , "bok"       , "iCmdHandle"							, "Get the next row of selection"                      , 0 },
	{ "QueryClose"       , Callback_sqlite3db_QueryClose       , "bok"       , "iCmdHandle"							, "Close selections, do not use with sqlcommandexecute", 0 },
	{ "Query"			 , Callback_sqlite3db_Query			   , "iCmdHandle", "iDbIndex,sParamSql"					, "Create new sql command with paramters"              , 0 },
	{ "QueryExecute"     , Callback_sqlite3db_QueryExecute     , "bok"       , "iCmdHandle"							, "Execute sql with params "                           , 0 },
	{ "Bind"		     , Callback_sqlite3db_Bind             , "bok"       , "iCmdHandle, iParam, sParamValue"	, "Set sql command parameter"                          , 0 },
	{ "GetLastErrorStr"  , Callback_sqlite3db_GetLastErrorStr  , "sMessage"	 , "iDbIndex"							, "Get last database error string"                     , 0 },
	{ "GetLastErrorCode" , Callback_sqlite3db_GetLastErrorCode , "iCode"     , "iDbIndex"							, "Get last operation error code"                      , 0 },
	{ "Commit"           , Callback_sqlite3db_Commit           , "bOk"       , "iDbIndex"							, "Commit sql transaction"                             , 0 },
	{ "Rollback"         , Callback_sqlite3db_Rollback         , "bOk"       , "iDbIndex"							, "Rollback  sql transaction"                          , 0 }, 
    { "StartTransaction" , Callback_sqlite3db_StartTransaction , "bOk"       , "iDbIndex"							, "Start   sql transaction"                            , 0 }, 
    { "SqlExecute"       , Callback_sqlite3db_SqlExecute       , "bOk"       , "iDbIndex, sParamSql"				, "Execute sql script"                                 , 0 }, 
    { "CreateMemDatabase", Callback_sqlite3db_CreateMemDatabase, "bOk, iDbIndex" , ""								, "Open new memory Sqlite3 database"                   , 0 }, 
    { "OpenDatabase"     , Callback_sqlite3db_OpenDatabase     , "bOk, iDbIndex" , "sDatabase"						, "Open exists Sqlite3 database"                       , 0 }, 
    { "CloseDatabase"    , Callback_sqlite3db_CloseDatabase    , "bOk"		     , "iDbIndex"						, "Close opened Sqlite3 database"                      , 0 }    
    //{ NULL, NULL, NULL, NULL, NULL, 0}
} ;

//-----------------------------------------------------------------------------
//  Constants table
//-----------------------------------------------------------------------------

static S3DX::AIConstant aMyConstants [ ] =
{
	{ "kSqlite_ErrorCalling"		, -1, "System calling error, invalid parameter or value", 0 },
	{ "kSqlite_Ok"					, 0, "Successful result", 0 },
	{ "kSqlite_SQLITE_ERROR"		, 1 , "SQL error or missing database", 0 },
	{ "kSqlite_SQLITE_INTERNAL"		, 2 , "Internal logic error in SQLite", 0 },
	{ "kSqlite_SQLITE_PERM"			, 3 , "Access permission denied", 0 },
	{ "kSqlite_SQLITE_ABORT"		, 4 , "Callback routine requested an abort", 0 },
	{ "kSqlite_SQLITE_BUSY"			, 5 , "The database file is locked", 0 },
	{ "kSqlite_SQLITE_LOCKED"		, 6 , "A table in the database is locked", 0 },
	{ "kSqlite_SQLITE_NOMEM"		, 7 , "A malloc() failed", 0 },
	{ "kSqlite_SQLITE_READONLY"		, 8 , "Attempt to write a readonly database", 0 },
	{ "kSqlite_SQLITE_INTERRUPT"	, 9 , "Operation terminated by sqlite3_interrupt()", 0 },
	{ "kSqlite_SQLITE_IOERR"		, 10, "Some kind of disk I/O error occurred", 0 },
	{ "kSqlite_SQLITE_CORRUPT"		, 11, "The database disk image is malformed", 0 },
	{ "kSqlite_SQLITE_NOTFOUND"		, 12, "NOT USED. Table or record not found", 0 },
	{ "kSqlite_SQLITE_FULL"			, 13, "Insertion failed because database is full", 0 },
	{ "kSqlite_SQLITE_CANTOPEN"		, 14, "Unable to open the database file", 0 },
	{ "kSqlite_SQLITE_PROTOCOL"		, 15, "Database lock protocol error", 0 },
	{ "kSqlite_SQLITE_EMPTY"		, 16, "Database is empty", 0 },
	{ "kSqlite_SQLITE_SCHEMA"		, 17, "The database schema changed", 0 },
	{ "kSqlite_SQLITE_TOOBIG"		, 18, "String or BLOB exceeds size limit", 0 },
	{ "kSqlite_SQLITE_CONSTRAINT"	, 19, "Abort due to constraint violation", 0 },
	{ "kSqlite_SQLITE_MISMATCH"		, 20, "Data type mismatch", 0 },
	{ "kSqlite_SQLITE_MISUSE"		, 21, "Library used incorrectly", 0 },
	{ "kSqlite_SQLITE_NOLFS"		, 22, "Uses OS features not supported on host", 0 },
	{ "kSqlite_SQLITE_AUTH"			, 23, "Authorization denied", 0 },
	{ "kSqlite_SQLITE_FORMAT"		, 24, "Auxiliary database format error", 0 },
	{ "kSqlite_SQLITE_RANGE"		, 25, "2nd parameter to sqlite3_bind out of range", 0 },
	{ "kSqlite_SQLITE_NOTADB"		, 26, "File opened that is not a database file", 0 },

	{ "kDataType_Integer"			, 1, "Column data type", 0 },
	{ "kDataType_Float"				, 2, "Column data type", 0 },
	{ "kDataType_Text"				, 3, "Column data type", 0 },
	{ "kDataType_Blob"				, 4, "Column data type", 0 },
	{ "kDataType_Null"				, 5, "Column data type", 0 }


} ;

//-----------------------------------------------------------------------------
//  Accessors
//-----------------------------------------------------------------------------

const char *sqlite3dbPackage::GetName ( ) const
{
    return "sqlite3db" ;
}

//-----------------------------------------------------------------------------

S3DX::uint32 sqlite3dbPackage::GetFunctionCount ( ) const
{
	if ( aMyFunctions[0].pName == NULL )
	{
		return 0 ;
	}
	else
	{
		return sizeof( aMyFunctions ) / sizeof( S3DX::AIFunction ) ;
	}
}

//-----------------------------------------------------------------------------

S3DX::uint32 sqlite3dbPackage::GetConstantCount ( ) const
{
	if ( aMyConstants[0].pName == NULL )
	{
		return 0 ;
	}
	else
	{
		return sizeof( aMyConstants ) / sizeof( S3DX::AIConstant ) ;
	}
}

//-----------------------------------------------------------------------------

const S3DX::AIFunction *sqlite3dbPackage::GetFunctionAt ( S3DX::uint32 _iIndex ) const
{
    return &aMyFunctions[ _iIndex ] ;
}

//-----------------------------------------------------------------------------

const S3DX::AIConstant *sqlite3dbPackage::GetConstantAt ( S3DX::uint32 _iIndex ) const
{
    return &aMyConstants[ _iIndex ] ;
}
