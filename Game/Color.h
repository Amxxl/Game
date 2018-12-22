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
            Color(unsigned int color) : color(color) {}
            Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
            {
                rgba[0] = r;
                rgba[1] = g;
                rgba[2] = b;
                rgba[3] = a;
            }

            Color& operator=(Color const& src) { color = src.color; }
            bool operator==(Color const& rhs) { return color == rhs.color; }
            bool operator!=(Color const& rhs) { return color != rhs.color; }

            constexpr unsigned char GetR() const { return rgba[0]; }
            void SetR(unsigned char r) { rgba[0] = r; }

            constexpr unsigned char GetG() const { return rgba[1]; }
            void SetG(unsigned char g) { rgba[1] = g; }

            constexpr unsigned char GetB() const { return rgba[2]; }
            void SetB(unsigned char b) { rgba[2] = b; }

            constexpr unsigned char GetA() const { return rgba[3]; }
            void SetA(unsigned char a) { rgba[3] = a; }

            constexpr unsigned int GetColor() const { return color; }
            void SetColor(unsigned int color) { this->color = color; }

        private:
            union
            {
                unsigned char rgba[4];
                unsigned int color;
            };
    };
}