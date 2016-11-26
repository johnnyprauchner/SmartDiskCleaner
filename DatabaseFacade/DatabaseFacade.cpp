#include "DatabaseFacade.h"
#include "File.h"
#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>

using namespace SmartDiskCleaner;

std::shared_ptr<DatabaseFacade> DatabaseFacade::m_instance = nullptr;

const std::string DatabaseFacade::DATABASE_PATH = "c:\\ProgramData\\SmartDiskCleaner\\db\\";

std::shared_ptr<DatabaseFacade> DatabaseFacade::create( )
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
            std::cout << "DatabaseFacade::createDirectoryStructure: " << dir.string() << " - Success" << "\n";
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


DatabaseFacade::DatabaseFacade(  )    
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
            m_database = std::make_unique<SQLite::Database>( stringStream.str() , SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE );
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
        m_database->exec( "CREATE TABLE Files (Name STRING Path STRING Size INTEGER Extension STRING Year INTEGER Month Integer Day INTEGER TypeDescription STRING)" );
    }
    catch( SQLite::Exception& ex )
    {
        std::cout << "DatabaseFacade::recreateDatabaseAndTable - SQLite exception: " << ex.what( ) << std::endl;
        result = false;
    }
    return result;
}

bool DatabaseFacade::recreateDatabase( std::list<File> files )
{
    return recreateDatabaseAndTable( );
}

bool DatabaseFacade::insert( File file )
{
    bool result = true;
    try
    {
        SQLite::Transaction transaction( *m_database );

        std::stringstream command;
        command << "INSERT INTO Files VALUES (" << file.name << ", " << file.path << ", " << file.sizeInBytes << ", " << file.extension << ", " << file.lastAccessedYear << ", " << file.lastAcessedMonth << ", " << file.lastAccessedDay << ", " << file.typeDescription << ")";
        int count = m_database->exec( command.str() );
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