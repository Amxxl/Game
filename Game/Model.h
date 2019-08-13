#pragma once

#include "Mesh.h"
#include "Node.h"

#include "Texture.h"

enum class TextureStorageType
{
    None,
    IndexCompressed,
    IndexNonCompressed,
    EmbeddedCompressed,
    EmbeddedNonCompressed,
    Disk
};

class Model
{
    public:
        Model(DX::DeviceResources* deviceResources, std::string const& fileName);
        void Draw(DX::DeviceResources* deviceResources, DirectX::FXMMATRIX transform) const;

    private:
        std::unique_ptr<Mesh> ParseMesh(DX::DeviceResources* deviceResources, aiScene const* pScene, aiMesh const& mesh, aiMaterial const* const* pMaterials);
        std::unique_ptr<Node> ParseNode(aiNode const& node);

    private:
        TextureStorageType DetermineTextureStorageType(aiScene const* pScene,
            aiMaterial* pMaterial, aiTextureType textureType);

        std::unique_ptr<Bind::Texture> LoadMaterialTextureType(DX::DeviceResources* deviceResources,
            aiScene const* pScene, aiMaterial* pMaterial, aiTextureType textureType, unsigned int slot = 0);

        std::unique_ptr<Bind::Texture> LoadMaterialColor(DX::DeviceResources* deviceResources,
            aiScene const* pScene, aiMaterial* pMaterial, aiTextureType textureType, unsigned int slot = 0);
        
        std::unique_ptr<Bind::Texture> LoadMaterialTexture(DX::DeviceResources* deviceResoruces,
            aiScene const* pScene, aiMaterial* pMaterial, aiTextureType textureType, unsigned int slot = 0);

        int GetTextureIndex(aiString* pStr);

    private:
        std::unique_ptr<Node> pRoot;
        std::vector<std::unique_ptr<Mesh>> meshPtrs;
        std::string directory = "";
};