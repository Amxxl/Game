#include "pch.h"
#include "BindableCache.h"

namespace Bind
{
    std::unordered_map<std::string, std::shared_ptr<Bindable>> BindableCache::binds;
}