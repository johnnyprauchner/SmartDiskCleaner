#include <windows.h>
#include <stdio.h>
#include <Strsafe.h>
#include "FileUtil.h"
#include "StringConversion.h"

using namespace SmartDiskCleaner;

bool FileUtil::isFile( boost::filesystem::path path )
{
    return boost::filesystem::exists( path ) && !boost::filesystem::is_directory( path );
}
bool FileUtil::isDirectory( boost::filesystem::path path )
{
    return boost::filesystem::exists( path ) && boost::filesystem::is_directory( path );
}

File FileUtil::createFile( boost::filesystem::path path )
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
        //std::cout << "FileUtil::createFile - CreateFile API failed. Error:" << GetLastError( ) << std::endl;
    }

    FILETIME ftAccess;

    // Retrieve the file times for the file.
    if( !GetFileTime( hFile , NULL , &ftAccess , NULL ) )
    {
        //std::cout << "FileUtil::createFile - GetFileTime failed - Error:" << GetLastError() << std::endl;
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
        SHGFI_TYPENAME );

    if( hr > 0 )
    {
        std::wstring typeDescription( sfi.szTypeName );
        file.typeDescription = StringConversion::wideStringToString( typeDescription );
    }
    else
    {
        //std::cout << "FileUtil::createFile - SHGetFileInfo failed. Error: " << GetLastError( ) << std::endl;
    }
    CloseHandle( hFile );

    return file;
}

int FileUtil::countFiles( boost::filesystem::path path )
{
    int count = 0;
    for( auto it : recursive_directory_range( path ) )
    {
        count++;
    }
    return count;
}