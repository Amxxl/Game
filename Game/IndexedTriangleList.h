#pragma once

#include "Vertex.h"
#include <vector>
#include <DirectXMath.h>

class IndexedTriangleList
{
    public:
        IndexedTriangleList() = default;
        IndexedTriangleList(dvt::VertexBuffer verts_in, std::vector<unsigned short> indices_in);

        void Transform(DirectX::FXMMATRIX matrix);
        void SetNormalsIndependentFlat();

    public:
        dvt::VertexBuffer vertices;
        std::vector<unsigned short> indices;
};

