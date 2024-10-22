#include "pch.h"
#include "StringHelper.h"
#include <iomanip>


std::wstring StringHelper::NarrowToWide(std::string const& str)
{
    wchar_t wide[512];
    mbstowcs_s(nullptr, wide, str.c_str(), _TRUNCATE);
    return wide;
}

std::string StringHelper::WideToNarrow(std::wstring const& wstr)
{
    char narrow[512];
    wcstombs_s(nullptr, narrow, wstr.c_str(), _TRUNCATE);
    return narrow;
}

std::string StringHelper::GetDirectoryFromPath(std::string const& filePath)
{
    size_t off1 = filePath.find_last_of('\\');
    size_t off2 = filePath.find_last_of('/');

    // If no slash or backslash
    if (off1 == std::string::npos && off2 == std::string::npos)
        return std::string();

    if (off1 == std::string::npos)
        return std::string(filePath.substr(0, off2));

    if (off2 == std::string::npos)
        return std::string(filePath.substr(0, off1));

    // If both exists, need to use the greater offset
    return std::string(filePath.substr(0, std::max(off1, off2)));
}

std::wstring StringHelper::GetDirectoryFromPath(std::wstring const& filePath)
{
    size_t off1 = filePath.find_last_of('\\');
    size_t off2 = filePath.find_last_of('/');

    // If no slash or backslash
    if (off1 == std::string::npos && off2 == std::string::npos)
        return std::wstring();

    if (off1 == std::string::npos)
        return std::wstring(filePath.substr(0, off2));

    if (off2 == std::string::npos)
        return std::wstring(filePath.substr(0, off1));

    // If both exists, need to use the greater offset
    return std::wstring(filePath.substr(0, std::max(off1, off2)));
}

std::string StringHelper::GetFileExtension(std::string const& fileName)
{
    size_t off = fileName.find_last_of('.');

    if (off == std::string::npos)
        return {};

    return std::string(fileName.substr(off + 1));
}

std::wstring StringHelper::GetFileExtension(std::wstring const& fileName)
{
    size_t off = fileName.find_last_of('.');

    if (off == std::string::npos)
        return {};

    return std::wstring(fileName.substr(off + 1));
}
