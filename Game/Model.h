#pragma once

#include "Mesh.h"
#include "MD5ModelShader.h"

class Model
{
    public:
        Model();
        ~Model();

        bool Initialize(std::string const& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

        void Draw(DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj);

    private:
        bool LoadModel(std::string const& filePath);
        void ProcessNode(aiNode* node, aiScene const* scene);
        Mesh ProcessMesh(aiMesh* mesh, aiScene const* scene);
        TextureStorageType DetermineTextureStorageType(aiScene const* pScene, aiMaterial* pMat, unsigned int index, aiTextureType textureType);
        std::vector<Texture> LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, aiScene const* pScene);

    private:
        MD5ModelShader shader;
        std::vector<Mesh> meshes;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
        ID3D11Device* device;
        ID3D11DeviceContext* deviceContext;
        std::string directory = "";
};

