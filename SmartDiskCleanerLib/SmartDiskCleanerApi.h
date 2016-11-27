#pragma once

#include <memory>
#include <list>
#include "File.h"
#include "FileSearcher.h"

namespace SmartDiskCleaner
{
    class SmartDiskCleanerApi
    {
    public:
        static std::shared_ptr<SmartDiskCleanerApi> create( int numThreads = 0 );
        virtual ~SmartDiskCleanerApi( );

        //std::list<std::string> listFileNames( );
        FileListPtr listFiles( );
        FileListPtr listFiles( const std::string& startingPath );
    
    private:
        SmartDiskCleanerApi( int numThreads = 0 );
        static std::shared_ptr<SmartDiskCleanerApi> m_instance;
        int m_numThreads;
        FileSearcherPtr m_fileSearcher;
    };

    typedef std::shared_ptr<SmartDiskCleanerApi> SmartDiskCleanerApiPtr;
}

