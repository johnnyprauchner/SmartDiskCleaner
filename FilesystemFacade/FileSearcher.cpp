#include "FileSearcher.h"
#include <iostream>
#include <windows.h>
#include "StringConversion.h"

using namespace SmartDiskCleaner;

FileSearcher::FileSearcher( )
{

}

std::list<File> FileSearcher::listFiles( const std::string& startingPath )
{
    std::list<File> result;
    if( startingPath == "" )
    {
        throw std::string( "FileSearcher::listFiles - need to specify starting path to search for files" );
    }

    boost::filesystem::path path( startingPath );

    std::cout << "FileSearcher::listFiles - Starting path: " << path.string( );
    if( !is_directory( path ) )
    {
        throw std::string( "FileSearcher::listFiles - invalid path" );
    }

    try
    {
        listFiles( path , result );
    }
    catch( const boost::filesystem::filesystem_error& ex )
    {
        std::cout << ex.what( ) << '\n';
    }

    return result;
}

void FileSearcher::listFiles( boost::filesystem::path path , std::list<File> &result )
{
    if( exists( path ) )
    {
        for( auto&& x : boost::filesystem::directory_iterator( path ) )
        {
            if( is_directory( x ) )
            {
                listFiles( x, result );
            }
            else
            {
                try
                {
                    File file = createFile( x.path( ) );

                    result.push_back( file );
                }
                catch( const boost::filesystem::filesystem_error& ex )
                {
                    std::cout << ex.what( ) << '\n';
                }
            }
        }

    }
    else
    {
        std::cout << path << " does not exist\n";
    }
}

File FileSearcher::createFile( boost::filesystem::path path )
{
    File file;
    file.name = path.filename( ).string( );
    file.path = path.parent_path( ).string( );
    file.extension = path.extension( ).string( );
    file.sizeInBytes = boost::filesystem::file_size( path );

    std::wstring pathWideString = StringConversion::stringToWideString( path.string( ) );

    HANDLE hFile = CreateFile( pathWideString.c_str( ) ,  // file name 
        GENERIC_READ ,          // open for reading 
        0 ,                     // do not share 
        NULL ,                  // default security 
        OPEN_EXISTING ,         // existing file only 
        FILE_ATTRIBUTE_NORMAL , // normal file 
        NULL );                 // no template 
    if( hFile == INVALID_HANDLE_VALUE )
    {
        std::cout << "CreateFile failed" << std::endl;
    }

    FILETIME ftAccess;

    // Retrieve the file times for the file.
    if( !GetFileTime( hFile , NULL, &ftAccess, NULL ) )
    {
        std::cout << "GetFileTime failed" << std::endl;
    }
    SYSTEMTIME systemTime;
    FileTimeToSystemTime( &ftAccess , &systemTime );

    file.lastAccessedDay = systemTime.wDay;
    file.lastAcessedMonth = systemTime.wMonth;
    file.lastAccessedYear = systemTime.wYear;

    CoInitialize( NULL );
    SHFILEINFOW sfi = { 0 };
    DWORD_PTR hr = SHGetFileInfo( pathWideString.c_str( ) ,
        ( DWORD ) -1 ,
        &sfi ,
        sizeof( sfi ) ,
        SHGFI_TYPENAME | SHGFI_DISPLAYNAME );

    if( hr > 0 )
    {
        std::wstring typeDescription( sfi.szTypeName );
        file.typeDescription = StringConversion::wideStringToString( typeDescription );
    }
    else
    {
        std::cout << "SHGetFileInfo failed. Error: " << GetLastError( ) << std::endl;
    }

    return file;
}
