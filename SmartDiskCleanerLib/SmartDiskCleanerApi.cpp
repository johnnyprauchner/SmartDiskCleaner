#include "SmartDiskCleanerApi.h"
#include "FileCrawler.h"

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

FileListPtr SmartDiskCleanerApi::listFiles( const std::string& startingPath )
{
    return m_fileCrawler->listFiles( startingPath );
}

bool SmartDiskCleanerApi::deleteFile( const std::string& path )
{
    m_fileDeleter->deleteFile( path );
}

bool SmartDiskCleanerApi::deleteFolder( const std::string& path )
{
    m_fileDeleter->deleteFolder( path );
}