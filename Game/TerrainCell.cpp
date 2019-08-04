#include "pch.h"
#include "TerrainCell.h"


TerrainCell::TerrainCell()
    : m_iVertexCount(0)
    , m_iIndexCount(0)
    , m_iLineIndexCount(0)
    , m_fMinWidth(0.0f)
    , m_fMinHeight(0.0f)
    , m_fMinDepth(0.0f)
    , m_fMaxWidth(0.0f)
    , m_fMaxHeight(0.0f)
    , m_fMaxDepth(0.0f)
{
}


TerrainCell::~TerrainCell()
{
}

bool TerrainCell::Initialize(ID3D11Device* device, void* terrainModelPtr, int nodeIndexX,
    int nodeIndexY, int cellWidth, int cellHeight, int terrainWidth)
{
    ModelType* terrainModel;
    bool result;

    terrainModel = (ModelType*)terrainModelPtr;

    result = InitializeBuffers(device, nodeIndexX, nodeIndexY, cellWidth, cellHeight, terrainWidth, terrainModel);

    if (!result)
        return false;

    terrainModel = nullptr;

    CalculateCellDimensions();

    result = BuildLineBuffers(device);
    
    if (!result)
        return false;

    return true;
}

void TerrainCell::Shutdown()
{
    ShutdownLineBuffers();
    ShutdownBuffers();
}

void TerrainCell::Draw(ID3D11DeviceContext* deviceContext)
{
    RenderBuffers(deviceContext);
}

void TerrainCell::DrawLineBuffers(ID3D11DeviceContext* deviceContext)
{
    unsigned int offset = 0;
    deviceContext->IASetVertexBuffers(0, 1, pLineVertexBuffer.GetAddressOf(), pLineVertexBuffer.StridePtr(), &offset);
    deviceContext->IASetIndexBuffer(pLineIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

int TerrainCell::GetVertexCount()
{
    return m_iVertexCount;
}

int TerrainCell::GetIndexCount()
{
    return m_iIndexCount;
}

int TerrainCell::GetLineBuffersIndexCount()
{
    return m_iLineIndexCount;
}

void TerrainCell::GetCellDimensions(float& minWidth, float& minHeight, float& minDepth,
    float& maxWidth, float& maxHeight, float& maxDepth)
{
    minWidth = m_fMinWidth;
    minHeight = m_fMinHeight;
    minDepth = m_fMinDepth;
    maxWidth = m_fMaxWidth;
    maxHeight = m_fMaxHeight;
    maxDepth = m_fMaxDepth;
}

bool TerrainCell::InitializeBuffers(ID3D11Device* device, int nodeIndexX, int nodeIndexY,
    int cellWidth, int cellHeight, int terrainWidth, ModelType* terrainModel)
{
    DirectX::VertexPositionNormalColorDualTexture* vertices;
    unsigned int* indices;
    int i, j, modelIndex, index;
    HRESULT result;

    // Calculate the number of vertices in this terrain cell.
    m_iVertexCount = (cellHeight - 1) * (cellWidth - 1) * 6;

    // Set the index count to the same as the vertex count.
    m_iIndexCount = m_iVertexCount;

    // Create the vertex array.
    vertices = new DirectX::VertexPositionNormalColorDualTexture[m_iVertexCount];
    if (!vertices)
        return false;

    // Create the index array.
    indices = new unsigned int[m_iIndexCount];
    if (!indices)
        return false;

    // Setup the indexes into the terrain model data and the local vertex/index array.
    modelIndex = ((nodeIndexX * (cellWidth - 1)) + (nodeIndexY * (cellHeight - 1) * (terrainWidth - 1))) * 6;

    index = 0;

    // Load the vertex array and index array width data.
    for (j = 0; j < (cellHeight - 1); ++j)
    {
        for (i = 0; i < ((cellWidth - 1) * 6); ++i)
        {
            vertices[index].position = DirectX::XMFLOAT3(terrainModel[modelIndex].x, terrainModel[modelIndex].y, terrainModel[modelIndex].z);
            //vertices[index].texture = DirectX::XMFLOAT2(terrainModel[modelIndex].tu, terrainModel[modelIndex].tv);
            vertices[index].normal = DirectX::XMFLOAT3(terrainModel[modelIndex].nx, terrainModel[modelIndex].ny, terrainModel[modelIndex].nz);
            //vertices[index].tangent = DirectX::XMFLOAT3(terrainModel[modelIndex].tx, terrainModel[modelIndex].ty, terrainModel[modelIndex].tz);
            //vertices[index].binormal = DirectX::XMFLOAT3(terrainModel[modelIndex].bx, terrainModel[modelIndex].by, terrainModel[modelIndex].bz);
            //vertices[index].color = DirectX::XMFLOAT3(terrainModel[modelIndex].b, terrainModel[modelIndex].g, terrainModel[modelIndex].b);
            //vertices[index].texture2 = DirectX::XMFLOAT2(terrainModel[modelIndex].tu2, terrainModel[modelIndex].tv2);
            vertices[index].textureCoordinate0 = DirectX::XMFLOAT2(terrainModel[modelIndex].u, terrainModel[modelIndex].v);
            vertices[index].textureCoordinate1 = DirectX::XMFLOAT2(terrainModel[modelIndex].u1, terrainModel[modelIndex].v1);
            indices[index] = index;
            modelIndex++;
            index++;
        }
        modelIndex += (terrainWidth * 6) - (cellWidth * 6);
    }

    // Create the vertex and index buffers.
    pVertexBuffer.Create(device, vertices, m_iVertexCount);
    pIndexBuffer.Create(device, indices, m_iIndexCount);

    // Create a public vertex array that will be used for accessing vertex information about this cell.
    pVertexList = new VectorType[m_iVertexCount];
    if (!pVertexList)
        return false;

    // Keep a local copy of the vertex position data for this cell.
    for (i = 0; i < m_iVertexCount; ++i)
    {
        pVertexList[i].x = vertices[i].position.x;
        pVertexList[i].y = vertices[i].position.y;
        pVertexList[i].z = vertices[i].position.z;
    }

    delete[] vertices;
    vertices = nullptr;

    delete[] indices;
    indices = nullptr;

    return true;
}

void TerrainCell::ShutdownBuffers()
{
    if (pVertexList)
    {
        delete[] pVertexList;
        pVertexList = nullptr;
    }
}

void TerrainCell::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
    unsigned int offset = 0;

    deviceContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), pVertexBuffer.StridePtr(), &offset);
    deviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void TerrainCell::CalculateCellDimensions()
{
    float width, height, depth;

    // Initialize the dimensions of the node.
    m_fMinWidth = 1000000.0f;
    m_fMinHeight = 1000000.0f;
    m_fMinDepth = 1000000.0f;

    m_fMaxWidth = -1000000.0f;
    m_fMaxHeight = -1000000.0f;
    m_fMaxDepth = -1000000.0f;

    for (unsigned int i = 0; i < m_iVertexCount; ++i)
    {
        width = pVertexList[i].x;
        height = pVertexList[i].y;
        depth = pVertexList[i].z;

        // Check if the width exceeds the minimum or maximum.
        if (width > m_fMaxWidth)
            m_fMaxWidth = width;

        if (width < m_fMinWidth)
            m_fMinWidth = width;

        // Check if the height exceeds the minimum or maximum.
        if (height > m_fMaxHeight)
            m_fMaxHeight = height;

        if (height < m_fMinHeight)
            m_fMinHeight = height;

        // Check if the depth exceeds the minimum or maximum.
        if (depth > m_fMaxDepth)
            m_fMaxDepth = depth;
        
        if (depth < m_fMinDepth)
            m_fMinDepth = depth;
    }

    // Calculate the center position of this cell.
    m_fPositionX = (m_fMaxWidth - m_fMinWidth) + m_fMinWidth;
    m_fPositionY = (m_fMaxHeight - m_fMinHeight) + m_fMinHeight;
    m_fPositionZ = (m_fMaxDepth - m_fMinDepth) + m_fMinDepth;
}

bool TerrainCell::BuildLineBuffers(ID3D11Device* device)
{
    std::vector<ColorVertexType> vertices;
    std::vector<unsigned int> indices;

    HRESULT result;
    DirectX::XMFLOAT4 lineColor = DirectX::XMFLOAT4(1.0f, 0.5f, 0.0f, 1.0f);

    int index = 0, vertexCount = 24, indexCount = vertexCount;

    // 8 Horizontal lines.
    indices.push_back(static_cast<int>(vertices.size()));
    vertices.push_back(ColorVertexType{ DirectX::XMFLOAT3(m_fMinWidth, m_fMinHeight, m_fMinDepth), lineColor });

    indices.push_back(static_cast<int>(vertices.size()));
    vertices.push_back(ColorVertexType{ DirectX::XMFLOAT3(m_fMaxWidth, m_fMinHeight, m_fMinDepth), lineColor });

    indices.push_back(static_cast<int>(vertices.size()));
    vertices.push_back(ColorVertexType{ DirectX::XMFLOAT3(m_fMinWidth, m_fMinHeight, m_fMaxDepth), lineColor });

    indices.push_back(static_cast<int>(vertices.size()));
    vertices.push_back(ColorVertexType{ DirectX::XMFLOAT3(m_fMaxWidth, m_fMinHeight, m_fMaxDepth), lineColor });

    indices.push_back(static_cast<int>(vertices.size()));
    vertices.push_back(ColorVertexType{ DirectX::XMFLOAT3(m_fMinWidth, m_fMinHeight, m_fMinDepth), lineColor });

    indices.push_back(static_cast<int>(vertices.size()));
    vertices.push_back(ColorVertexType{ DirectX::XMFLOAT3(m_fMinWidth, m_fMinHeight, m_fMaxDepth), lineColor });

    indices.push_back(static_cast<int>(vertices.size()));
    vertices.push_back(ColorVertexType{ DirectX::XMFLOAT3(m_fMaxWidth, m_fMinHeight, m_fMinDepth), lineColor });

    indices.push_back(static_cast<int>(vertices.size()));
    vertices.push_back(ColorVertexType{ DirectX::XMFLOAT3(m_fMaxWidth, m_fMinHeight, m_fMaxDepth), lineColor });

    indices.push_back(static_cast<int>(vertices.size()));
    vertices.push_back(ColorVertexType{ DirectX::XMFLOAT3(m_fMinWidth, m_fMaxHeight, m_fMinDepth), lineColor });

    indices.push_back(static_cast<int>(vertices.size()));
    vertices.push_back(ColorVertexType{ DirectX::XMFLOAT3(m_fMaxWidth, m_fMaxHeight, m_fMinDepth), lineColor });

    indices.push_back(static_cast<int>(vertices.size()));
    vertices.push_back(ColorVertexType{ DirectX::XMFLOAT3(m_fMinWidth, m_fMaxHeight, m_fMaxDepth), lineColor });

    indices.push_back(static_cast<int>(vertices.size()));
    vertices.push_back(ColorVertexType{ DirectX::XMFLOAT3(m_fMaxWidth, m_fMaxHeight, m_fMaxDepth), lineColor });

    indices.push_back(static_cast<int>(vertices.size()));
    vertices.push_back(ColorVertexType{ DirectX::XMFLOAT3(m_fMinWidth, m_fMaxHeight, m_fMinDepth), lineColor });

    indices.push_back(static_cast<int>(vertices.size()));
    vertices.push_back(ColorVertexType{ DirectX::XMFLOAT3(m_fMinWidth, m_fMaxHeight, m_fMaxDepth), lineColor });

    indices.push_back(static_cast<int>(vertices.size()));
    vertices.push_back(ColorVertexType{ DirectX::XMFLOAT3(m_fMaxWidth, m_fMaxHeight, m_fMinDepth), lineColor });

    indices.push_back(static_cast<int>(vertices.size()));
    vertices.push_back(ColorVertexType{ DirectX::XMFLOAT3(m_fMaxWidth, m_fMaxHeight, m_fMaxDepth), lineColor });

    // 4 Verticle lines.
    indices.push_back(static_cast<int>(vertices.size()));
    vertices.push_back(ColorVertexType{ DirectX::XMFLOAT3(m_fMaxWidth, m_fMaxHeight, m_fMaxDepth), lineColor });

    indices.push_back(static_cast<int>(vertices.size()));
    vertices.push_back(ColorVertexType{ DirectX::XMFLOAT3(m_fMaxWidth, m_fMinHeight, m_fMaxDepth), lineColor });

    indices.push_back(static_cast<int>(vertices.size()));
    vertices.push_back(ColorVertexType{ DirectX::XMFLOAT3(m_fMinWidth, m_fMaxHeight, m_fMaxDepth), lineColor });

    indices.push_back(static_cast<int>(vertices.size()));
    vertices.push_back(ColorVertexType{ DirectX::XMFLOAT3(m_fMinWidth, m_fMinHeight, m_fMaxDepth), lineColor });

    indices.push_back(static_cast<int>(vertices.size()));
    vertices.push_back(ColorVertexType{ DirectX::XMFLOAT3(m_fMaxWidth, m_fMaxHeight, m_fMinDepth), lineColor });

    indices.push_back(static_cast<int>(vertices.size()));
    vertices.push_back(ColorVertexType{ DirectX::XMFLOAT3(m_fMaxWidth, m_fMinHeight, m_fMinDepth), lineColor });

    indices.push_back(static_cast<int>(vertices.size()));
    vertices.push_back(ColorVertexType{ DirectX::XMFLOAT3(m_fMinWidth, m_fMaxHeight, m_fMinDepth), lineColor });

    indices.push_back(static_cast<int>(vertices.size()));
    vertices.push_back(ColorVertexType{ DirectX::XMFLOAT3(m_fMinWidth, m_fMinHeight, m_fMinDepth), lineColor });

    pLineVertexBuffer.Create(device, &vertices[0], static_cast<uint32>(vertices.size()));
    pLineIndexBuffer.Create(device, &indices[0], static_cast<uint32>(indices.size()));

    m_iLineIndexCount = pLineIndexBuffer.IndexCount();

    return true;
}

void TerrainCell::ShutdownLineBuffers()
{
}
