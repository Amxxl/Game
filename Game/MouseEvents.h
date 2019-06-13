#pragma once

#include "Event.h"
#include "Vector2.h"
#include "Input.h"

class EventMouseMoved : public Event
{
    public:
        EventMouseMoved(Vector2i const& position, Vector2i const& old_position)
            : m_vecPosition(position)
            , m_vecOldPosition(old_position)
        {
        }

        Vector2i GetPosition() const { return m_vecPosition; }
        Vector2i GetOldPosition() const { return m_vecOldPosition; }
        Vector2i GetPositionDelta() const { return m_vecPosition - m_vecOldPosition; }

    private:
        Vector2i m_vecPosition;
        Vector2i m_vecOldPosition;
};

class EventMouseMovedRaw : public Event
{
    public:
        EventMouseMovedRaw(Vector2i const& position)
            : m_vecPosition(position)
        {
        }

        Vector2i GetPosition() const { return m_vecPosition; }

    private:
        Vector2i m_vecPosition;
};

class EventMouseWheelScrolled : public Event
{
    public:
        EventMouseWheelScrolled(Vector2i const& position, float const delta)
            : m_vecPosition(position)
            , m_fDelta(delta)
        {
        }

        Vector2i GetPosition() const { return m_vecPosition; }
        float GetDelta() const { return m_fDelta; }

    private:
        Vector2i m_vecPosition;
        float m_fDelta;
};

class EventMouseButtonPressed : public Event
{
    public:
        EventMouseButtonPressed(Vector2i const& position, Input::MouseButton const button)
            : m_vecPosition(position)
            , m_button(button)
        {
        }

        Input::MouseButton GetButton() const { return m_button; }
        Vector2i GetPosition() const { return m_vecPosition; }

    private:
        Input::MouseButton m_button;
        Vector2i m_vecPosition;
};

class EventMouseButtonReleased : public Event
{
    public:
        EventMouseButtonReleased(Vector2i const& position, Input::MouseButton const button)
            : m_vecPosition(position)
            , m_button(button)
        {
        }

        Input::MouseButton GetButton() const { return m_button; }
        Vector2i GetPosition() const { return m_vecPosition; }

    private:
        Input::MouseButton m_button;
        Vector2i m_vecPosition;
};

class EventMouseButtonDoubleClicked : public Event
{
    public:
        EventMouseButtonDoubleClicked(Vector2i const& position, Input::MouseButton const button)
            : m_vecPosition(position)
            , m_button(button)
        {
        }

        Input::MouseButton GetButton() const { return m_button; }
        Vector2i GetPosition() const { return m_vecPosition; }

    private:
        Input::MouseButton m_button;
        Vector2i m_vecPosition;
};