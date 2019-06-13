//
// Event.h
//

#pragma once

#define ON_EVENT_DISPATCHED(callback) EventManager::AddListener(std::function(callback))
#define DISPATCH_EVENT(e) EventManager::Dispatch(e);

class Event
{
    public:
        virtual ~Event() = default;
};

class EventManager
{
    public:
        template<typename T>
        static void AddListener(std::function<void(T const*)> callback)
        {
            auto callbackFunc = [=](Event const* args) {
                callback(dynamic_cast<T const*>(args));
            };

            auto index = std::type_index(typeid(T));
            s_mapCallbacks[index].emplace_back(callbackFunc);
        }

        template<typename T>
        static void Dispatch(T const& e)
        {
            auto iter = s_mapCallbacks.find(std::type_index(typeid(T)));

            if (iter != s_mapCallbacks.end())
            {
                auto const& callbacks = iter->second;

                for (auto const& callback : callbacks)
                    callback(&e);
            }
        }

    private:
        static std::unordered_map<std::type_index, std::vector<std::function<void(Event const*)>>> s_mapCallbacks;
};