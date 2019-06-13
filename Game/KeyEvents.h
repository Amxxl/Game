#pragma once

#include "Event.h"

class EventKeyPressed : public Event
{
    public:
        EventKeyPressed(size_t const key)
            : m_iKey(key)
        {
        }

        size_t GetKey() const { return m_iKey; }

    private:
        size_t m_iKey;
};

class EventKeyReleased : public Event
{
    public:
        EventKeyReleased(size_t const key)
            : m_iKey(key)
        {
        }

        size_t GetKey() const { return m_iKey; }

    private:
        size_t m_iKey;
};