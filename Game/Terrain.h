//
// Terrain.h
//

#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Terrain
{
    public:
        Terrain();
        ~Terrain();

    private:
        VertexBuffer<DirectX::VertexPositionColor> vertexBuffer;
        IndexBuffer<DWORD> indexBuffer;
};

