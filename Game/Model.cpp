#include "pch.h"
#include "Model.h"
#include "StringHelper.h"


Model::Model(DX::DeviceResources* deviceResources, std::string const& fileName)
{
    directory = StringHelper::GetDirectoryFromPath(fileName);

    Assimp::Importer importer;
    auto const pScene = importer.ReadFile(fileName,
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ConvertToLeftHanded |
        aiProcess_GenNormals);

    for (size_t i = 0; i < pScene->mNumMeshes; ++i)
    {
        meshPtrs.push_back(ParseMesh(deviceResources, pScene, *pScene->mMeshes[i], pScene->mMaterials));
    }

    pRoot = ParseNode(*pScene->mRootNode);
}

void Model::Draw(DX::DeviceResources* deviceResources, DirectX::FXMMATRIX transform) const
{
    pRoot->Draw(deviceResources, transform);
}

std::unique_ptr<Mesh> Model::ParseMesh(DX::DeviceResources* deviceResources, aiScene const* pScene, aiMesh const& mesh, aiMaterial const* const* pMaterials)
{
    VertexBufferData vbuf(std::move(
        VertexLayout{}
        << VertexLayout::Position3D
        << VertexLayout::Normal
        << VertexLayout::Texture2D
    ));

    for (unsigned int i = 0; i < mesh.mNumVertices; ++i)
    {
        vbuf.EmplaceBack(
            *reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mVertices[i]),
            *reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mNormals[i]),
            *reinterpret_cast<DirectX::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
        );
    }

    std::vector<unsigned int> indices;
    indices.reserve(static_cast<size_t>(mesh.mNumFaces) * 3);

    for (unsigned int i = 0; i < mesh.mNumFaces; ++i)
    {
        auto const& face = mesh.mFaces[i];
        assert(face.mNumIndices == 3);
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }

    std::vector<std::shared_ptr<Bind::Bindable>> bindablePtrs;

    bool hasSpecularMap = false;
    float shininess = 32.0f;

    if (mesh.mMaterialIndex >= 0)
    {
        auto& material = *pMaterials[mesh.mMaterialIndex];

        //aiMaterial* pMaterial = pScene->mMaterials[mesh.mMaterialIndex];//&(*pMaterials[mesh.mMaterialIndex]);
        aiString texFileName;
        material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName);
        
        bindablePtrs.push_back(Bind::Texture::Resolve(deviceResources, directory + '\\' + texFileName.C_Str(), 0));
    
        //bindablePtrs.push_back(LoadMaterialTextureType(deviceResources, pScene, pMaterial, aiTextureType_DIFFUSE, 0));

        //hasSpecularMap = true;
        //bindablePtrs.push_back(LoadMaterialTextureType(deviceResources, pScene, pMaterial, aiTextureType_SPECULAR, 1));
        
        if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
        {
            bindablePtrs.push_back(Bind::Texture::Resolve(deviceResources, directory + '\\' + texFileName.C_Str(), 1));
            hasSpecularMap = true;
        }
        else
        {
            material.Get(AI_MATKEY_SHININESS, shininess);
        }

        bindablePtrs.push_back(Bind::Sampler::Resolve(deviceResources, Bind::Sampler::State::ANISOTROPIC_WRAP));
    }

    bindablePtrs.push_back(std::make_unique<Bind::VertexBuffer<VertexBufferData>>(deviceResources, vbuf));
    bindablePtrs.push_back(std::make_unique<Bind::IndexBuffer<unsigned int>>(deviceResources, indices));

    auto pvs = Bind::VertexShader::Resolve(deviceResources, "VertexShader.vs");
    auto pvsbc = pvs->GetBytecode();
    bindablePtrs.push_back(std::move(pvs));

    bindablePtrs.push_back(Bind::InputLayout::Resolve(deviceResources, vbuf.GetLayout(), pvsbc));


    if (hasSpecularMap)
    {
        bindablePtrs.push_back(Bind::PixelShader::Resolve(deviceResources, "Data/Shaders/PixelShaderSpec.ps"));
    }
    else
    {
        bindablePtrs.push_back(Bind::PixelShader::Resolve(deviceResources, "PixelShader.ps"));


        struct PSMaterialConstant
        {
            float specularIntensity = 0.8f;
            float specularPower;
            float padding[2];
        } pmc;

        pmc.specularPower = shininess;

        bindablePtrs.push_back(Bind::PixelConstantBuffer<PSMaterialConstant>::Resolve(deviceResources, pmc, 1u));
    }

    return std::make_unique<Mesh>(deviceResources, std::move(bindablePtrs));
}

std::unique_ptr<Node> Model::ParseNode(aiNode const& node)
{
    auto const transform = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(
        reinterpret_cast<DirectX::XMFLOAT4X4 const*>(&node.mTransformation)
    ));

    std::vector<Mesh*> curMeshPtrs;
    curMeshPtrs.reserve(node.mNumMeshes);
    for (size_t i = 0; i < node.mNumMeshes; ++i)
    {
        auto const meshIndex = node.mMeshes[i];
        curMeshPtrs.push_back(meshPtrs.at(meshIndex).get());
    }

    auto pNode = std::make_unique<Node>(std::move(curMeshPtrs), transform);
    for (size_t i = 0; i < node.mNumChildren; ++i)
    {
        pNode->AddChild(ParseNode(*node.mChildren[i]));
    }

    return pNode;
}

TextureStorageType Model::DetermineTextureStorageType(aiScene const* pScene,
    aiMaterial* pMaterial, aiTextureType textureType)
{
    if (!pMaterial->GetTextureCount(textureType))
        return TextureStorageType::None;

    aiString path;
    pMaterial->GetTexture(textureType, 0, &path);
    std::string texturePath = path.C_Str();

    if (texturePath[0] == '*')
    {
        if (pScene->mTextures[0]->mHeight == 0)
            return TextureStorageType::IndexCompressed;
        else
            return TextureStorageType::IndexNonCompressed;
    }
    else if (auto pTexture = pScene->GetEmbeddedTexture(texturePath.c_str()))
    {
        if (pTexture->mHeight == 0)
            return TextureStorageType::EmbeddedCompressed;
        else
            return TextureStorageType::EmbeddedNonCompressed;
    }
    else if (texturePath.find('.') != std::string::npos)
        return TextureStorageType::Disk;

    return TextureStorageType::None;
}

std::unique_ptr<Bind::Texture> Model::LoadMaterialTextureType(DX::DeviceResources* deviceResources,
    aiScene const* pScene, aiMaterial* pMaterial, aiTextureType textureType, unsigned int slot)
{
    auto storageType = DetermineTextureStorageType(pScene, pMaterial, textureType);

    if (storageType == TextureStorageType::None)
        return LoadMaterialColor(deviceResources, pScene, pMaterial, textureType, slot);
    else
        return LoadMaterialTexture(deviceResources, pScene, pMaterial, textureType, slot);

    return nullptr;
}

std::unique_ptr<Bind::Texture> Model::LoadMaterialColor(DX::DeviceResources* deviceResources,
    aiScene const* pScene, aiMaterial* pMaterial, aiTextureType textureType, unsigned int slot)
{
    Color color(255, 0, 0);
    return std::make_unique<Bind::Texture>(deviceResources, color, slot);
}

std::unique_ptr<Bind::Texture> Model::LoadMaterialTexture(DX::DeviceResources* deviceResources,
    aiScene const* pScene, aiMaterial* pMaterial, aiTextureType textureType, unsigned int slot)
{
    for (unsigned int i = 0; i < pMaterial->GetTextureCount(textureType) && i < 1; ++i)
    {
        aiString path;
        pMaterial->GetTexture(textureType, i, &path);
        auto storageType = DetermineTextureStorageType(pScene, pMaterial, textureType);

        switch (storageType)
        {
            case TextureStorageType::IndexCompressed:
            {
                int index = GetTextureIndex(&path);
                return std::make_unique<Bind::Texture>(deviceResources,
                    reinterpret_cast<uint8*>(pScene->mTextures[index]->pcData),
                    pScene->mTextures[index]->mWidth, 0);
            }
            case TextureStorageType::IndexNonCompressed:
            {
                int index = GetTextureIndex(&path);
                return std::make_unique<Bind::Texture>(deviceResources,
                    reinterpret_cast<uint8*>(pScene->mTextures[index]->pcData),
                    pScene->mTextures[index]->mWidth * pScene->mTextures[index]->mHeight, slot);
            }
            case TextureStorageType::EmbeddedCompressed:
            {
                aiTexture const* pTexture = pScene->GetEmbeddedTexture(path.C_Str());
                return std::make_unique<Bind::Texture>(deviceResources,
                    reinterpret_cast<uint8*>(pTexture->pcData), pTexture->mWidth, slot);
            }
            case TextureStorageType::EmbeddedNonCompressed:
            {
                aiTexture const* pTexture = pScene->GetEmbeddedTexture(path.C_Str());
                return std::make_unique<Bind::Texture>(deviceResources,
                    reinterpret_cast<uint8*>(pTexture->pcData),
                    pTexture->mWidth * pTexture->mHeight, slot);
            }
            case TextureStorageType::Disk:
            {
                std::string dir = directory + '\\' + path.C_Str();
                return std::make_unique<Bind::Texture>(deviceResources, dir, slot);
            }
            default:
                return nullptr;
        }
    }
    return nullptr;
}

int Model::GetTextureIndex(aiString* pStr)
{
    assert(pStr->length >= 2 || pStr->C_Str()[0] != '*');
    return std::stoi(&pStr->C_Str()[1]);
}