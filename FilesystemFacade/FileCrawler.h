#pragma once
#include <list>
#include <memory>
#include <mutex>
#include <boost/filesystem.hpp>
#include "File.h"
#include "FileCrawlerStatus.h"

namespace SmartDiskCleaner
{
    
    class FileCrawler
    {
    public:
        FileCrawler( );
        FileListPtr listFiles( const std::string& startingPath );
        FileCrawlerStatus getStatus( );

    private:
        void listFiles( boost::filesystem::path path );
        void addFilesFromStartingPath( const std::string& startingPath );
        boost::filesystem::recursive_directory_iterator createRecursiveIterator( boost::filesystem::path path );

        FileListPtr m_fileList;
        std::mutex m_mutex;
        FileCrawlerStatus m_status;
    };

    typedef std::shared_ptr<FileCrawler> FileCrawlerPtr;
}