#pragma once

#include <memory>
#include <list>
#include "File.h"
#include "FileSearcher.h"
#include "FileDeleter.h"

namespace SmartDiskCleaner
{
    class SmartDiskCleanerApi
    {
    public:
        static std::shared_ptr<SmartDiskCleanerApi> create( int numThreads = 0 );
        virtual ~SmartDiskCleanerApi( );

        //std::list<std::string> listFileNames( );
        FileListPtr listFiles( const std::string& startingPath );

        bool deleteFile( const std::string& path );
        bool deleteFolder( const std::string& path );
    
    private:
        SmartDiskCleanerApi( int numThreads = 0 );
        static std::shared_ptr<SmartDiskCleanerApi> m_instance;
        int m_numThreads;
        FileSearcherPtr m_fileSearcher;
        FileDeleterPtr m_fileDeleter;
    };

    typedef std::shared_ptr<SmartDiskCleanerApi> SmartDiskCleanerApiPtr;
}

