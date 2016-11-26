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

    //std::list<File> files = api->listFiles( "C:\\temp" );
    std::list<File> files = api->listFiles( "C:\\Program Files" );
    std::cout << "Found " << files.size() << " Files from filesystem:" << std::endl;
    //File::printToStdOutFull( files );
    
    databaseFacade->insert( files );
    std::list<File> filesFromDatabase = databaseFacade->listAllFiles( );

    std::cout << "Found " << filesFromDatabase.size() << " files from database:" << std::endl;
    //File::printToStdOutFull( filesFromDatabase );
    //File::printToStdOutShort( files );
}