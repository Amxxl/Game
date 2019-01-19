//
// Texture.h
//

#pragma once

#include "Color.h"

namespace axec
{
    class Texture
    {
        public:
            Texture();
            ~Texture();

            bool Create(ID3D11Device* device, uint16 width, uint16 height, Color const* color);

        private:
            Microsoft::WRL::ComPtr<ID3D11Resource> texture = nullptr;
            Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView = nullptr;
    };
}