#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>
#include "DatabaseFacade.h"
#include "File.h"
#include "SQLCommandFactory.h"

using namespace SmartDiskCleaner;

std::shared_ptr<DatabaseFacade> DatabaseFacade::m_instance = nullptr;

const std::string DatabaseFacade::DATABASE_PATH = "c:\\ProgramData\\SmartDiskCleaner\\db\\";

std::shared_ptr<DatabaseFacade> DatabaseFacade::getInstance( )
{
    if( !m_instance )
    {
        m_instance.reset( new DatabaseFacade( ) );
        return m_instance;
    }
    return m_instance;
}

DatabaseFacade::~DatabaseFacade( )
{

}

bool DatabaseFacade::createDirectoryStructure( )
{
    bool result = true;
    boost::filesystem::path dir( DATABASE_PATH );
    if( boost::filesystem::exists( dir ) )
    {
        std::cout << "DatabaseFacade::createDirectoryStructure: " << dir.string( ) << " - Already exists" << "\n";
        return false;
    }

    try
    {
        if( boost::filesystem::create_directories( dir ) )
        {
            std::cout << "DatabaseFacade::createDirectoryStructure: " << dir.string( ) << " - Success" << "\n";
        }
        else
        {
            std::cout << "DatabaseFacade::createDirectoryStructure - Could not create directory " << dir.string( ) << "\n";
            result = false;
        }
    }
    catch( boost::filesystem::filesystem_error& e )
    {
        result = false;
        std::cout << "DatabaseFacade::createDirectoryStructure - Could not create directory " << dir.string( ) << ". Exception " << e.what( ) << "\n";
    }

    return result;
}


DatabaseFacade::DatabaseFacade( )
{
    createDirectoryStructure( );
    openDatabase( );
}

bool DatabaseFacade::openDatabase( )
{
    std::stringstream stringStream;
    stringStream << DATABASE_PATH << "Files.db";
    bool result = true;
    {
        try
        {
            m_database = std::make_unique<SQLite::Database>( stringStream.str( ) , SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE );
        }
        catch( std::exception& ex )
        {
            std::cout << "DatabaseFacade::openDatabase - SQLite exception: " << ex.what( ) << std::endl;
            result = false;
        }
    }
    return result;
}


bool DatabaseFacade::recreateDatabaseAndTable( )
{
    bool result = true;
    try
    {
        m_database->exec( "DROP TABLE IF EXISTS Files" );
        m_database->exec( "CREATE TABLE Files (Name STRING, Path STRING, Size INTEGER, Extension STRING, Year INTEGER, Month INTEGER, Day INTEGER, TypeDescription STRING)" );
    }
    catch( SQLite::Exception& ex )
    {
        std::cout << "DatabaseFacade::recreateDatabaseAndTable - SQLite exception: " << ex.what( ) << std::endl;
        result = false;
    }
    return result;
}

bool DatabaseFacade::recreateDatabase( )
{
    return recreateDatabaseAndTable( );
}

bool DatabaseFacade::insert( FileListPtr files )
{
    bool result = true;
    for( File file : *files )
    { 
        result = insert( file );
    }
    return result;
}

bool DatabaseFacade::insert( File file )
{
    bool result = true;
    try
    {
        SQLite::Transaction transaction( *m_database );
        std::string command = SQLCommandFactory::createInsertCommand( file );
        //std::string command = "INSERT INTO Files VALUES (\"delMameClones.bat\", \"C:\\temp\", 77590, \".bat\", 2014, 8, 10, \"Windows Batch File\")";

        int count = m_database->exec( command );
        result = ( count > 0 ) ? true : false;

        transaction.commit( );
    }
    catch( std::exception& e )
    {
        std::cout << "DatabaseFacade::insert - SQLite exception: " << e.what( ) << std::endl;
        result = false;
    }
    return result;
}

FileListPtr DatabaseFacade::listAllFiles( )
{
    FileListPtr result = std::make_shared<std::list<File>>();
    try
    {
        std::string command = SQLCommandFactory::createListAllCommand( );
        SQLite::Statement query( *m_database , command );

        while( query.executeStep( ) )
        {
            File file;
            file.name = query.getColumn( 0 );
            file.path = query.getColumn( 1 );
            file.sizeInBytes = query.getColumn( 2 );
            file.extension = query.getColumn( 3 );
            file.lastAccessedYear = query.getColumn( 4 );
            file.lastAcessedMonth = query.getColumn( 5 );
            file.lastAccessedDay = query.getColumn( 6 );
            file.typeDescription = query.getColumn( 7 );

            result->push_back( file );
        }
    }
    catch( std::exception& e )
    {
        std::cout << "DatabaseFacade::listAllFiles - SQLite exception: " << e.what( ) << std::endl;
    }
    return result;
}