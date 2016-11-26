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
        static std::shared_ptr<DatabaseFacade> create( );
        virtual ~DatabaseFacade( );

        bool recreateDatabase( std::list<File> files);
        bool insert( std::list<File> file );
        bool insert( File file);
        bool remove( std::list<File> files );
        bool remove( File file);

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
