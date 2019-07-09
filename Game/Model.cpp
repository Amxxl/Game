#include "pch.h"
#include "Model.h"
#include "StringHelper.h"

Model::Model()
{
}


Model::~Model()
{
}

bool Model::Initialize(std::string const& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
    this->device = device;
    this->deviceContext = deviceContext;
    
    if (!LoadModel(filePath))
        return false;

    shader.InitializeShaders(deviceContext);
    return true;
}

void Model::Draw(DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj)
{
    for (uint32 i = 0; i < meshes.size(); ++i)
    {
        shader.SetShaderParameters(deviceContext, meshes[i].GetTransformMatrix() * world, view, proj);
        meshes[i].Draw();
        shader.RenderShader(deviceContext, meshes[i].GetIndexCount());
    }
}

bool Model::LoadModel(std::string const& filePath)
{
    directory = StringHelper::GetDirectoryFromPath(filePath);
    Assimp::Importer importer;

    aiScene const* pScene = importer.ReadFile(filePath, aiProcess_Triangulate |
        aiProcess_ConvertToLeftHanded);

    if (pScene == nullptr)
        return false;

    this->ProcessNode(pScene->mRootNode, pScene, DirectX::XMMatrixIdentity());
    return true;
}

void Model::ProcessNode(aiNode* node, aiScene const* scene, DirectX::XMMATRIX const& parentTransformMatrix)
{
    DirectX::XMMATRIX nodeTransformMatrix = DirectX::XMMatrixTranspose(DirectX::XMMATRIX(&node->mTransformation.a1)) * parentTransformMatrix;

    for (uint32 i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene, nodeTransformMatrix));
    }

    for (uint32 i = 0; i < node->mNumChildren; ++i)
    {
        ProcessNode(node->mChildren[i], scene, nodeTransformMatrix);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, aiScene const* scene, DirectX::XMMATRIX const& transformMatrix)
{
    std::vector<MD5Vertex> vertices;
    std::vector<DWORD> indices;

    // Get vertices
    for (uint32 i = 0; i < mesh->mNumVertices; ++i)
    {
        MD5Vertex vertex;
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

        if (mesh->mTextureCoords[0])
        {
            vertex.textureCoordinate.x = static_cast<float>(mesh->mTextureCoords[0][i].x);
            vertex.textureCoordinate.y = static_cast<float>(mesh->mTextureCoords[0][i].y);
        }

        vertices.push_back(vertex);
    }

    // Get indices
    for (uint32 i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];

        for (uint32 j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }

    std::vector<Texture> textures;
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseTextures = LoadMaterialTextures(material, aiTextureType::aiTextureType_DIFFUSE, scene);
    textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

    return Mesh(device, deviceContext, vertices, indices, textures, transformMatrix);
}

TextureStorageType Model::DetermineTextureStorageType(aiScene const* pScene, aiMaterial* pMat, unsigned int index, aiTextureType textureType)
{
    if (pMat->GetTextureCount(textureType) == 0)
        return TextureStorageType::None;

    aiString path;
    pMat->GetTexture(textureType, index, &path);
    std::string texturePath = path.C_Str();

    // Check if texture is an embedded indexed texture by seeing if the file path is an index #
    if (texturePath[0] == '*')
    {
        if (pScene->mTextures[0]->mHeight == 0)
        {
            return TextureStorageType::EmbeddedIndexCompressed;
        }
        else
        {
            assert("SUPPORT DOES NOT EXIST YET FOR INDEXED NON COMPRESSED TEXTURES!" && 0);
            return TextureStorageType::EmbeddedIndexNonCompressed;
        }
    }
    // Check if texture is an embedded texture but not indexed (path will be the texture's name instead of #)
    if (auto pTex = pScene->GetEmbeddedTexture(texturePath.c_str()))
    {
        if (pTex->mHeight == 0)
        {
            return TextureStorageType::EmbeddedCompressed;
        }
        else
        {
            assert("SUPPORT DOES NOT EXISTS YET FOR EMBEDDED NON COMPRESSED TEXTURES!" && 0);
            return TextureStorageType::EmbeddedIndexNonCompressed;
        }
    }
    //Lastly check if texture is a filepath by checking for period before extension name.
    if (texturePath.find('.') != std::string::npos)
    {
        return TextureStorageType::Disk;
    }
    return TextureStorageType::None; // No texture exists
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, aiScene const* pScene)
{
    std::vector<Texture> materialTextures;
    TextureStorageType storeType = TextureStorageType::Invalid;
    unsigned int textureCount = pMaterial->GetTextureCount(textureType);

    if (textureCount == 0) // If there are no textures.
    {
        storeType = TextureStorageType::None;
        aiColor3D aiColor(0.0f, 0.0f, 0.0f);

        switch (textureType)
        {
            case aiTextureType::aiTextureType_DIFFUSE:
            {
                pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
                if (aiColor.IsBlack())
                {
                    materialTextures.push_back(Texture(device, Colors::UnloadedTextureColor, textureType));
                    return materialTextures;
                }

                materialTextures.push_back(Texture(device, Color(static_cast<uint8>(aiColor.r * 255), static_cast<uint8>(aiColor.g * 255), static_cast<uint8>(aiColor.b * 255)), textureType));
                return materialTextures;
            }
        }
    }
    else
    {
        for (UINT i = 0; i < textureCount; ++i)
        {
            aiString path;
            pMaterial->GetTexture(textureType, i, &path);
            storeType = DetermineTextureStorageType(pScene, pMaterial, i, textureType);

            switch (storeType)
            {
                case TextureStorageType::EmbeddedIndexCompressed:
                {
                    int index = GetTextureIndex(&path);
                    Texture embeddedIndexedTexture(device, reinterpret_cast<uint8*>(pScene->mTextures[index]->pcData),
                        pScene->mTextures[index]->mWidth, textureType);
                    materialTextures.push_back(embeddedIndexedTexture);
                    break;
                }
                case TextureStorageType::EmbeddedCompressed:
                {
                    aiTexture const* pTexture = pScene->GetEmbeddedTexture(path.C_Str());
                    Texture embeddedTexture(device, reinterpret_cast<uint8*>(pTexture->pcData),
                        pTexture->mWidth, textureType);
                    materialTextures.push_back(embeddedTexture);
                    break;
                }
                case TextureStorageType::Disk:
                {
                    std::string fileName = directory + '\\' + path.C_Str();
                    Texture diskTexture(device, fileName, textureType);
                    materialTextures.push_back(diskTexture);
                    break;
                }
            }
        }
    }

    if (materialTextures.size() == 0)
    {
        materialTextures.push_back(Texture(device, Colors::UnhandledTextureColor, textureType));
    }
    return materialTextures;
}

int Model::GetTextureIndex(aiString* pStr)
{
    assert(pStr->length >= 2);
    return atoi(&pStr->C_Str()[1]);
}