#pragma once
#include <string>

namespace SmartDiskCleaner
{
    namespace StringConversion
    {
        std::wstring stringToWideString( const std::string& string );
        std::string wideStringToString( const std::wstring& wideString );
    }
}

