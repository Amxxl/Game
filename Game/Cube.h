#pragma once

#include <optional>
#include "Vertex.h"
#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include <array>

class Cube
{
    public:
        static IndexedTriangleList MakeIndependent(dvt::VertexLayout layout);
        static IndexedTriangleList MakeIndependentTextured();
};