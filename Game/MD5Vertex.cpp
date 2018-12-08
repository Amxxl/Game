//
// MD5Vertex.cpp
//

#include "pch.h"
#include "MD5Vertex.h"

const D3D11_INPUT_ELEMENT_DESC MD5Vertex::InputElements[] = {
    { "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

// Check the size of vertex struct 8 byte will not be send to shader.
static_assert(sizeof(MD5Vertex) - 8 == 32, "Vertex struct/layout mismatch");