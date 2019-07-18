#pragma once

class Vector3 : public DirectX::XMFLOAT3
{
    public:
        Vector3() = default;
        Vector3(float x, float y, float z) : XMFLOAT3(x, y, z)
        {
        }

        Vector3(DirectX::XMFLOAT3 const& src) : XMFLOAT3(src.x, src.y, src.z)
        {
        }

        Vector3& operator=(DirectX::XMFLOAT3 const& src)
        {
            x = src.x;
            y = src.y;
            z = src.z;
            return *this;
        }
};

using Vector3f = Vector3;