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
        void recreateDatabase( const std::string& startingPath );
        FileCrawlerStatusPtr getStatus( );
        FileListPtr getFileList( );

    private:
        void recreateDatabase( boost::filesystem::path path );
        void addFilesFromStartingPath( const std::string& startingPath );
        boost::filesystem::recursive_directory_iterator createRecursiveIterator( boost::filesystem::path path );

        FileListPtr m_fileList;
        std::mutex m_mutex;
        FileCrawlerStatusPtr m_status;
        int m_totalFiles;
    };

    typedef std::shared_ptr<FileCrawler> FileCrawlerPtr;
}