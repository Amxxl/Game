#pragma once

/*
class Texture;
class VertexShader;
class PixelShader;

template<typename T>
using Resource = std::shared_ptr<T>;

template<typename T>
using ResourceMap = std::unordered_map<std::string, Resource<T>>;

class ResourceManager
{
    public:
        template<typename T>
        static Resource<T> Get(std::string const& path)
        {
            if constexpr (std::is_same_v<T, Texture>)
                return ResourceManager::GetTexture(path);
            else if constexpr (std::is_same_v<T, VertexShader>)
                return ResourceManager::GetVertexShader(path);
            else if constexpr (std::is_same_v<T, PixelShader>)
                return ResourceManager::GetPixelShader(path);
            else
            {
                assert("ResourceManager: Wtf you doing?" && false);
                return nullptr;
            }
        }

    private:
        static Resource<Texture> GetTexture(std::string const& path);
        static Resource<VertexShader> GetVertexShader(std::string const& path);
        static Resource<PixelShader> GetPixelShader(std::string const& path);


    private:
        static ResourceMap<Texture> s_texturesMap;
        static ResourceMap<VertexShader> s_vertexShadersMap;
        static ResourceMap<PixelShader> s_pixelShadersMap;
};*/