#pragma once
#include <string>

namespace StringConversion
{
    std::wstring stringToWideString( const std::string& string );
    std::string wideStringToString( const std::wstring& wideString );
}
