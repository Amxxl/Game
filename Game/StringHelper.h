#pragma once

class StringHelper
{
    public:
        static std::wstring StringToWide(std::string str);
        static std::string GetDirectoryFromPath(std::string const& filePath);
        static std::string GetFileExtension(std::string const& fileName);
};