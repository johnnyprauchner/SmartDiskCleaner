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
        std::list<File> listFiles( );
    
    private:
        SmartDiskCleanerApi( int numThreads = 0 );
        static std::shared_ptr<SmartDiskCleanerApi> m_instance;
        int m_numThreads;
        FileSearcherPtr m_fileSearcher;
    };

    typedef std::shared_ptr<SmartDiskCleanerApi> SmartDiskCleanerApiPtr;
}

