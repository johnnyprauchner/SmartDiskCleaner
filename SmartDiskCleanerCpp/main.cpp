#include <memory>
#include "SmartDiskCleanerApi.h"
#include "DatabaseFacade.h"

using namespace SmartDiskCleaner;

int main( int argc , char** argv )
{
    std::cout << "Smart Disk Cleaner Cli - argc: " << argc << " argv:" << argv << std::endl;
    if( argc < 2 )
    {
        std::cout << "Usage: SmartDiskCleaner <Path>" << std::endl;
        return -1;
    }
    SmartDiskCleanerApiPtr api = SmartDiskCleanerApi::getInstance( );
    
    DatabaseFacadePtr databaseFacade = DatabaseFacade::getInstance( );
    databaseFacade->recreateDatabase( );

    auto start = std::chrono::system_clock::now( );
    
    FileListPtr files = api->listFiles( argv[1] );

    auto end = std::chrono::system_clock::now( );
    auto elapsed =  std::chrono::duration_cast< std::chrono::seconds >( end - start );

    //File::printToStdOutFull( files );
    File::printToStdOutShort( files );
    std::cout << "Took " << elapsed.count( ) << " seconds to traverse filesystem." << std::endl;
    std::cout << "Found " << files->size( ) << " Files from filesystem:" << std::endl;


    start = std::chrono::system_clock::now( );
    
    databaseFacade->insert( files );
    
    end = std::chrono::system_clock::now( );
    elapsed = std::chrono::duration_cast< std::chrono::seconds >( end - start );
    FileListPtr filesFromDatabase = databaseFacade->listAllFiles( );
    
    //File::printToStdOutFull( filesFromDatabase );
    File::printToStdOutShort( filesFromDatabase );
    std::cout << "Took " << elapsed.count( ) << " seconds to insert files into database." << std::endl;
    std::cout << "Found " << filesFromDatabase->size( ) << " files from database:" << std::endl;
    
    return 0;
 }