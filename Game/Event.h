//
// Event.h
//

#pragma once

template<typename Event>
struct EventCallback
{
    explicit EventCallback(std::function<void(Event const&)> callback)
        : callback(callback)
    {
    }

    void operator()(void const* event)
    {
        callback(*(static_cast<Event const*>(event)));
    }

    std::function<void(Event const&)> callback;
};

class EventDispatcher
{
    public:
        template<typename Event, typename Listener>
        void AddEventListener(Listener& listener)
        {
            assert(!IsListeningForEvent<Event>(listener)); // Attempted to listen to the same event more than once.
            void(Listener::*listen_func)(Event const&) = &Listener::OnEvent;

            auto callback = EventCallback<Event>(std::bind(listen_func, &listener, std::placeholders::_1));
            m_callbacks[std::type_index(typeid(Event))].emplace_back(&listener, callback);
        }

        template<typename Event, typename Listener>
        bool RemoveEventListener(Listener& listener)
        {
            auto& listeners = m_callbacks[std::type_index(typeid(Event))];

            for (size_t i = 0; i < listeners.size(); ++i)
            {
                if (listeners[i].address == &listener)
                {
                    listeners.erase(listeners.begin() + i);
                    return true;
                }
            }

            return false;
        }

        template<typename Event, typename Listener>
        inline bool IsListeningForEvent(Listener& listener)
        {
            auto& listeners = m_callbacks[std::type_index(typeid(Event))];

            for (size_t i = 0; i < listeners.size(); ++i)
            {
                if (listeners[i].address == &listener)
                    return true;
            }

            return false;
        }

        template<typename Event, typename Listener>
        void OnEventDispatched(Listener& listener)
        {
            EventCallback<Event> callback(listener);
            m_callbacks[std::type_index(typeid(Event))].emplace_back(nullptr, callback);
        }

    protected:
        template<typename Event, typename... Args>
        void DispatchEvent(Args&&... args)
        {
            auto const& listeners = m_callbacks[std::type_index(typeid(Event))];

            if (listeners.empty())
                return;

            Event event = Event{ std::forward<Args>(args)... };

            for (auto listener : listeners)
                listener.callback(&event);
        }

    private:
        struct EventListener
        {
            explicit EventListener(void* address, std::function<void(void const*)> callback)
                : address(address)
                , callback(callback)
            {
            }

            void* address;
            std::function<void(void const*)> callback;
        };

    private:
        std::unordered_map<std::type_index, std::vector<EventListener>> m_callbacks{ 20 };
};