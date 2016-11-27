#include "SmartDiskCleanerApi.h"
#include "FileSearcher.h"

using namespace SmartDiskCleaner;

std::shared_ptr<SmartDiskCleanerApi> SmartDiskCleanerApi::m_instance = nullptr;

std::shared_ptr<SmartDiskCleanerApi> SmartDiskCleanerApi::create( int numThreads )
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
    m_fileSearcher = std::make_shared<FileSearcher>( );
}

FileListPtr SmartDiskCleanerApi::listFiles( )
{
    return listFiles( "C:\\temp" );
}

FileListPtr SmartDiskCleanerApi::listFiles( const std::string& startingPath )
{
    return m_fileSearcher->listFiles( startingPath );
}