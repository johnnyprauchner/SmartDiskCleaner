#include <iostream>
#include <boost/range/iterator_range.hpp>
#include <windows.h>
#include <thread>
#include "StringConversion.h"
#include "FileUtil.h"
#include "FileCrawler.h"
#include "DatabaseFacade.h"

using namespace SmartDiskCleaner;

FileCrawler::FileCrawler( )
    :m_fileList( std::make_shared<std::list<File>>( ) )
{

}

FileListPtr FileCrawler::listFiles( const std::string& startingPath )
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
        if( FileUtil::isDirectory( entry ) )
        {
            try
            {
                std::thread thread( [ = ] { listFiles( entry ); } );

                threads.push_back( std::move( thread ) );
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

void FileCrawler::addFilesFromStartingPath( const std::string& startingPath )
{
    if( boost::filesystem::is_directory( startingPath ) )
    {
        for( auto& entry : boost::make_iterator_range( boost::filesystem::directory_iterator( startingPath ) , { } , { } ) )
        {
            if( FileUtil::isFile( entry ) )
            {
                try
                {
                    File  file = FileUtil::createFile( entry );
                    m_fileList->push_back( file );
                }
                catch( boost::filesystem::filesystem_error& ex )
                {
                    std::cout << "FileSearcher::addFilesFromStartingPath - Error creating file: " << ex.what( ) << std::endl;
                    m_status.unreadableFiles++;
                }
            }
            m_status.fileCount++;
        }
    }
}

void FileCrawler::listFiles( boost::filesystem::path path )
{
    boost::filesystem::recursive_directory_iterator it = createRecursiveIterator( path );
    boost::filesystem::recursive_directory_iterator end;

    while( it != end )
    {
        if( FileUtil::isFile( *it ) )
        {
            try
            {
                std::unique_lock<std::mutex> lock( m_mutex );

                File  file = FileUtil::createFile( *it );
                m_fileList->push_back( file );
            }
            catch( boost::filesystem::filesystem_error& ex )
            {
                std::cout << "FileSearcher::listFiles - Error creating file: " << ex.what( ) << std::endl;
                m_status.unreadableFiles++;
            }
            m_status.fileCount++;
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

boost::filesystem::recursive_directory_iterator SmartDiskCleaner::FileCrawler::createRecursiveIterator( boost::filesystem::path path )
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

FileCrawlerStatus FileCrawler::getStatus( )
{
    return m_status;
}



