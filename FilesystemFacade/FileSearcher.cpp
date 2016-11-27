#include "FileSearcher.h"
#include <iostream>
#include <boost/range/iterator_range.hpp>
#include <windows.h>
#include <thread>
#include "StringConversion.h"

using namespace SmartDiskCleaner;

FileSearcher::FileSearcher( )
    :m_fileList( std::make_shared<std::list<File>>() )
{

}

FileListPtr FileSearcher::listFiles( const std::string& startingPath )
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
    addFilesFromStartingPath( startingPath );

    std::vector<std::thread> threads;
    for( auto& entry : boost::make_iterator_range( boost::filesystem::directory_iterator( startingPath ) , { } , { } ) )
    {
        if( boost::filesystem::exists( entry ) && boost::filesystem::is_directory( entry ) )
        {
            try
            {
                std::thread thread( [ = ] { listFiles( entry ); } );

                threads.push_back( std::move(thread) );
            }
            catch( const boost::filesystem::filesystem_error& ex )
            {
                std::cout << " FileSearcher::listFiles - error " << ex.what( ) << std::endl;
            }
        }
    }

    for( auto&& thread : threads )
    {
        thread.join( );
    }


    return m_fileList;
}

void FileSearcher::addFilesFromStartingPath( const std::string& startingPath )
{
    if( boost::filesystem::is_directory( startingPath ) ) 
    {

        for( auto& entry : boost::make_iterator_range( boost::filesystem::directory_iterator( startingPath ) , { } , { } ) )
        {
            if( boost::filesystem::exists( entry ) && !boost::filesystem::is_directory( entry ) )
            {
                try
                {
                    File  file = createFile( entry );
                    m_fileList->push_back( file );
                }
                catch( boost::filesystem::filesystem_error& ex )
                {
                    std::cout << "FileSearcher::addFilesFromStartingPath - Error creating file: " << ex.what( ) << std::endl;
                }
            }
        }
    }
}

void FileSearcher::listFiles( boost::filesystem::path path )
{
    boost::filesystem::recursive_directory_iterator it = createRecursiveIterator( path );
    boost::filesystem::recursive_directory_iterator end;

    while( it != end )
    {
        if( exists( *it ) && !boost::filesystem::is_directory( *it ) )
        {
            try
            {
                std::unique_lock<std::mutex> lock( m_mutex );

                File  file = createFile( *it );
                m_fileList->push_back( file );
            }
            catch( boost::filesystem::filesystem_error& ex )
            {
                std::cout << "FileSearcher::listFiles - Error creating file: " << ex.what( ) << std::endl;
            }
        }

        if( boost::filesystem::is_directory( *it ) && boost::filesystem::is_symlink( *it ) )
        {
            it.no_push( );
        }

        try
        {
            ++it; 
        }
        catch( std::exception& ex )
        {
            std::cout << "FileSearcher::listFiles - Error incrementing iterator: " << ex.what( ) << std::endl;
            it.no_push( );
            try 
            { 
                ++it; 
            }
            catch( ... ) 
            { 
                std::cout << "FileSearcher::listFiles - Fatal (unknown) error incrementing iterator: " << std::endl;
                return; 
            } 
        }
    }

}

boost::filesystem::recursive_directory_iterator SmartDiskCleaner::FileSearcher::createRecursiveIterator( boost::filesystem::path path )
{
    boost::filesystem::recursive_directory_iterator it;
    try
    {
        it = boost::filesystem::recursive_directory_iterator( path );
    }
    catch( boost::filesystem::filesystem_error& fex )
    {
        std::cout << fex.what( ) << std::endl;
        it = boost::filesystem::recursive_directory_iterator( );
    }
    return it;
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
        //std::cout << "FileSearcher::createFile - GetFileTime failed - Error:" << GetLastError() << std::endl;
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
        std::cout << "SHGetFileInfo failed. Error: " << GetLastError( ) << std::endl;
    }
    CloseHandle( hFile );

    return file;
}


