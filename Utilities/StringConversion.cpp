#include "StringConversion.h"
#include <windows.h>
#include <codecvt>

using namespace StringConversion;

std::wstring StringConversion::stringToWideString( const std::string& string )
{
    int len;
    int slength = ( int ) string.length( ) + 1;
    len = MultiByteToWideChar( CP_ACP , 0 , string.c_str( ) , slength , 0 , 0 );
    wchar_t* buf = new wchar_t[ len ];
    MultiByteToWideChar( CP_ACP , 0 , string.c_str( ) , slength , buf , len );
    std::wstring result( buf );
    delete[ ] buf;
    return result;
}

std::string StringConversion::wideStringToString( const std::wstring& wideString )
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX , wchar_t> converterX;

    return converterX.to_bytes( wideString );
}