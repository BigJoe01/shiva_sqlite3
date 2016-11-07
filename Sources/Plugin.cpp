//-----------------------------------------------------------------------------
#include "PrecompiledHeader.h"
//-----------------------------------------------------------------------------
#include <string.h>
//-----------------------------------------------------------------------------
S3DX_IMPLEMENT_AIVARIABLE_STRING_POOL   ( 524288 ) ;
S3DX_IMPLEMENT_AIENGINEAPI              ( sqlite3db ) ;
S3DX_IMPLEMENT_PLUGIN                   ( sqlite3db ) ;

//-----------------------------------------------------------------------------
//  AI Packages
//-----------------------------------------------------------------------------

#include "sqlite3db.h"

//-----------------------------------------------------------------------------
//  Constructor / Destructor
//-----------------------------------------------------------------------------

sqlite3db::sqlite3db ( )
{
    S3DX_REGISTER_PLUGIN  ( "com.rawengroup.sqlite3" ) ;
    aContentsDirectory[0] = '\0' ;
	
	//Instanciate AI Packages
    S3DX::uint32 iAIPackageIndex = 0 ;

    if ( iAIPackageIndex < PLUGIN_AIPACKAGES_COUNT ) aAIPackages [iAIPackageIndex++] = new sqlite3dbPackage ( ) ;

	for ( ; iAIPackageIndex < PLUGIN_AIPACKAGES_COUNT; iAIPackageIndex ++ )
	{
        aAIPackages[iAIPackageIndex] = NULL  ;		
	}

}

//-----------------------------------------------------------------------------

sqlite3db::~sqlite3db ( )
{
	for ( S3DX::uint32 iAIPackageIndex = 0 ; iAIPackageIndex < PLUGIN_AIPACKAGES_COUNT; iAIPackageIndex ++ )
	{
		if ( aAIPackages [iAIPackageIndex] )
		{
			delete aAIPackages [iAIPackageIndex] ; 
			aAIPackages[iAIPackageIndex] = NULL  ;
		}
	}
}


//-----------------------------------------------------------------------------
//  Plugin content directory
//-----------------------------------------------------------------------------

        void                    sqlite3db::SetContentsDirectory  ( const char *_pDirectory ) { strcpy ( aContentsDirectory, _pDirectory ) ; }

//-----------------------------------------------------------------------------
//  AI packages
//-----------------------------------------------------------------------------

        S3DX::uint32            sqlite3db::GetAIPackageCount     ( )                      const { return PLUGIN_AIPACKAGES_COUNT ; }
const   S3DX::AIPackage        *sqlite3db::GetAIPackageAt        ( S3DX::uint32 _iIndex ) const { return (_iIndex < PLUGIN_AIPACKAGES_COUNT) ? aAIPackages[_iIndex] : NULL ; }

