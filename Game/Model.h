#pragma once

#include "Mesh.h"
#include "Node.h"
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
        void ProcessNode(aiNode* node, aiScene const* scene, DirectX::XMMATRIX const& parentTransformMatrix);
        Mesh ProcessMesh(aiMesh* mesh, aiScene const* scene, DirectX::XMMATRIX const& transformMatrix);
        TextureStorageType DetermineTextureStorageType(aiScene const* pScene, aiMaterial* pMat, unsigned int index, aiTextureType textureType);
        std::vector<Texture> LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, aiScene const* pScene);
        int GetTextureIndex(aiString* pStr);

    private:
        MD5ModelShader shader;
        std::vector<Mesh> meshes;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
        ID3D11Device* device;
        ID3D11DeviceContext* deviceContext;
        std::string directory = "";
};

namespace expr
{
    class Model
    {
        public:
            Model(DX::DeviceResources* deviceResources, std::string const& fileName);
            void Draw(DX::DeviceResources* deviceResources, DirectX::FXMMATRIX transform) const;

        private:
            std::unique_ptr<Mesh> ParseMesh(DX::DeviceResources* deviceResources, aiMesh const& mesh, aiMaterial const* const* pMaterials);
            std::unique_ptr<Node> ParseNode(aiNode const& node);


        private:
            /*
            void LoadMaterialTextures(DX::DeviceResources* deviceResources, aiMesh const& mesh,
                std::vector<std::unique_ptr<Bind::Bindable>>& bindables, aiTextureType textureType,
                aiScene const* pScene, aiMaterial const* const* pMaterials);

            TextureStorageType DetermineTextureStorageType(aiScene const* pScene, aiMaterial* pMaterial,
                unsigned int index, aiTextureType textureType);

            int GetTextureIndex(aiString* pStr);*/

        private:
            std::unique_ptr<Node> pRoot;
            std::vector<std::unique_ptr<Mesh>> meshPtrs;
            std::string directory = "";
    };
}