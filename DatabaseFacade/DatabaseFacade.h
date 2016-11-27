#pragma once
#include <memory>
#include <string>
#include <list>
#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>
#include "File.h"

namespace SmartDiskCleaner
{
    class DatabaseFacade
    {
    public:
        static std::shared_ptr<DatabaseFacade> getInstance( );
        virtual ~DatabaseFacade( );

        bool recreateDatabase( );
        bool insert( FileListPtr file );
        bool insert( File file);
        bool remove( FileListPtr files );
        bool remove( File file);
        FileListPtr listAllFiles( );

    private:
        DatabaseFacade( );

        bool openDatabase( );
        bool createDirectoryStructure( );

        bool recreateDatabaseAndTable( );

        static std::shared_ptr<DatabaseFacade> m_instance;
        std::unique_ptr<SQLite::Database> m_database;
        const static std::string DATABASE_PATH;
    };

    typedef std::shared_ptr<DatabaseFacade> DatabaseFacadePtr;
}
