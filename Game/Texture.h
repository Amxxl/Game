//
// Texture.h
//

#pragma once

#include "Color.h"
#include "Bindable.h"

namespace Bind
{
    class Texture : public Bindable
    {
        public:
            Texture(DX::DeviceResources* deviceResources, std::wstring const& file, unsigned int slot = 0);
            Texture(DX::DeviceResources* deviceResources, std::string const& file, unsigned int slot = 0);
            Texture(DX::DeviceResources* deviceResources, uint8 const* pData, size_t size, unsigned int slot = 0);
            Texture(DX::DeviceResources* deviceResources, uint16 width, uint16 height, Color const* color, unsigned int slot = 0);
            Texture(DX::DeviceResources* deviceResources, Color color, unsigned int slot = 0);

            virtual void Bind(DX::DeviceResources* deviceResources) noexcept override;

            static std::shared_ptr<Texture> Resolve(DX::DeviceResources* deviceResources, std::string const& path, unsigned int slot = 0);
            static std::string GenerateUID(std::string const& path, UINT slot = 0);
            std::string const& GetUID() const noexcept override;

        protected:
            Microsoft::WRL::ComPtr<ID3D11Resource> pTexture = nullptr;
            Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView = nullptr;

        private:
            std::string path;
            unsigned int slot;
    };
}