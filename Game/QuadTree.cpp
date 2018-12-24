//
// QuadTree.cpp
//

#include "pch.h"
#include "QuadTree.h"


QuadTree::QuadTree()
{
    m_vertexList = nullptr;
    m_parentNode = nullptr;
}


QuadTree::~QuadTree()
{
}

bool QuadTree::Initialize(Terrain* terrain, ID3D11DeviceContext* deviceContext)
{
    DX::ExecutionTimer timer;
    int vertexCount;
    float centerX, centerZ, width;

    // Get the number of vertices in the terrain vertex array.
    vertexCount = terrain->GetVertexCount();

    // Store the total triangle count for the vertex list.
    m_triangleCount = vertexCount / 3;

    // Create a vertex array to hold all of the terrain vertices.
    m_vertexList = new DirectX::VertexPositionNormalColorDualTexture[vertexCount];
    if (!m_vertexList)
        return false;

    // Copy the terrain vertices into the vertex list.
    terrain->CopyVertexArray(static_cast<void*>(m_vertexList));

    // Calculate the center x, z and the size of the mesh.
    CalculateMeshDimensions(vertexCount, centerX, centerZ, width);

    // Create the parent node for the quad tree.
    m_parentNode = new NodeType();
    if (!m_parentNode)
        return false;

    // Recursively build the quad tree based on the vertex list data and mesh dimensions.
    CreateTreeNode(m_parentNode, centerX, centerZ, width, deviceContext);

    // Release the vertex list since the quad tree now has the vertices in each node.
    if (m_vertexList)
    {
        delete[] m_vertexList;
        m_vertexList = nullptr;
    }

    shader.InitializeShaders(deviceContext);

    return true;
}

void QuadTree::Draw(ID3D11DeviceContext* deviceContext, Frustum* frustum, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj/*, TerrainShader* shader*/)
{
    shader.SetShaderParameters(deviceContext, world, view, proj);

    // Reset the number of triangles that are drawn for this frame.
    m_drawCount = 0;

    // Render each node that is visible starting at the parent node and moving down the tree.
    DrawNode(deviceContext, frustum, m_parentNode, world, view, proj);
}

void QuadTree::Shutdown()
{
}

void QuadTree::CalculateMeshDimensions(int vertexCount, float& centerX, float& centerZ, float& meshWidth)
{
    float minWidth, maxWidth, width, minDepth, maxDepth, depth, maxX, maxZ;

    // Initialise the center position of the mesh to zero.
    centerX = 0.0f;
    centerZ = 0.0f;

    // Sum all vertices in the mesh.
    for (int i = 0; i < vertexCount; ++i)
    {
        centerX += m_vertexList[i].position.x;
        centerZ += m_vertexList[i].position.z;
    }

    // And then divide it by the number of vertices to find the mid-point of the mesh.
    centerX = centerX / static_cast<float>(vertexCount);
    centerZ = centerZ / static_cast<float>(vertexCount);

    // Initialize the maximum and minimum size of the mesh.
    minWidth = fabsf(m_vertexList[0].position.x - centerX);
    maxWidth = 0.0f;

    minDepth = fabsf(m_vertexList[0].position.z - centerZ);
    maxDepth = 0.0f;

    // Go through all the vertices and find the maximum and minimum size and depth of the mesh.
    for (int i = 0; i < vertexCount; ++i)
    {
        width = fabsf(m_vertexList[i].position.x - centerX);
        depth = fabsf(m_vertexList[i].position.z - centerZ);

        if (width < minWidth) { minWidth = width; }
        if (width > maxWidth) { maxWidth = width; }
        if (depth < minDepth) { minDepth = depth; }
        if (depth > maxDepth) { maxDepth = depth; }
    }

    // Find the absolute maximum value between the min and max depth and size.
    maxX = static_cast<float>(std::max(fabs(minWidth), fabs(maxWidth)));
    maxZ = static_cast<float>(std::max(fabs(minDepth), fabs(maxDepth)));

    // Calculate the maximum diameter of the mesh.
    meshWidth = std::max(maxX, maxZ) * 2.0f;
}

void QuadTree::CreateTreeNode(NodeType* node, float positionX, float positionZ, float width, ID3D11DeviceContext* deviceContext/*, ID3D11Device* device*/)
{
    ID3D11Device* device = DX::GetDevice(deviceContext);
    if (device == nullptr)
        return;

    int numTriangles, i, count, vertexCount, index, vertexIndex;
    float offsetX, offsetZ;
    DirectX::VertexPositionNormalColorDualTexture* vertices;
    unsigned long* indices;
    bool result;

    // Store the node position and size.
    node->positionX = positionX;
    node->positionZ = positionZ;
    node->width = width;

    // Initialize the triangle count to zero for the node.
    node->triangleCount = 0;

    // Initialize the children nodes of this node to nullptr.
    node->nodes[0] = nullptr;
    node->nodes[1] = nullptr;
    node->nodes[2] = nullptr;
    node->nodes[3] = nullptr;

    // Count the number of triangles that are inside this node.
    numTriangles = CountTriangles(positionX, positionZ, width);
    
    // Case 1: If there are no tirnalges in this node then return as it is empty and requires no processing.
    if (numTriangles == 0)
        return;

    // Case 2: If there are too many tirangles in this node then split it into four equal sized smaller tree nodes.
    if (numTriangles > MAX_TRIANGLES)
    {
        for (i = 0; i < 4; ++i)
        {
            // Calculate the position offsets for the new child node.
            offsetX = (((i % 2) < 1) ? -1.0f : 1.0f) * (width / 4.0f);
            offsetZ = (((i % 4) < 2) ? -1.0f : 1.0f) * (width / 4.0f);

            // Set if there are any triangles in the new node.
            count = CountTriangles((positionX + offsetX), (positionZ + offsetZ), (width / 2.0f));
            if (count > 0)
            {
                // If there are triangles inside where this new node would be then create the child node.
                node->nodes[i] = new NodeType();

                // Extend the tree starting from this new child node new.
                CreateTreeNode(node->nodes[i], (positionX + offsetX), (positionZ + offsetZ), (width / 2.0f), deviceContext);
            }
        }

        return;
    }

    // Case 3: If this node is not empty and the triangle count for it is less than the max then.
    // this node is at the bottom of the tree so create the list of triangles to store in it.
    node->triangleCount = numTriangles;

    // Calculate the number of vertices.
    vertexCount = numTriangles * 3;

    // Create the vertex array.
    vertices = new DirectX::VertexPositionNormalColorDualTexture[vertexCount];

    // Create the index array.
    indices = new unsigned long[vertexCount];

    // Initialize the index for this new vertex and index array.
    index = 0;

    // Ge through all the triangles in the vertex list.
    for (i = 0; i < m_triangleCount; ++i)
    {
        // If the triangle is inside this node then add it to the vertex array.
        result = IsTriangleContained(i, positionX, positionZ, width);
        if (result == true)
        {
            // Calculate the index into the terrain vertex list.
            vertexIndex = i * 3;

            // Get the three vertices of this triangle from the vertex list.
            vertices[index].position = m_vertexList[vertexIndex].position;
            vertices[index].normal = m_vertexList[vertexIndex].normal;
            vertices[index].color = m_vertexList[vertexIndex].color;
            vertices[index].textureCoordinate0 = m_vertexList[vertexIndex].textureCoordinate0;
            vertices[index].textureCoordinate1 = m_vertexList[vertexIndex].textureCoordinate1;
            indices[index] = index;
            index++;

            vertexIndex++;
            vertices[index].position = m_vertexList[vertexIndex].position;
            vertices[index].normal = m_vertexList[vertexIndex].normal;
            vertices[index].color = m_vertexList[vertexIndex].color;
            vertices[index].textureCoordinate0 = m_vertexList[vertexIndex].textureCoordinate0;
            vertices[index].textureCoordinate1 = m_vertexList[vertexIndex].textureCoordinate1;
            indices[index] = index;
            index++;

            vertexIndex++;
            vertices[index].position = m_vertexList[vertexIndex].position;
            vertices[index].normal = m_vertexList[vertexIndex].normal;
            vertices[index].color = m_vertexList[vertexIndex].color;
            vertices[index].textureCoordinate0 = m_vertexList[vertexIndex].textureCoordinate0;
            vertices[index].textureCoordinate1 = m_vertexList[vertexIndex].textureCoordinate1;
            indices[index] = index;
            index++;
        }
    }

    // Create vertex and index buffers for this node.
    node->vertexBuffer.Create(device, &vertices[0], vertexCount);
    node->indexBuffer.Create(device, &indices[0], vertexCount);

    // Release the vertex and index arrays now that the data is stored in the buffers in the node.
    delete[] vertices;
    vertices = nullptr;

    delete[] indices;
    indices = nullptr;
}

int QuadTree::CountTriangles(float positionX, float positionZ, float width)
{
    // initialize the count to zero.
    int count = 0;

    // Go through all the triangles in the entire mesh and check which ones should be inside this node.
    for (int i = 0; i < m_triangleCount; ++i)
    {
        // If the triangle is inside the node then increment the count by one.
        if (IsTriangleContained(i, positionX, positionZ, width))
            count++;
    }

    return count;
}

bool QuadTree::IsTriangleContained(int index, float positionX, float positionZ, float width)
{
    float radius;
    int vertexIndex;

    float x1, z1, x2, z2, x3, z3;
    float minimumX, maximumX, minimumZ, maximumZ;

    // Calculate the radius of this node.
    radius = width / 2.0f;

    // Get the index into the vertex list.
    vertexIndex = index * 3;


    // Get the three vertices of this triangle from the vertexList.
    x1 = m_vertexList[vertexIndex].position.x;
    z1 = m_vertexList[vertexIndex].position.z;
    vertexIndex++;

    x2 = m_vertexList[vertexIndex].position.x;
    z2 = m_vertexList[vertexIndex].position.z;
    vertexIndex++;

    x3 = m_vertexList[vertexIndex].position.x;
    z3 = m_vertexList[vertexIndex].position.z;

    // Check to see if the minimum of the x coordinates of the triangle is inside the node.
    minimumX = std::min(x1, std::min(x2, x3));
    if (minimumX > (positionX + radius))
        return false;

    // Check to see if the maximum of the x coordinates of the triangle is inside the node.
    maximumX = std::max(x1, std::max(x2, x3));
    if (maximumX < (positionX - radius))
        return false;

    // Check to see if the minimum of the z coordinates of the triangle is inside the node.
    minimumZ = std::min(z1, std::min(z2, z3));
    if (minimumZ > (positionZ + radius))
        return false;

    // Check to see if the maximum of the z coordinates of the triangle is inside the node.
    maximumZ = std::max(z1, std::max(z2, z3));
    if (maximumZ < (positionZ - radius))
        return false;

    return true;
}

void QuadTree::ReleaseNode(NodeType* node)
{
}

void QuadTree::DrawNode(ID3D11DeviceContext* deviceContext, Frustum* frustum, NodeType* node, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj/*, TerrainShader* shader*/)
{
    bool result;
    int count, i, indexCount;

    // Check to see if the node can be viewed, height doesn't matter in a quad tree.
    result = frustum->CheckCube(DirectX::XMFLOAT3(node->positionX, 0.0f, node->positionZ), (node->width / 2.0f));

    // If it can't be seen then none of its children can either so don't continue down the tree, this is where the speed is gained.
    if (!result)
        return;

    // If it can be seen then check all four child nodes to see if they can also be seen.
    count = 0;
    for (i = 0; i < 4; ++i)
    {
        if (node->nodes[i] != nullptr)
        {
            count++;
            DrawNode(deviceContext, frustum, node->nodes[i], world, view, proj);
        }
    }

    // If there were any children nodes then there is no need to continue as parent nodes won't contain any triangles to render.
    if (count != 0)
        return;

    // Otherwise if this node can be seen and has triangles in it then render these triangles.

    // Set vertex buffer stride and offset.
    unsigned int offset = 0;

    // Set the vertex buffer to active in the input assambler so it can be rendered.
    deviceContext->IASetVertexBuffers(0, 1, node->vertexBuffer.GetAddressOf(), node->vertexBuffer.StridePtr(), &offset);

    // Set the index buffer to active in the 
    deviceContext->IASetIndexBuffer(node->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Determine the number of indices in this node.
    indexCount = node->triangleCount * 3;

    // Increase the count of the number of polygons that have been rendered during this frame.
    m_drawCount += node->triangleCount;

    // Call the terrain shader to render the polygons in this node.
    shader.RenderShader(deviceContext, indexCount);
}