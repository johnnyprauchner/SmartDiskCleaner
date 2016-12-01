#include "SmartDiskCleanerApi.h"
#include "FileCrawler.h"
#include "DatabaseFacade.h"

using namespace SmartDiskCleaner;

std::shared_ptr<SmartDiskCleanerApi> SmartDiskCleanerApi::m_instance = nullptr;

std::shared_ptr<SmartDiskCleanerApi> SmartDiskCleanerApi::getInstance( int numThreads )
{
    if( !m_instance )
    {
        m_instance.reset( new SmartDiskCleanerApi( numThreads ) );
        return m_instance;
    }
    return m_instance;
}

SmartDiskCleanerApi::~SmartDiskCleanerApi( )
{

}

SmartDiskCleanerApi::SmartDiskCleanerApi( int numThreads )
    :m_numThreads( numThreads )
{
    m_fileCrawler = std::make_shared<FileCrawler>( );
    m_fileDeleter = std::make_shared<FileDeleter>( );
}

void SmartDiskCleanerApi::recreateDatabase( const std::string& startingPath )
{
    m_fileCrawler->recreateDatabase( startingPath );
}

FileListPtr SmartDiskCleanerApi::getFileList( void )
{
    return m_fileCrawler->getFileList(  );
}

bool SmartDiskCleanerApi::deleteFile( const std::string& path )
{
    return m_fileDeleter->deleteFile( path );
}

bool SmartDiskCleanerApi::deleteFolder( const std::string& path )
{
    return m_fileDeleter->deleteFolder( path );
}

FileCrawlerStatusPtr SmartDiskCleanerApi::getStatus( )
{
    return m_fileCrawler->getStatus( );
}

FileListPtr SmartDiskCleanerApi::getFileList( QueryParameters queryParameters )
{
    DatabaseFacadePtr databaseFacade = DatabaseFacade::getInstance( );
    return databaseFacade->getFileList( queryParameters );
}