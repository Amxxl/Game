//
// Vector2.h
//

#pragma once

template<typename T>
class _Vector2
{
    public:
        _Vector2() = default;
        _Vector2(T x, T y) : x(x), y(y) { }

        _Vector2 operator-(_Vector2 const& rhs) const
        {
            return { x - rhs.x, y - rhs.y };
        }

    public:
        T x, y;
};

using Vector2i = _Vector2<int>;
using Vector2d = _Vector2<double>;

class Vector2 : public DirectX::XMFLOAT2
{
    public:
        Vector2() = default;
        Vector2(float x, float y) : DirectX::XMFLOAT2(x, y) { }

        Vector2 operator-(Vector2 const& rhs) const
        {
            return { x - rhs.x, y - rhs.y };
        }
};

using Vector2f = Vector2;