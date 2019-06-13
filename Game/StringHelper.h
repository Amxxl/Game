#pragma once

class StringHelper
{
    public:
        static std::wstring StringToWide(std::string_view str);
        static std::string WideToString(std::wstring_view wstr);
        static std::string GetDirectoryFromPath(std::string_view filePath);
        static std::wstring GetDirectoryFromPath(std::wstring_view filePath);
        static std::string GetFileExtension(std::string_view fileName);
        static std::wstring GetFileExtension(std::wstring_view fileName);
};