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
    public:
        Drawable() = default;
        Drawable(Drawable const&) = delete;
        virtual ~Drawable() = default;

        void Draw(DX::DeviceResources* deviceResources) const;

        virtual DirectX::XMMATRIX GetTransform() const noexcept = 0;

    protected:
        template<class T>
        T* QueryBindable() noexcept
        {
            for (auto& pBind : binds)
            {
                if (auto pTemp = dynamic_cast<T*>(pBind.get()))
                    return pTemp;
            }
            return nullptr;
        }

        void AddBind(std::shared_ptr<Bind::Bindable> bind);

    private:
        Bind::IndexBuffer<unsigned int> const* pIndexBuffer = nullptr;
        std::vector<std::shared_ptr<Bind::Bindable>> binds;
};