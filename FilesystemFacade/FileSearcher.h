#pragma once
#include <list>
#include <memory>
#include <mutex>
#include <boost/filesystem.hpp>
#include "File.h"

namespace SmartDiskCleaner
{
    
    class FileSearcher
    {
    public:
        FileSearcher( );
        FileListPtr listFiles( const std::string& startingPath );

    private:
        void listFiles( boost::filesystem::path path );
        void addFilesFromStartingPath( const std::string& startingPath );
        boost::filesystem::recursive_directory_iterator createRecursiveIterator( boost::filesystem::path path );

        FileListPtr m_fileList;
        std::mutex m_mutex;

    };

    typedef std::shared_ptr<FileSearcher> FileSearcherPtr;
}