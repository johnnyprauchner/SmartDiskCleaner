#pragma once

namespace SmartDiskCleaner
{
    struct FileCrawlerStatus
    {
        int fileCount;
        int unreadableFiles;
        bool isDone;
        FileCrawlerStatus( int fileCount = 0 , int unreadableFiles = 0 , bool isDone = false )
            :fileCount(  fileCount ),
            unreadableFiles( unreadableFiles),
            isDone( isDone )
        {

        }
    };
}