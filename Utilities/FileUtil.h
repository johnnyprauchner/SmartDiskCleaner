#pragma once
#include <boost/filesystem.hpp>
#include "File.h"

namespace SmartDiskCleaner
{
    namespace FileUtil
    {
        bool isFile( boost::filesystem::path path );
        
        bool isDirectory( boost::filesystem::path path );
        
        File createFile( boost::filesystem::path path );
    }
}