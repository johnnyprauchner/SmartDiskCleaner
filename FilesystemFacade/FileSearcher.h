#pragma once
#include <list>
#include <boost/filesystem.hpp>
#include "File.h"

namespace SmartDiskCleaner
{
    class FileSearcher
    {
    public:
        FileSearcher( );
        std::list<File> listFiles( const std::string& startingPath );

    private:
        File FileSearcher::createFile( boost::filesystem::path path );
        void FileSearcher::listFiles( boost::filesystem::path path , std::list<File> &result );
    };

    typedef std::shared_ptr<FileSearcher> FileSearcherPtr;
}