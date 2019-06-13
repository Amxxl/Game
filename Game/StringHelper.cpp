#include "pch.h"
#include "StringHelper.h"


std::wstring StringHelper::StringToWide(std::string_view str)
{
    return std::wstring(str.begin(), str.end());
}

std::string StringHelper::WideToString(std::wstring_view wstr)
{
    return std::string(wstr.begin(), wstr.end());
}

std::string StringHelper::GetDirectoryFromPath(std::string_view filePath)
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

std::wstring StringHelper::GetDirectoryFromPath(std::wstring_view filePath)
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

std::string StringHelper::GetFileExtension(std::string_view fileName)
{
    size_t off = fileName.find_last_of('.');

    if (off == std::string::npos)
        return {};

    return std::string(fileName.substr(off + 1));
}

std::wstring StringHelper::GetFileExtension(std::wstring_view fileName)
{
    size_t off = fileName.find_last_of('.');

    if (off == std::string::npos)
        return {};

    return std::wstring(fileName.substr(off + 1));
}
