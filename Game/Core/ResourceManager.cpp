#include "pch.h"
#include "ResourceManager.h"

/*
ResourceMap<Texture> ResourceManager::s_texturesMap;
ResourceMap<VertexShader> ResourceManager::s_vertexShadersMap;
ResourceMap<PixelShader> ResourceManager::s_pixelShadersMap;

Resource<Texture> ResourceManager::GetTexture(std::string const& path)
{
    auto iter = s_texturesMap.find(path);

    if (iter != s_texturesMap.end())
        return iter->second;

    Resource<Texture> pTexture = std::make_shared<Texture>(path);
    if (pTexture == nullptr)
        return nullptr;

    s_texturesMap[path] = pTexture;
    return pTexture;
}

Resource<VertexShader> ResourceManager::GetVertexShader(std::string const& path)
{
    auto iter = s_vertexShadersMap.find(path);

    if (iter != s_vertexShadersMap.end())
        return iter->second;

    Resource<VertexShader> pVShader = std::make_shared<VertexShader>(path);
    if (pVShader == nullptr)
        return nullptr;

    s_vertexShadersMap[path] = pVShader;
    return pVShader;
}

Resource<PixelShader> ResourceManager::GetPixelShader(std::string const& path)
{
    auto iter = s_pixelShadersMap.find(path);

    if (iter != s_pixelShadersMap.end())
        return iter->second;

    Resource<PixelShader> pPShader = std::make_shared<PixelShader>(path);
    if (pPShader == nullptr)
        return nullptr;

    s_pixelShadersMap[path] = pPShader;
    return pPShader;
}*/
