#pragma once
#include <boost/filesystem.hpp>
#include <windows.h>
#include "File.h"

namespace SmartDiskCleaner
{
    namespace FileUtil
    {
        struct recursive_directory_range
        {
            recursive_directory_range( boost::filesystem::path path ) : m_path( path ) { }

            boost::filesystem::recursive_directory_iterator begin( ) 
            { 
                return boost::filesystem::recursive_directory_iterator( m_path );
            }
            boost::filesystem::recursive_directory_iterator end( ) 
            { 
                return boost::filesystem::recursive_directory_iterator( ); 
            }

            boost::filesystem::path m_path;
        };


        bool isFile( boost::filesystem::path path );
        
        bool isDirectory( boost::filesystem::path path );
        
        File createFile( boost::filesystem::path path );

        int countFiles( boost::filesystem::path path );
    }
}