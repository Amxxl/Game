#pragma once

class Vector3 : public DirectX::XMFLOAT3
{
    public:
        Vector3() = default;
        Vector3(float x, float y, float z) : XMFLOAT3(x, y, z)
        {
        }
};

using Vector3f = Vector3;