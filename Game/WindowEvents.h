#pragma once

#include "Event.h"

class EventWindowResized : public Event
{
    public:
        EventWindowResized(int width, int height)
            : m_iWidth(width)
            , m_iHeight(height)
        {
        }

        int const GetWidth() const { return m_iWidth; }
        int const GetHeight() const { return m_iHeight; }

    private:
        int m_iWidth;
        int m_iHeight;
};

class EventWindowMoved : public Event
{
    public:
        EventWindowMoved(int x, int y)
            : m_iX(x)
            , m_iY(y)
        {
        }

        int const GetPositionX() const { return m_iX; }
        int const GetPositionY() const { return m_iY; }

    private:
        int m_iX, m_iY;
};

class EventWindowClosed : public Event
{
    public:
        EventWindowClosed() = default;
};