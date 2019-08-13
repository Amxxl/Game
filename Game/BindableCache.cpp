#include "pch.h"
#include "BindableCache.h"

namespace Bind
{
    std::unordered_map<std::string, std::shared_ptr<Bindable>> BindableCache::binds;

    void BindableCache::Clear()
    {
        auto iter = binds.begin();

        while (iter != binds.end())
        {
            if (iter->second.use_count() == 1)
                iter = binds.erase(iter);
            else
                iter++;
        }
    }
}