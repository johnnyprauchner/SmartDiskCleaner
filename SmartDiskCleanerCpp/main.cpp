#include <memory>
#include "SmartDiskCleanerApi.h"
#include "DatabaseFacade.h"

using namespace SmartDiskCleaner;

int main( int argc , char** argv )
{
    std::cout << "Smart Disk Cleaner Cli - argc: " << argc << " argv:" << argv << std::endl;
    SmartDiskCleanerApiPtr api = SmartDiskCleanerApi::create( );
    
    DatabaseFacadePtr databaseFacade = DatabaseFacade::create( );
    databaseFacade->recreateDatabase( );

    auto start = std::chrono::system_clock::now( );
    
    //std::list<File> files = api->listFiles( "C:\\temp" );
    FileListPtr files = api->listFiles( "C:\\Program Files" );

    auto end = std::chrono::system_clock::now( );
    auto elapsed =  std::chrono::duration_cast< std::chrono::seconds >( end - start );
    std::cout << "Took " << elapsed.count( ) << " seconds to traverse filesystem." << std::endl;
    
    std::cout << "Found " << files->size() << " Files from filesystem:" << std::endl;
    //File::printToStdOutFull( files );
    
    
    start = std::chrono::system_clock::now( );
    
    databaseFacade->insert( files );
    
    end = std::chrono::system_clock::now( );
    elapsed = std::chrono::duration_cast< std::chrono::seconds >( end - start );
    std::cout << "Took " << elapsed.count( ) << " seconds to insert files into database." << std::endl;

    FileListPtr filesFromDatabase = databaseFacade->listAllFiles( );

    std::cout << "Found " << filesFromDatabase->size() << " files from database:" << std::endl;
    //File::printToStdOutFull( filesFromDatabase );
    //File::printToStdOutShort( files );
}