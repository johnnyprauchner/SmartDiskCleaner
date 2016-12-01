#include <memory>
#include <thread>
#include <chrono>
#include <limits>
#include <iomanip>
#include "SmartDiskCleanerApi.h"
#include "DatabaseFacade.h"

using namespace SmartDiskCleaner;

int main( int argc , char** argv )
{
    std::cout << "Smart Disk Cleaner Cli - argc: " << argc << " argv:" << argv << std::endl;
    //if( argc < 2 )
    //{
    //    std::cout << "Usage: SmartDiskCleaner <Path>" << std::endl;
    //    return -1;
    //}
    SmartDiskCleanerApiPtr api = SmartDiskCleanerApi::getInstance( );

    //DatabaseFacadePtr databaseFacade = DatabaseFacade::getInstance( );
    //databaseFacade->recreateDatabase( );

    //auto start = std::chrono::system_clock::now( );

    //std::thread crawler( &SmartDiskCleanerApi::recreateDatabase , api , argv[ 1 ] );

    //while( !api->getStatus( )->getIsDone( ) )
    //{
    //    std::this_thread::sleep_for( std::chrono::milliseconds( 4000 ) );
    //    FileCrawlerStatusPtr status = api->getStatus( );
    //    if( status->getFileCount( ) > 0 )
    //    {
    //        std::cout << "Added " << status->getFileCount( ) << " files to database." << std::endl;
    //        //std::cout << std::fixed << std::setprecision( 1 ) << status->getPercentComplete( ) << "% complete." << std::endl;
    //        std::cout << "\n";
    //    }
    //}

    //crawler.join( );

    //auto end = std::chrono::system_clock::now( );
    //auto elapsed = std::chrono::duration_cast< std::chrono::seconds >( end - start );

    ////File::printToStdOutFull( files );
    //FileListPtr files = api->getFileList( );
    ////File::printToStdOutShort( files );
    //std::cout << "Took " << elapsed.count( ) << " seconds to recreate database." << std::endl;
    //std::cout << "Added " << files->size( ) << " files in database." << std::endl;
    //std::cout << "Found " << api->getStatus( )->getUnreadableFiles( ) << " unreadable files." << std::endl;


    //FileListPtr filesFromDatabase = databaseFacade->listAllFiles( );
    QueryParameters parameters;
    FileListPtr filesFromDatabase = api->getFileList( parameters );
    std::cout << "Found " << filesFromDatabase->size( ) << " files from database." << std::endl;

    return 0;
}