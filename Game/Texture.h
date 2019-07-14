//
// Texture.h
//

#pragma once

#include "Color.h"
#include <assimp/material.h>

#include "Bindable.h"

enum class TextureStorageType
{
    Invalid,
    None,
    EmbeddedIndexCompressed,
    EmbeddedIndexNonCompressed,
    EmbeddedCompressed,
    EmbeddedNonCompressed,
    Disk
};

class Texture
{
    public:
        Texture(ID3D11Device* device, Color const& color, aiTextureType type);
        Texture(ID3D11Device* device, uint16 width, uint16 height, Color const* color, aiTextureType type);
        Texture(ID3D11Device* device, std::string const& filePath, aiTextureType type);
        Texture(ID3D11Device* device, uint8 const* pData, size_t size, aiTextureType type);

        aiTextureType const& GetType() const { return type; }

        ID3D11ShaderResourceView* Get() const { return textureView.Get(); }
        ID3D11ShaderResourceView* const* GetAddressOf() const { return textureView.GetAddressOf(); }

    private:
        void Create1x1ColorTexture(ID3D11Device* device, Color const& color, aiTextureType type);
        void CreateColorTexture(ID3D11Device* device, uint16 width, uint16 height, Color const* color, aiTextureType type);

    private:
        Microsoft::WRL::ComPtr<ID3D11Resource> texture = nullptr;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView = nullptr;
        aiTextureType type = aiTextureType::aiTextureType_UNKNOWN;
};

namespace Bind
{
    class Texture : public Bindable
    {
        public:
            Texture(DX::DeviceResources* deviceResources, std::wstring const& file);

            virtual void Bind(DX::DeviceResources* deviceResources) noexcept override;

        private:
            Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
    };
}