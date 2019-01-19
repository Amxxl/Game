//
// Color.h
//

#pragma once

namespace axec//alibur
{
    class Color
    {
        public:
            Color() : color(0) {}
            Color(uint32 color) : color(color) {}
            Color(uint8 r, uint8 g, uint8 b, uint8 a = 255)
            {
                rgba[0] = r;
                rgba[1] = g;
                rgba[2] = b;
                rgba[3] = a;
            }

            Color& operator=(Color const& src) { color = src.color; }
            bool operator==(Color const& rhs) { return color == rhs.color; }
            bool operator!=(Color const& rhs) { return color != rhs.color; }

            constexpr uint8 GetR() const { return rgba[0]; }
            void SetR(uint8 r) { rgba[0] = r; }

            constexpr uint8 GetG() const { return rgba[1]; }
            void SetG(uint8 g) { rgba[1] = g; }

            constexpr uint8 GetB() const { return rgba[2]; }
            void SetB(uint8 b) { rgba[2] = b; }

            constexpr uint8 GetA() const { return rgba[3]; }
            void SetA(uint8 a) { rgba[3] = a; }

            constexpr uint32 GetColor() const { return color; }
            void SetColor(uint32 color) { this->color = color; }

        private:
            union
            {
                uint8 rgba[4];
                uint32 color;
            };
    };

    enum class Colors : unsigned int
    {
        BLACK = 0x000000ff,
        WHITE = 0xffffffff
    };
}