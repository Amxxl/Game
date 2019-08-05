#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"

namespace Bind
{
    class Bindable;

    template<typename T>
    class IndexBuffer;
}

class Drawable
{
    template<class T>
    friend class DrawableBase;

    public:
        Drawable() = default;
        Drawable(Drawable const&) = delete;
        virtual ~Drawable() = default;

        virtual DirectX::XMMATRIX GetTransform() const noexcept = 0;
        void Draw(DX::DeviceResources* deviceResources) const;
        virtual void Update(DX::StepTimer const& timer) noexcept {}

    protected:
        template<class T>
        T* QueryBindable() noexcept
        {
            for (auto& pb : binds)
            {
                if (auto pt = dynamic_cast<T*>(pb.get()))
                    return pt;
            }
            return nullptr;
        }
        void AddBind(std::unique_ptr<Bind::Bindable> bind);
        void AddIndexBuffer(std::unique_ptr<Bind::IndexBuffer<unsigned int>> indexBuffer);

    private:
        virtual std::vector<std::unique_ptr<Bind::Bindable>> const& GetStaticBinds() const noexcept = 0;

    private:
        Bind::IndexBuffer<unsigned int> const* pIndexBuffer = nullptr;
        std::vector<std::unique_ptr<Bind::Bindable>> binds;
};