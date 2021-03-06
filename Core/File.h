#pragma once
#include <string>
#include <chrono>
#include <list>
#include <iostream>
#include <ctime>
#include <memory>

namespace SmartDiskCleaner
{
    struct File
    {
        std::string name;
        std::string path;
        unsigned long long sizeInBytes;
        int lastAccessedYear;
        int lastAcessedMonth;
        int lastAccessedDay;
        std::string extension;
        std::string typeDescription;

        static void printToStdOutFull( std::shared_ptr<std::list<File>> files )
        {
            for( File file : *files )
            {
                std::cout << "\tName: " << file.name << std::endl;
                std::cout << "\tPath: " << file.path << std::endl;
                std::cout << "\tExtension: " << file.extension << std::endl;
                std::cout << "\tType Description" << file.typeDescription << std::endl;
                std::cout << "\tSize in bytes: " << file.sizeInBytes << std::endl;

                std::cout << "\tLast Accessed: " << file.lastAccessedDay << "/" << file.lastAcessedMonth << "/" << file.lastAccessedYear << std::endl;
                std::cout << "\n";
            }
        }

        static void printToStdOutShort( std::shared_ptr<std::list<File>> files )
        {
            for( File file : *files )
            {
                std::cout << "\t" << file.path << "\\" << file.name << std::endl;
            }
            std::cout << "\n" << std::endl;
        }
    };

    typedef std::shared_ptr<std::list<File>> FileListPtr;

}