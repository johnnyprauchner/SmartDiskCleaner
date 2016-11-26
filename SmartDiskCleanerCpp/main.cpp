#include <memory>
#include "SmartDiskCleanerApi.h"
#include "DatabaseFacade.h"

using namespace SmartDiskCleaner;

int main( int argc , char** argv )
{
    std::cout << "Smart Disk Cleaner Cli - argc: " << argc << " argv:" << argv << std::endl;
    SmartDiskCleanerApiPtr api = SmartDiskCleanerApi::create( );
    DatabaseFacadePtr databaseFacade = DatabaseFacade::create( );
    std::list<File> files = api->listFiles( );
    File::printToStdOutFull( files );
    //File::printToStdOutShort( files );
}