#pragma once

#include <string>
#include <sstream>
#include "File.h"

namespace SmartDiskCleaner
{
    namespace SQLCommandFactory
    {
        std::string createInsertCommand( File file )
        {
            std::stringstream command;
            command << "INSERT INTO Files VALUES ("
                << "\""
                << file.name 
                << "\""
               
                << ", "
                
                << "\""
                << file.path 
                << "\""
                
                << ", "
                
                << file.sizeInBytes 
                
                << ", "
                
                << "\""
                << file.extension 
                << "\""
                
                << ", "

                << file.lastAccessedYear
                
                << ", " 
                
                << file.lastAcessedMonth 
                
                << ", " 
                
                << file.lastAccessedDay 
                
                << ", " 
                
                << "\""
                << file.typeDescription 
                << "\""
                
                << ")" 
                << std::endl;
            
            
            //std::string command = "INSERT INTO Files VALUES (\"delMameClones.bat\", \"C:\\temp\", 77590, \".bat\", 2014, 8, 10, \"Windows Batch File\")";
            return command.str( );
        }

        std::string createListAllCommand( void )
        {
            std::stringstream command;
            command << "Select * FROM Files " << std::endl;
            return command.str( );
        }
    }
}