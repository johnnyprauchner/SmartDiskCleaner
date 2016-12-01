#pragma once

#include <memory>
#include <list>
#include "File.h"
#include "FileCrawler.h"
#include "FileDeleter.h"
#include "QueryParameters.h"

namespace SmartDiskCleaner
{
    class SmartDiskCleanerApi
    {
    public:
        static std::shared_ptr<SmartDiskCleanerApi> getInstance( int numThreads = 0 );
        virtual ~SmartDiskCleanerApi( );

        void recreateDatabase( const std::string& startingPath );
        FileListPtr getFileList( );
        FileListPtr getFileList( QueryParameters queryParameters );
        FileCrawlerStatusPtr getStatus( );
        bool deleteFile( const std::string& path );
        bool deleteFolder( const std::string& path );
    
    private:
        SmartDiskCleanerApi( int numThreads = 0 );
        static std::shared_ptr<SmartDiskCleanerApi> m_instance;
        int m_numThreads;
        FileCrawlerPtr m_fileCrawler;
        FileDeleterPtr m_fileDeleter;
    };

    typedef std::shared_ptr<SmartDiskCleanerApi> SmartDiskCleanerApiPtr;
}

