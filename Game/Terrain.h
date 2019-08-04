//
// Terrain.h
//

#pragma once

#include "VertexBuffer.h"
#include "VertexBufferTypes.h"
#include "IndexBuffer.h"
#include "TerrainShader.h"
#include "CommonStates.h"
#include "Frustum.h"
#include "TerrainCell.h"

class Terrain
{
    public:
        Terrain();
        ~Terrain();

        struct HeightMapType
        {
            float x, y, z;
            float nx, ny, nz;
            float u, v;
            float u1, v1;
        };

        struct ModelType
        {
            float x, y, z;
            float nx, ny, nz;
            float u, v;
            float u1, v1;
        };
        
        struct VectorType
        {
            float x, y, z;
        };

        void Initialize(ID3D11DeviceContext* deviceContext);
        void CopyVertexArray(void* vertexList);

        int GetVertexCount() const { return m_vertexCount; }

        bool DrawCell(ID3D11DeviceContext* deviceContext, int cellId, Frustum* frustum);
        void DrawCellLines(ID3D11DeviceContext* deviceContext, int cellId);

        int GetCellIndexCount(int cellId);
        int GetCellLinesIndexCount(int cellId);
        int GetCellCount();
        int GetDrawCount();
        int GetCellsDrawn();
        int GetCellsCulled();

        bool GetHeightAtPosition(float x, float z, float& height);

    private:
        bool LoadRawHeightMap(char const* fileName);

        void SetTerrainCoordinates();
        void SetTextureCoordinates();
        void SetTextureCoordinates1();
        void CalculateNormals();

        bool BuildTerrainModel();
        void ShutdownTerrainModel();

        bool LoadTerrainCells(ID3D11Device* device);
        void ShutdownTerrainCells();

        bool GetHeightOfTriangle(float x, float z, float& height, float v0[3], float v1[3], float v2[3]);

    private:
        static constexpr int const TEXTURE_REPEAT = 16;

        int m_terrainWidth, m_terrainHeight;
        int m_vertexCount;
        float m_heightScale;
        HeightMapType* m_heightMap;
        ModelType* m_terrainModel;
        TerrainCell* m_pTerrainCells;
        DirectX::VertexPositionNormalColorDualTexture* m_vertices;

    private:
        int m_iCellCount, m_iDrawCount, m_iCellsDrawn, m_iCellsCulled;
};