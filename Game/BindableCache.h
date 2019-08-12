#pragma once

#include "Bindable.h"

namespace Bind
{
    class BindableCache
    {
        public:
            template<class T, typename... Params>
            static std::shared_ptr<T> Resolve(DX::DeviceResources* deviceResources, Params&&... params)
            {
                static_assert(std::is_base_of_v<Bindable, T>, "Can only resolve classes derived from Bindable");

                auto const key = T::GenerateUID(std::forward<Params>(params)...);
                auto const i = binds.find(key);

                if (i == binds.end())
                {
                    auto bind = std::make_shared<T>(deviceResources, std::forward<Params>(params)...);
                    binds[key] = bind;
                    return bind;
                }
                return std::static_pointer_cast<T>(i->second);
            }

        private:
            static std::unordered_map<std::string, std::shared_ptr<Bindable>> binds;
    };
}