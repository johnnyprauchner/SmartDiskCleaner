#pragma once

#include <string>

namespace SmartDiskCleaner
{
    struct QueryParameters
    {
    public:
        QueryParameters( )
            :m_sizeGreater( -1 ) ,
            m_sizeSmaller( -1 )
        {

        }

        //Ex: ".exe, .bin, .sys"
        void excludeExtensions( const std::string& extensions )
        {
            m_excludeExtensions = extensions;
        }

        //Ex: ".txt, .doc, .html"
        void includeExtensions( const std::string& extensions )
        {
            m_includeExtensions = extensions;
        }

        void sizeGreaterThan( int sizeInBytes )
        {
            m_sizeGreater = sizeInBytes;
        }

        void sizeSmallerThan( int sizeInBytes )
        {
            m_sizeSmaller = sizeInBytes;
        }

        void directoryContains( const std::string& path, bool recursive = true )
        {
            m_directoryContains = path;
            m_directoryRecursive = recursive;
        }

        void directoryDoesntContain( const std::string& path, bool recursive = true )
        {
            m_directoryDoesntContain = path;
            m_directoryRecursive = recursive;
        }

        //Ex: 11/02/1980
        void olderThan( const std::string& date )
        {
            m_olderThan = date;
        }

        void newerThan( const std::string& date )
        {
            m_NewerThan = date;
        }

        void exactDate( const std::string& date )
        {
            m_exactDate = date;
        }

        void fileNameExact( const std::string& path )
        {
            m_fileNameExact = path;
        }

        void fileNameContains( const std::string& path )
        {
            m_fileNameContains = path;
        }
        
        void fileNameDoesntContain( const std::string& path )
        {
            m_fileNameDoesntContain = path;
        }

    private:
        std::string m_excludeExtensions;
        std::string m_includeExtensions;
        
        int m_sizeGreater;
        int m_sizeSmaller;
        
        std::string m_directory;
        std::string m_directoryContains;
        std::string m_directoryDoesntContain;
        bool m_directoryRecursive;
        
        std::string m_olderThan;
        std::string m_NewerThan;
        std::string m_exactDate;
        
        std::string m_fileNameExact;
        std::string m_fileNameContains;
        std::string m_fileNameDoesntContain;
    };
}