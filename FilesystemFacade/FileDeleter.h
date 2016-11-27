#pragma once

#include <memory>
#include <boost/filesystem.hpp>

namespace SmartDiskCleaner
{

    class FileDeleter
    {
    public:
        bool deleteFile( const std::string& path );
        bool deleteFolder( const std::string& path );

    };

    typedef std::shared_ptr<FileDeleter> FileDeleterPtr;
}