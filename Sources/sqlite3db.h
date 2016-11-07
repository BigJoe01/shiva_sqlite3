//-----------------------------------------------------------------------------
#ifndef __sqlite3db_h__
#define __sqlite3db_h__
//-----------------------------------------------------------------------------
#include "S3DXAIPackage.h"
#include "sqlite3.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Package declaration
//-----------------------------------------------------------------------------
class sqlite3dbPackage : public S3DX::AIPackage
{
public :

    //-------------------------------------------------------------------------
    //  Constructor / Destructor
	//-------------------------------------------------------------------------

	sqlite3dbPackage         ( ) ;
                               ~sqlite3dbPackage         ( ) ;

	//-------------------------------------------------------------------------
    //  Accessors
	//-------------------------------------------------------------------------

    const   char               *GetName             ( ) const ;
            S3DX::uint32        GetFunctionCount    ( ) const ;
            S3DX::uint32        GetConstantCount    ( ) const ;
    const   S3DX::AIFunction   *GetFunctionAt       ( S3DX::uint32 _iIndex ) const ;
    const   S3DX::AIConstant   *GetConstantAt       ( S3DX::uint32 _iIndex ) const ;

} ;

//-----------------------------------------------------------------------------
// Package API declaration
//-----------------------------------------------------------------------------
class sqlite3dbAPI
{
public :

    //-------------------------------------------------------------------------
    //  API Constructor
	//-------------------------------------------------------------------------
                                sqlite3dbAPI       ( ) 
                                {
                                    pfn_sqlite3db_GetColumnType     = NULL ;
                                    pfn_sqlite3db_AttachDatabase    = NULL ;
                                    pfn_sqlite3db_DetachDatabase    = NULL ;
                                    pfn_sqlite3db_Pragma            = NULL ;
                                    pfn_sqlite3db_RollbackSavePoint = NULL ;
                                    pfn_sqlite3db_ReleaseSavePoint  = NULL ;
                                    pfn_sqlite3db_SavePoint         = NULL ;
                                    pfn_sqlite3db_isInTransaction   = NULL ;
                                    pfn_sqlite3db_GetColumnCount    = NULL ;
                                    pfn_sqlite3db_GetColumnAt       = NULL ;
                                    pfn_sqlite3db_RowChanges        = NULL ;
                                    pfn_sqlite3db_RowCount          = NULL ;
                                    pfn_sqlite3db_GetFieldValue     = NULL ;
                                    pfn_sqlite3db_QueryNext         = NULL ;
                                    pfn_sqlite3db_QueryClose        = NULL ;
                                    pfn_sqlite3db_Query             = NULL ;
                                    pfn_sqlite3db_QueryExecute	    = NULL ;
                                    pfn_sqlite3db_Bind              = NULL ;
                                    pfn_sqlite3db_GetLastErrorStr   = NULL ;
                                    pfn_sqlite3db_GetLastErrorCode  = NULL ;
                                    pfn_sqlite3db_Commit            = NULL ;
                                    pfn_sqlite3db_Rollback          = NULL ;
                                    pfn_sqlite3db_StartTransaction  = NULL ;
                                    pfn_sqlite3db_SqlExecute        = NULL ;
                                    pfn_sqlite3db_CreateMemDatabase = NULL ;
                                    pfn_sqlite3db_OpenDatabase      = NULL ;
                                    pfn_sqlite3db_CloseDatabase     = NULL ;

                                }

	//-------------------------------------------------------------------------
	//  API Callbacks 
	//-------------------------------------------------------------------------

    S3DX::AICallback        pfn_sqlite3db_GetColumnType ;
    S3DX::AICallback        pfn_sqlite3db_AttachDatabase ;
    S3DX::AICallback        pfn_sqlite3db_DetachDatabase ;
    S3DX::AICallback        pfn_sqlite3db_Pragma ;
    S3DX::AICallback        pfn_sqlite3db_RollbackSavePoint ;
    S3DX::AICallback        pfn_sqlite3db_ReleaseSavePoint ;
    S3DX::AICallback        pfn_sqlite3db_SavePoint ;
    S3DX::AICallback        pfn_sqlite3db_isInTransaction ;
    S3DX::AICallback        pfn_sqlite3db_GetColumnCount ;
    S3DX::AICallback        pfn_sqlite3db_GetColumnAt ;
    S3DX::AICallback        pfn_sqlite3db_RowChanges ;
    S3DX::AICallback        pfn_sqlite3db_RowCount ;
    S3DX::AICallback        pfn_sqlite3db_GetFieldValue ;
    S3DX::AICallback        pfn_sqlite3db_QueryNext ;
    S3DX::AICallback        pfn_sqlite3db_QueryClose ;
    S3DX::AICallback        pfn_sqlite3db_Query ;
    S3DX::AICallback        pfn_sqlite3db_QueryExecute ;
    S3DX::AICallback        pfn_sqlite3db_Bind ;
    S3DX::AICallback        pfn_sqlite3db_GetLastErrorStr ;
    S3DX::AICallback        pfn_sqlite3db_GetLastErrorCode ;
    S3DX::AICallback        pfn_sqlite3db_Commit ;
    S3DX::AICallback        pfn_sqlite3db_Rollback ;
    S3DX::AICallback        pfn_sqlite3db_StartTransaction ;
    S3DX::AICallback        pfn_sqlite3db_SqlExecute ;
    S3DX::AICallback        pfn_sqlite3db_CreateMemDatabase ;
    S3DX::AICallback        pfn_sqlite3db_OpenDatabase ;
    S3DX::AICallback        pfn_sqlite3db_CloseDatabase ;

	//-------------------------------------------------------------------------
	//  API Functions 
	//-------------------------------------------------------------------------

    inline S3DX::AIVariable     GetColumnType     ( const S3DX::AIVariable& iCmdHandle, const S3DX::AIVariable& nsIndexName ) { S3DX_DECLARE_VIN_02( iCmdHandle, nsIndexName ) ; S3DX::AIVariable vOut ; if ( pfn_sqlite3db_GetColumnType ) pfn_sqlite3db_GetColumnType ( 2, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     AttachDatabase    ( const S3DX::AIVariable& iDbIndex,   const S3DX::AIVariable& sFileName, const S3DX::AIVariable& sDatabaseName ) { S3DX_DECLARE_VIN_03(iDbIndex, sFileName, sDatabaseName ) ; S3DX::AIVariable vOut ; if ( pfn_sqlite3db_AttachDatabase ) pfn_sqlite3db_AttachDatabase ( 3, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     DetachDatabase    ( const S3DX::AIVariable& iDbIndex,   const S3DX::AIVariable& sDatabaseName ) { S3DX_DECLARE_VIN_02(iDbIndex, sDatabaseName ) ; S3DX::AIVariable vOut ; if ( pfn_sqlite3db_DetachDatabase ) pfn_sqlite3db_DetachDatabase ( 2, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     Pragma            ( const S3DX::AIVariable& iDbIndex,   const S3DX::AIVariable& sName, const S3DX::AIVariable& sValue ) { S3DX_DECLARE_VIN_03( iDbIndex, sName, sValue ) ; S3DX::AIVariable vOut ; if ( pfn_sqlite3db_Pragma ) pfn_sqlite3db_Pragma ( 3, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     RollbackSavePoint ( const S3DX::AIVariable& iDbIndex,   const S3DX::AIVariable& sName ) { S3DX_DECLARE_VIN_02( iDbIndex, sName ) ; S3DX::AIVariable vOut ; if ( pfn_sqlite3db_RollbackSavePoint ) pfn_sqlite3db_RollbackSavePoint ( 2, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     ReleaseSavePoint  ( const S3DX::AIVariable& iDbIndex,   const S3DX::AIVariable& sName ) { S3DX_DECLARE_VIN_02( iDbIndex, sName ) ; S3DX::AIVariable vOut ; if ( pfn_sqlite3db_ReleaseSavePoint ) pfn_sqlite3db_ReleaseSavePoint ( 2, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     SavePoint         ( const S3DX::AIVariable& iDbIndex,   const S3DX::AIVariable& sName ) { S3DX_DECLARE_VIN_02( iDbIndex, sName ) ; S3DX::AIVariable vOut ; if ( pfn_sqlite3db_SavePoint ) pfn_sqlite3db_SavePoint ( 2, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     isInTransaction   ( const S3DX::AIVariable& iDbIndex   ) { S3DX_DECLARE_VIN_01(iDbIndex);  S3DX::AIVariable vOut ; if ( pfn_sqlite3db_isInTransaction ) pfn_sqlite3db_isInTransaction ( 2, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     GetColumnCount    ( const S3DX::AIVariable& iCmdHandle ) { S3DX_DECLARE_VIN_01( iCmdHandle ) ; S3DX::AIVariable vOut ; if ( pfn_sqlite3db_GetColumnCount ) pfn_sqlite3db_GetColumnCount ( 1, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     GetColumnAt       ( const S3DX::AIVariable& iCmdHandle, const S3DX::AIVariable& nIndex ) { S3DX_DECLARE_VIN_02( iCmdHandle, nIndex ) ; S3DX::AIVariable vOut ; if ( pfn_sqlite3db_GetColumnAt ) pfn_sqlite3db_GetColumnAt ( 2, vIn, &vOut ); return vOut ; }
	inline S3DX::AIVariable     RowChanges        ( const S3DX::AIVariable& iDbIndex   ) { S3DX_DECLARE_VIN_01(iDbIndex);  S3DX::AIVariable vOut; if (pfn_sqlite3db_RowChanges) pfn_sqlite3db_RowChanges( 1, vIn, &vOut); return vOut; }
    inline S3DX::AIVariable     RowCount          ( const S3DX::AIVariable& iCmdHandle ) { S3DX_DECLARE_VIN_01( iCmdHandle ) ; S3DX::AIVariable vOut ; if ( pfn_sqlite3db_RowCount ) pfn_sqlite3db_RowCount ( 1, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     GetFieldValue     ( const S3DX::AIVariable& iCmdHandle, const S3DX::AIVariable& siFieldIndexName ) { S3DX_DECLARE_VIN_02( iCmdHandle, siFieldIndexName ) ; S3DX::AIVariable vOut ; if ( pfn_sqlite3db_GetFieldValue ) pfn_sqlite3db_GetFieldValue ( 2, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     QueryNext         ( const S3DX::AIVariable& iCmdHandle ) { S3DX_DECLARE_VIN_01( iCmdHandle ) ; S3DX::AIVariable vOut ; if ( pfn_sqlite3db_QueryNext ) pfn_sqlite3db_QueryNext ( 1, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     QueryClose        ( const S3DX::AIVariable& iCmdHandle ) { S3DX_DECLARE_VIN_01( iCmdHandle ) ; S3DX::AIVariable vOut ; if ( pfn_sqlite3db_QueryClose  ) pfn_sqlite3db_QueryClose ( 1, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     Query             ( const S3DX::AIVariable& iDbIndex, const S3DX::AIVariable& sParamSql ) { S3DX_DECLARE_VIN_02( iDbIndex, sParamSql ) ; S3DX::AIVariable vOut ; if ( pfn_sqlite3db_Query ) pfn_sqlite3db_Query ( 2, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     QueryExecute      ( const S3DX::AIVariable& iCmdHandle ) { S3DX_DECLARE_VIN_01( iCmdHandle ) ; S3DX::AIVariable vOut ; if ( pfn_sqlite3db_QueryExecute ) pfn_sqlite3db_QueryExecute ( 1, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     Bind              ( const S3DX::AIVariable& iCmdHandle, const S3DX::AIVariable& sParamName, const S3DX::AIVariable& sParamValue ) { S3DX_DECLARE_VIN_03( iCmdHandle, sParamName, sParamValue ) ; S3DX::AIVariable vOut ; if ( pfn_sqlite3db_Bind ) pfn_sqlite3db_Bind ( 3, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     GetLastErrorStr   ( const S3DX::AIVariable& sDatabase) { S3DX_DECLARE_VIN_01(sDatabase); S3DX::AIVariable vOut ; if ( pfn_sqlite3db_GetLastErrorStr )  pfn_sqlite3db_GetLastErrorStr (  1, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     GetLastErrorCode  ( const S3DX::AIVariable& sDatabase) { S3DX_DECLARE_VIN_01(sDatabase); S3DX::AIVariable vOut ; if ( pfn_sqlite3db_GetLastErrorCode ) pfn_sqlite3db_GetLastErrorCode ( 1, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     Commit            ( const S3DX::AIVariable& sDatabase) { S3DX_DECLARE_VIN_01(sDatabase); S3DX::AIVariable vOut ; if ( pfn_sqlite3db_Commit ) pfn_sqlite3db_Commit ( 1, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     Rollback          ( const S3DX::AIVariable& sDatabase) { S3DX_DECLARE_VIN_01(sDatabase); S3DX::AIVariable vOut ; if ( pfn_sqlite3db_Rollback ) pfn_sqlite3db_Rollback ( 1, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     StartTransaction  ( const S3DX::AIVariable& sDatabase ) { S3DX_DECLARE_VIN_01(sDatabase); S3DX::AIVariable vOut ; if ( pfn_sqlite3db_StartTransaction ) pfn_sqlite3db_StartTransaction ( 1, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     SqlExecute        ( const S3DX::AIVariable& iDbIndex, const S3DX::AIVariable& sParamSql ) { S3DX_DECLARE_VIN_02( iDbIndex, sParamSql ) ; S3DX::AIVariable vOut ; if ( pfn_sqlite3db_SqlExecute ) pfn_sqlite3db_SqlExecute ( 2, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariables<2> CreateMemDatabase (  ) { S3DX::AIVariables<2> vOut; if ( pfn_sqlite3db_CreateMemDatabase ) pfn_sqlite3db_CreateMemDatabase ( 0, NULL, vOut ); return vOut ; }
    inline S3DX::AIVariables<2> OpenDatabase      ( const S3DX::AIVariable& sDatabase ) { S3DX_DECLARE_VIN_01( sDatabase ) ; S3DX::AIVariables<2> vOut; if ( pfn_sqlite3db_OpenDatabase ) pfn_sqlite3db_OpenDatabase ( 1, vIn, vOut ); return vOut ; }
	inline S3DX::AIVariable     CloseDatabase     ( const S3DX::AIVariable& iDbIndex) { S3DX_DECLARE_VIN_01(iDbIndex);  S3DX::AIVariable vOut; if (pfn_sqlite3db_CloseDatabase) pfn_sqlite3db_CloseDatabase( 1, NULL, &vOut); return vOut; }

 	//-------------------------------------------------------------------------
	//  API Constants 
	//-------------------------------------------------------------------------

	S3DX::AIVariable kSqlite_ErrorCalling;
	S3DX::AIVariable kSqlite_Ok;
	S3DX::AIVariable kSqlite_SQLITE_ERROR;
	S3DX::AIVariable kSqlite_SQLITE_INTERNAL;
	S3DX::AIVariable kSqlite_SQLITE_PERM;
	S3DX::AIVariable kSqlite_SQLITE_ABORT;
	S3DX::AIVariable kSqlite_SQLITE_BUSY;
	S3DX::AIVariable kSqlite_SQLITE_LOCKED;
	S3DX::AIVariable kSqlite_SQLITE_NOMEM;
	S3DX::AIVariable kSqlite_SQLITE_READONLY;
	S3DX::AIVariable kSqlite_SQLITE_INTERRUPT;
	S3DX::AIVariable kSqlite_SQLITE_IOERR;
	S3DX::AIVariable kSqlite_SQLITE_CORRUPT;
	S3DX::AIVariable kSqlite_SQLITE_NOTFOUND;
	S3DX::AIVariable kSqlite_SQLITE_FULL;
	S3DX::AIVariable kSqlite_SQLITE_CANTOPEN;
	S3DX::AIVariable kSqlite_SQLITE_PROTOCOL;
	S3DX::AIVariable kSqlite_SQLITE_EMPTY;
	S3DX::AIVariable kSqlite_SQLITE_SCHEMA;
	S3DX::AIVariable kSqlite_SQLITE_TOOBIG;
	S3DX::AIVariable kSqlite_SQLITE_CONSTRAINT;
	S3DX::AIVariable kSqlite_SQLITE_MISMATCH;
	S3DX::AIVariable kSqlite_SQLITE_MISUSE;
	S3DX::AIVariable kSqlite_SQLITE_NOLFS;
	S3DX::AIVariable kSqlite_SQLITE_AUTH;
	S3DX::AIVariable kSqlite_SQLITE_FORMAT;
	S3DX::AIVariable kSqlite_SQLITE_RANGE;
	S3DX::AIVariable kSqlite_SQLITE_NOTADB;	


	S3DX::AIVariable kDataType_Integer;
	S3DX::AIVariable kDataType_Float;
	S3DX::AIVariable kDataType_Text;
	S3DX::AIVariable kDataType_Blob;
	S3DX::AIVariable kDataType_Null;


} ;

extern sqlite3dbAPI sqlite3db;

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
