#pragma once

class StringHelper
{
    public:
        static std::wstring NarrowToWide(std::string const& str);
        static std::string WideToNarrow(std::wstring const& wstr);
        static std::string GetDirectoryFromPath(std::string const& filePath);
        static std::wstring GetDirectoryFromPath(std::wstring const& filePath);
        static std::string GetFileExtension(std::string const& fileName);
        static std::wstring GetFileExtension(std::wstring const& fileName);
};