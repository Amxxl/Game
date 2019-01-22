#include "pch.h"
#include "Model.h"


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
    LoadModel(filePath);


    DirectX::CreateWICTextureFromFile(device, L"Data/WoodCabinDif.jpg", nullptr, texture.ReleaseAndGetAddressOf());

    shader.InitializeShaders(deviceContext);
    return false;
}

void Model::Draw(DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj)
{
    shader.SetShaderParameters(deviceContext, world, view, proj);

    for (uint32 i = 0; i < meshes.size(); ++i)
    {
        shader.SetTexture(deviceContext, texture.Get());
        meshes[i].Draw();
        shader.RenderShader(deviceContext, meshes[i].GetIndexCount());
    }
}

bool Model::LoadModel(std::string const &filePath)
{
    Assimp::Importer importer;

    aiScene const* pScene = importer.ReadFile(filePath, aiProcess_Triangulate |
        aiProcess_ConvertToLeftHanded);

    if (pScene == nullptr)
        return false;

    this->ProcessNode(pScene->mRootNode, pScene);
    return false;
}

void Model::ProcessNode(aiNode* node, aiScene const* scene)
{
    for (uint32 i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }

    for (uint32 i = 0; i < node->mNumChildren; ++i)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh * mesh, aiScene const * scene)
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

    return Mesh(device, deviceContext, vertices, indices);
}
