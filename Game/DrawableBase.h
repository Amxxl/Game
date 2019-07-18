#pragma once

#include "pch.h"
#include "Drawable.h"

template<class T>
class DrawableBase : public Drawable
{
    protected:
        static bool IsStaticInitialized() noexcept
        {
            return !staticBinds.empty();
        }

        static void AddStaticBind(std::unique_ptr<Bind::Bindable> bind)
        {
            assert("Must use AddStaticIndexBffer to bind index buffer" && typeid(*bind) != typeid(Bind::IndexBuffer));
            staticBinds.push_back(std::move(bind));
        }

        void AddStaticIndexBuffer(std::unique_ptr<Bind::IndexBuffer<unsigned short>> indexBuffer)
        {
            assert("Attempting to add index buffer more than once." && pIndexBuffer == nullptr);
            pIndexBuffer = indexBuffer.get();
            staticBinds.push_back(std::move(indexBuffer));
        }

        void SetIndexFromStatic()
        {
            assert("Attempting to add index buffer more than once" && pIndexBuffer == nullptr);

            for (auto const& b : staticBinds)
            {
                if (auto const p = dynamic_cast<Bind::IndexBuffer*>(b.get()))
                {
                    pIndexBuffer = p;
                    return;
                }
            }
            assert("Failed to find index buffer in static binds" && pIndexBuffer != nullptr);
        }
    private:
        std::vector<std::unique_ptr<Bind::Bindable>> const& GetStaticBinds() const noexcept override
        {
            return staticBinds;
        }

    private:
        static std::vector<std::unique_ptr<Bind::Bindable>> staticBinds;
};

template<class T>
std::vector<std::unique_ptr<Bind::Bindable>> DrawableBase<T>::staticBinds;
