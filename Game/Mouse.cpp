#include "pch.h"
#include "Mouse.h"


Mouse* Mouse::s_mouse;

Mouse::Mouse()
{
    s_mouse = this;
}

Mouse::~Mouse()
{
    s_mouse = nullptr;
}

void Mouse::OnLeftPressed(int x, int y)
{
    is_left_down = true;
    eventBuffer.emplace(Event(Event::Type::LeftButtonPressed, x, y));
}

void Mouse::OnLeftReleased(int x, int y)
{
    is_left_down = false;
    eventBuffer.emplace(Event(Event::Type::LeftButtonReleased, x, y));
}

void Mouse::OnRightPressed(int x, int y)
{
    is_right_down = true;
    eventBuffer.emplace(Event(Event::Type::RightButtonPressed, x, y));
}

void Mouse::OnRightReleased(int x, int y)
{
    is_right_down = false;
    eventBuffer.emplace(Event(Event::Type::RightButtonReleased, x, y));
}

void Mouse::OnMiddlePressed(int x, int y)
{
    is_middle_down = true;
    eventBuffer.emplace(Event(Event::Type::MiddleButtonPressed, x, y));
}

void Mouse::OnMiddleReleased(int x, int y)
{
    is_middle_down = false;
    eventBuffer.emplace(Event(Event::Type::MiddleButtonReleased, x, y));
}

void Mouse::OnXButton1Pressed(int x, int y)
{
    is_xbutton1_down = true;
    eventBuffer.emplace(Event(Event::Type::XButton1Pressed, x, y));
}

void Mouse::OnXButton1Released(int x, int y)
{
    is_xbutton1_down = false;
    eventBuffer.emplace(Event(Event::Type::XButton1Released, x, y));
}

void Mouse::OnXButton2Pressed(int x, int y)
{
    is_xbutton2_down = true;
    eventBuffer.emplace(Event(Event::Type::XButton2Pressed, x, y));
}

void Mouse::OnXButton2Released(int x, int y)
{
    is_xbutton2_down = false;
    eventBuffer.emplace(Event(Event::Type::XButton2Released, x, y));
}

void Mouse::OnWheelUp(int x, int y)
{
    eventBuffer.emplace(Event(Event::Type::WheelMoveUp, x, y));
}

void Mouse::OnWheelDown(int x, int y)
{
    eventBuffer.emplace(Event(Event::Type::WheelMoveDown, x, y));
}

void Mouse::OnMouseMove(int x, int y)
{
    this->x = x; this->y = y;
    eventBuffer.emplace(Event(Event::Type::Move, x, y));
}

void Mouse::OnMouseMoveRaw(int x, int y)
{
    eventBuffer.emplace(Event(Event::Type::MoveRaw, x, y));
}

bool Mouse::IsConnected() const
{
    return GetSystemMetrics(SM_MOUSEPRESENT) != 0;
}

bool Mouse::EventBufferIsEmpty()
{
    return eventBuffer.empty();
}

Mouse::Event Mouse::ReadEvent()
{
    if (eventBuffer.empty())
        return Mouse::Event();

    Mouse::Event mouseEvent = eventBuffer.front();
    eventBuffer.pop();
    return mouseEvent;
}

Mouse& Mouse::Get()
{
    return *s_mouse;
}