//
// MD5Model.cpp
//

#include "pch.h"
#include "MD5Model.h"


MD5Model::MD5Model()
{
}


MD5Model::~MD5Model()
{
    for (int i = 0; i < m_Meshes.size(); ++i)
    {
        delete m_Meshes[i].vertexBuffer;
        m_Meshes[i].vertexBuffer = nullptr;

        delete m_Meshes[i].indexBuffer;
        m_Meshes[i].indexBuffer = nullptr;
    }
    m_Meshes.clear();
}

bool MD5Model::LoadModel(ID3D11DeviceContext* deviceContext, std::wstring const& fileName)
{
    ID3D11Device* device = DX::GetDevice(deviceContext);
    if (device == nullptr)
        return false;

    std::wifstream fileIn(fileName.c_str());
    if (!fileIn.is_open())
        return false;

    std::wstring data;

    while (!fileIn.eof())
    {
        fileIn >> data;

        if (data == L"MD5Version")
        {
            fileIn >> m_iMD5Version; // Store version of md5mesh file.

            if (m_iMD5Version != 10) // MD5 version must be 10
                return false;
        }
        else if (data == L"commandline")
        {
            std::getline(fileIn, data); // Ignore the rest of this line.
        }
        else if (data == L"numJoints")
        {
            fileIn >> m_iNumJoints; // Store number of joints.
        }
        else if (data == L"numMeshes")
        {
            fileIn >> m_iNumMeshes; // Store number of meshes.
        }
        else if (data == L"joints")
        {
            Joint joint;

            fileIn >> data; // Skip the "{"

            for (int i = 0; i < m_iNumJoints; i++)
            {
                fileIn >> joint.m_Name;

                // Sometimes the names might contain spaces. If that is the case, we need to continue
                // to read the name until we get to the closing " (quatation mark).
                if (joint.m_Name[joint.m_Name.size() - 1] != '"')
                {
                    wchar_t character;
                    bool found = false;
                    while (!found)
                    {
                        character = fileIn.get();

                        if (character == '"')
                            found = true;

                        joint.m_Name += character;
                    }
                }

                fileIn >> joint.m_ParentId; // Store Parent joint's Id.
                fileIn >> data; // Skip the "("

                // Store position of this joint.
                fileIn >> joint.m_Pos.x >> joint.m_Pos.z >> joint.m_Pos.y;
                fileIn >> data >> data; // Skip the ")" and "("

                // Store orientation of this joint
                fileIn >> joint.m_Orient.x >> joint.m_Orient.z >> joint.m_Orient.y;

                // Remove quotation marks from joints name.
                joint.m_Name.erase(0, 1);
                joint.m_Name.erase(joint.m_Name.size() - 1, 1);

                // Compute the w axis of the quaternion.
                this->QuaternionComputeW(joint.m_Orient);

                std::getline(fileIn, data); // Skip rest of this lane.
                m_Joints.push_back(joint); // Store the joint into joint vector.
            }

            fileIn >> data; // Skip the "}"
        }
        else if (data == L"mesh")
        {
            Mesh mesh;
            int numVerts, numTris, numWeights;

            fileIn >> data; // Skip the "{"

            fileIn >> data;
            
            while (data != L"}") // Read until "}"
            {
                if (data == L"shader")
                {
                    std::wstring filePath;
                    fileIn >> filePath;

                    // Take spaces into account if file path has a space in it.
                    if (filePath[filePath.size() - 1] != '"')
                    {
                        wchar_t character;
                        bool found = false;

                        while (!found)
                        {
                            character = fileIn.get();

                            if (character == '"')
                                found = true;

                            filePath += character;
                        }
                    }

                    // Remove the quotation marks from texture path
                    filePath.erase(0, 1);
                    filePath.erase(filePath.size() - 1, 1);

                    mesh.m_Shader = filePath;

                    // Load texture.
                    DirectX::CreateWICTextureFromFile(device, mesh.m_Shader.c_str(), nullptr, mesh.texture.ReleaseAndGetAddressOf());

                    std::getline(fileIn, data); // Skip rest of this line.
                }
                else if (data == L"numverts")
                {
                    fileIn >> numVerts;
                    std::getline(fileIn, data); // Skip rest of this line.

                    for (int i = 0; i < numVerts; i++)
                    {
                        MD5Vertex vertex;

                        fileIn >> data >> data >> data; // Skip "vert # ("

                        // Store texture coordinates
                        fileIn >> vertex.textureCoordinate.x >> vertex.textureCoordinate.y;

                        fileIn >> data; // Skip ")"
                        fileIn >> vertex.StartWeight; // Index of first weight this vert will be weighted to
                        fileIn >> vertex.WeightCount; // Number of weights for this vertex.

                        std::getline(fileIn, data); // Skip rest of this lane.

                        mesh.vertices.push_back(vertex); // Push back this vertex into mesh vertices vector.
                    }
                }
                else if (data == L"numtris")
                {
                    fileIn >> numTris;
                    mesh.trianglesCount = numTris;

                    std::getline(fileIn, data); // Skip rest of this lane.

                    for (int i = 0; i < numTris; i++)
                    {
                        DWORD index;
                        fileIn >> data >> data; // Skip "tri #".

                        for (int k = 0; k < 3; k++)
                        {
                            fileIn >> index;
                            mesh.indices.push_back(index);
                        }

                        std::getline(fileIn, data); // Skip rest of this lane.
                    }
                }
                else if (data == L"numweights")
                {
                    fileIn >> numWeights;

                    std::getline(fileIn, data); // Skip rest of this lane.

                    for (int i = 0; i < numWeights; i++)
                    {
                        Weight weight;
                        fileIn >> data >> data; // Skip "weight #"

                        fileIn >> weight.m_JointId;
                        fileIn >> weight.m_Bias;
                        fileIn >> data; // Skip "(".

                        // Store weight's pos in joint's local space
                        fileIn >> weight.m_Pos.x >> weight.m_Pos.z >> weight.m_Pos.y;

                        std::getline(fileIn, data); // Skip rest of this lane.

                        mesh.m_Weights.push_back(weight); // Push back weight into mesh weight vector.
                    }
                }
                else
                    std::getline(fileIn, data); // Skip anything else.

                fileIn >> data; // Skip "}".
            }

            PrepareMesh(mesh);
            PrepareNormals(mesh);

            // Create vertex and index buffers for this mesh.
            mesh.vertexBuffer = new DynamicVertexBuffer<MD5Vertex>(device, &mesh.vertices[0], static_cast<UINT>(mesh.vertices.size()));
            mesh.indexBuffer = new IndexBuffer<DWORD>(device, &mesh.indices[0], mesh.trianglesCount * 3);

            m_Meshes.push_back(mesh);
        }
    }

    shader.InitializeShaders(deviceContext);

    return true;
}

void MD5Model::SetMatrices(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj)
{
    shader.SetShaderParameters(deviceContext, world, view, proj);
}

void MD5Model::Render(ID3D11DeviceContext* deviceContext)
{
    for (int i = 0; i < m_iNumMeshes; i++)
    {
        deviceContext->IASetIndexBuffer(m_Meshes[i].indexBuffer->Get(), DXGI_FORMAT_R32_UINT, 0);

        UINT stride = m_Meshes[i].vertexBuffer->Stride();
        UINT offset = 0;
        deviceContext->IASetVertexBuffers(0, 1, m_Meshes[i].vertexBuffer->GetAddressOf(), &stride, &offset);

        shader.SetTexture(deviceContext, m_Meshes[i].texture.Get());
        shader.RenderShader(deviceContext, static_cast<UINT>(m_Meshes[i].indices.size()));
    }
}

void MD5Model::PrepareMesh(Mesh& mesh)
{
    // Find each vertex position using the joint and weight.
    for (int i = 0; i < mesh.vertices.size(); ++i)
    {
        MD5Vertex vertex = mesh.vertices[i];
        vertex.position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

        for (int j = 0; j < vertex.WeightCount; ++j)
        {
            Weight weight = mesh.m_Weights[vertex.StartWeight + j];
            Joint joint = m_Joints[weight.m_JointId];

            DirectX::XMVECTOR tempJointOrientation = DirectX::XMVectorSet(joint.m_Orient.x, joint.m_Orient.y, joint.m_Orient.z, joint.m_Orient.w);
            DirectX::XMVECTOR tempWeightPos = DirectX::XMVectorSet(weight.m_Pos.x, weight.m_Pos.y, weight.m_Pos.z, 0.0f);

            DirectX::XMVECTOR tempJointOrientationConjugate = DirectX::XMVectorSet(-joint.m_Orient.x, -joint.m_Orient.y, -joint.m_Orient.z, joint.m_Orient.w);

            DirectX::XMFLOAT3 rotatedPoint;
            DirectX::XMStoreFloat3(&rotatedPoint, DirectX::XMQuaternionMultiply(DirectX::XMQuaternionMultiply(tempJointOrientation, tempWeightPos), tempJointOrientationConjugate));

            vertex.position.x += (joint.m_Pos.x + rotatedPoint.x) * weight.m_Bias;
            vertex.position.y += (joint.m_Pos.y + rotatedPoint.y) * weight.m_Bias;
            vertex.position.z += (joint.m_Pos.z + rotatedPoint.z) * weight.m_Bias;
        }

        // Put the vertex position into vertices for this mesh.
        mesh.vertices[i].position = vertex.position;
    }
}

// Calculate vertex normals using normal averaging.
void MD5Model::PrepareNormals(Mesh& mesh)
{
    using namespace DirectX;
    std::vector<XMFLOAT3> tempNormal;

    // normalized and unormalized normals.
    XMFLOAT3 unnormalized = XMFLOAT3(0.0f, 0.0f, 0.0f);

    // Used to get vectors (sides) from the position of the verts
    float vecX, vecY, vecZ;

    // Two edges of our triangle
    XMVECTOR edge1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    XMVECTOR edge2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

    // Compute face normals
    for (unsigned int i = 0; i < mesh.trianglesCount; ++i)
    {
        // Get the vector describing one edge of our triangle (edge, 0, 2).
        vecX = mesh.vertices[mesh.indices[(i * 3)]].position.x - mesh.vertices[mesh.indices[(i * 3) + 2]].position.x;
        vecY = mesh.vertices[mesh.indices[(i * 3)]].position.y - mesh.vertices[mesh.indices[(i * 3) + 2]].position.y;
        vecZ = mesh.vertices[mesh.indices[(i * 3)]].position.z - mesh.vertices[mesh.indices[(i * 3) + 2]].position.z;
        edge1 = XMVectorSet(vecX, vecY, vecZ, 0.0f); // Create our first edge.

        // Get the vector describing another edge of our triangle (edge 2, 1).
        vecX = mesh.vertices[mesh.indices[(i * 3) + 2]].position.x - mesh.vertices[mesh.indices[(i * 3) + 1]].position.x;
        vecY = mesh.vertices[mesh.indices[(i * 3) + 2]].position.y - mesh.vertices[mesh.indices[(i * 3) + 1]].position.y;
        vecZ = mesh.vertices[mesh.indices[(i * 3) + 2]].position.z - mesh.vertices[mesh.indices[(i * 3) + 1]].position.z;
        edge2 = XMVectorSet(vecX, vecY, vecZ, 0.0f); // Create our second edge.

        // Cross multiply the two edge vectors to get the un-normalized face normal.
        XMStoreFloat3(&unnormalized, XMVector3Cross(edge1, edge2));

        tempNormal.push_back(unnormalized);
    }

    // Compute vertex normals (normal Averaging).
    XMVECTOR normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    int facesUsing = 0;
    float tX, tY, tZ; // temp axis variables.

    // Go through each vertex
    for (unsigned int i = 0; i < mesh.vertices.size(); ++i)
    {
        // Check which triangles use this vertex
        for (unsigned int j = 0; j < mesh.trianglesCount; ++j)
        {
            if (mesh.indices[j * 3] == i || mesh.indices[(j * 3) + 1] == i || mesh.indices[(j * 3) + 2] == i)
            {
                tX = XMVectorGetX(normalSum) + tempNormal[j].x;
                tY = XMVectorGetY(normalSum) + tempNormal[j].y;
                tZ = XMVectorGetZ(normalSum) + tempNormal[j].z;

                // If a face is using the vertex, add the unnormalized face normal to the normal Sum.
                normalSum = XMVectorSet(tX, tY, tZ, 0.0f);

                facesUsing++;
            }
        }

        // Get the actual normal by dividing the normalSum by the number of faces sharing the vertex.
        normalSum = normalSum / static_cast<float>(facesUsing);

        // Normalize the normalSum vector
        normalSum = XMVector3Normalize(normalSum);

        // Store the normal in our current vertex
        mesh.vertices[i].normal.x = -XMVectorGetX(normalSum);
        mesh.vertices[i].normal.y = -XMVectorGetY(normalSum);
        mesh.vertices[i].normal.z = -XMVectorGetZ(normalSum);

        MD5Vertex vertex = mesh.vertices[i]; // Get the current vertex

        // Create the joint space normal for easy normal calculations in animation.
        //mesh.jointSpaceNormals.push_back(XMFLOAT3(0.0f, 0.0f, 0.0f)); // Push back a blank normal
        
        XMVECTOR normal = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); // Clear normal

        for (int k = 0; k < vertex.WeightCount; k++)
        {
            Joint joint = m_Joints[mesh.m_Weights[vertex.StartWeight + k].m_JointId];
            XMVECTOR jointOrientation = XMVectorSet(joint.m_Orient.x, joint.m_Orient.y, joint.m_Orient.z, joint.m_Orient.w);

            // Calculate normal based off joints orientation (turn into joint space).
            normal = XMQuaternionMultiply(XMQuaternionMultiply(XMQuaternionInverse(jointOrientation), normalSum), jointOrientation);

            XMStoreFloat3(&mesh.m_Weights[vertex.StartWeight + k].m_Normal, XMVector3Normalize(normal));
        }

        // Clear normalSum, facesUsing for next vertex
        normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
        facesUsing = 0;
    }
}

void MD5Model::QuaternionComputeW(DirectX::XMFLOAT4& q)
{
    float t = 1.0f - (q.x * q.x) - (q.y * q.y) - (q.z * q.z);
    t < 0.0f ? q.w = 0.0f : q.w = -sqrtf(t);
}