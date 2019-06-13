//
// Event.cpp
//

#include "pch.h"
#include "Event.h"

std::unordered_map<std::type_index, std::vector<std::function<void(Event const*)>>> EventManager::s_mapCallbacks;