#include <iostream>
#include "FileUtil.h"
#include "FileDeleter.h"

using namespace SmartDiskCleaner;

bool FileDeleter::deleteFile( const std::string& path )
{
    bool result = true;

    boost::filesystem::path file( path );
    if( FileUtil::isFile( file ) )
    {
        try
        {
            boost::filesystem::remove( file );
        }
        catch( boost::filesystem::filesystem_error& e )
        {
            std::cout << "FileDeleter::deleteFile - caught exception deleteing file: " << e.what( ) << std::endl;
        }
    }
    else
    {
        std::cout << "FileDeleter::deleteFile - invalid path: " << path << std::endl;
    }

    return result;
}

bool FileDeleter::deleteFolder( const std::string& path )
{
    bool result = true;

    boost::filesystem::path folder( path );
    if( FileUtil::isDirectory( folder ) )
    {
        try
        {
            boost::filesystem::remove_all( folder );
        }
        catch( boost::filesystem::filesystem_error& e )
        {
            std::cout << "FileDeleter::deleteFolder - caught exception deleteing file: " << e.what( ) << std::endl;
        }
    }
    else
    {
        std::cout << "FileDeleter::deleteFolder - invalid path: " << path << std::endl;
    }

    return result;
}
