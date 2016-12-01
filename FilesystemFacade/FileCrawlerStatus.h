#pragma once

#include <memory>

namespace SmartDiskCleaner
{
    struct FileCrawlerStatus
    {
        FileCrawlerStatus( int fileCount = 0 , int unreadableFiles = 0 , bool isDone = false , float percentComplete = 0.0f )
            :m_fileCount( fileCount ) ,
            m_unreadableFiles( unreadableFiles ) ,
            m_isDone( isDone ) ,
            m_percentComplete( percentComplete )
        {

        }
        int getFileCount( )
        {
            return m_fileCount;
        }

        int getUnreadableFiles( )
        {
            return m_unreadableFiles;
        }
        bool getIsDone( )
        {
            return m_isDone;
        }

        float getPercentComplete( )
        {
            return m_percentComplete;
        }

        void incrementFileCount(  )
        {
            m_fileCount++;
        }

        void increamentUnreadableFiles( )
        {
            m_unreadableFiles++;
        }

        void setIsDone( bool isDone )
        {
            m_isDone = isDone;
        }

        void setPercentComplete( float percentComplete )
        {
            m_percentComplete = percentComplete;
        }

    private:
        int m_fileCount;
        int m_unreadableFiles;
        bool m_isDone;
        float m_percentComplete;
    };
    typedef std::shared_ptr<FileCrawlerStatus> FileCrawlerStatusPtr;
}