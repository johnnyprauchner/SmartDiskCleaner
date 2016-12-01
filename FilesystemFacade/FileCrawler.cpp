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
    :m_fileList( std::make_shared<std::list<File>>( ) ) ,
    m_status( std::make_shared<FileCrawlerStatus>( ) )
{

}

void FileCrawler::recreateDatabase( const std::string& startingPath )
{
    std::list<File> result;
    if( startingPath == "" )
    {
        throw std::string( "FileCrawler::listFiles - need to specify starting path to search for files" );
    }

    boost::filesystem::path path( startingPath );

    std::cout << "FileCrawler::listFiles - Starting path: " << path.string( ) << std::endl;
    if( !is_directory( path ) )
    {
        throw std::string( "FileCrawler::listFiles - invalid path" );
    }

    //std::cout << "FileCrawler::listFiles - Counting files... " << std::endl;
    //m_totalFiles = static_cast< int >( std::distance( boost::filesystem::recursive_directory_iterator( path ) , boost::filesystem::recursive_directory_iterator( ) ) );
    ////m_totalFiles = FileUtil::countFiles( path );
    //std::cout << "FileCrawler::listFiles - Count: " << m_totalFiles << std::endl;
    //std::cout << "\n";

    m_status->setPercentComplete( 0.0f );

    addFilesFromStartingPath( startingPath );

    std::vector<std::thread> threads;
    for( auto& entry : boost::make_iterator_range( boost::filesystem::directory_iterator( startingPath ) , { } , { } ) )
    {
        if( FileUtil::isDirectory( entry ) )
        {
            try
            {
                std::thread thread( [ = ] { recreateDatabase( entry ); } );

                threads.push_back( std::move( thread ) );
            }
            catch( const boost::filesystem::filesystem_error& ex )
            {
                std::cout << " FileCrawler::listFiles - error " << ex.what( ) << std::endl;
            }
        }
    }

    for( auto&& thread : threads )
    {
        thread.join( );
    }


    m_status->setIsDone( true );
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
                    DatabaseFacade::getInstance( )->insert( file );
                    m_status->incrementFileCount( );
                }
                catch( boost::filesystem::filesystem_error& ex )
                {
                    std::cout << "FileCrawler::addFilesFromStartingPath - Error creating file: " << ex.what( ) << std::endl;
                    m_status->increamentUnreadableFiles( );
                }
            }
        }
    }
}

void FileCrawler::recreateDatabase( boost::filesystem::path path )
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
                DatabaseFacade::getInstance( )->insert( file );
                m_status->incrementFileCount( );
                //m_status->setPercentComplete( ( static_cast< float >( m_status->getFileCount( ) ) / static_cast< float >( m_totalFiles ) ) * 100.0f );
            }
            catch( boost::filesystem::filesystem_error& ex )
            {
                std::cout << "FileCrawler::listFiles - Error creating file: " << ex.what( ) << std::endl;
                m_status->increamentUnreadableFiles( );
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
            std::cout << "FileCrawler::listFiles - Error incrementing iterator: " << ex.what( ) << std::endl;
            it.no_push( );
            try
            {
                ++it;
            }
            catch( ... )
            {
                std::cout << "FileCrawler::listFiles - Fatal (unknown) error incrementing iterator: " << std::endl;
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

FileCrawlerStatusPtr FileCrawler::getStatus( )
{
    return m_status;
}


FileListPtr FileCrawler::getFileList( )
{
    return m_fileList;
}
